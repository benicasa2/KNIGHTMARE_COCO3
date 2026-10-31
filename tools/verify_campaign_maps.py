"""Run the real 6809 decoder and compare every tile of all 8 MSX maps."""
from pathlib import Path
import argparse,sys,json,hashlib
from verify_ai import boot,shim
from verify_probe import symbols
from verify_controls_scroll import call,enter_game
ROOT=Path(__file__).resolve().parents[1]
def verify(exe,lw,rom,name):
 sys.path.insert(0,str(ROOT/'third_party/knightmare'));import mapas
 source=mapas.Rom(str(rom),0x4000);sy=symbols(name);p,log,r=boot(exe,name)
 try:
  call(r,sy,'main_loop');enter_game(r,sy)
  before=r.read(0xe000,sy['aux_end']-0xe000);checks=[]
  for stage in range(8):
   r.write(sy['campaign_stage'],bytes([stage]))
   body=f"""        orcc #$50
        lda #12
        sta $ffa4
        ldy #$8000
        sty $0be0
        clr $0be2
map_band_loop
        lda $0be2
        jsr ${sy['campaign_decode_band']:04x}
        ldx #$0400
        ldy $0be0
        ldb #128
map_copy_band
        lda ,x+
        sta ,y+
        decb
        bne map_copy_band
        sty $0be0
        inc $0be2
        lda $0be2
        cmpa #61
        blo map_band_loop
"""
   shim(r,sy,lw,'map_bands',body);actual=r.read(0x8000,7808)
   expected=bytes(v for row in mapas.monta_la_fase(source,stage) for v in row)
   assert actual==expected,dict(stage=stage+1,first=[i for i,(a,b) in enumerate(zip(actual,expected)) if a!=b][:10])
   assert before==r.read(0xe000,len(before)),'auxiliary data was overwritten'
   checks.append(dict(stage=stage+1,tiles_compared=len(expected),sha256=hashlib.sha256(actual).hexdigest()))
  body=f"        clr ${sy['campaign_stage']:04x}\n        jsr ${sy['campaign_map_load']:04x}\n        ldd #64\n        jsr ${sy['campaign_map_row']:04x}\n"
  shim(r,sy,lw,'moving_guardian_band',body)
  assert r.read(0x0400,128)==(ROOT/'assets/stage1-combat-map.bin').read_bytes()[8*32:12*32]
  report=dict(status='passed',variant=name,tested_disk_sha256=hashlib.sha256((ROOT/f'bin/{name}.DSK').read_bytes()).hexdigest(),compressed_bytes=3047,total_tiles_compared=8*7808,all_eight_maps_match_original=True,first_stage_moving_guardian_patch_matches=True,auxiliary_bank_unchanged=True,checks=checks)
  (ROOT/f'analysis/{name}-campaign-map-verification.json').write_text(json.dumps(report,indent=2),encoding='utf-8');print(json.dumps(report,indent=2),flush=True)
 finally:
  r.s.close();p.terminate();p.wait(timeout=10);log.close()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--xroar',type=Path,required=True);ap.add_argument('--lwasm',type=Path,required=True);ap.add_argument('--rom',type=Path,required=True);a=ap.parse_args()
 for name in ['DAC6','ORCH90']:verify(a.xroar,a.lwasm,a.rom,name)
if __name__=='__main__':main()
