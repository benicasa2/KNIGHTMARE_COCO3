"""Traverse the real first-stage loop and defeat its boss with native shots.

Controlled fixture: I-key test invincibility and floating tokens suppressed, so this
checks integration rather than player skill. Camera/enemy/boss/hit positions,
HP and framebuffer are never supplied by the host.
"""
from pathlib import Path
import argparse,hashlib,json
from verify_ai import boot
from verify_probe import symbols
from verify_controls_scroll import call,enter_game,val,word,keyboard_tick
from verify_combat import tick,picture
ROOT=Path(__file__).resolve().parents[1]

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--xroar',type=Path,required=True);ap.add_argument('--variant',choices=['DAC6','ORCH90']);a=ap.parse_args()
    for name in ([a.variant] if a.variant else ['DAC6','ORCH90']):
        p,log,r=boot(a.xroar,name)
        try:
            r.s.settimeout(60);sy=symbols(name)
            call(r,sy,'main_loop');r.cmd(f'P8={sy["start"]:04x}');call(r,sy,'main_loop');enter_game(r,sy)
            keyboard_tick(r,sy,{1:2},0);keyboard_tick(r,sy,{},0)
            assert val(r,sy,'test_invincible')==1
            r.write(sy['token_index'],b'\xff')
            call(r,sy,'start_boss')
            assert word(r,sy,'camera_y')==120 and word(r,sy,'scroll_steps')==1656
            entry_tick=word(r,sy,'combat_frames')
            print(f'{name}: native traversal reached guardian at camera 120',flush=True)
            call(r,sy,'boss_fight_frame')
            assert val(r,sy,'boss_phase')==0 and val(r,sy,'boss_y')==32
            assert 48<=word(r,sy,'camera_y')<=120
            call(r,sy,'game_return')
            first=picture(r,sy,f'{name}-native-stage1-boss-entry')
            for n in range(900):
                tick(r,sy,16)
                if val(r,sy,'boss_state')==2:break
            else:raise AssertionError('Native arrows did not defeat guardian')
            assert val(r,sy,'boss_hp')==0
            assert word(r,sy,'shots_fired')>=20
            assert word(r,sy,'score')>=10000
            for _ in range(104):tick(r,sy)
            assert val(r,sy,'combat_status')==3 and val(r,sy,'boss_state')==3
            final=picture(r,sy,f'{name}-native-stage1-completed')
            result=dict(status='native first-stage traversal and boss defeat passed',variant=name,ram_kib=128,
                        tested_disk_sha256=hashlib.sha256((ROOT/f'bin/{name}.DSK').read_bytes()).hexdigest(),
                        guardian_entry_native_tick=entry_tick,guardian_defeated_by_native_arrows=True,
                        attack_updates=n+1,shots_fired=word(r,sy,'shots_fired'),score=word(r,sy,'score'),
                        entry_frame_sha256=first,clear_frame_sha256=final,
                        scope='real scrolling, waves, guardian entry, shots and damage; test invincibility enabled through native I-key decoding and tokens suppressed; not an unassisted playthrough')
            (ROOT/f'analysis/{name}-stage1-flow-verification.json').write_text(json.dumps(result,indent=2),encoding='utf-8')
            print(json.dumps(result,indent=2),flush=True)
        finally:
            r.s.close();p.terminate();p.wait(timeout=10);log.close()

if __name__=='__main__':main()
