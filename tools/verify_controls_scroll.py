"""Execute the real 6809 renderer, input boundaries and audio in XRoar/128 KiB.
The independent renderer computes expected images directly from map/tile/sprite data.
Synthetic PIA read values are explicitly injected at LDA boundaries for input tests.
"""
from pathlib import Path
import argparse,socket,subprocess,time,json,hashlib,struct
from verify_probe import RSP,symbols
from render_msx import png
ROOT=Path(__file__).resolve().parents[1]
def val(r,sy,n):return r.read(sy[n],1)[0]
def word(r,sy,n):return int.from_bytes(r.read(sy[n],2),'big')
def screen(r,head):
 out=bytearray();p=head*128;end=p+24576
 while p<end:
  bank=p//8192;off=p%8192;n=min(8192-off,end-p)
  r.write(0xffa4,bytes([bank]));out+=r.read(0x8000+off,n);p+=n
 return bytes(out)
def item_tilemap(r,sy):
 stage=val(r,sy,'campaign_stage')
 m=bytearray((ROOT/('assets/stage1-combat-map.bin' if stage==0 else f'assets/stages/stage{stage+1}-map.bin')).read_bytes())
 meta=json.loads((ROOT/'analysis/items-assets.json').read_text());objects=meta['all_stage_objects'][stage]
 states=r.read(0x2980,len(objects)*2);label='fixed_item_table' if stage==0 else 'phase2_fixed_items';records=r.read(sy[label],len(objects)*6)
 for i,o in enumerate(objects):
  state=states[i*2];p=records[i*6:i*6+6];wy=int.from_bytes(p[:2],'big');col=p[2]//8;kind=p[3]&7
  if not state or state>=5:continue
  if state==4:row=wy//8-2;col-=1;n=4;tiles=meta['bridge_tiles']
  else:row=wy//8;n=2;k=7 if state==1 else 6 if state==3 else kind;tiles=meta['item_tiles'][k]
  for y in range(n):
   for x in range(n):
    if 0<=row+y<244 and 0<=col+x<32:m[(row+y)*32+col+x]=tiles[y*n+x]
 return m

def enter_game(r,sy):
 call(r,sy,'apply_input');r.write(sy['input_bits'],b'\x40');call(r,sy,'frame_complete')

def expected(r,sy):
 if val(r,sy,'combat_enabled'):
  from verify_combat import expected as combat_expected
  return combat_expected(r,sy)
 cam=word(r,sy,'camera_y');sx=val(r,sy,'player_x');sypos=val(r,sy,'player_y');f=val(r,sy,'player_frame')
 tilemap=item_tilemap(r,sy);tiles=(ROOT/f"assets/stage{val(r,sy,'campaign_stage')+1}-packed-tiles.bin").read_bytes();out=bytearray()
 for y in range(176):
  wy=cam+y
  for x in range(32):
   tile=tilemap[(wy//8)*32+x];base=tile*32+(wy&7)*4;out+=tiles[base:base+4]
 out+=r.read(0xd000,2048)
 player=(ROOT/'assets/player-walk-indexed.bin').read_bytes()[f*256:(f+1)*256]
 for y in range(16):
  for x in range(16):
   c=player[y*16+x]
   if c:
    dx=sx+x;p=(sypos+y)*128+dx//2
    out[p]=(out[p]&15)|(c<<4) if dx%2==0 else (out[p]&240)|c
 return bytes(out)
def screenshot(r,sy,label):
 actual=screen(r,val(r,sy,'ring_head'));want=expected(r,sy)
 if actual!=want:
  dif=[i for i,(a,b) in enumerate(zip(actual,want)) if a!=b]
  (ROOT/f'analysis/{label}-differences.json').write_text(json.dumps(dict(count=len(dif),first=dif[:40],state=r.read(sy['state_begin'],sy['state_end']-sy['state_begin']).hex()),indent=2))
  raise AssertionError(f'{label}: {len(dif)} framebuffer differences, first {dif[:8]}')
 colors=[c&63 for c in r.read(0xffb0,16)]
 rgb=[((((v>>5)&1)*2+((v>>2)&1))*85,(((v>>4)&1)*2+((v>>1)&1))*85,(((v>>3)&1)*2+(v&1))*85) for v in colors]
 pixels=bytearray()
 for b in actual:pixels+=bytes([b>>4,b&15])
 png(ROOT/f'analysis/{label}.png',pixels,palette=rgb)
 return hashlib.sha256(actual).hexdigest()
def inject_a(r,value):
 regs=bytes.fromhex(r.cmd('g'))
 assert r.cmd(f'P1={value:02x}')=='OK'
 cc=(regs[0]&~14)|(8 if value&128 else 0)|(4 if value==0 else 0)
 assert r.cmd(f'P0={cc:02x}')=='OK'
 # Skip the three-byte LDA $FF00 whose external signal is being supplied.
 pc=int.from_bytes(regs[-2:],'big')
 assert r.cmd(f'P8={pc+3:04x}')=='OK'
def call(r,sy,name):
 # Entry is reached by normal game execution, never by replacing its caller stack.
 r.bp(sy[name]);r.go();r.unbp(sy[name])
def finish_tick(r,sy):call(r,sy,'game_return')
def tick(r,sy,bits):
 call(r,sy,'apply_input');r.write(sy['input_bits'],bytes([bits]));finish_tick(r,sy)
def keyboard_tick(r,sy,cols,mode=None):
 call(r,sy,'poll_input')
 if mode is not None:r.write(sy['input_mode'],bytes([mode]))
 r.bp(sy['keyboard_read'])
 for c in range(8):
  r.go();assert val(r,sy,'key_column')==(255^(1<<c))
  inject_a(r,255^cols.get(c,0))
 r.unbp(sy['keyboard_read'])
 finish_tick(r,sy)
def joystick_tick(r,sy,mode,xlow,xhigh,ylow,yhigh):
 call(r,sy,'read_joystick');r.write(sy['input_mode'],bytes([mode]));r.write(sy['input_bits'],b'\0')
 for n,value in [('joystick_x_low',xlow),('joystick_x_high',xhigh),('joystick_y_low',ylow),('joystick_y_high',yhigh)]:
  if value is None:continue
  call(r,sy,n)
  assert r.read(0xff03,1)[0]&8==(8 if mode==2 else 0)
  assert r.read(0xff01,1)[0]&8==(8 if '_y_' in n else 0)
  assert r.read(0xff20,1)[0]&252==(96 if n.endswith('_low') else 160)
  inject_a(r,value)
 call(r,sy,'apply_input')
 bits=val(r,sy,'input_bits');finish_tick(r,sy);return bits
def verify(exe,name):
 sy=symbols(name)
 with socket.socket() as s:s.bind(('127.0.0.1',0));port=s.getsockname()[1]
 log=(ROOT/f'analysis/{name}-controls-xroar.log').open('wb')
 args=[str(exe),'-machine','coco3','-ram','128','-no-bas','-extbas',str(exe.parent/'coco3.rom'),'-cart','rsdos','-cart-rom',str(exe.parent/'disk11.rom'),'-load-fd0',str(ROOT/f'bin/{name}.DSK'),'-load-fd1',str(ROOT/'bin/LEVELS.DSK'),'-type','RUN"START"\r','-no-ratelimit','-tv-input','rgb','-ui','null','-ao','null','-gdb','-gdb-port',str(port),'-no-disk-write-back']
 if name=='ORCH90':
  ix=args.index('-load-fd0');args[ix:ix]=['-cart','mpi','-mpi-slot','3','-mpi-load-cart','0=orch90','-mpi-load-cart','3=rsdos']
 p=subprocess.Popen(args,stdout=log,stderr=log);r=None
 try:
  for _ in range(60):
   try:r=RSP(port);break
   except OSError:time.sleep(.1)
  assert r
  call(r,sy,'main_loop')
  assert r.cmd(f'P8={sy["start"]:04x}')=='OK'
  call(r,sy,'main_loop')
  enter_game(r,sy)
  r.write(sy['combat_enabled'],b'\0') # Isolate the retained scrolling/control path.
  checks={};checks['initial_frame_sha256']=screenshot(r,sy,f'{name}-interactive-initial')
  x=val(r,sy,'player_x');y=val(r,sy,'player_y')
  keyboard_tick(r,sy,{7:4,1:1},0) # W + A, real column decoding.
  assert val(r,sy,'input_bits')&15==5
  assert (val(r,sy,'player_x'),val(r,sy,'player_y'))==(x-2,y-2)
  checks['keyboard_matrix_diagonal']=True
  keyboard_tick(r,sy,{6:8,4:8},0) # right + down arrows.
  assert (val(r,sy,'player_x'),val(r,sy,'player_y'))==(x,y)
  keyboard_tick(r,sy,{7:8},0);assert val(r,sy,'input_bits')&16
  checks['space_fire_input']=True
  r.write(sy['player_x'],bytes([237]));tick(r,sy,8)
  assert val(r,sy,'player_x')==239
  checks['odd_pixel_and_right_boundary']=screenshot(r,sy,f'{name}-interactive-odd')
  tick(r,sy,8);assert val(r,sy,'player_x')==239
  r.write(sy['player_x'],b'\0');r.write(sy['player_y'],b'\0');tick(r,sy,5)
  assert val(r,sy,'player_x')==val(r,sy,'player_y')==0
  checks['top_left_clamp']=True
  # Exercise mode keys through the keyboard decoder, not a RAM-only selector.
  keyboard_tick(r,sy,{2:2},0);assert val(r,sy,'input_mode')==1
  bits=joystick_tick(r,sy,1,255,255,127,None);assert bits&15==9
  checks['right_joystick_comparator_diagonal']=True
  keyboard_tick(r,sy,{4:2},0);assert val(r,sy,'input_mode')==2
  bits=joystick_tick(r,sy,2,127,None,255,255);assert bits&15==6
  checks['left_joystick_comparator_diagonal']=True
  bits=joystick_tick(r,sy,2,255,127,255,127);assert bits&15==0
  checks['joystick_dead_zone']=True
  keyboard_tick(r,sy,{3:2},0);assert val(r,sy,'input_mode')==0
  checks['keyboard_mode_selection']=True
  tick(r,sy,0);tick(r,sy,32);assert val(r,sy,'paused')==1
  cam=word(r,sy,'camera_y');px=val(r,sy,'player_x')
  for _ in range(4):tick(r,sy,8)
  assert word(r,sy,'camera_y')==cam and val(r,sy,'player_x')==px
  checks['pause']=True
  tick(r,sy,32);assert val(r,sy,'paused')==0
  checks['moved_frame_sha256']=screenshot(r,sy,f'{name}-interactive-moved')
  # One break after a full 192-line hardware ring rollover.
  call(r,sy,'ring_wrapped');call(r,sy,'frame_complete')
  assert val(r,sy,'ring_head')==0 and val(r,sy,'ring_wraps')>=1
  assert word(r,sy,'scroll_steps')==192
  checks['scroll_192_pixels_and_ring_wrap']=screenshot(r,sy,f'{name}-interactive-wrap')
  assert word(r,sy,'camera_y')==1776-192
  call(r,sy,'camera_stopped')
  assert word(r,sy,'camera_y')==0 and word(r,sy,'scroll_steps')==1776
  checks['entire_stage_scroll_1776_pixels']=screenshot(r,sy,f'{name}-interactive-stage-end')
  for _ in range(3):tick(r,sy,0)
  assert word(r,sy,'camera_y')==0 and word(r,sy,'scroll_steps')==1776
  checks['camera_stops_at_map_boundary']=True
  r.bp(sy['apply_input']);r.go();r.unbp(sy['apply_input']);r.write(sy['input_bits'],b'\x40')
  call(r,sy,'frame_complete')
  assert word(r,sy,'camera_y')==1776 and word(r,sy,'scroll_steps')==0
  checks['restart']=screenshot(r,sy,f'{name}-interactive-restart')
  from verify_combat import verify_audio
  checks['event_sfx_without_music']=verify_audio(r,sy,name)
  result=dict(status='controls and scrolling regression passed with combat disabled',variant=name,ram_kib=128,orchestra_cartridge_configured=name=='ORCH90',input_test_method='synthetic external PIA input values at actual 6809 LDA boundaries; real matrix/mux decoder and movement execute in emulator',checks=checks)
  (ROOT/f'analysis/{name}-controls-verification.json').write_text(json.dumps(result,indent=2),encoding='utf-8');print(json.dumps(result,indent=2),flush=True)
 except Exception:
  if r:
   try:
    r.s.sendall(b'\x03');r.recv()
    diagnostic=dict(registers=r.cmd('g'),state=r.read(sy['state_begin'],sy['state_end']-sy['state_begin']).hex(),screen_text=r.read(0x400,512).hex())
    (ROOT/f'analysis/{name}-controls-diagnostic.json').write_text(json.dumps(diagnostic,indent=2))
   except Exception:pass
  raise
 finally:
  if r:r.s.close()
  p.terminate();p.wait(timeout=10);log.close()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--xroar',type=Path,required=True);ap.add_argument('--variant',choices=['DAC6','ORCH90']);args=ap.parse_args()
 for name in ([args.variant] if args.variant else ['DAC6','ORCH90']):verify(args.xroar,name)
if __name__=='__main__':main()
