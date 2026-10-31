"""Native intro/inventory scenarios in XRoar, plus independent pixel checks."""
from pathlib import Path
import argparse,json,socket,subprocess,time,hashlib,struct
from verify_probe import RSP,symbols
from verify_controls_scroll import call,val,word,screen,enter_game
from verify_combat import tick,slot,clear_slots,picture,expected,ROOT,META,BASE
from render_msx import png
ITEMS=json.loads((ROOT/'analysis/items-assets.json').read_text())

def scene_tick(r,sy,bits=0):
 call(r,sy,'apply_input');r.write(sy['input_bits'],bytes([bits]));call(r,sy,'game_return')

def scene_image(r,sy,name,want):
 actual=screen(r,val(r,sy,'ring_head'));assert actual==want,f'{name}: intro raster differs'
 pix=bytes(c for b in actual for c in (b>>4,b&15))
 pal=[v&63 for v in r.read(0xffb0,16)];rgb=[((((v>>5)&1)*2+((v>>2)&1))*85,(((v>>4)&1)*2+((v>>1)&1))*85,(((v>>3)&1)*2+(v&1))*85) for v in pal]
 png(ROOT/f'analysis/{name}.png',pix,palette=rgb)
 return hashlib.sha256(actual).hexdigest()

def reset_fixture(r,sy):
 clear_slots(r,sy);r.write(sy['token_index'],b'\xff');r.write(sy['player_invul'],b'\xff');tick(r,sy)

def token(r,sy,family,selection,x=120,y=135):
 # Native token moves/oscillates after this controlled placement.
 r.write(sy['token_family'],bytes([family]));r.write(sy['token_selection'],bytes([selection]));r.write(sy['token_center'],bytes([x]));r.write(sy['token_angle'],b'\0')
 slot(r,18,0,x,y,ITEMS['token_power_art'][selection] if family else ITEMS['token_weapon_art']['0'])

def verify(exe,name):
 sy=symbols(name)
 with socket.socket() as s:s.bind(('127.0.0.1',0));port=s.getsockname()[1]
 log=(ROOT/f'analysis/{name}-items-xroar.log').open('wb')
 args=[str(exe),'-machine','coco3','-ram','128','-no-bas','-extbas',str(exe.parent/'coco3.rom'),'-cart','rsdos','-cart-rom',str(exe.parent/'disk11.rom')]
 if name=='ORCH90':args+=['-cart','mpi','-mpi-slot','3','-mpi-load-cart','0=orch90','-mpi-load-cart','3=rsdos']
 args+=['-load-fd0',str(ROOT/f'bin/{name}.DSK'),'-load-fd1',str(ROOT/'bin/LEVELS.DSK'),'-type','RUN"START"\r','-no-ratelimit','-ui','null','-ao','null','-tv-input','rgb','-gdb','-gdb-port',str(port),'-no-disk-write-back']
 p=subprocess.Popen(args,stdout=log,stderr=log);r=None
 try:
  for _ in range(60):
   try:r=RSP(port);break
   except OSError:time.sleep(.1)
  assert r
  call(r,sy,'main_loop');r.cmd(f'P8={sy["start"]:04x}');call(r,sy,'main_loop')
  checks={};assert val(r,sy,'scene_mode')==1
  assert r.read(0xffb0,1)==r.read(0xffb4,1)
  checks['konami_blue_background_from_original_r7_e4']=True
  for _ in range(14):scene_tick(r,sy)
  assert val(r,sy,'intro_y')==56
  want=bytearray(24576);logo=(ROOT/'assets/intro-logo.4bpp').read_bytes()
  for y in range(24):want[(56+y)*128+40:(56+y)*128+88]=logo[y*48:(y+1)*48]
  checks['konami_fourteen_steps']=scene_image(r,sy,f'{name}-intro-konami',want)
  for _ in range(66):scene_tick(r,sy)
  assert val(r,sy,'scene_mode')==2
  assert r.read(0xffb0,1)==b'\0'
  checks['title_restores_original_black_background']=True
  checks['original_title_raster']=scene_image(r,sy,f'{name}-intro-title',(ROOT/'assets/title.4bpp').read_bytes())
  scene_tick(r,sy,16);assert val(r,sy,'scene_mode')==0 and val(r,sy,'player_lives')==3
  checks['space_starts_game']=True
  picture(r,sy,f'{name}-items-game-start')
  reset_fixture(r,sy)
  # Exercise real scrolling from the initial map to the first scripted token.
  r.write(sy['token_index'],b'\0')
  call(r,sy,'apply_input');r.write(sy['input_bits'],b'\0')
  r.bp(sy['token_select_art']);r.go();r.unbp(sy['token_select_art']);call(r,sy,'combat_frame_complete')
  assert val(r,sy,'token_index')==1 and val(r,sy,'token_family')==1
  assert word(r,sy,'camera_y')<=ITEMS['tokens'][0][0]['world_y']
  for _ in range(60):tick(r,sy)
  checks['first_token_scripted_spawn']=picture(r,sy,f'{name}-items-first-token')
  # Isolate token interactions; each fixture lets the native update collect it.
  reset_fixture(r,sy)
  for selection,weapon in [(3,2),(4,8),(5,6),(6,4),(7,10)]:
   r.write(sy['weapon_kind'],b'\0');token(r,sy,0,selection)
   tick(r,sy);assert val(r,sy,'weapon_kind')==weapon and r.read(BASE+18*32,1)==b'\0',(selection,val(r,sy,'weapon_kind'))
  checks['five_weapon_rewards']=True
  token(r,sy,0,6);tick(r,sy);assert val(r,sy,'weapon_kind')==4
  token(r,sy,0,6);tick(r,sy);assert val(r,sy,'weapon_kind')==5
  checks['repeat_weapon_upgrades_odd_form']=True
  token(r,sy,0,6);tick(r,sy);assert val(r,sy,'weapon_kind')==5
  for selection in range(3):
   old=word(r,sy,'score');token(r,sy,0,selection);tick(r,sy);assert word(r,sy,'score')==old+1000
  checks['blank_tokens_give_1000_points']=True
  for _ in range(6):token(r,sy,1,3);tick(r,sy)
  assert val(r,sy,'speed_level')==4;checks['speed_level_caps_at_four']=True
  # Verify fractional movement speed from original pairs without scenery blocking.
  r.write(sy['player_x'],b'\x78');r.write(sy['player_y'],b'\x88');r.write(sy['player_fx'],b'\0');r.write(sy['player_fy'],b'\0')
  x=val(r,sy,'player_x');tick(r,sy,8);assert val(r,sy,'player_x')==x+4
  checks['original_fractional_speed_table']=True
  r.write(sy['player_x'],b'\x78');r.write(sy['player_fx'],b'\0')
  token(r,sy,1,4);tick(r,sy);assert val(r,sy,'ability_kind')==1 and val(r,sy,'shield_hits')==30
  r.write(sy['player_invul'],b'\0');lives=val(r,sy,'player_lives')
  slot(r,10,8,128,130,META['ids']['bullet']);tick(r,sy)
  assert val(r,sy,'player_lives')==lives and val(r,sy,'shield_hits')==26 and r.read(BASE+10*32,1)==b'\0'
  checks['shield_stops_frontal_projectile']=True
  token(r,sy,1,3,120,120);tick(r,sy)
  checks['shield_original_art_overlapping_token']=picture(r,sy,f'{name}-items-shield')
  clear_slots(r,sy);tick(r,sy)
  token(r,sy,1,5);tick(r,sy);assert val(r,sy,'ability_kind')==2 and word(r,sy,'ability_timer')==450
  slot(r,0,14,128,144,META['enemy_ids']['14'][0]);slot(r,10,8,128,144,META['ids']['bullet']);tick(r,sy)
  assert val(r,sy,'player_lives')==lives
  checks['invisibility_contact_and_bullets']=True
  checks['invisibility_raster']=picture(r,sy,f'{name}-items-invisibility')
  clear_slots(r,sy);tick(r,sy);token(r,sy,1,6);tick(r,sy)
  assert val(r,sy,'ability_kind')==3 and word(r,sy,'ability_timer')==450
  slot(r,0,14,128,144,META['enemy_ids']['14'][0]);tick(r,sy,16)
  assert r.read(BASE,1)==b'\x02' and val(r,sy,'player_lives')==lives
  assert not any(r.read(BASE+i*32,1)==b'\x01' for i in range(7,10))
  checks['invincibility_kills_by_contact_and_disables_fire']=True
  r.write(sy['ability_timer'],b'\0\1');tick(r,sy);assert val(r,sy,'ability_kind')==1 and val(r,sy,'shield_hits')==26
  checks['timed_ability_restores_existing_shield']=True
  r.write(sy['shield_hits'],b'\0');r.write(sy['ability_kind'],b'\x02');r.write(sy['ability_timer'],b'\0\1');tick(r,sy);assert val(r,sy,'ability_kind')==0
  checks['timed_ability_expires_without_shield']=True
  reset_fixture(r,sy)
  # A projectile cycles the token and is consumed; all four flash colors appear.
  token(r,sy,1,0,150,80);slot(r,7,0,150,100,META['ids']['arrow'],vy=-4096);tick(r,sy)
  assert val(r,sy,'token_selection')==1 and r.read(BASE+7*32,1)==b'\0'
  colors=set();xs=set()
  for _ in range(8):
   before_y=int.from_bytes(r.read(BASE+18*32+4,2),'big');before_angle=val(r,sy,'token_angle')
   tick(r,sy);colors.add(val(r,sy,'token_ink'));xs.add(r.read(BASE+18*32+2,1)[0])
   assert int.from_bytes(r.read(BASE+18*32+4,2),'big')-before_y==159
   assert val(r,sy,'token_angle')==(before_angle+64)&255
  assert colors==set(ITEMS['token_color_cycle'])
  assert xs=={118,150,181};checks['token_original_30_hz_motion']=True
  checks['shoot_to_cycle_and_original_flash_colors']=picture(r,sy,f'{name}-items-token')
  # Original fixed-object HP and tile transitions at the first visible block.
  reset_fixture(r,sy);r.write(sy['ability_kind'],b'\0');r.write(sy['speed_level'],b'\0')
  fixed=ITEMS['objects'][0];wy=fixed['world_y'];r.write(sy['camera_y'],(wy-80).to_bytes(2,'big'))
  # Rebuild the whole reference background only after this explicit checkpoint jump.
  rebuild_background(r,sy)
  for n in range(fixed['hp']):
   slot(r,7,0,fixed['x'],100,META['ids']['arrow'],vy=-4096);tick(r,sy)
  assert r.read(0x2980,1)==b'\x02' and r.read(0x2981,1)==b'\0'
  checks['fixed_block_original_hp_and_reward']=picture(r,sy,f'{name}-items-fixed-reward')
  r.write(sy['player_x'],bytes([fixed['x']]));r.write(sy['player_y'],bytes([wy-word(r,sy,'camera_y')]))
  old=word(r,sy,'score');tick(r,sy);assert word(r,sy,'score')==old+500 and r.read(0x2980,1)==b'\x03'
  checks['fixed_500_points_and_spent_tile']=picture(r,sy,f'{name}-items-fixed-spent')
  # Isolate each fixed reward at its original position, then let native contact
  # apply it. This includes rewards that are normally hidden behind blocks.
  for kind in [0,2,3]:
   obj_index=next(i for i,o in enumerate(ITEMS['objects']) if o['kind']==kind and not o['passage'])
   obj=ITEMS['objects'][obj_index];fixed_fixture(r,sy,obj_index,state=2)
   lives=val(r,sy,'player_lives');score=word(r,sy,'score')
   if kind==2:
    for i in range(3):slot(r,i,14,32+i*32,50,META['enemy_ids']['14'][0])
    slot(r,10,8,40,60,META['ids']['bullet'])
   tick(r,sy)
   assert r.read(0x2980+obj_index*2,1)==b'\x03'
   if kind==0:assert val(r,sy,'player_lives')==lives+1;checks['extra_life_reward']=True
   if kind==2:
    assert all(r.read(BASE+i*32,1)==b'\x02' for i in range(3)) and r.read(BASE+10*32,1)==b'\0'
    assert word(r,sy,'score')==score+30;checks['bomb_clears_enemies_and_hostile_shots']=True
   if kind==3:
    assert word(r,sy,'freeze_ticks')==300
    slot(r,0,14,40,40,META['enemy_ids']['14'][0],vy=256);cam=word(r,sy,'camera_y');pos=r.read(BASE+2,4)
    for _ in range(4):tick(r,sy)
    assert word(r,sy,'camera_y')==cam and r.read(BASE+2,4)==pos
    r.write(sy['freeze_ticks'],b'\0\1')
    for _ in range(3):tick(r,sy)
    assert word(r,sy,'freeze_ticks')==0 and word(r,sy,'camera_y')<cam and r.read(BASE+2,4)!=pos
    checks['freeze_stops_world_and_enemies_then_expires']=True
  hidden=next(i for i,o in enumerate(ITEMS['objects']) if o['kind']==1 and not o['visible'] and not o['passage'])
  obj=ITEMS['objects'][hidden];fixed_fixture(r,sy,hidden,state=0,collect=False)
  slot(r,7,0,obj['x'],100,META['ids']['arrow'],vy=-4096);tick(r,sy)
  assert r.read(0x2980+hidden*2,2)==bytes([1,obj['hp']-1])
  checks['hidden_block_revealed_by_first_shot']=picture(r,sy,f'{name}-items-hidden-revealed')
  # Gray blocks and bridges are implemented although absent from phase one's
  # original list. Change one record only as a controlled verification fixture.
  record_address=sy['fixed_item_table'];saved_record=r.read(record_address,6)
  modified=bytearray(saved_record);modified[3]=0x14;modified[4]=4;r.write(record_address,modified)
  fixed_fixture(r,sy,0,state=2,collect=False)
  r.write(sy['terrain_overlays'],b'\x01')
  sx=(modified[2]//8)*8;sypos=int.from_bytes(modified[:2],'big')-word(r,sy,'camera_y')
  r.write(sy['player_x'],bytes([sx-13]));r.write(sy['player_y'],bytes([sypos-5]));r.write(sy['player_fx'],b'\0');tick(r,sy,8)
  assert val(r,sy,'player_x')==sx-13
  checks['gray_block_is_solid']=picture(r,sy,f'{name}-items-gray-block')
  modified[3]=0x16;modified[4]=1;r.write(record_address,modified)
  r.write(0x2980,bytes([0,1]));rebuild_background(r,sy)
  slot(r,7,0,modified[2],sypos+20,META['ids']['arrow'],vy=-4096);tick(r,sy)
  assert r.read(0x2980,1)==b'\x04'
  checks['bridge_created_by_breaking_block']=picture(r,sy,f'{name}-items-bridge')
  r.write(sy['player_x'],bytes([sx-13]));r.write(sy['player_y'],bytes([sypos-5]));r.write(sy['player_fx'],b'\0');tick(r,sy,8)
  assert val(r,sy,'player_x')==sx-11;checks['bridge_terrain_is_walkable']=True
  r.write(record_address,saved_record)
  for edge in [1,248]:
   portal=next(i for i,o in enumerate(ITEMS['objects']) if o['passage'] and o['x']==edge)
   fixed_fixture(r,sy,portal,state=5,collect=False)
   r.write(sy['player_x'],bytes([0 if edge==1 else 239]));r.write(sy['player_y'],b'\x5a');r.write(sy['player_fx'],b'\0')
   tick(r,sy,4 if edge==1 else 8)
   assert val(r,sy,'player_x')==(239 if edge==1 else 1)
  checks['lateral_passages_both_directions']=True
  exit_index=next(i for i,o in enumerate(ITEMS['objects']) if o['kind']==5)
  r.write(sy['unlocked_exit'],b'\x01');enter_game(r,sy)
  assert r.read(0x2980+exit_index*2,1)==b'\0'
  fixed_fixture(r,sy,exit_index,state=2);tick(r,sy)
  assert val(r,sy,'combat_status')==3
  checks['exit_unlock_and_first_stage_clear_only']=True
  # All projectile families, including the original three-way pattern and return.
  enter_game(r,sy);reset_fixture(r,sy)
  for weapon in range(12):
   clear_slots(r,sy);r.write(sy['weapon_kind'],bytes([weapon]));r.write(sy['fire_clock'],b'\0');tick(r,sy,16)
   count=sum(r.read(BASE+i*32,1)==b'\x01' for i in range(7,10));assert count==(3 if weapon in (8,9) else 1),(weapon,count)
   if weapon in (0,2,4):assert int.from_bytes(r.read(BASE+7*32+8,2),'big',signed=True)=={0:-2048,2:-2432,4:-2304}[weapon]
   picture(r,sy,f'{name}-weapon-{weapon}')
  checks['all_twelve_weapon_forms_and_three_way']=True
  clear_slots(r,sy);tick(r,sy)
  slot(r,0,9,128,90,META['enemy_ids']['9'][0],hp=3)
  slot(r,7,10,120,100,ITEMS['weapon_art'][10],vy=-2048);tick(r,sy)
  assert r.read(BASE,1)==b'\x02' and r.read(BASE+7*32,1)==b'\x01'
  checks['strong_weapon_pierces_and_kills_armored_enemy']=picture(r,sy,f'{name}-items-piercing')
  clear_slots(r,sy);r.write(sy['weapon_kind'],b'\x06');r.write(sy['fire_clock'],b'\0');tick(r,sy,16)
  lowest=255;returning=False
  for _ in range(45):
   tick(r,sy);lowest=min(lowest,r.read(BASE+7*32+4,1)[0]);returning|=r.read(BASE+7*32+12,1)==b'\x01'
  assert returning and r.read(BASE+7*32,1)==b'\0'
  checks['boomerang_original_ramp_and_return']=True
  result=dict(variant=name,ram_kib=128,status='intro and item scenarios passed',scope='first-stage natural spawn plus controlled collision and pickup fixtures; other stages are extracted metadata, not playable',checks=checks)
  (ROOT/f'analysis/{name}-items-verification.json').write_text(json.dumps(result,indent=2),encoding='utf-8');print(json.dumps(result,indent=2),flush=True)
 finally:
  if r:r.s.close()
  p.terminate();p.wait(timeout=10);log.close()

def rebuild_background(r,sy):
 # Setup utility for checkpoint jumps; never used to implement actual effects.
 r.write(sy['sprite_valid'],b'\0');r.write(sy['boss_drawn'],b'\0')
 r.write(sy['shield_drawn'],b'\0')
 for i in range(19):r.write(BASE+i*32+17,b'\0')
 old=val(r,sy,'combat_status');r.write(sy['combat_status'],b'\x02');raw=expected(r,sy);r.write(sy['combat_status'],bytes([old]))
 head=val(r,sy,'ring_head');pos=head*128
 for k in range(0,len(raw),128):
  addr=pos+k;r.write(0xffa4,bytes([addr//8192]));r.write(0x8000+addr%8192,raw[k:k+128])
 tick(r,sy)

def fixed_fixture(r,sy,index,state,collect=True):
 clear_slots(r,sy);r.write(sy['token_index'],b'\xff');r.write(sy['ability_kind'],b'\0');r.write(sy['freeze_ticks'],b'\0\0');r.write(sy['player_invul'],b'\xff')
 r.write(0x2980,bytes([6,1])*len(ITEMS['objects']))
 record=r.read(sy['fixed_item_table']+index*6,6);wy=int.from_bytes(record[:2],'big')
 r.write(0x2980+index*2,bytes([state,record[4]]));r.write(sy['camera_y'],(wy-80).to_bytes(2,'big'))
 r.write(sy['player_x'],b'\x78');r.write(sy['player_y'],b'\x88')
 rebuild_background(r,sy)
 if collect:
  r.write(sy['player_x'],bytes([record[2]]));r.write(sy['player_y'],bytes([wy-word(r,sy,'camera_y')]))

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--xroar',type=Path,required=True);ap.add_argument('--variant',choices=['DAC6','ORCH90']);a=ap.parse_args()
 for name in ([a.variant] if a.variant else ['DAC6','ORCH90']):verify(a.xroar,name)
if __name__=='__main__':main()
