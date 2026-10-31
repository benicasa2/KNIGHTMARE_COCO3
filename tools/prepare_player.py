"""Extract original walking frames, preserving sprite planes and transparent pixels."""
from pathlib import Path
import argparse,sys,json,hashlib
ROOT=Path(__file__).resolve().parents[1]
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--rom',type=Path,required=True);args=ap.parse_args()
 assert hashlib.sha256(args.rom.read_bytes()).hexdigest()==json.loads((ROOT/'analysis/rom.json').read_text())['sha256']
 sys.path.insert(0,str(ROOT/'third_party/knightmare'))
 import sprites,mapas
 rom=mapas.Rom(str(args.rom),0x4000);v=sprites.vram_con(rom,[0]);frames=[]
 for anim,index,data in sprites.fotogramas_del_jugador(rom):
  if anim!=0:continue
  pixels=sprites.dibuja_al_jugador(rom,v,data,sprites.ATRIBUTOS[anim])
  used=[y for y,row in enumerate(pixels) if any(c is not None for c in row)]
  pixels=pixels[min(used):max(used)+1]
  assert len(pixels)==16
  # Remove transparent borders only, preserving all original visible pixels.
  image=bytearray(16*16)
  for y,row in enumerate(pixels):
   for x,c in enumerate(row):
    if c is not None:image[y*16+x]=mapas.PALETA.index(c) or 1
  frames.append(image)
 assert len(frames)>=2
 (ROOT/'assets/player-walk-indexed.bin').write_bytes(b''.join(frames))
 (ROOT/'analysis/player-frames.json').write_text(json.dumps(dict(width=16,height=16,frames=len(frames),source='original animation 0, composed original sprite planes, transparent borders cropped',transparent=0),indent=2),encoding='utf-8')
 print('walking frames',len(frames))
if __name__=='__main__':main()
