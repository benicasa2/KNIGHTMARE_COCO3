"""Bound the sprite-erasure interval in both stages using emulated clocks."""
from pathlib import Path
import argparse,hashlib,json,statistics
from verify_ai import boot
from verify_probe import symbols
from verify_controls_scroll import call,enter_game,val,keyboard_tick
from verify_combat import clear_slots,slot,META,tick
from verify_phase2_edges import invoke_disk
ROOT=Path(__file__).resolve().parents[1]

def cycles(r):
 return int(bytes.fromhex(r.cmd('qRcmd,'+b'cycles'.hex())).decode().split()[0])

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--xroar',type=Path,required=True);ap.add_argument('--output',type=Path,required=True);a=ap.parse_args();sy=symbols('DAC6');cases={}
 for stage in [0,1]:
  p,log,r=boot(a.xroar,'DAC6')
  try:
   call(r,sy,'main_loop');enter_game(r,sy)
   if stage:
    invoke_disk(r,sy,'phase_advance')
    regs=bytes.fromhex(r.cmd('g'));r.cmd(f'P0={regs[0]&0xaf:02x}');r.cmd(f'P8={sy["main_loop"]:04x}')
   r.write(sy['test_invincible'],b'\x01');r.write(sy['token_index'],b'\xff')
   for mode,on in [('idle',False),('crowded',False),('crowded',True),('boss',False),('wide',False)]:
    busy=mode=='crowded'
    if bool(val(r,sy,'music_enabled'))!=on:keyboard_tick(r,sy,{5:2},0);keyboard_tick(r,sy,{},0)
    clear_slots(r,sy);r.write(sy['boss_state'],b'\0');tick(r,sy)
    if busy:
     for i in range(5 if on else 7):slot(r,i,14,32+i*28,70,META['enemy_ids']['14'][0])
     for i in range(7,10):slot(r,i,0,24+(i-7)*18,150,META['ids']['arrow'])
     for i in range(10,18):slot(r,i,8,30+(i-10)*24,100,META['ids']['bullet'],vx=64,vy=64)
    # Measure the normal game path, including enemy logic and scroll cadence.
    r.write(sy['freeze_ticks'],bytes(2))
    if mode=='boss':
     for field,value in [('boss_state',1),('boss_phase',0),('boss_x',120 if stage else 112),('boss_y',48),('boss_hp',30 if stage else 20),('boss_wait',0)]:r.write(sy[field],bytes([value]))
     r.write(sy['freeze_ticks'],(60000).to_bytes(2,'big'))
    elif mode=='wide':
     slot(r,0,7,100,70,META['stage2_type_frames']['7'][4],hp=3)
     slot(r,1,14,120,80,META['enemy_ids']['14'][0])
     r.write(sy['freeze_ticks'],(60000).to_bytes(2,'big'))
    tick(r,sy)
    samples=[]
    for n in range(12):
     call(r,sy,'apply_input');r.write(sy['input_bits'],b'\0');start=cycles(r)
     call(r,sy,'render_begin');commit=cycles(r)
     r.bp(sy['erase_return']);r.bp(sy['combat_frame_complete']);r.go()
     retained=int.from_bytes(bytes.fromhex(r.cmd('g'))[-2:],'big')==sy['combat_frame_complete']
     r.unbp(sy['erase_return']);r.unbp(sy['combat_frame_complete']);gone=cycles(r)
     if not retained:call(r,sy,'combat_frame_complete')
     end=cycles(r)
     samples.append(dict(frame_ms=(end-start)/14318.18,commit_ms=(end-commit)/14318.18,player_erased_to_commit_ms=(end-gone)/14318.18,retained_frame=int(retained)))
    key=f'stage{stage+1}-'+mode+('-music' if on else '')
    cases[key]=dict(samples=samples,**{f'mean_{k}':statistics.mean(s[k] for s in samples) for k in samples[0]})
  finally:r.s.close();p.terminate();p.wait(timeout=10);log.close()
 result=dict(method='XRoar master-clock ticks; 12 controlled updates per case; commit and player erasure intervals are timing proxies, not captured-display flicker; boss/wide cases freeze AI to isolate composition cost; no hardware test',tested_disk_sha256=hashlib.sha256((ROOT/'bin/DAC6.DSK').read_bytes()).hexdigest(),tested_levels_sha256=hashlib.sha256((ROOT/'bin/LEVELS.DSK').read_bytes()).hexdigest(),cases=cases)
 a.output.write_text(json.dumps(result,indent=2),encoding='utf-8');print(json.dumps({k:{n:v for n,v in c.items() if n!='samples'} for k,c in cases.items()},indent=2),flush=True)

if __name__=='__main__':main()
