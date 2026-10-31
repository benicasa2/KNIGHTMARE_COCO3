"""Build native 128 KiB game with compact eight-map scripts and auxiliary bank."""
from pathlib import Path
import argparse,struct,subprocess,json,hashlib,math
ROOT=Path(__file__).resolve().parents[1]
def rec(addr,data):return b'\0'+struct.pack('>HH',len(data),addr)+data
def decb(segments,entry):return b''.join(rec(a,d) for a,d in segments)+b'\xff\0\0'+struct.pack('>H',entry)
def disk_image(files):
 disk=bytearray(b'\xff'*(35*18*256));fat=bytearray(b'\xff'*256);granule=0
 for index,(name,typ,asc,data) in enumerate(files):
  chain=[]
  for pos in range(0,len(data),2304):
   assert granule<68
   chain.append(granule);track=granule//2
   if track>=17:track+=1
   dest=(track*18+(granule%2)*9)*256
   part=data[pos:pos+2304];disk[dest:dest+len(part)]=part;granule+=1
  for a,b in zip(chain,chain[1:]):fat[a]=b
  fat[chain[-1]]=0xc0+((len(data)-1)%2304//256+1)
  stem,ext=name.split('.');e=bytearray(32)
  e[:8]=stem.encode().ljust(8,b' ');e[8:11]=ext.encode().ljust(3,b' ')
  e[11:14]=bytes([typ,asc,chain[0]]);struct.pack_into('>H',e,14,(len(data)-1)%256+1)
  dest=(17*18+2)*256+index*32;disk[dest:dest+32]=e
 dest=(17*18+1)*256;disk[dest:dest+256]=fat
 return disk
def prepare():
 assert max(d['span'] for d in json.loads((ROOT/'analysis/combat-assets.json').read_text())['descriptors']) <= 32, 'Sprite restore width exceeds unrolled table'
 import sys
 sys.path.insert(0,str(ROOT/'tools'))
 from render_msx import RGB,render
 ref=ROOT/'reference'
 v=(ref/'action-VRAM.bin').read_bytes();r=(ref/'action-VDP_regs.bin').read_bytes()
 pix=render(v,r)
 packed=bytes(pix[i]*16+pix[i+1] for i in range(0,len(pix),2))
 (ROOT/'assets/scene.4bpp').write_bytes(packed)
 patterns=(ROOT/'assets/stages/stage1-patterns.bin').read_bytes()
 colors=(ROOT/'assets/stages/stage1-colors.bin').read_bytes()
 tiles=bytearray()
 for tile in range(256):
  for y in range(8):
   bits=patterns[tile*8+y];color=colors[tile*8+y]
   row=[(color>>4) if bits&(128>>x) else color&15 for x in range(8)]
   tiles+=bytes(row[x]*16+row[x+1] for x in range(0,8,2))
 (ROOT/'assets/stage1-packed-tiles.bin').write_bytes(tiles)
 source=(ROOT/'assets/player-walk-indexed.bin').read_bytes()
 assert len(source)==512
 sprites=bytearray()
 for frame in range(2):
  for align in range(2):
   for y in range(16):
    row=[0]*align+list(source[frame*256+y*16:frame*256+(y+1)*16])+[0]*(2-align)
    for x in range(0,18,2):
     a,b=row[x:x+2];sprites+=bytes([(0 if a else 0xf0)|(0 if b else 15),a*16+b])
 assert len(sprites)==1152
 hud=packed[176*128:192*128]
 bank=bytearray(sprites+bytes(4096-len(sprites))+hud+bytes(2048))
 for patch in json.loads((ROOT/'assets/items-bank13-patches.json').read_text()):
  part=bytes.fromhex(patch['data']);bank[patch['offset']:patch['offset']+len(part)]=part
 (ROOT/'assets/player-hud-bank.bin').write_bytes(bank)
 palette=[]
 for rgb in RGB:
  r,g,b=[min(3,round(x/85)) for x in rgb]
  palette.append(((r>>1)<<5)|((g>>1)<<4)|((b>>1)<<3)|((r&1)<<2)|((g&1)<<1)|(b&1))
 (ROOT/'src/palette.inc').write_text('        fcb '+','.join(f'${x:02x}' for x in palette)+'\n',encoding='ascii')
 pcm=bytearray();labels=[]
 for name in ['shot','hit','explosion','boss','cycle','pickup']:
  labels.append(f'pcm_{name} equ ${0x8000+len(pcm):04x}')
  pcm+=(ROOT/f'assets/sfx-{name}.pcm').read_bytes()
  labels.append(f'pcm_{name}_end equ ${0x8000+len(pcm):04x}')
 assert len(pcm)<=8192
 (ROOT/'assets/pcm-bank7.bin').write_bytes(pcm)
 (ROOT/'src/pcm_locations.inc').write_text('\n'.join(labels)+'\n',encoding='ascii')
 return dict(palette=palette,music_enabled=False,music_optional=True,music_toggle_key='M',music_sample_rate_min=(3579545/1824),music_sample_rate_max=(3579545/912),music_enemy_limit=5,normal_enemy_limit=7,audio='optional two-voice original music DDS mixed with priority event PCM; timer FIRQ idle with both off',sample_rate=3579545/912,game_clock='GIME vertical-border IRQ, independent of audio')

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--lwasm',required=True);args=ap.parse_args();meta=prepare()
 variants={}
 for orch,name in [(0,'DAC6'),(1,'ORCH90')]:
  output=ROOT/f'bin/{name}.BIN'
  subprocess.run([args.lwasm,'--format=decb',f'--define=ORCH90={orch}',f'--output={output}',f'--list={ROOT}/analysis/{name}.lst',f'--symbol-dump={ROOT}/analysis/{name}.sym','hardware_probe.asm'],cwd=ROOT/'src',check=True)
  data=output.read_bytes();pos=0;core=[];extra=[]
  while data[pos]==0:
   n,addr=struct.unpack_from('>HH',data,pos+1);part=data[pos+5:pos+5+n];pos+=5+n
   (extra if addr>=0xe000 else core).append((addr,part))
  assert data[pos:pos+3]==b'\xff\0\0'
  entry=struct.unpack_from('>H',data,pos+3)[0]
  assert extra and all(0xe000<=addr and addr+len(part)<=0xfef4 for addr,part in extra)
  image=bytearray(max(addr+len(part) for addr,part in extra)-0xe000)
  for addr,part in extra:image[addr-0xe000:addr-0xe000+len(part)]=part
  aux=bytes(image)
  output.write_bytes(decb(core,entry));variants[name]=(output,aux)
  (ROOT/f'assets/{name}-aux-bank6.bin').write_bytes(aux)
 for name,(output,aux) in variants.items():
  files=[];loader=['10 CLEAR 200,&H2BFF','20 PRINT "KNIGHTMARE - MAPAS COMPACTOS"','30 PRINT "SETAS/WASD ESPACO  I TESTE R RESET"']
  # 48 KiB scrolling ring; bank 6 holds intro plus all 8 original map scripts.
  # Bank 7 stores PCM; map decoding uses a 128-byte row-band cache.
  resources=[(6,aux),(7,(ROOT/'assets/pcm-bank7.bin').read_bytes()),(12,(ROOT/'assets/stage1-packed-tiles.bin').read_bytes()),(13,(ROOT/'assets/player-hud-bank.bin').read_bytes()),(14,(ROOT/'assets/combat-bank.bin').read_bytes()),(15,(ROOT/'assets/items-bank15.bin').read_bytes())]
  assert len(resources[-1][1]) <= 0x1ef4, 'Bank 15 overlaps Disk BASIC bootstrap vectors'
  chunks=[(bank,pos,data[pos:pos+4096]) for bank,data in resources for pos in range(0,len(data),4096)]
  for i,(bank,pos,part) in enumerate(chunks):
   address=0x8000+pos
   # PSHS CC; ORCC; all RAM; map bank; copy; restore map/ROM; PULS CC,PC.
   helper=bytes.fromhex('34011a50b7ffdf86')+bytes([bank])+bytes.fromhex('b7ffa48e0e00ce')+struct.pack('>H',address)+bytes.fromhex('108e')+struct.pack('>H',len(part))+bytes.fromhex('a680a7c0313f26f8863cb7ffa4b7ffde3581')
   payload=decb([(0x0e00,part),(0x2c00,helper)],0x2c00)
   resource_name=f'DAT{i}.BIN';files.append((resource_name,2,0,payload));(ROOT/'bin'/f'{name}-{resource_name}').write_bytes(payload)
   loader.append(f'{40+i*10} LOADM"DAT{i}":EXEC')
  basic=loader+[f'{40+len(chunks)*10} LOADM"{name}":EXEC']
  disk=disk_image([('START.BAS',0,255,('\r'.join(basic)+'\r').encode('ascii')),*files,(f'{name}.BIN',2,0,output.read_bytes())])
  syms=(ROOT/f'analysis/{name}.sym').read_text()
  import re
  assert int(re.search(r'program_end EQU \$([0-9A-Fa-f]+)',syms)[1],16)<=0x8000,'Program exceeds fixed CPU window'
  assert int(re.search(r'state_end EQU \$([0-9A-Fa-f]+)',syms)[1],16)<=0x0d00,'Direct page overflow'
  (ROOT/f'bin/{name}.DSK').write_bytes(disk)
  meta[name]=dict(disk_sha256=hashlib.sha256(disk).hexdigest(),binary_bytes=output.stat().st_size)
 meta.update(status='two playable stages with original exit walk, ROM-derived AI and segment checkpoints; stages 3-8 pending; visual/audio adaptations remain',playable_stages=2,ram_kib=128,framebuffer_ring_bytes=49152,aux_bank=6,aux_bytes=len(aux),compressed_campaign_map_bytes=3047,compressed_campaign_maps=8,active_map_bytes=128,pcm_bank=7,framebuffer_physical_start=0,video='256x192 16 colors RGB, 176-line playfield',map_cache_cpu_address=0x400,tiles_bank=12,player_hud_bank=13,combat_bank=14,items_bank=15,introduction=True,inventory=True,nominal_game_update_hz=(3579545/912)/(131/2)/2,nominal_scroll_pixels_per_second=(3579545/912)/(131/2)/8,source_rom_sha256='589b2fd276d84827bb1b033f43a0cd5ab0aa9da3c8784cac2f62474fed00aae3',enemy_logic_kinds=[0,1,2,3,4,7,9,11,12,14],first_boss_satellite_kind=12,first_stage_checkpoints=True)
 meta.update(render_policy='retain unchanged empty frames; logic first, specialized reverse restore and unchanged painter order',stationary_frame_retention=True,specialized_background_restore=True,flicker_eliminated=False,full_double_buffer=False)
 meta.update(game_over_returns_to_konami_intro=True,test_invincibility_key='I',test_mode_default=False,test_mode_indicator='blue GIME border; black when disabled')
 (ROOT/'analysis/build.json').write_text(json.dumps(meta,indent=2),encoding='utf-8');print(json.dumps(meta,indent=2))
if __name__=='__main__':main()
