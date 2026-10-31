"""Check that game logic leaves the displayed frame intact until commit.
This checks framebuffer writes, not captured video or physical display flicker.
"""
from pathlib import Path
import argparse,json,hashlib
from verify_ai import boot
from verify_probe import symbols
from verify_controls_scroll import call,enter_game,val,screen
from verify_combat import expected,slot,META
from verify_items import fixed_fixture,ITEMS
ROOT=Path(__file__).resolve().parents[1]
def verify(exe,name):
 p,log,r=boot(exe,name);sy=symbols(name)
 try:
  call(r,sy,'main_loop');enter_game(r,sy)
  r.write(sy['test_invincible'],bytes([1]));r.write(sy['token_index'],bytes([255]))
  slot(r,0,14,40,70,META['enemy_ids']['14'][0])
  checks=[]
  def update(label,bits,queued=False):
   call(r,sy,'apply_input');r.write(sy['input_bits'],bytes([bits]))
   head=val(r,sy,'ring_head');old=screen(r,head)
   call(r,sy,'render_begin')
   assert screen(r,head)==old,label+': logic changed the displayed frame'
   assert val(r,sy,'render_deferred')==0
   if queued:assert val(r,sy,'render_refresh_pending')==1,label+': fixed refresh not queued'
   call(r,sy,'combat_frame_complete')
   assert val(r,sy,'render_refresh_pending')==0
   actual=screen(r,val(r,sy,'ring_head'))
   want=expected(r,sy)
   if actual!=want:
    dif=[i for i,(a,b) in enumerate(zip(actual,want)) if a!=b]
    (ROOT/f'analysis/{name}-pipeline-diff.json').write_text(json.dumps(dict(label=label,count=len(dif),first=[[i,i//128,(i%128)*2,actual[i],want[i]] for i in dif[:30]],head=val(r,sy,'ring_head'),camera=r.read(sy['camera_y'],2).hex(),player=list(r.read(sy['player_x'],2))),indent=2))
    raise AssertionError(label+': final raster differs')
   checks.append(dict(case=label,display_unchanged_during_logic=True,final_raster_sha256=hashlib.sha256(actual).hexdigest(),fixed_refresh_deferred=queued))
  for tick in range(8):update('movement-shots-scroll-'+str(tick),(8 if tick<12 else 4)|16)
  for kind in [0,2,3]:
   index=next(i for i,x in enumerate(ITEMS['objects']) if x['kind']==kind and not x['passage'])
   fixed_fixture(r,sy,index,state=2)
   update('fixed-reward-'+str(kind),0,queued=True)
  report=dict(status='deferred rendering pipeline passed',variant=name,ram_kib=128,tested_disk_sha256=hashlib.sha256((ROOT/f'bin/{name}.DSK').read_bytes()).hexdigest(),frames_checked=len(checks),checks=checks,scope='controlled movement, shots, scroll and item refresh; byte comparisons before commit and after final composition; not captured video or physical hardware')
  (ROOT/f'analysis/{name}-flicker-verification.json').write_text(json.dumps(report,indent=2),encoding='utf-8');print(name+': 11 frames preserve the previous raster during logic and match final composition',flush=True)
 finally:
  r.s.close();p.terminate();p.wait(timeout=10);log.close()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--xroar',type=Path,required=True);a=ap.parse_args()
 for name in ['DAC6','ORCH90']:verify(a.xroar,name)
if __name__=='__main__':main()
