"""Second-stage checkpoints, new projectile families and wide hurt sprites."""
from pathlib import Path
import argparse,hashlib,json
from verify_ai import boot,shim,REF,verify_waves
from verify_probe import symbols
from verify_controls_scroll import call,enter_game,val,word
from verify_combat import picture,tick,slot,clear_slots,META,BASE
from prepare_ai import machine,wave_reference
ROOT=Path(__file__).resolve().parents[1]

def invoke_disk(r,sy,label):
 # FD scratch uses $0B00; a six-byte caller at $0900 remains outside it.
 address=sy[label];r.write(0x900,bytes([0xbd,address>>8,address&255,0x20,0xfe]))
 r.cmd('P7=2fff');r.cmd('P8=0900');r.bp(0x903);r.go();r.unbp(0x903)

def verify(a,name):
 p,log,r=boot(a.xroar,name);sy=symbols(name);raw=a.rom.read_bytes();checks={}
 try:
  call(r,sy,'main_loop');enter_game(r,sy);invoke_disk(r,sy,'phase_advance')
  assert val(r,sy,'campaign_stage')==1 and not val(r,sy,'FD_ERROR')
  assert r.read(0xa000,8192)==(ROOT/'assets/stage2-packed-tiles.bin').read_bytes()
  # All 41 rewind indices are obtained by executing the original mount code.
  rewinds=[]
  for index in range(41):
   z=machine(raw);z.mem[0xe061]=2;z.mem[0xe062]=1;z.mem[0xe073]=1;z.mem[0xe09f]=index
   z.mem[0x568c]=z.mem[0x569d]=0xc9;z.corre_hasta(0x53d3,0x54b5)
   r.write(sy['camera_y'],(1680).to_bytes(2,'big'));r.write(sy['wave_index'],bytes([index]))
   shim(r,sy,a.lwasm,'phase2_rewind',f'        jsr ${sy["checkpoint_restore"]:04x}\n')
   assert val(r,sy,'wave_index')==z.mem[0xe09f],(index,val(r,sy,'wave_index'),z.mem[0xe09f])
   rewinds.append(val(r,sy,'wave_index'))
  checks['original_wave_rewinds']=rewinds
  items=json.loads((ROOT/'analysis/items-assets.json').read_text())
  for segment in range(10):
   restored=1776-segment*192;camera=restored-95 if segment<9 else 0
   r.write(sy['camera_y'],camera.to_bytes(2,'big'));r.write(sy['wave_index'],b'\x0d')
   r.write(sy['boss_state'],bytes([int(segment==9)]));r.write(sy['boss_phase'],b'\0')
   r.write(sy['boss_hp'],b'\x13');r.write(sy['boss_x'],b'\x78');r.write(sy['boss_y'],b'\x20')
   r.write(sy['B2_VX'],bytes.fromhex('0123fedc'));r.write(sy['boss_lifetime'],(1234).to_bytes(2,'big'))
   r.write(sy['weapon_kind'],b'\x09');r.write(sy['ability_kind'],b'\x01')
   shim(r,sy,a.lwasm,'phase2_segment',f'        jsr ${sy["checkpoint_restore"]:04x}\n')
   assert word(r,sy,'camera_y')==restored and val(r,sy,'checkpoint_segment')==segment
   assert not val(r,sy,'weapon_kind') and not val(r,sy,'ability_kind')
   assert val(r,sy,'token_index')==sum(t['world_y']>restored for t in items['tokens'][1])
   if segment==9:
    assert val(r,sy,'boss_hp')==19 and word(r,sy,'boss_lifetime')==1234
    assert r.read(sy['B2_VX'],4)==bytes.fromhex('0123fedc')
   picture(r,sy,f'{name}-phase2-checkpoint-{segment}')
  checks['ten_native_segment_redraws']=True;checks['boss2_checkpoint_motion_preserved']=True
  # Actual burst routines versus executed Z80, including the all-or-none gate.
  bursts=[];r.write(sy['boss_state'],b'\0')
  for kind in [5,6]:
   for x,y in [(40,40),(120,40),(216,40),(60,151),(192,151)]:
    for occupied in [0,5,8]:
     z=machine(raw);z.ix=0xe100;z.mem[0xe105]=x;z.mem[0xe103]=y;z.b=kind
     r.write(BASE+10*32,bytes(256))
     for i in range(occupied):z.mem[0xe200+i*16]=2;r.write(BASE+(10+i)*32,b'\x02')
     z.corre(0x857f)
     r.write(sy['aim_x'],bytes([x]));r.write(sy['aim_y'],bytes([y]));r.write(sy['ai_shot_kind'],bytes([kind]))
     shim(r,sy,a.lwasm,'phase2_burst',f'        jsr ${sy["phase2_enemy_burst"]:04x}\n')
     got=[];want=[];data=r.read(BASE+10*32,256)
     for i in range(8):
      q=data[i*32:(i+1)*32];o=z.mem[0xe200+i*16:0xe210+i*16]
      if q[0]==1:got.append([q[1],int.from_bytes(q[2:4],'big'),int.from_bytes(q[4:6],'big'),int.from_bytes(q[6:8],'big'),int.from_bytes(q[8:10],'big')])
      if o[0]==1:want.append([o[1],o[5]*256+o[4],o[3]*256+o[2],o[9]*256+o[8],o[7]*256+o[6]])
     assert got==want,dict(kind=kind,x=x,y=y,occupied=occupied,got=got,want=want)
     bursts.append(dict(kind=kind,origin=[x,y],occupied=occupied,emitted=len(got)))
  checks['burst_scenarios']=bursts
  # A hit enters the special four-hit enemy's grow/wait/shrink states. Compare
  # every frame and independently render its widest 31-byte scanline image.
  clear_slots(r,sy);r.write(sy['camera_y'],(1776).to_bytes(2,'big'))
  shim(r,sy,a.lwasm,'phase2_rebuild',f'        jsr ${sy["game_rebuild"]:04x}\n')
  slot(r,0,7,100,70,META['enemy_ids']['7'][0],hp=3);r.write(BASE+12,b'\x04')
  z=machine(raw);z.ix=0xe100;z.mem[0xe100]=4;z.mem[0xe101]=7;z.mem[0xe105]=100;z.mem[0xe103]=70
  z.mem[0xe114]=3
  frames=META['stage2_type_frames']['7']
  for t in range(60):
   z.mem[0xe003]=(2*t+1)&255;z.corre(0x75e5)
   r.write(sy['ai_clock'],bytes([(2*t+1)&255]))
   shim(r,sy,a.lwasm,'phase2_hurt',f'        ldx #$2700\n        jsr ${sy["ai_enemy_tick"]:04x}\n')
   q=r.read(BASE,32);o=z.mem[0xe100:0xe120]
   assert q[12]==o[0] and int.from_bytes(q[2:4],'big')==o[5]*256+o[4] and int.from_bytes(q[4:6],'big')==o[3]*256+o[2],(t,q.hex(),bytes(o).hex())
   assert q[14]==frames[o[10]-17],(t,q[14],o[10])
  checks['hurt_state_original_samples']=60
  regs=bytes.fromhex(r.cmd('g'));r.cmd(f'P0={regs[0]&0xaf:02x}');r.cmd(f'P8={sy["main_loop"]:04x}')
  # Paint/erase every expanded frame through the normal renderer.
  r.write(sy['test_invincible'],b'\x01')
  for art in frames[2:]:
   clear_slots(r,sy);tick(r,sy)
   slot(r,0,7,100,70,art,hp=3);r.write(BASE+12,b'\x06');r.write(BASE+27,b'\x20')
   tick(r,sy);picture(r,sy,f'{name}-phase2-wide-{art}')
  checks['wide_hurt_frames_draw_and_erase']=True
  # Lose the final life in phase 2 through the native hostile-shot path.
  clear_slots(r,sy);tick(r,sy)
  for field,value in [('test_invincible',0),('player_invul',0),('ability_kind',0),('shield_hits',0),('player_lives',1)]:r.write(sy[field],bytes([value]))
  slot(r,10,8,val(r,sy,'player_x')+8,val(r,sy,'player_y')+8,META['ids']['bullet'])
  tick(r,sy);assert val(r,sy,'combat_status')==1 and val(r,sy,'player_lives')==0
  for _ in range(48):tick(r,sy)
  assert val(r,sy,'scene_mode')==1 and r.read(0xffb0,1)==r.read(0xffb4,1)
  checks['phase2_last_life_returns_blue_konami']=True
  # Real R input must reload phase 1's tile bank and discard the phase 2 song.
  tick(r,sy,64)
  assert val(r,sy,'campaign_stage')==0 and not val(r,sy,'music_enabled')
  assert r.read(0xa000,8192)==(ROOT/'assets/stage1-packed-tiles.bin').read_bytes()
  picture(r,sy,f'{name}-phase2-r-restart');checks['r_reloads_first_stage']=True
  r.write(sy['campaign_stage'],b'\x01')
  shim(r,sy,a.lwasm,'phase2_profile',f'        jsr ${sy["phase_profile"]:04x}\n')
  REF['wave_trace']=wave_reference(raw,stage=1)
  checks['wave_births']=verify_waves(r,sy,a.lwasm)
  report=dict(status='phase2 checkpoint, projectile and hurt-sprite scenarios passed',variant=name,tested_disk_sha256=hashlib.sha256((ROOT/f'bin/{name}.DSK').read_bytes()).hexdigest(),tested_levels_sha256=hashlib.sha256((ROOT/'bin/LEVELS.DSK').read_bytes()).hexdigest(),checks=checks)
  (ROOT/f'analysis/{name}-phase2-edges-verification.json').write_text(json.dumps(report,indent=2),encoding='utf-8');print(name+' phase2 edges passed',flush=True)
 finally:r.s.close();p.terminate();p.wait(timeout=10);log.close()

if __name__=='__main__':
 ap=argparse.ArgumentParser();ap.add_argument('--xroar',type=Path,required=True);ap.add_argument('--lwasm',type=Path,required=True);ap.add_argument('--rom',type=Path,required=True);a=ap.parse_args()
 for name in ['DAC6','ORCH90']:verify(a,name)
