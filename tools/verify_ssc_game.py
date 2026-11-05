"""Exercise the unmodified main loop, real keyboard matrix and SSC fault paths."""
from pathlib import Path
import argparse,hashlib,json,re,subprocess
ROOT=Path(__file__).resolve().parents[1]

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--mame',type=Path,required=True);ap.add_argument('--rompath',type=Path,required=True);ap.add_argument('--output',type=Path,required=True);a=ap.parse_args();a.output.mkdir(parents=True,exist_ok=True)
 results={}
 for variant,mode in [('DAC6','normal'),('SSC','normal'),('SSC','absent'),('SSC','busy')]:
  sy={k:int(v,16) for k,v in re.findall(r'^(\w+) EQU \$([\dA-Fa-f]+)',(ROOT/f'analysis/{variant}.sym').read_text(),re.M)}
  script=LUA.replace('@SYMBOLS@','{'+','.join(f'{k}={v}' for k,v in sy.items())+'}').replace('@MODE@',mode).replace('@SSC@','true' if variant=='SSC' else 'false').replace('@ASSETS@',str(ROOT/'assets').replace('\\','/'))
  key=variant+'-'+mode;folder=a.output/key;folder.mkdir(exist_ok=True);lua=folder/'run.lua';lua.write_text(script)
  cmd=[str(a.mame.resolve()),'coco3','-ram','128k','-rompath',str(a.rompath.resolve()),'-ext','multi','-ext:multi:slot4','fdc']
  if variant=='SSC' and mode!='absent':cmd+=['-ext:multi:slot1','ssc']
  cmd+=['-flop1',str(ROOT/f'bin/{variant}.DSK'),'-flop2',str(ROOT/'bin/LEVELS.DSK'),'-video','none','-sound','none','-nothrottle','-skip_gameinfo','-seconds_to_run','230','-autoboot_script',str(lua.resolve())]
  for opt in ['cfg','nvram','diff','snapshot']:cmd+=['-'+opt+'_directory',str(folder.resolve()/opt)]
  run=subprocess.run(cmd,capture_output=True,text=True,timeout=120);log=run.stdout+run.stderr;(folder/'run.log').write_text(log)
  print(key,log[:1200],flush=True)
  assert run.returncode==0 and 'GAME_VERIFIED' in log and '[LUA ERROR]' not in log,key
  rates={k:float(v.replace(',','.')) for k,v in re.findall(r'RATE\s+(\w+)\s+([\d.,]+)',log)}
  results[key]=dict(passed=True,updates_per_second=rates,disk_sha256=hashlib.sha256((ROOT/f'bin/{variant}.DSK').read_bytes()).hexdigest())
 report=dict(scope='MAME 0.289, actual disk boot and continuous native main loop, real keyboard matrix; controlled stage-exit and game-over state fixtures; normal rates are 600 video frames of natural play per case, not a worst-case guarantee; no physical hardware',results=results)
 (ROOT/'analysis/SSC-game-verification.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))

LUA=r'''
local sy=@SYMBOLS@
local mode='@MODE@'
local ssc=@SSC@
local cpu=manager.machine.devices[':maincpu']
local mem=cpu.spaces['program']
local n=0
local state='boot'
local since=0
local beginning=0
local fault=false
local function b(k) return mem:read_u8(sy[k]) end
local function w(k) return mem:read_u16(sy[k]) end
local function set(k,v) mem:write_u8(sy[k],v) end
local function key(row,mask,on)
 local f=manager.machine.ioport.ports[':row'..row]:field(mask)
 if on then f:set_value(1) else f:clear_value() end
end
local function step(s) state=s;since=n end
ssc_busy_tap=mem:install_read_tap(0xff7e,0xff7e,'ssc_stuck',function(off,data) if fault then return 0 end end)
local function rate(name) print('RATE',name,(w('game_ticks')-beginning)*59.9227434/600) end
local function tiles(stage)
 local f=assert(io.open('@ASSETS@/stage'..stage..'-packed-tiles.bin','rb'))
 local data=f:read('*a');f:close()
 local old=mem:read_u8(0xffa5);mem:write_u8(0xffa5,12)
 local mismatch=nil
 for i=1,#data do if mem:read_u8(0x9fff+i)~=data:byte(i) then mismatch=i;break end end
 mem:write_u8(0xffa5,old)
 assert(not mismatch,'Disk tiles differ at '..tostring(mismatch))
 print('TILES_VERIFIED',stage,#data)
end
emu.register_frame_done(function()
 n=n+1
 local age=n-since
 if n==180 then manager.machine.natkeyboard:post('RUN"START"\n') end
 if state=='boot' and n>300 and b('scene_mode')==2 then
  if ssc then assert(b('SSC_FAULT')==(mode=='absent' and 1 or 0),'Wrong presence detection') end
  key(3,128,true);step('start')
 elseif state=='start' and age==60 then key(3,128,false)
 elseif state=='start' and age>500 then
  assert(b('scene_mode')==0 and b('FD_ERROR')==0,'Native game did not load: scene '..b('scene_mode')..' error '..b('FD_ERROR')..' LSN '..w('FD_LSN')..' PC '..cpu.state['PC'].value)
  tiles(1)
  key(1,2,true);step('protection')
 elseif state=='protection' and age==30 then
  key(1,2,false);assert(b('test_invincible')==1,'I key failed')
  key(1,32,true);step('music')
 elseif state=='music' and age==60 then
  assert(b('music_enabled')==(mode=='absent' and 0 or 1),'M key failed/debounce')
  assert(w('enemy_limit_end')==0x2700+(mode=='absent' and 7 or 5)*32,'Enemy cap')
  key(1,32,false);step('release')
 elseif state=='release' and age==30 then
  if mode=='busy' then fault=true;step('fault')
  elseif mode=='absent' then beginning=w('game_ticks');step('silent')
  else beginning=w('game_ticks');step('bench_on') end
 elseif state=='fault' and age==90 then
  assert(b('SSC_FAULT')==1 and b('music_enabled')==0,'Stuck BUSY not disabled')
  assert(w('enemy_limit_end')==0x2700+7*32,'Fault enemy cap')
  beginning=w('game_ticks');step('silent')
 elseif state=='silent' and age==120 then
  assert(w('game_ticks')>beginning+10,'Game stalled on absent/busy cartridge')
  print('GAME_VERIFIED',mode);manager.machine:exit()
 elseif state=='bench_on' and age==600 then
  rate('stage1_music');key(1,32,true);step('off')
 elseif state=='off' and age==30 then
  key(1,32,false);assert(b('music_enabled')==0,'Music did not turn off')
  assert(w('enemy_limit_end')==0x2700+7*32,'Seven enemy cap')
  beginning=w('game_ticks');step('bench_off')
 elseif state=='bench_off' and age==600 then
  rate('stage1_silent');key(1,4,true);step('joystick')
 elseif state=='joystick' and age==30 then
  key(1,4,false);assert(b('input_mode')==1,'J key failed')
  key(1,8,true);step('keyboard')
 elseif state=='keyboard' and age==30 then
  key(1,8,false);assert(b('input_mode')==0,'K key failed')
  -- Enter the actual native exit/drive-1 loader using a prepared end state.
  set('EXIT_STATE',3);set('boss_state',3);step('stage2')
 elseif state=='stage2' and age>600 then
  assert(b('campaign_stage')==1 and b('scene_mode')==0 and b('FD_ERROR')==0,'Stage 2 load failed')
  tiles(2)
  if ssc then assert(b('SSC_FAULT')==0,'SSC lost across disk load') end
  key(1,32,true);step('stage2_music')
 elseif state=='stage2_music' and age==30 then
  key(1,32,false);assert(b('music_enabled')==1,'Stage 2 M key failed')
  beginning=w('game_ticks');step('stage2_bench')
 elseif state=='stage2_bench' and age==600 then
  rate('stage2_music');assert(b('MUSIC_TRACK')==2,'Wrong stage 2 music')
  set('respawn_pending',2);step('game_over')
 elseif state=='game_over' and age>120 then
  assert(b('scene_mode')==1 or b('scene_mode')==2,'Game over did not return to introduction')
  assert(b('music_enabled')==0 and b('test_invincible')==0,'Game over did not reset options')
  print('GAME_VERIFIED',mode);manager.machine:exit()
 end
end)
'''
if __name__=='__main__':main()
