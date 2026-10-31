"""Native 128-KiB regression for three-life game over and the real I key.

PIA signals are supplied at the actual LDA boundaries, as in the controls
suite. Damage fixtures place entities, but the normal collision/death/scene
code performs all transitions. No expected framebuffer data is injected.
"""
from pathlib import Path
import argparse,hashlib,json
from verify_ai import boot
from verify_probe import symbols
from verify_controls_scroll import call,enter_game,keyboard_tick,val,word,inject_a
from verify_combat import tick,slot,clear_slots,picture,META,BASE
from verify_items import scene_tick,scene_image
ROOT=Path(__file__).resolve().parents[1]

def press_i(r,sy,expected_border=None):
    if expected_border is None:
        keyboard_tick(r,sy,{1:2},0)
        return
    # FF9A is a write-only GIME register. Verify A at the actual STA,
    # rather than treating a debugger read of FF9A as a colour readback.
    call(r,sy,'poll_input');r.write(sy['input_mode'],b'\0')
    r.bp(sy['keyboard_read'])
    for col in range(8):
        r.go();assert val(r,sy,'key_column')==(255^(1<<col))
        inject_a(r,253 if col==1 else 255)
    r.unbp(sy['keyboard_read'])
    call(r,sy,'test_border_store')
    assert bytes.fromhex(r.cmd('g'))[1]==expected_border
    r.step();call(r,sy,'game_return')

def release(r,sy):
    keyboard_tick(r,sy,{},0)

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--xroar',type=Path,required=True);ap.add_argument('--variant',choices=['DAC6','ORCH90']);a=ap.parse_args()
    for name in ([a.variant] if a.variant else ['DAC6','ORCH90']):
        p,log,r=boot(a.xroar,name)
        try:
            sy=symbols(name);call(r,sy,'main_loop');r.cmd(f'P8={sy["start"]:04x}');call(r,sy,'main_loop')
            checks={}
            # I in the intro is ignored. The initial game starts without cheats.
            press_i(r,sy);assert val(r,sy,'test_invincible')==0
            release(r,sy);enter_game(r,sy)
            assert val(r,sy,'test_invincible')==0
            checks['intro_ignores_test_toggle']=True
            press_i(r,sy,expected_border=r.read(0xffb4,1)[0])
            assert val(r,sy,'test_invincible')==1
            # Keeping the physical key down must not retrigger its toggle.
            for _ in range(10):press_i(r,sy)
            assert val(r,sy,'test_invincible')==1
            release(r,sy)
            checks['real_i_key_toggles_once_until_released']=True
            checks['blue_border_marks_test_mode']=True
            # The flag must not replace equipment with the original ability 3,
            # whose gameplay rules intentionally prohibit player firing.
            assert val(r,sy,'ability_kind')==0 and val(r,sy,'speed_level')==0
            lives=val(r,sy,'player_lives');score=word(r,sy,'score')
            for _ in range(320):tick(r,sy,16)
            assert val(r,sy,'test_invincible')==1 and val(r,sy,'combat_status')==0
            assert val(r,sy,'player_lives')==lives and word(r,sy,'shots_fired')>2
            checks['protection_persists_and_player_can_fire']=True
            clear_slots(r,sy);tick(r,sy)
            r.write(sy['player_invul'],b'\0');r.write(sy['ability_kind'],b'\0')
            px=val(r,sy,'player_x');py=val(r,sy,'player_y')
            slot(r,10,8,px+8,py+8,META['ids']['bullet'])
            tick(r,sy);assert val(r,sy,'combat_status')==0 and val(r,sy,'player_lives')==lives
            clear_slots(r,sy);tick(r,sy)
            slot(r,0,14,px+8,py+8,META['enemy_ids']['14'][0])
            tick(r,sy);assert val(r,sy,'combat_status')==0 and val(r,sy,'player_lives')==lives
            checks['hostile_bullet_and_enemy_contact_cannot_kill']=True
            checks['test_mode_render']=picture(r,sy,f'{name}-testmode-enabled')
            clear_slots(r,sy);tick(r,sy)
            press_i(r,sy,expected_border=0);release(r,sy)
            assert val(r,sy,'test_invincible')==0
            checks['second_i_press_disables_protection']=True
            # Start clean, then lose all three lives by real collisions.
            tick(r,sy,64);assert val(r,sy,'player_lives')==3
            r.write(sy['high_score'],(4321).to_bytes(2,'big'))
            stack_before=int.from_bytes(bytes.fromhex(r.cmd('g'))[-4:-2],'big')
            for remaining in [2,1,0]:
                clear_slots(r,sy);tick(r,sy)
                slot(r,10,8,val(r,sy,'player_x')+8,val(r,sy,'player_y')+8,META['ids']['bullet'])
                tick(r,sy)
                assert val(r,sy,'combat_status')==1 and val(r,sy,'player_lives')==remaining
                for _ in range(47):tick(r,sy,16 if remaining==0 else 0)
                assert val(r,sy,'combat_status')==1
                tick(r,sy,16 if remaining==0 else 0)
                if remaining:
                    assert val(r,sy,'scene_mode')==0 and val(r,sy,'player_lives')==remaining
                else:
                    assert val(r,sy,'scene_mode')==1 and val(r,sy,'intro_clock')==0
            assert val(r,sy,'test_invincible')==0 and val(r,sy,'boss_state')==0
            assert val(r,sy,'unlocked_exit')==0 and val(r,sy,'paused')==0
            assert r.read(0xffb0,1)==r.read(0xffb4,1)
            assert r.read(BASE,19*32)==bytes(19*32)
            assert word(r,sy,'high_score')==4321
            assert int.from_bytes(bytes.fromhex(r.cmd('g'))[-4:-2],'big')==stack_before
            checks['three_lives_return_to_blue_intro_without_stack_leak']=True
            # Holding fire over the transition cannot skip the opening.
            frames=word(r,sy,'frames_played')
            for _ in range(14):scene_tick(r,sy,16)
            assert val(r,sy,'scene_mode')==1 and val(r,sy,'intro_y')==56
            assert word(r,sy,'frames_played')!=frames
            want=bytearray(24576);logo=(ROOT/'assets/intro-logo.4bpp').read_bytes()
            for y in range(24):want[(56+y)*128+40:(56+y)*128+88]=logo[y*48:(y+1)*48]
            checks['konami_raster_after_game_over']=scene_image(r,sy,f'{name}-gameover-konami',want)
            checks['held_fire_does_not_skip_intro_and_video_keeps_running']=True
            # Release/repress: title, then a new three-life game, no cheats.
            scene_tick(r,sy,0);scene_tick(r,sy,16)
            assert val(r,sy,'scene_mode')==2
            scene_tick(r,sy,0);scene_tick(r,sy,16)
            assert val(r,sy,'scene_mode')==0 and val(r,sy,'player_lives')==3
            assert word(r,sy,'score')==0 and word(r,sy,'camera_y')==1776
            assert val(r,sy,'test_invincible')==0 and word(r,sy,'high_score')==4321
            checks['fresh_game_from_title']=picture(r,sy,f'{name}-gameover-new-game')
            # R also clears a test toggle in the middle of a game.
            press_i(r,sy);release(r,sy);assert val(r,sy,'test_invincible')==1
            tick(r,sy,64);assert val(r,sy,'test_invincible')==0
            checks['r_restart_clears_test_mode']=True
            result=dict(status='game-over intro and I-key test-mode scenarios passed',variant=name,ram_kib=128,
                        tested_disk_sha256=hashlib.sha256((ROOT/f'bin/{name}.DSK').read_bytes()).hexdigest(),
                        checks=checks,scope='native keyboard matrix, entity collision fixtures, all three lives lost through normal death code, real intro/title/new-game flow; no physical hardware test')
            (ROOT/f'analysis/{name}-gameover-testmode-verification.json').write_text(json.dumps(result,indent=2),encoding='utf-8')
            print(json.dumps(result,indent=2),flush=True)
        finally:
            r.s.close();p.terminate();p.wait(timeout=10);log.close()

if __name__=='__main__':main()
