"""Export stage tables and original audio data from the exact supplied ROM."""
from pathlib import Path
import argparse,hashlib,json,sys
ROOT=Path(__file__).resolve().parents[1]
SHA='589b2fd276d84827bb1b033f43a0cd5ab0aa9da3c8784cac2f62474fed00aae3'
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--rom',type=Path,required=True);args=ap.parse_args()
 data=args.rom.read_bytes();assert hashlib.sha256(data).hexdigest()==SHA,'This extraction profile is for the supplied ROM only'
 sys.path.insert(0,str(ROOT/'third_party/knightmare'));import mapas
 rom=mapas.Rom(str(args.rom),0x4000);inherited=None;meta=[]
 for stage in range(8):
  v=mapas.vram_de_la_fase(rom,stage,inherited);inherited=v
  rows=mapas.monta_la_fase(rom,stage)
  for suffix,blob in [('map',bytes(n for row in rows for n in row)),('patterns',bytes(v[0x2000:0x2800])),('colors',bytes(v[:0x800]))]:
   path=ROOT/f'assets/stages/stage{stage+1}-{suffix}.bin';path.write_bytes(blob)
   meta.append(dict(path=path.relative_to(ROOT).as_posix(),bytes=len(blob),sha256=hashlib.sha256(blob).hexdigest()))
 regions=[('audio-sequencer-z80',0x4901,0x4b2c),('audio-periods',0x4b2c,0x4b38),('audio-voice-pointers',0x4b38,0x4bb6),('audio-score-data',0x4bb6,0x53d3)]
 for name,a,b in regions:
  path=ROOT/f'assets/{name}.bin';path.write_bytes(data[a-0x4000:b-0x4000]);meta.append(dict(path=path.relative_to(ROOT).as_posix(),start=hex(a),end_exclusive=hex(b),bytes=b-a,classification='original Z80 code or data, not converted 6809 code'))
 observed=(ROOT/'reference/action-VRAM.bin').read_bytes();expected=mapas.vram_de_la_fase(rom,0)
 checks={}
 for name,a,n in [('colors',0,6144),('patterns',0x2000,6144)]:
  differences=sum(expected[a+i]!=observed[a+i] for i in range(n))
  checks[name]=dict(bytes_compared=n,differences=differences,reference='reference/action-VRAM.bin')
  assert differences==0
 (ROOT/'analysis/stage1-tables-check.json').write_text(json.dumps(checks,indent=2),encoding='utf-8')
 (ROOT/'analysis/extracted-resources.json').write_text(json.dumps(dict(source_sha256=SHA,files=meta,other_stages_emulator_verified=False),indent=2),encoding='utf-8')
 print('8 stage tables and original audio data exported; stage 1 graphics match openMSX')
if __name__=='__main__':main()
