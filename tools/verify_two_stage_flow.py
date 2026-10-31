from pathlib import Path
import argparse,sys,json,hashlib
OUT=Path(__file__).resolve().parents[1];ROOT=OUT.parents[1];sys.path.insert(0,str(OUT/'tools'))
from verify_ai import boot,shim
from verify_probe import symbols
from verify_controls_scroll import call,enter_game,keyboard_tick,val,word
from verify_combat import tick,slot,picture,META
ap=argparse.ArgumentParser();ap.add_argument('--variant',default='DAC6',choices=['DAC6','ORCH90']);ap.add_argument('--rom',type=Path,default=ROOT/'work/knightmare.rom');ap.add_argument('--lwasm',type=Path,default=Path('C:/Users/Anderson/Documents/Codex/2026-10-25/ol/work/tools/lwtools/lwtools-4.25-win64/lwasm.exe'));ap.add_argument('--xroar',type=Path,default=Path('C:/Users/Anderson/Downloads/xroar-1.12-w64/xroar.exe'));a=ap.parse_args();name=a.variant;sy=symbols(name);lw=a.lwasm
p,log,r=boot(a.xroar,name)
try:
 call(r,sy,'main_loop');enter_game(r,sy)
 # Isolate the real end-of-phase path; no framebuffer or expected AI is supplied.
 for field,value in [('test_invincible',1),('player_lives',4),('weapon_kind',8),('speed_level',2),('ability_kind',1),('shield_hits',5),('token_index',255)]:r.write(sy[field],bytes([value]))
 r.write(sy['score'],(1234).to_bytes(2,'big'));r.write(sy['high_score'],(4321).to_bytes(2,'big'));r.write(sy['camera_y'],(48).to_bytes(2,'big'))
 shim(r,sy,lw,'flow_rebuild',f'        jsr ${sy["game_rebuild"]:04x}\n')
 regs=bytes.fromhex(r.cmd('g'));r.cmd(f'P0={regs[0]&0xaf:02x}');r.cmd(f'P8={sy["main_loop"]:04x}')
 keyboard_tick(r,sy,{5:2},0);keyboard_tick(r,sy,{},0)
 for n in range(400):
  tick(r,sy)
  if val(r,sy,'boss_state')==1 and val(r,sy,'boss_phase')==0:break
 else:raise AssertionError('first boss did not finish entry')
 print('first boss ready',n,flush=True)
 for n in range(20):
  slot(r,7,0,val(r,sy,'boss_x')+16,val(r,sy,'boss_y')+12,META['ids']['arrow'])
  tick(r,sy)
 assert val(r,sy,'boss_hp')==0 and val(r,sy,'boss_state')==2
 for n in range(130):
  tick(r,sy)
  if val(r,sy,'combat_status')==3:break
 else:raise AssertionError('death did not reach exit sequence')
 print('boss killed, score',word(r,sy,'score'),flush=True)
 samples=[]
 for n in range(150):
  tick(r,sy);samples.append([val(r,sy,'player_x'),val(r,sy,'player_y')])
  if val(r,sy,'campaign_stage')==1 and val(r,sy,'scene_mode')==0:break
 else:raise AssertionError('exit sequence never entered stage 2')
 assert word(r,sy,'score')==11234 and val(r,sy,'player_lives')==5
 for f,v in [('weapon_kind',8),('speed_level',2),('ability_kind',1),('shield_hits',5),('test_invincible',1),('music_enabled',1),('PHASE_WAVE_COUNT',41),('PHASE_FIXED_COUNT',42)]:assert val(r,sy,f)==v,(f,val(r,sy,f),v)
 assert word(r,sy,'camera_y')==1776 and word(r,sy,'enemy_limit_end')==0x2700+5*32
 assert r.read(0xa000,8192)==(OUT/'assets/stage2-packed-tiles.bin').read_bytes(),'wrong stage2 tiles'
 image=picture(r,sy,f'{name}-stage2-start');print('stage2 start',n,image,flush=True)
 # Let the unmodified wave engine traverse the entire second stage, protected
 # by the user's I test mode. Tokens are suppressed only for this fixture.
 r.write(sy['token_index'],bytes([255]));call(r,sy,'boss2_update');call(r,sy,'combat_frame_complete')
 assert word(r,sy,'camera_y')<=120 and val(r,sy,'boss_state')==1
 for n in range(400):
  tick(r,sy)
  if val(r,sy,'boss_phase')==0:break
 else:raise AssertionError('second boss never finished entry')
 picture(r,sy,f'{name}-stage2-boss');print('second boss ready',word(r,sy,'camera_y'),val(r,sy,'boss_hp'),flush=True)
 r.write(sy['weapon_kind'],bytes([0]))
 for n in range(100):
  if val(r,sy,'boss_state')!=1:break
  slot(r,7,0,val(r,sy,'boss_x')+4,val(r,sy,'boss_y')+4,META['ids']['arrow'])
  tick(r,sy)
 assert val(r,sy,'boss_hp')==0 and val(r,sy,'boss_state')==2
 print('second boss killed with native arrows',n,flush=True)
 for n in range(300):
  tick(r,sy)
  if val(r,sy,'scene_mode')==1:break
 else:raise AssertionError('second exit sequence did not complete')
 report=dict(tested_disk_sha256=hashlib.sha256((OUT/f'bin/{name}.DSK').read_bytes()).hexdigest(),tested_levels_sha256=hashlib.sha256((OUT/'bin/LEVELS.DSK').read_bytes()).hexdigest(),status='two-stage victory, original exit walk and stage2 traversal passed',stage1_exit_updates=len(samples),preserved_equipment=True,stage2_bonus_life=True,stage2_start_sha256=image,scope='controlled first boss setup and arrow fixtures; natural second-stage waves and scroll under I test protection; not unassisted playthrough')
 (OUT/f'analysis/{name}-two-stage-flow-verification.json').write_text(json.dumps(report,indent=2))
 print(json.dumps(report,indent=2),flush=True)
finally:r.s.close();p.terminate();p.wait(timeout=10);log.close()
