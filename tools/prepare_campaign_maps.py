"""Keep the original eight-stage dictionary/repetition stream, not flat maps."""
from pathlib import Path
import argparse,hashlib,json,sys
ROOT=Path(__file__).resolve().parents[1]
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--rom',type=Path,required=True);a=ap.parse_args()
 raw=a.rom.read_bytes();assert len(raw)==32768
 blob=raw[0x99cd-0x4000:0xa5b4-0x4000]
 (ROOT/'assets/campaign-maps.rom.bin').write_bytes(blob)
 original=(ROOT/'assets/stages/stage1-map.bin').read_bytes()
 moving=(ROOT/'assets/stage1-combat-map.bin').read_bytes()
 lines=['; Native moving guardian replaces the original baked-in tile image.']
 for i,(old,new) in enumerate(zip(original,moving)):
  if old!=new:lines+=['        lda #$%02x'%new,'        sta $%04x'%(0x8000+i)]
 (ROOT/'src/campaign_map_patch.inc').write_text('\n'.join(lines)+'\n',encoding='ascii')
 report=dict(source_rom_sha256=hashlib.sha256(raw).hexdigest(),source_range=['99CD','A5B4 exclusive'],compressed_bytes=len(blob),expanded_bytes=8*244*32,stages=8,shared_dictionary_bytes=976,dictionary_pointer_bytes=128,stage_pointer_bytes=32,stage_scripts_bytes=len(blob)-976-128-32,technique='original overlapping 4x4 tile dictionary, repeated block codes, horizontal mirror, overlapping segment seams',native_decoder='src/campaign_maps.asm')
 (ROOT/'analysis/campaign-map-compression.json').write_text(json.dumps(report,indent=2),encoding='utf-8');print(json.dumps(report,indent=2))
if __name__=='__main__':main()
