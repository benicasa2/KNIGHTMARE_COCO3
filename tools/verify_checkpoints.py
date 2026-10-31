"""Check segment/world/equipment restoration through the native 6809 path."""
from pathlib import Path
import argparse,hashlib,json
from verify_ai import boot,shim,REF
from verify_probe import symbols
from verify_controls_scroll import call,enter_game,val,word
from verify_combat import picture,tick
ROOT=Path(__file__).resolve().parents[1]
ITEMS=json.loads((ROOT/'analysis/items-assets.json').read_text())

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--xroar',type=Path,required=True);ap.add_argument('--lwasm',type=Path,required=True);ap.add_argument('--variant',choices=['DAC6','ORCH90']);a=ap.parse_args()
    for name in ([a.variant] if a.variant else ['DAC6','ORCH90']):
        p,log,r=boot(a.xroar,name)
        try:
            sy=symbols(name);call(r,sy,'main_loop');r.cmd(f'P8={sy["start"]:04x}');call(r,sy,'main_loop');enter_game(r,sy)
            cases=[]
            for segment in range(10):
                camera=1776-segment*192-95 if segment<9 else 0
                restored=1776-segment*192
                r.write(sy['camera_y'],camera.to_bytes(2,'big'))
                for field,value in [('weapon_kind',9),('speed_level',4),('ability_kind',2),('shield_hits',23),('player_lives',2),('combat_status',1),('boss_state',1),('boss_phase',0),('boss_hp',7),('boss_x',100),('boss_y',32),('boss_wait',37),('boss_target',88),('boss_velocity',255)]:r.write(sy[field],bytes([value]))
                r.write(sy['ability_timer'],(300).to_bytes(2,'big'));r.write(sy['freeze_ticks'],(100).to_bytes(2,'big'))
                r.write(sy['satellite_clocks'],bytes([9,88,201]))
                for i in range(3):
                    r.write(0x2700+i*32+3,bytes([11+i]));r.write(0x2700+i*32+5,bytes([21+i]))
                r.write(sy['score'],(1234).to_bytes(2,'big'));r.write(sy['high_score'],(4321).to_bytes(2,'big'));r.write(sy['boss_lifetime'],(1234).to_bytes(2,'big'));r.write(sy['wave_index'],b'\x0d')
                r.write(0x2980,bytes([3,0])*36)
                # Calls the native restore routine, including full map redraw.
                # No framebuffer or expected checkpoint bytes are supplied.
                shim(r,sy,a.lwasm,'checkpoint',f'        jsr ${sy["checkpoint_restore"]:04x}\n')
                assert word(r,sy,'camera_y')==restored,(segment,word(r,sy,'camera_y'),restored)
                assert word(r,sy,'scroll_steps')==segment*192
                assert val(r,sy,'checkpoint_segment')==segment
                assert val(r,sy,'wave_index')==REF['checkpoint_wave_rewinds'][13]
                assert val(r,sy,'player_x')==120 and val(r,sy,'player_y')==135
                for field in ['weapon_kind','speed_level','ability_kind','shield_hits','player_invul','combat_status','respawn_pending']:assert val(r,sy,field)==0,(segment,field)
                assert word(r,sy,'ability_timer')==word(r,sy,'freeze_ticks')==0
                assert word(r,sy,'score')==1234 and word(r,sy,'high_score')==4321 and val(r,sy,'player_lives')==2
                empty=bytearray(19*32)
                if segment==9:
                    for i in range(3):empty[i*32+3]=11+i;empty[i*32+5]=21+i
                assert r.read(0x2700,19*32)==empty
                assert list(r.read(sy['satellite_clocks'],3))==REF['boss_checkpoint_observation']['satellite_clocks']
                expected_token=sum(t['world_y']>restored for t in ITEMS['tokens'][0])
                assert val(r,sy,'token_index')==expected_token
                states=r.read(0x2980,72)
                for i,item in enumerate(ITEMS['objects']):
                    state=5 if item['passage'] else 6 if item['type']&31==21 else int(item['visible'])
                    assert states[i*2:i*2+2]==bytes([state,item['hp']]),(segment,i)
                if segment==9:
                    for field,value in [('boss_state',1),('boss_hp',7),('boss_x',100),('boss_y',32),('boss_wait',37),('boss_target',88),('boss_velocity',255)]:assert val(r,sy,field)==value,(field,val(r,sy,field))
                    assert word(r,sy,'boss_lifetime')==1234
                else:assert val(r,sy,'boss_state')==0
                image=picture(r,sy,f'{name}-checkpoint-{segment}')
                cases.append(dict(segment=segment,camera_before=camera,camera_after=restored,framebuffer_sha256=image,boss_state_retained=segment==9))
            # All possible next-wave indices, against actual $549A execution.
            rewinds=[]
            for index,want in enumerate(REF['checkpoint_wave_rewinds']):
                r.write(sy['camera_y'],(1680).to_bytes(2,'big'));r.write(sy['wave_index'],bytes([index]))
                shim(r,sy,a.lwasm,'rewind',f'        jsr ${sy["checkpoint_restore"]:04x}\n')
                actual=val(r,sy,'wave_index');assert actual==want,(index,actual,want);rewinds.append(actual)
            # Resume the real game and lose a life through an actual bullet hit.
            r.cmd(f'P8={sy["start"]:04x}');call(r,sy,'main_loop');enter_game(r,sy)
            from verify_combat import slot,META
            slot(r,10,8,val(r,sy,'player_x')+8,val(r,sy,'player_y')+8,META['ids']['bullet'])
            tick(r,sy);assert val(r,sy,'combat_status')==1 and val(r,sy,'player_lives')==2
            for _ in range(47):tick(r,sy)
            assert val(r,sy,'combat_status')==1
            tick(r,sy);assert val(r,sy,'combat_status')==0 and val(r,sy,'player_lives')==2
            assert word(r,sy,'camera_y')==1776 and val(r,sy,'player_invul')==0
            result=dict(status='native checkpoint scenarios passed',variant=name,ram_kib=128,
                        tested_disk_sha256=hashlib.sha256((ROOT/f'bin/{name}.DSK').read_bytes()).hexdigest(),
                        segments=cases,wave_rewinds=rewinds,real_death_interval_native_ticks=48,
                        scope='ten segment restoration fixtures, native full redraws, all 34 ROM rewind cases and actual hostile-shot death/respawn; not a full campaign playthrough')
            (ROOT/f'analysis/{name}-checkpoints-verification.json').write_text(json.dumps(result,indent=2),encoding='utf-8')
            print(f'{name}: ten segment redraws, 34 ROM rewinds and real death/respawn passed',flush=True)
        finally:
            r.s.close();p.terminate();p.wait(timeout=10);log.close()

if __name__=='__main__':main()
