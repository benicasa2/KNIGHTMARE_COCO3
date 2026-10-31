"""Extract observed SCREEN 2 tables from a captured frame, with provenance."""
from pathlib import Path
import json
from render_msx import render,png,RGB
ROOT=Path(__file__).resolve().parents[1]
ref=ROOT/'reference';assets=ROOT/'assets'
label='action';v=(ref/f'{label}-VRAM.bin').read_bytes();r=(ref/f'{label}-VDP_regs.bin').read_bytes()
regions=[('color-table',0,0x1800),('sprite-patterns',0x1800,0x800),('background-patterns',0x2000,0x1800),('name-table',0x3800,0x300),('sprite-attributes',0x3b00,128)]
for name,start,size in regions:(assets/f'{name}.bin').write_bytes(v[start:start+size])
atlas=bytearray(256*128)
for n in range(64):
 for y in range(16):
  for x in range(16):
   bits=v[0x1800+n*32+(x//8)*16+y]
   atlas[((n//16)*32+y)*256+(n%16)*16+x]=15 if bits&(128>>(x&7)) else 1
png(assets/'sprite-atlas.png',atlas,h=128)
(ROOT/'analysis/vram-regions.json').write_text(json.dumps(dict(source=f'reference/{label}-VRAM.bin',registers=list(r[:8]),regions=[dict(name=n,start=f'0x{s:04x}',bytes=z) for n,s,z in regions],classification='observed VRAM tables from one frame; not all ROM graphics'),indent=2),encoding='utf-8')
for name in ['title','start','play','later','action','end']:
 path=ref/f'{name}-VRAM.bin'
 if path.exists():
  pix=render(path.read_bytes(),(ref/f'{name}-VDP_regs.bin').read_bytes());png(ref/f'{name}.png',pix)
print('Extracted observed graphics tables and screenshots')
