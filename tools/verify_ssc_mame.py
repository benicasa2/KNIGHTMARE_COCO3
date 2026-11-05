"""Boot actual SSC.DSK and compare native AY targets with original PSG traces.

MAME executes the 6809, TMS7040 firmware, AY8913, GIME and disk controller.
The isolated score test invokes native routines; it never supplies note data.
"""
from pathlib import Path
import argparse,hashlib,json,re,subprocess,wave,array
ROOT=Path(__file__).resolve().parents[1]
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--mame',type=Path,required=True);ap.add_argument('--rompath',type=Path,required=True);ap.add_argument('--output',type=Path,required=True);a=ap.parse_args();a.output.mkdir(parents=True,exist_ok=True)
 sy={k:int(v,16) for k,v in re.findall(r'^(\w+) EQU \$([\dA-Fa-f]+)',(ROOT/'analysis/SSC.sym').read_text(),re.M)}
 assets=json.loads((ROOT/'analysis/music-assets.json').read_text(encoding='utf-8'));lengths=[2*t['frames']-t['loop_frame'] for t in assets['tracks']]
 effects=json.loads((ROOT/'analysis/ssc-assets.json').read_text())['effects']
 script=LUA.replace('@SYMBOLS@','{'+','.join(f'{k}={v}' for k,v in sy.items())+'}').replace('@LENGTHS@','{'+','.join(map(str,lengths))+'}').replace('@EFFECT_LENGTHS@','{'+','.join(str(q['frames']) for q in effects)+'}').replace('@CSV@',str(a.output.resolve()/'notes.csv').replace('\\','/')).replace('@FXCSV@',str(a.output.resolve()/'effects.csv').replace('\\','/'))
 (a.output/'run.lua').write_text(script,encoding='utf-8')
 cmd=[str(a.mame.resolve()),'coco3','-ram','128k','-rompath',str(a.rompath.resolve()),'-ext','multi','-ext:multi:slot1','ssc','-ext:multi:slot4','fdc','-flop1',str(ROOT/'bin/SSC.DSK'),'-flop2',str(ROOT/'bin/LEVELS.DSK'),'-video','none','-sound','none','-nothrottle','-skip_gameinfo','-seconds_to_run','400','-autoboot_script',str(a.output.resolve()/'run.lua'),'-wavwrite',str(a.output.resolve()/'actual.wav')]
 for opt in ['cfg','nvram','diff','snapshot']:cmd+=['-'+opt+'_directory',str(a.output.resolve()/opt)]
 run=subprocess.run(cmd,capture_output=True,text=True,timeout=180);log=run.stdout+run.stderr;(a.output/'run.log').write_text(log,encoding='utf-8');print(log[-2000:],flush=True)
 assert run.returncode==0 and 'SSC_AUDIO_VERIFIED' in log and '[LUA ERROR]' not in log
 lines=[x.split(',') for x in (a.output/'notes.csv').read_text().splitlines()];checks=[]
 for k,t in enumerate(assets['tracks']):
  raw=(ROOT/f'reference/music-{t["original_id"]:02x}-psg.bin').read_bytes();fr=[raw[i:i+14] for i in range(0,len(raw),14)];rows=[x for x in lines if int(x[0])==k]
  assert len(rows)==lengths[k],(k,len(rows),lengths[k])
  for n,row in enumerate(rows):
   expected=fr[n if n<len(fr) else t['loop_frame']+(n-len(fr))%(len(fr)-t['loop_frame'])];actual=bytes.fromhex(row[2]);want=expected[:4]+bytes([0,0,0,0x38])+expected[8:10]+bytes([0])
   assert actual==want,dict(track=k,frame=n,actual=actual.hex(),expected=want.hex())
  checks.append(dict(original_id=hex(t['original_id']),frames_compared=len(rows),full_cycle_and_loop=True))
 fxrows=[x.split(',') for x in (a.output/'effects.csv').read_text().splitlines()]
 effect_checks=[]
 for k,q in enumerate(effects):
  raw=(ROOT/f'reference/sfx-{q["original_id"]}-psg.bin').read_bytes()
  for mode in range(2):
   rows=[x for x in fxrows if int(x[0])==k+mode*6]
   assert len(rows)==q['frames'],(k,mode,len(rows))
   for n,row in enumerate(rows):
    f=raw[n*16:(n+1)*16];want=bytes([f[4],f[5]&15,f[6]&31,0x18|(f[7]&0x24),f[10]])
    assert bytes.fromhex(row[2])==want,(k,mode,n,row[2],want.hex())
  effect_checks.append(dict(name=q['name'],frames=q['frames'],music_off_and_on=True))
 with wave.open(str(a.output/'actual.wav'),'rb') as w:
  assert w.getsampwidth()==2;data=array.array('h',w.readframes(w.getnframes()));audible=sum(abs(x)>256 for x in data);assert audible>10000
  sound=dict(rate=w.getframerate(),channels=w.getnchannels(),non_silent_samples=audible,peak=max(map(abs,data)))
 report=dict(status='native SSC music and actual cartridge protocol passed',variant='SSC',tested_disk_sha256=hashlib.sha256((ROOT/'bin/SSC.DSK').read_bytes()).hexdigest(),cpu_mhz=0.895,music=checks,effects=effect_checks,audio=sound,scope='real disk boot then isolated native four-track playback and six effects with music off/on; actual TMS7040/AY8913, target comparison each decoded frame and settled hardware-register checks; not physical hardware',speech_tested=False,speech_rom_warning='The supplied SP0256 image has a checksum warning; speech is never invoked. The TMS7040 ROM and AY music path are used.')
 (ROOT/'analysis/SSC-music-verification.json').write_text(json.dumps(report,indent=2),encoding='utf-8');print(json.dumps(report,indent=2))

LUA=r'''
local sy=@SYMBOLS@
local lengths=@LENGTHS@
local cpu=manager.machine.devices[':maincpu']
local mem=cpu.spaces['program']
local ay=manager.machine.devices[':ext:multi:slot1:ssc:cocossc_ay']
local regs=emu.item(ay.items['0/m_regs'])
assert(emu.item(ay.items['0/m_unscaled_clock']):read()==1789772,'Unexpected AY clock')
local notes=assert(io.open('@CSV@','w'))
local fxnotes=assert(io.open('@FXCSV@','w'))
local effect_lengths=@EFFECT_LENGTHS@
local effect_names={'shot','hit','explosion','boss','cycle','pickup'}
local phase='music'
local effect=0
local n=0
local track=0
local count=0
local active=false
local pending=false
local settle=0
local quiet_checks=0
local last_sent=''
local last_host=0
local host_trace={}
ssc_host_tap=mem:install_write_tap(0xff7e,0xff7e,'ssc_host',function(off,data) last_host=n;table.insert(host_trace,n..':'..data);if #host_trace>50 then table.remove(host_trace,1) end end)
local installed=false
local function invoke(addr)
 if not installed then
  -- Install once. Subsequent calls use a mailbox, never alter a CPU mid-IRQ.
  local code={0x1a,0x50,0x10,0xce,0x2f,0xff,0x1c,0xaf,0x7d,0x09,0x20,0x27,0xfb,0x7f,0x09,0x20,0xbd,math.floor(addr/256),addr%256,0x20,0xf3}
  for k,v in ipairs(code) do mem:write_u8(0x8ff+k,v) end
  cpu.state['PC'].value=0x900
  installed=true
 end
 mem:write_u8(0x911,math.floor(addr/256));mem:write_u8(0x912,addr%256)
 mem:write_u8(0x920,1)
end
local function start_track()
 mem:write_u8(sy.music_enabled,0)
 mem:write_u8(sy.campaign_stage,math.floor(track/2))
 mem:write_u8(sy.boss_state,track%2)
 mem:write_u8(sy.sfx_priority,0)
 count=0;active=true;pending=false
 invoke(sy.music_toggle)
 print('TRACK_START',track,n)
end
local function start_effect()
 local k=effect%6+1
 mem:write_u8(sy.music_enabled,effect<6 and 0 or 1)
 mem:write_u8(sy.MUSIC_TRACK,255)
 mem:write_u8(sy.campaign_stage,0);mem:write_u8(sy.boss_state,0)
 mem:write_u8(sy.sfx_priority,0)
 count=0;active=true;pending=false
 invoke(sy['sfx_'..effect_names[k]])
end
ssc_test_tap=mem:install_write_tap(0xffa4,0xffa4,'ssc_frame',function(off,data,mask)
 if cpu.state['PC'].value==sy.ssc_tick_committed and active then
  if phase=='effects' then
   if mem:read_u8(sy.sfx_priority)==0 then return end
   local s=''
   for _,i in ipairs({4,5,6,7,10}) do s=s..string.format('%02x',mem:read_u8(sy.SSC_TARGET+i)) end
   fxnotes:write(string.format('%d,%d,%s\n',effect,count,s))
   count=count+1
   if count>=effect_lengths[effect%6+1] then active=false;pending=true;settle=n;mem:write_u8(sy.music_enabled,0) end
   return
  end
  local s=''
  for i=0,10 do s=s..string.format('%02x',mem:read_u8(sy.SSC_TARGET+i)) end
  notes:write(string.format('%d,%d,%s\n',track,count,s))
  count=count+1
  if count>=lengths[track+1] then active=false;pending=true;settle=n;mem:write_u8(sy.music_enabled,0) end
 end
end)
emu.register_frame_done(function()
 n=n+1
 if n==180 then manager.machine.natkeyboard:post('RUN"START"\n') end
 if phase=='music' and not active and not pending and track==0 and n>300 and mem:read_u8(sy.scene_mode)==2 then
  assert(mem:read_u8(sy.SSC_FAULT)==0,'Cartridge initialization failed')
  start_track()
 end
 if active or pending then
  assert(mem:read_u8(sy.SSC_FAULT)==0,'Cartridge transport fault')
  local sent=''
  local target=''
  for i=0,10 do sent=sent..string.char(mem:read_u8(sy.SSC_SENT+i));target=target..string.char(mem:read_u8(sy.SSC_TARGET+i)) end
  if n-last_host>8 and mem:read_u8(sy.SSC_PHASE)==0 and sent==last_sent and sent==target then
   for i=0,10 do if regs:read(i)~=mem:read_u8(sy.SSC_SENT+i) then print(table.concat(host_trace,',')) end;assert(regs:read(i)==mem:read_u8(sy.SSC_SENT+i),'AY differs: register '..i..' got '..regs:read(i)..' expected '..mem:read_u8(sy.SSC_SENT+i)..' frame '..n..' phase '..phase..' effect '..effect..' host age '..(n-last_host)) end
   quiet_checks=quiet_checks+1
  end
  last_sent=sent
 end
 if pending and n-settle>20 then
  if phase=='effects' then
   effect=effect+1
   if effect==12 then
    assert(quiet_checks>100,'No settled hardware observations')
    notes:close();fxnotes:close();print('SSC_AUDIO_VERIFIED',quiet_checks);manager.machine:exit()
   else start_effect() end
   return
  end
  print('TRACK_END',track,n,'QUIET',quiet_checks)
  track=track+1
  if track==4 then
   phase='effects';start_effect()
  else start_track() end
 end
end)
'''
if __name__=='__main__':main()
