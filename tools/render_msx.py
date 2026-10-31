"""Decode captured TMS9918 SCREEN 2 VRAM, including sprite priority and early-clock.
Pure Python, no third party modules. One frame is evidence, not a game engine.
"""
from pathlib import Path
import struct,zlib,binascii,json
ROOT=Path(__file__).resolve().parents[1]
RGB=[(0,0,0),(0,0,0),(33,200,66),(94,220,120),(84,85,237),(125,118,252),(212,82,77),(66,235,245),(252,85,84),(255,121,120),(212,193,84),(230,206,128),(33,176,59),(201,91,186),(204,204,204),(255,255,255)]
def png(path,pix,w=256,h=192,palette=RGB):
 raw=b''.join(b'\0'+b''.join(bytes(palette[p]) for p in pix[y*w:(y+1)*w]) for y in range(h))
 def chunk(k,d):return struct.pack('>I',len(d))+k+d+struct.pack('>I',binascii.crc32(k+d)&0xffffffff)
 Path(path).write_bytes(b'\x89PNG\r\n\x1a\n'+chunk(b'IHDR',struct.pack('>IIBBBBB',w,h,8,2,0,0,0))+chunk(b'IDAT',zlib.compress(raw,9))+chunk(b'IEND',b''))
def render(v,r,sprites=True):
 assert r[0]&2, 'Only SCREEN 2 supported'
 nt=(r[2]&15)*1024;pt=(r[4]&4)*2048;ct=(r[3]&128)*64
 pm=((r[4]&3)<<8)|255;cm=((r[3]&127)<<6)|63
 back=r[7]&15;pix=bytearray(256*192)
 for y in range(192):
  for x in range(256):
   n=v[nt+(y//8)*32+x//8];off=((y//64)*2048+n*8+(y&7))
   bits=v[pt+(off&((pm<<3)|7))];col=v[ct+(off&((cm<<3)|7))]
   c=(col>>4) if bits&(128>>(x&7)) else col&15
   pix[y*256+x]=c or back
 if not sprites:return pix
 sat=(r[5]&127)*128;spt=(r[6]&7)*2048;size=16 if r[1]&2 else 8;mag=2 if r[1]&1 else 1
 drawn=bytearray(256*192);count=[0]*192
 for n in range(32):
  sy,sx,pat,col=v[sat+n*4:sat+n*4+4]
  if sy==208:break
  sy=sy+1 if sy<224 else sy-255
  if col&128:sx-=32
  color=col&15
  if size==16:pat&=252
  for yy in range(size*mag):
   dy=sy+yy
   if not 0<=dy<192:continue
   count[dy]+=1
   if count[dy]>4:continue
   for xx in range(size*mag):
    dx=sx+xx
    if not 0<=dx<256:continue
    px,py=xx//mag,yy//mag
    addr=spt+pat*8+(px//8)*16+py
    p=dy*256+dx
    if color and v[addr]&(128>>(px&7)) and not drawn[p]:pix[p]=color;drawn[p]=1
 return pix
def main():
 for label in ['title','start','play','later']:
  v=(ROOT/f'reference/{label}-VRAM.bin').read_bytes();r=(ROOT/f'reference/{label}-VDP_regs.bin').read_bytes()
  pix=render(v,r);png(ROOT/f'reference/{label}.png',pix)
  (ROOT/f'assets/{label}.4bpp').write_bytes(bytes((pix[i]<<4)|pix[i+1] for i in range(0,len(pix),2)))
  print(label,'VDP',r[:8].hex(' '),'colors',sorted(set(pix)))
if __name__=='__main__':main()
