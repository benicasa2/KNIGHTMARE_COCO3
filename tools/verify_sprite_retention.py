"""Retained frames, visual invalidation and all specialized restore widths."""
from pathlib import Path
import argparse,hashlib,json
from verify_ai import boot,shim
from verify_probe import symbols
from verify_controls_scroll import call,enter_game,val,screen
from verify_combat import clear_slots,slot,tick,expected,META,BASE
from verify_phase2_edges import invoke_disk
ROOT=Path(__file__).resolve().parents[1]

def verify(a,name):
 sy=symbols(name);p,log,r=boot(a.xroar,name);checks=[]
 try:
  call(r,sy,'main_loop');enter_game(r,sy)
  for stage in [0,1]:
   if stage:
    invoke_disk(r,sy,'phase_advance')
    regs=bytes.fromhex(r.cmd('g'));r.cmd(f'P0={regs[0]&0xaf:02x}');r.cmd(f'P8={sy["main_loop"]:04x}')
   clear_slots(r,sy);r.write(sy['token_index'],b'\xff');r.write(sy['test_invincible'],b'\x01');r.write(sy['freeze_ticks'],(60000).to_bytes(2,'big'));tick(r,sy)
   def update(label,retain,bits=0):
    call(r,sy,'apply_input');r.write(sy['input_bits'],bytes([bits]));head=val(r,sy,'ring_head');old=screen(r,head)
    call(r,sy,'render_begin');assert screen(r,head)==old,label+' changed the framebuffer during logic'
    r.bp(sy['render_commit_begin']);r.bp(sy['combat_frame_complete']);r.go()
    kept=int.from_bytes(bytes.fromhex(r.cmd('g'))[-2:],'big')==sy['combat_frame_complete']
    r.unbp(sy['render_commit_begin']);r.unbp(sy['combat_frame_complete']);assert kept==retain,(stage,label,kept,retain)
    if not kept:call(r,sy,'combat_frame_complete')
    actual=screen(r,val(r,sy,'ring_head'));assert actual==expected(r,sy),label+' differs from independent pixel composition'
    if kept:assert actual==old,label+' changed a retained frame'
    checks.append(dict(stage=stage+1,case=label,retained=kept,final_raster_sha256=hashlib.sha256(actual).hexdigest()))
   update('stationary-empty',True)
   update('stationary-empty-again',True)
   update('move-right',False,8)
   update('stop-moving',True)
   r.write(sy['ability_kind'],b'\x01');r.write(sy['ability_timer'],(200).to_bytes(2,'big'))
   update('shield-appears',False)
   r.write(sy['ability_kind'],b'\0');update('shield-removed',False)
   update('after-shield',True)
   r.write(sy['ability_kind'],b'\x02');r.write(sy['ability_timer'],(200).to_bytes(2,'big'))
   update('invisible-image',False)
   r.write(sy['ability_timer'],b'\0\x01');update('invisibility-expires',False)
   update('after-invisibility',True)
   slot(r,0,14,val(r,sy,'player_x')+2,val(r,sy,'player_y')-2,META['enemy_ids']['14'][0])
   update('overlapping-enemy',False)
   r.write(BASE,b'\0');update('removed-enemy-old-background',False)
   update('after-enemy-removal',True)
   # A new arrow invalidates the empty screen even without player movement.
   update('player-fires',False,16)
   clear_slots(r,sy);update('erase-last-projectile',False)
   update('after-projectile',True)
   r.write(sy['freeze_ticks'],bytes(2));r.write(sy['scroll_clock'],b'\x03')
   update('scroll-forces-commit',False)
   # Low/high-level painter order must remain correct with both kinds of
   # sprites intersecting the player and one another.
   r.write(sy['freeze_ticks'],(60000).to_bytes(2,'big'))
   slot(r,0,7,100,100,META['stage2_type_frames']['7'][4],hp=3)
   slot(r,1,14,110,100,META['enemy_ids']['14'][0])
   update('wide-enemy-overlaps-enemy',False)
   clear_slots(r,sy);update('erase-overlapping-wide-frames',False)
  # Direct native copy checks include 8/9-byte player rows, 20-byte bosses,
  # and the widest four-hit-enemy image. Guard bytes catch overruns.
  widths=[];source=bytes((i*37+9)&255 for i in range(32))
  for width in range(33):
   r.write(0x900,source);r.write(0x940,b'\xa5'+bytes(32)+b'\x5a')
   body=f'''        lda #{width}
        sta ${sy['span_n']:04x}
        jsr ${sy['restore_span_select']:04x}
        ldx #$0900
        ldu #$0941
        jsr [${sy['blit_fn']:04x}]
        stx $0be4
        stu $0be6
'''
   shim(r,sy,a.lwasm,'restore_all_widths',body)
   assert r.read(0x940,34)==b'\xa5'+source[:width]+bytes(32-width)+b'\x5a',width
   assert r.read(0xbe4,4)==(0x900+width).to_bytes(2,'big')+(0x941+width).to_bytes(2,'big'),width
   widths.append(width)
  report=dict(status='sprite retention and specialized background restore passed',variant=name,tested_disk_sha256=hashlib.sha256((ROOT/f'bin/{name}.DSK').read_bytes()).hexdigest(),tested_levels_sha256=hashlib.sha256((ROOT/'bin/LEVELS.DSK').read_bytes()).hexdigest(),checks=checks,restore_widths=widths,scope='native framebuffer/route assertions and independent final pixels; timing is measured separately; no physical-display flicker claim')
  (ROOT/f'analysis/{name}-sprite-retention-verification.json').write_text(json.dumps(report,indent=2),encoding='utf-8');print(name+': retained-frame invalidation and 33 restore widths passed',flush=True)
 finally:r.s.close();p.terminate();p.wait(timeout=10);log.close()

if __name__=='__main__':
 ap=argparse.ArgumentParser();ap.add_argument('--xroar',type=Path,required=True);ap.add_argument('--lwasm',type=Path,required=True);a=ap.parse_args()
 for name in ['DAC6','ORCH90']:verify(a,name)
