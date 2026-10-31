"""Measure emulated hardware ticks, never host execution/wall time."""
from pathlib import Path
import argparse,json,socket,subprocess,time,statistics
from verify_probe import RSP,symbols
from verify_controls_scroll import call,val,enter_game
from verify_combat import slot,clear_slots,META,ROOT

def cycles(r):
 return int(bytes.fromhex(r.cmd('qRcmd,'+b'cycles'.hex())).decode().split()[0])

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--xroar',type=Path,required=True);ap.add_argument('--output',type=Path,required=True);args=ap.parse_args()
 exe=args.xroar;sy=symbols('DAC6')
 with socket.socket() as s:s.bind(('127.0.0.1',0));port=s.getsockname()[1]
 log=args.output.with_suffix('.log').open('wb')
 cmd=[str(exe),'-machine','coco3','-ram','128','-no-bas','-extbas',str(exe.parent/'coco3.rom'),'-cart','rsdos','-cart-rom',str(exe.parent/'disk11.rom'),'-load-fd0',str(ROOT/'bin/DAC6.DSK'),'-load-fd1',str(ROOT/'bin/LEVELS.DSK'),'-type','RUN"START"\r','-no-ratelimit','-ui','null','-ao','null','-gdb','-gdb-port',str(port),'-no-disk-write-back']
 p=subprocess.Popen(cmd,stdout=log,stderr=log);r=None
 try:
  for _ in range(60):
   try:r=RSP(port);break
   except OSError:time.sleep(.1)
  assert r
  call(r,sy,'main_loop');r.cmd(f'P8={sy["start"]:04x}');call(r,sy,'main_loop')
  enter_game(r,sy)
  results={}
  for busy in (False,True):
   clear_slots(r,sy);call(r,sy,'combat_frame_complete')
   r.write(sy['player_invul'],b'\xff')
   if busy:
    for i in range(7):slot(r,i,14,32+i*28,70,META['enemy_ids']['14'][0])
    for i in range(7,10):slot(r,i,0,24+(i-7)*18,150,META['ids']['arrow'])
    for i in range(10,18):slot(r,i,8,30+(i-10)*24,100,META['ids']['bullet'],vx=64,vy=64)
   measurements=[]
   for n in range(12):
    call(r,sy,'apply_input');r.write(sy['input_bits'],b'\0')
    start=cycles(r);call(r,sy,'combat_frame_complete');measurements.append((cycles(r)-start)/14318.18)
   results['crowded' if busy else 'idle']=dict(samples_ms=measurements,mean_ms=statistics.mean(measurements),max_ms=max(measurements))
  result=dict(method='XRoar monitor cycles: 14,318,180 master ticks/s, apply_input to combat_frame_complete; includes interrupts/render/logic, excludes waiting for next tick; twelve samples with original one-in-four native scroll cadence; controlled 18-object fixture, not campaign FPS',ram_kib=128,variant='DAC6',cases=results)
  args.output.write_text(json.dumps(result,indent=2),encoding='utf-8');print(json.dumps(result,indent=2),flush=True)
 finally:
  if r:r.s.close()
  p.terminate();p.wait(timeout=10);log.close()
if __name__=='__main__':main()
