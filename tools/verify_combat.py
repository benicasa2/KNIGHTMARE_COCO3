"""Real 6809 combat scenarios plus an independent indexed-pixel renderer."""
from pathlib import Path
import argparse,json,socket,subprocess,time,hashlib,struct
from verify_probe import RSP,symbols
from verify_controls_scroll import screen,val,word,call,keyboard_tick,enter_game,item_tilemap
from render_msx import png
ROOT=Path(__file__).resolve().parents[1];BASE=0x2700
META=json.loads((ROOT/'analysis/combat-assets.json').read_text())
def expected(r,sy):
 cam=word(r,sy,'camera_y');tilemap=item_tilemap(r,sy);tiles=(ROOT/f"assets/stage{val(r,sy,'campaign_stage')+1}-packed-tiles.bin").read_bytes();pixels=bytearray(256*192)
 for y in range(176):
  wy=cam+y
  for tx in range(32):
   t=tilemap[(wy//8)*32+tx];p=t*32+(wy&7)*4
   for k,b in enumerate(tiles[p:p+4]):pixels[y*256+tx*8+k*2:y*256+tx*8+k*2+2]=bytes([b>>4,b&15])
 hud=r.read(0xd000,2048)
 for i,b in enumerate(hud):pixels[176*256+i*2:176*256+i*2+2]=bytes([b>>4,b&15])
 def put(image,x,y):
  for yy,row in enumerate(image):
   for xx,c in enumerate(row):
    if c and 0<=x+xx<256 and 0<=y+yy<176:pixels[(y+yy)*256+x+xx]=c
 bs=val(r,sy,'boss_state');wait=val(r,sy,'boss_wait')
 if not val(r,sy,'campaign_stage') and bs in (1,2) and 4<=val(r,sy,'boss_y')<133 and not(bs==2 and wait&2):
  bx=val(r,sy,'boss_x')&254;by=val(r,sy,'boss_y');hp=val(r,sy,'boss_hp');f=(word(r,sy,'boss_clock')&16)//16
  for y in range(4):
   for x in range(40):pixels[(by-4+y)*256+bx+x]=15 if 20-x//2<=hp else 1
  bank=(ROOT/'assets/combat-bank.bin').read_bytes();p=META['boss_offsets'][f]-0xc000
  for y in range(40):
   for k,b in enumerate(bank[p+y*20:p+(y+1)*20]):pixels[(by+y)*256+bx+k*2:(by+y)*256+bx+k*2+2]=bytes([b>>4,b&15])
 if val(r,sy,'campaign_stage') and bs in (1,2) and 14<=val(r,sy,'boss_y')<145 and 11<=val(r,sy,'boss_x')<228 and not(bs==2 and wait&2):
  bx=val(r,sy,'boss_x')-11;by=val(r,sy,'boss_y')-10;hp=val(r,sy,'boss_hp');bar=2*hp//3
  for y in range(4):
   for x in range(40):pixels[(by-4+y)*256+(bx//2)*2+x]=15 if 20-x//2<=bar else 1
  color=15 if hp>20 else 10 if hp>10 else 7
  put([[color if c==15 else c for c in row] for row in META['indexed'][META['boss2_art']]['pixels']],bx,by)
 ent=r.read(BASE,19*32)
 for i in range(19):
  s=ent[i*32:(i+1)*32]
  if not s[0] or s[14]==255:continue
  desc=META['descriptors'][s[14]];im=META['indexed'][s[14]]
  x=s[2]+desc['dx'];y=s[4]+desc['dy']
  if x<0 or y<0 or y+desc['height']>176 or x+desc['span']*2>=256:continue
  sprite=im['pixels']
  if i==18 and val(r,sy,'token_family'):sprite=[[val(r,sy,'token_ink') if c==14 else c for c in row] for row in sprite]
  put(sprite,x,y)
 status=val(r,sy,'combat_status');inv=val(r,sy,'player_invul');tick=word(r,sy,'game_ticks')
 if status not in (1,2) and not(inv and tick&2):
  f=val(r,sy,'player_frame');b=(ROOT/'assets/player-walk-indexed.bin').read_bytes()[f*256:(f+1)*256]
  items=json.loads((ROOT/'analysis/items-assets.json').read_text());ability=val(r,sy,'ability_kind');px=val(r,sy,'player_x');py=val(r,sy,'player_y')
  if ability==1 and py>=9:put(items['shield_image'],px,py-9)
  if ability==2 and (word(r,sy,'ability_timer')>=180 or not tick&4):body=items['invisibility_images'][f]
  else:body=[b[y*16:y*16+16] for y in range(16)]
  put(body,px,py)
 return bytes(pixels[i]*16+pixels[i+1] for i in range(0,len(pixels),2))
def picture(r,sy,label):
 actual=screen(r,val(r,sy,'ring_head'));want=expected(r,sy)
 pal=[v&63 for v in r.read(0xffb0,16)];rgb=[((((v>>5)&1)*2+((v>>2)&1))*85,(((v>>4)&1)*2+((v>>1)&1))*85,(((v>>3)&1)*2+(v&1))*85) for v in pal]
 pix=bytearray()
 for b in actual:pix+=bytes([b>>4,b&15])
 png(ROOT/f'analysis/{label}.png',pix,palette=rgb)
 if actual!=want:
  dif=[i for i,(a,b) in enumerate(zip(actual,want)) if a!=b]
  (ROOT/f'analysis/{label}-diff.json').write_text(json.dumps(dict(count=len(dif),first=dif[:60],entities=r.read(BASE,576).hex(),state=r.read(sy['state_begin'],sy['state_end']-sy['state_begin']).hex()),indent=2))
  raise AssertionError(f'{label}: {len(dif)} framebuffer differences {dif[:10]}')
 return hashlib.sha256(actual).hexdigest()
def tick(r,sy,bits=0):
 call(r,sy,'apply_input');r.write(sy['input_bits'],bytes([bits]));call(r,sy,'game_return')
def slot(r,index,kind,x,y,art,hp=1,vx=0,vy=0):
 s=bytearray(32);s[0]=1;s[1]=kind;struct.pack_into('>HHhh',s,2,x*256,y*256,vx,vy);s[10]=hp;s[13]=100;s[14]=art;r.write(BASE+index*32,s)
def clear_slots(r,sy):
 # Mark inactive and allow the DUT to erase previously drawn objects first.
 for i in range(19):r.write(BASE+i*32,b'\0')
 r.write(sy['wave_left'],b'\0');r.write(sy['wave_wait'],b'\xff\xff')

def verify_audio(r,sy,name):
 # Restart, then trigger a real player shot and inspect its PCM DAC writes.
 tick(r,sy,64)
 call(r,sy,'apply_input');r.write(sy['input_bits'],b'\x10')
 address=0xff7b if name=='ORCH90' else 0xff20
 r.cmd(f'Z2,{address:x},1');samples=[];times=[]
 for i in range(128):
  r.go();samples.append(bytes.fromhex(r.cmd('g'))[1])
  if i in (0,127):times.append(int(bytes.fromhex(r.cmd('qRcmd,'+b'cycles'.hex())).decode().split()[0]))
  r.step()
 r.cmd(f'z2,{address:x},1');assert len(set(samples))>1
 pcm=(ROOT/'assets/sfx-shot.pcm').read_bytes()[:128]
 assert samples==[b if name=='ORCH90' else b&252 for b in pcm],'DAC data differs from original-effect PCM'
 actual_rate=127*14318180/(times[1]-times[0]);assert abs(actual_rate-3579545/912)<120
 # Let the short effect finish; silence must not keep consuming PCM samples.
 for _ in range(10):tick(r,sy)
 assert val(r,sy,'sfx_priority')==0
 ptr=word(r,sy,'sfx_ptr');clock=word(r,sy,'frames_played')
 for _ in range(4):tick(r,sy)
 assert word(r,sy,'sfx_ptr')==ptr and word(r,sy,'frames_played')>clock
 return dict(mode='event PCM, no background music',address=hex(address),sample_count=len(samples),levels=sorted(set(samples)),dac_matches_pcm=True,measured_sample_rate_hz=actual_rate,idle_playback_stopped_while_video_clock_continues=True)
def verify(exe,name):
 sy=symbols(name)
 with socket.socket() as s:s.bind(('127.0.0.1',0));port=s.getsockname()[1]
 log=(ROOT/f'analysis/{name}-combat-xroar.log').open('wb')
 args=[str(exe),'-machine','coco3','-ram','128','-no-bas','-extbas',str(exe.parent/'coco3.rom'),'-cart','rsdos','-cart-rom',str(exe.parent/'disk11.rom')]
 if name=='ORCH90':args+=['-cart','mpi','-mpi-slot','3','-mpi-load-cart','0=orch90','-mpi-load-cart','3=rsdos']
 args+=['-load-fd0',str(ROOT/f'bin/{name}.DSK'),'-load-fd1',str(ROOT/'bin/LEVELS.DSK'),'-type','RUN"START"\r','-no-ratelimit','-tv-input','rgb','-ui','null','-ao','null','-gdb','-gdb-port',str(port),'-no-disk-write-back']
 p=subprocess.Popen(args,stdout=log,stderr=log);r=None
 try:
  for _ in range(60):
   try:r=RSP(port);break
   except OSError:time.sleep(.1)
  assert r
  call(r,sy,'main_loop');r.cmd(f'P8={sy["start"]:04x}');call(r,sy,'main_loop')
  enter_game(r,sy)
  checks={};checks['initial']=picture(r,sy,f'{name}-combat-initial')
  # Regression: decode actual CoCo arrow/WASD matrix coordinates with combat ON.
  # The original faulty terrain predicate rejected every move on starting grass.
  assert val(r,sy,'combat_enabled')==1
  directions=[('up',{3:8},0,-2),('down',{4:8},0,2),('left',{5:8},-2,0),('right',{6:8},2,0),('W',{7:4},0,-2),('S',{3:4},0,2),('A',{1:1},-2,0),('D',{4:1},2,0)]
  positions=[]
  for key,cols,dx,dy in directions:
   x,y=val(r,sy,'player_x'),val(r,sy,'player_y')
   keyboard_tick(r,sy,cols,0)
   actual=(val(r,sy,'player_x'),val(r,sy,'player_y'))
   assert actual==(x+dx,y+dy),f'{key}: movement blocked on initial grass: {(x,y)} -> {actual}'
   positions.append(dict(key=key,before=[x,y],after=list(actual)))
  checks['arrows_and_wasd_on_starting_grass_with_combat']=positions
  for _ in range(8):tick(r,sy,16)
  assert word(r,sy,'shots_fired')>=2
  assert sum(bool(r.read(BASE+i*32,1)[0]) for i in range(7,10))<=3
  checks['held_fire_three_slots']=True
  checks['wave_and_arrows']=picture(r,sy,f'{name}-combat-waves')
  clear_slots(r,sy);tick(r,sy)
  # Moving a foot into a solid original map tile must reject the whole move.
  m=(ROOT/'assets/stage1-combat-map.bin').read_bytes();cam=word(r,sy,'camera_y')
  def passable(x,y):return m[((cam+y)//8)*32+x//8] not in (*range(0x43,0x80),*range(0xa3,0xe0))
  candidate=next((x,y) for y in range(8,150) for x in range(8,230) if all(passable(x+d,y+13) for d in (4,11)) and not all(passable(x+2+d,y+13) for d in (4,11)))
  x,y=candidate;r.write(sy['player_x'],bytes([x]));r.write(sy['player_y'],bytes([y]));r.write(sy['scroll_clock'],b'\0')
  tick(r,sy,8);assert (val(r,sy,'player_x'),val(r,sy,'player_y'))==candidate
  checks['solid_terrain_blocks_movement']=True
  r.write(sy['player_x'],b'\x78');r.write(sy['player_y'],b'\x88');r.write(sy['player_invul'],b'\xff')
  # Overlap multiple saved backgrounds with each other and with the player.
  slot(r,0,14,128,141,META['enemy_ids']['14'][0])
  slot(r,1,1,130,142,META['enemy_ids']['1'][0])
  slot(r,10,8,131,142,META['ids']['bullet'])
  tick(r,sy);checks['overlapping_sprites']=picture(r,sy,f'{name}-combat-overlap')
  clear_slots(r,sy);tick(r,sy);checks['overlap_background_restored']=picture(r,sy,f'{name}-combat-erased')
  # Deterministic swept-hit test with original type-1 HP and score data.
  r.write(sy['player_invul'],b'\xff')
  slot(r,0,14,128,80,META['enemy_ids']['14'][0])
  slot(r,7,0,120,100,META['ids']['arrow'],vy=-4096)
  old=word(r,sy,'score');tick(r,sy)
  assert r.read(BASE,1)==b'\x02' and word(r,sy,'score')==old+10
  assert r.read(BASE+7*32,1)==b'\0'
  assert val(r,sy,'sfx_priority')==3
  checks['enemy_explosion_sound_event']=True
  checks['arrow_hit_damage_score']=True
  checks['explosion']=picture(r,sy,f'{name}-combat-explosion')
  for _ in range(18):tick(r,sy)
  assert r.read(BASE,1)==b'\0';checks['explosion_lifetime']=True
  clear_slots(r,sy);tick(r,sy)
  # Three hits are required for type 9.
  slot(r,0,9,128,80,META['enemy_ids']['9'][0],hp=3)
  for expected_hp in [2,1,0]:
   slot(r,7,0,120,100,META['ids']['arrow'],vy=-4096);tick(r,sy)
   assert r.read(BASE+10,1)[0]==expected_hp
  checks['three_hit_enemy']=True
  # Contact costs one life, freezes the death interval and then respawns.
  clear_slots(r,sy);tick(r,sy);r.write(sy['player_invul'],b'\0')
  px=val(r,sy,'player_x');py=val(r,sy,'player_y');slot(r,0,14,px+8,py+8,META['enemy_ids']['14'][0])
  lives=val(r,sy,'player_lives');tick(r,sy)
  assert val(r,sy,'combat_status')==1 and val(r,sy,'player_lives')==lives-1
  assert val(r,sy,'sfx_priority')==4
  checks['player_death_sound_event']=True
  checks['contact_and_life_loss']=True
  for _ in range(48):tick(r,sy)
  assert val(r,sy,'combat_status')==0 and val(r,sy,'player_invul')==0
  checks['checkpoint_respawn_without_artificial_immunity']=True
  clear_slots(r,sy);tick(r,sy)
  # Hostile shot collision and game over.
  r.write(sy['player_invul'],b'\0');r.write(sy['player_lives'],b'\x01')
  slot(r,10,8,val(r,sy,'player_x')+8,val(r,sy,'player_y')+8,META['ids']['bullet'])
  tick(r,sy);assert val(r,sy,'combat_status')==1
  for _ in range(48):tick(r,sy)
  assert val(r,sy,'scene_mode')==1 and r.read(0xffb0,1)==r.read(0xffb4,1)
  checks['hostile_shot_and_return_to_konami_intro']=True
  tick(r,sy,64);assert val(r,sy,'combat_status')==0 and val(r,sy,'player_lives')==3
  checks['restart']=picture(r,sy,f'{name}-combat-restarted')
  # Establish a valid arena by skipping native camera through its final two rows;
  # rebuild complete background with the independent data only for this setup.
  clear_slots(r,sy);tick(r,sy)
  r.write(sy['camera_y'],b'\0\x01');r.write(sy['scroll_clock'],b'\x01');r.write(sy['player_invul'],b'\xff')
  tick(r,sy)
  assert val(r,sy,'boss_state')==1 and val(r,sy,'boss_hp')==20
  # The checkpoint jump above invalidates old background. Reinitialize its visible
  # raster from map data, then let native sprites save/restore from that reference.
  r.write(sy['sprite_valid'],b'\0');r.write(sy['boss_drawn'],b'\0')
  for i in range(19):r.write(BASE+i*32+17,b'\0')
  saved_status=val(r,sy,'combat_status');r.write(sy['combat_status'],b'\x02');r.write(sy['boss_state'],b'\0')
  raw=expected(r,sy);head=val(r,sy,'ring_head');pos=head*128
  for k in range(0,len(raw),128):
   addr=pos+k;r.write(0xffa4,bytes([addr//8192]));
   for j in range(0,128,128):r.write(0x8000+addr%8192+j,raw[k+j:k+j+128])
  r.write(sy['combat_status'],bytes([saved_status]));r.write(sy['boss_state'],b'\x01')
  r.write(sy['boss_phase'],b'\0');r.write(sy['boss_y'],b'\x20') # Skip entry only in this arena fixture.
  tick(r,sy);checks['boss_arena']=picture(r,sy,f'{name}-combat-boss')
  for _ in range(90):tick(r,sy)
  assert any(r.read(BASE+i*32,2)==bytes([1,12]) for i in range(7))
  assert any(r.read(BASE+i*32,1)==b'\x01' for i in range(10,18))
  checks['boss_satellite_and_hostile_fire']=picture(r,sy,f'{name}-combat-boss-active')
  bx=val(r,sy,'boss_x');by=val(r,sy,'boss_y')
  slot(r,7,10,bx+10,by+36,13,vy=-4096);tick(r,sy)
  assert val(r,sy,'boss_hp')==18
  checks['strong_weapon_deals_two_boss_damage']=True
  r.write(sy['boss_hp'],b'\x14') # Reset this fixture before the 20 basic hits.
  # Controlled collision setup supplies 20 distinct original-strength hits.
  for n in range(20):
   bx=val(r,sy,'boss_x');by=val(r,sy,'boss_y')
   slot(r,7,0,bx+10,by+36,META['ids']['arrow'],vy=-4096)
   tick(r,sy);assert val(r,sy,'boss_hp')==19-n
  assert val(r,sy,'boss_state')==2
  assert val(r,sy,'sfx_priority')==5
  checks['boss_defeat_sound_event']=True
  for _ in range(104):tick(r,sy)
  assert val(r,sy,'combat_status')==3 and val(r,sy,'boss_state')==3
  checks['twenty_hit_boss_defeat_and_clear']=True
  checks['stage_clear']=picture(r,sy,f'{name}-combat-clear')
  checks['event_sfx_without_music']=verify_audio(r,sy,name)
  result=dict(variant=name,ram_kib=128,status='native combat functional scenarios passed',test_scope='controlled emulator scenarios, including injected entity positions and boss hits; not an unassisted full playthrough',checks=checks)
  (ROOT/f'analysis/{name}-combat-verification.json').write_text(json.dumps(result,indent=2),encoding='utf-8');print(json.dumps(result,indent=2),flush=True)
 except Exception:
  if r:
   try:
    r.s.sendall(b'\x03');r.recv();(ROOT/f'analysis/{name}-combat-diagnostic.json').write_text(json.dumps(dict(regs=r.cmd('g'),state=r.read(sy['state_begin'],sy['state_end']-sy['state_begin']).hex(),entities=r.read(BASE,576).hex()),indent=2))
   except Exception:pass
  raise
 finally:
  if r:r.s.close()
  p.terminate();p.wait(timeout=10);log.close()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--xroar',required=True,type=Path);ap.add_argument('--variant',choices=['DAC6','ORCH90']);args=ap.parse_args()
 for n in ([args.variant] if args.variant else ['DAC6','ORCH90']):verify(args.xroar,n)
if __name__=='__main__':main()
