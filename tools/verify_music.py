"""Native music oracle, real M matrix, enemy cap and actual DAC mixing."""
from pathlib import Path
import argparse,json,hashlib
from verify_ai import boot,shim
from verify_probe import symbols
from verify_controls_scroll import call,enter_game,val,word,keyboard_tick
from verify_combat import tick,slot,META,BASE,picture,clear_slots
ROOT=Path(__file__).resolve().parents[1]
ASSETS=json.loads((ROOT/'analysis/music-assets.json').read_text())

def sequence(r,sy,lw,track):
 r.write(sy['music_enabled'],bytes([0]));r.write(sy['campaign_stage'],bytes([track//2]));r.write(sy['boss_state'],bytes([track%2]))
 shim(r,sy,lw,'music_enable',f"        orcc #$50\n        jsr ${sy['music_toggle']:04x}\n")
 info=ASSETS['tracks'][track];raw=(ROOT/f"reference/music-{info['original_id']:02x}-psg.bin").read_bytes();fr=[raw[i:i+14] for i in range(0,len(raw),14)]
 dictionary=ASSETS.get('dictionaries',[ASSETS['dictionary']])[track//2];expected=[]
 for i in range(len(fr)+(len(fr)-info['loop_frame'])):
  q=fr[i] if i<len(fr) else fr[info['loop_frame']+(i-len(fr))%(len(fr)-info['loop_frame'])]
  ids=[next(j for j,d in enumerate(dictionary) if d['period']==int.from_bytes(q[ch*2:ch*2+2],'little') and d['volume']==q[8+ch]) for ch in range(2)]
  half=all(not dictionary[j]['amplitude'] or dictionary[j]['step']<=16384 for j in ids)
  expected.append(b''.join(dictionary[j]['step'].to_bytes(2,'big') for j in ids)+bytes([dictionary[j]['amplitude'] for j in ids]+ids)+b''.join(((dictionary[j]['step']*(2 if half else 1))&65535).to_bytes(2,'big') for j in ids)+bytes([int(half)]))
 for first in range(0,len(expected),240):
  n=min(240,len(expected)-first)
  r.write(0xbe2,bytes([n]))
  body=f"""        lda #12
        sta $ffa4
        ldy #$8000
        sty $0be0
music_verify_sample
        ldy $0be0
        ldd $0d8e
        std ,y++
        ldd $0d9e
        std ,y++
        lda $0d87
        sta ,y+
        lda $0d97
        sta ,y+
        lda $0d8b
        sta ,y+
        lda $0d9b
        sta ,y+
        ldd ${sy['music_step0']:04x}
        std ,y++
        ldd ${sy['music_step1']:04x}
        std ,y++
        lda ${sy['music_half_rate']:04x}
        sta ,y+
        sty $0be0
        jsr ${sy['music_tick']:04x}
        dec $0be2
        bne music_verify_sample
"""
  shim(r,sy,lw,'music_sequence',body);actual=r.read(0x8000,n*13);want=b''.join(expected[first:first+n])
  assert actual==want,dict(track=track,first=first,differences=[i for i,(a,b) in enumerate(zip(actual,want)) if a!=b][:12])
 return dict(track=hex(info['original_id']),native_frames_compared=len(expected),exact_event_loop=True,dds_rate_scaling_verified=True)

def keyboard_audio(exe,name,lw):
 p,log,r=boot(exe,name);sy=symbols(name)
 try:
  call(r,sy,'main_loop');keyboard_tick(r,sy,{5:2},0);assert not val(r,sy,'music_enabled')
  keyboard_tick(r,sy,{},0);enter_game(r,sy)
  assert not val(r,sy,'music_enabled') and word(r,sy,'enemy_limit_end')==BASE+7*32
  for i in range(7):slot(r,i,14,24+i*28,60,META['enemy_ids']['14'][0])
  tick(r,sy);keyboard_tick(r,sy,{5:2},0)
  assert val(r,sy,'music_enabled')==1 and word(r,sy,'enemy_limit_end')==BASE+5*32
  assert r.read(BASE+5*32,1)==r.read(BASE+6*32,1)==bytes([0])
  for _ in range(3):keyboard_tick(r,sy,{5:2},0)
  assert val(r,sy,'music_enabled')==1
  picture(r,sy,f'{name}-music-five-enemies')
  # Resume enough to populate audible notes, then capture real FIRQ DAC writes.
  keyboard_tick(r,sy,{},0)
  for _ in range(6):tick(r,sy)
  assert r.read(sy['MUSIC_VOICE0']+7,1)[0] or r.read(sy['MUSIC_VOICE1']+7,1)[0]
  address=0xff7b if name=='ORCH90' else 0xff20
  r.cmd(f'Z2,{address:x},1');samples=[]
  for _ in range(96):r.go();samples.append(bytes.fromhex(r.cmd('g'))[1]);r.step()
  r.cmd(f'z2,{address:x},1');assert len(set(samples))>1,'music is silent'
  # Actual mixed writes must equal the unmodified original PCM plus music.
  clear_slots(r,sy);call(r,sy,'apply_input');r.write(sy['input_bits'],bytes([16]));r.bp(sy['sfx_denied']);r.go();r.unbp(sy['sfx_denied']);assert val(r,sy,'sfx_priority')==1 and word(r,sy,'sfx_ptr')==sy['pcm_shot'] and not val(r,sy,'music_half_rate')
  # A shot is now pending. Watch the output of each timer sample.
  r.cmd(f'Z2,{address:x},1');matched=0
  for _ in range(64):
   r.go();a=bytes.fromhex(r.cmd('g'))[1]
   ptr=word(r,sy,'sfx_ptr');oldbank=r.read(0xffa4,1);r.write(0xffa4,bytes([7]));pcm=r.read(ptr-1,1)[0];r.write(0xffa4,oldbank);off=r.read(sy['MUSIC_SAMPLE'],1)[0];want=(pcm+off)&255
   if name=='DAC6':want &=252
   assert a==want,(name,a,want,pcm,off);matched+=1;r.step()
  r.cmd(f'z2,{address:x},1')
  keyboard_tick(r,sy,{5:2},0)
  assert not val(r,sy,'music_enabled') and word(r,sy,'enemy_limit_end')==BASE+7*32
  for _ in range(20):tick(r,sy)
  assert not val(r,sy,'sfx_priority')
  keyboard_tick(r,sy,{},0);keyboard_tick(r,sy,{5:2},0);assert val(r,sy,'music_enabled')
  tick(r,sy,64);assert not val(r,sy,'music_enabled')
  return dict(real_m_key=True,held_key_debounced=True,enemy_limit_off=7,enemy_limit_on=5,excess_enemies_erased=True,music_dac_samples=len(samples),music_levels=sorted(set(samples)),mixed_sfx_samples_verified=matched,default_off_and_reset_off=True)
 finally:r.s.close();p.terminate();p.wait(timeout=10);log.close()

def verify(exe,lw,name):
 p,log,r=boot(exe,name);sy=symbols(name)
 try:
  call(r,sy,'main_loop');enter_game(r,sy)
  traces=[sequence(r,sy,lw,track) for track in [0,1,2,3]]
  r.write(sy['campaign_stage'],bytes([0]))
  for i in range(7):r.write(BASE+i*32,bytes([1 if i<5 else 0]))
  body=f"        lda #1\n        sta $0a00\n        ldy #$0a00\n        jsr ${sy['ai_find_enemy_slot']:04x}\n"
  shim(r,sy,lw,'music_cap_on',body)
  assert r.read(BASE+5*32,1)==r.read(BASE+6*32,1)==bytes([0])
  shim(r,sy,lw,'music_cap_off',f"        jsr ${sy['music_toggle']:04x}\n"+body)
  assert r.read(BASE+5*32,1)==bytes([1]),'normal seventh-slot pool did not return'

 finally:r.s.close();p.terminate();p.wait(timeout=10);log.close()
 checks=keyboard_audio(exe,name,lw);checks['native_spawn_cap_verified']=True;checks['intro_ignores_m']=True
 report=dict(status='music M-key and reduced-enemy scenarios passed',variant=name,ram_kib=128,tested_disk_sha256=hashlib.sha256((ROOT/f'bin/{name}.DSK').read_bytes()).hexdigest(),music_sequences=traces,checks=checks,scope='native sequence vs full-cycle executed Z80 PSG events, real keyboard PIA, actual DAC samples; adapted square-wave sound, no physical hardware test')
 (ROOT/f'analysis/{name}-music-verification.json').write_text(json.dumps(report,indent=2),encoding='utf-8');print(json.dumps(report,indent=2),flush=True)
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--xroar',type=Path,required=True);ap.add_argument('--lwasm',type=Path,required=True);a=ap.parse_args()
 for name in ['DAC6','ORCH90']:verify(a.xroar,a.lwasm,name)
if __name__=='__main__':main()
