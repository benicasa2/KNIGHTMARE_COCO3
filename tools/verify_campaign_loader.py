"""Execute native FDC/MMU/PackBits loader in a 128-KiB XRoar CoCo 3.

Only reads emulator RAM for verification; never injects decoded resources.
This is a resource test, not a gameplay or boss equivalence test.
"""
from pathlib import Path
import argparse, hashlib, json, re, socket, subprocess, time
from verify_probe import RSP
from render_msx import png

ROOT = Path(__file__).resolve().parents[1]
BASE = ROOT/'campaign'


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--xroar', required=True, type=Path)
    a = ap.parse_args()
    exe = a.xroar
    sy = {m[1]:int(m[2],16) for m in re.finditer(r'(?m)^(\w+) EQU \$([0-9a-fA-F]+)', (BASE/'analysis/LOADER.sym').read_text())}
    meta = json.loads((BASE/'analysis/stages.json').read_text())
    with socket.socket() as s:
        s.bind(('127.0.0.1',0))
        port = s.getsockname()[1]
    log = (BASE/'analysis/loader-xroar.log').open('wb')
    p = subprocess.Popen([str(exe), '-machine','coco3','-ram','128','-no-bas',
                          '-extbas',str(exe.parent/'coco3.rom'), '-cart','rsdos',
                          '-cart-rom',str(exe.parent/'disk11.rom'),
                          '-load-fd0',str(BASE/'bin/BOOT.DSK'),
                          '-load-fd1',str(BASE/'bin/CAMPAIGN.DSK'),
                          '-type','RUN"START"\r','-no-ratelimit','-tv-input','rgb',
                          '-ui','null','-ao','null','-gdb','-gdb-port',str(port),
                          '-no-disk-write-back'], stdout=log, stderr=log)
    r = None
    try:
        for _ in range(100):
            try:
                r = RSP(port)
                break
            except OSError:
                time.sleep(.1)
        assert r
        r.s.settimeout(60)
        # XRoar runs while the debugger connects. First stop at the terminal
        # loop, then restart the actual CPU program to observe EVERY stage.
        r.bp(sy['waiting_for_space'])
        r.bp(sy['loader_failed'])
        r.go()
        assert r.read(sy['loader_error'],1) == b'\0'
        assert r.cmd(f'P8={sy["start"]:04x}') == 'OK'
        r.unbp(sy['waiting_for_space'])
        r.bp(sy['all_stages_loaded'])
        r.bp(sy['stage_loaded'])
        checks = []
        for stage in meta['stages']:
            r.go()
            error = r.read(sy['loader_error'],1)[0]
            assert error == 0, dict(stage=stage['stage'], loader_error=error, registers=r.cmd('g'))
            index = r.read(sy['stage_index'],1)[0]
            assert index == stage['stage']-1, (index, stage['stage'], r.cmd('g'))
            r.write(sy['auto_advance'],b'\x01')
            hashes = {}
            for res in stage['resources']:
                if res['bank'] != 255:
                    r.write(0xffa4,bytes([res['bank']]))
                    addr = 0x8000
                else:
                    addr = 0x2000
                actual = r.read(addr,res['bytes'])
                expected = (BASE/res['path']).read_bytes()
                assert actual == expected, (stage['stage'],res['path'],next((i for i,(x,y) in enumerate(zip(actual,expected)) if x!=y),None))
                hashes[res['path']] = hashlib.sha256(actual).hexdigest()
            # Three camera fixtures exercise the native map renderer independently
            # from the disk test. Host changes the camera, never framebuffer data.
            tilemap = (BASE/stage['resources'][0]['path']).read_bytes()
            tiles = (BASE/stage['resources'][1]['path']).read_bytes()
            screenshots = {}
            palette = r.read(0xffb0,16)
            rgb = [tuple((((c >> hi)&1)*2+((c >> lo)&1))*85 for hi,lo in [(5,2),(4,1),(3,0)]) for c in palette]
            for camera in [1760,888,0]:
                r.write(sy['camera_y'],camera.to_bytes(2,'big'))
                r.bp(sy['preview_release'])
                assert r.cmd(f'P8={sy["preview_render"]:04x}') == 'OK'
                r.go()
                r.unbp(sy['preview_release'])
                actual = bytearray()
                for bank in range(3):
                    r.write(0xffa4,bytes([bank]))
                    actual += r.read(0x8000,8192)
                expected = bytearray()
                for y in range(camera,camera+192):
                    for x in range(32):
                        tile = tilemap[(y//8)*32+x]
                        offset = tile*32+(y&7)*4
                        expected += tiles[offset:offset+4]
                assert actual == expected, ('raster',stage['stage'],camera)
                path = BASE/f'analysis/stage{stage["stage"]}-camera{camera}.png'
                png(path,[n for b in actual for n in (b>>4,b&15)],palette=rgb)
                screenshots[str(camera)] = hashlib.sha256(actual).hexdigest()
            assert r.cmd(f'P8={sy["stage_loaded"]:04x}') == 'OK'
            checks.append(dict(stage=stage['stage'], loaded_bytes=sum(x['bytes'] for x in stage['resources']), hashes=hashes, native_raster_sha256=screenshots))
            print(f"Stage {stage['stage']}: native map/tiles/script bytes verified", flush=True)
            r.step()  # advance off the NOP before resuming
        r.go()
        assert r.read(sy['loaded_stages'],1) == b'\x08'
        assert r.read(sy['loader_error'],1) == b'\0'
        result = dict(status='all eight stage resource packs loaded by native 6809', ram_kib=128,
                      test_scope='Disk BASIC boot; drive 1 WD2793 reads; native PackBits decoding; MMU writes; byte-for-byte verification; native renderer at three camera fixtures per stage. Not gameplay.',
                      host_injected_resource_bytes=0, resource_disk_sha256=meta['resource_disk_sha256'],
                      boot_disk_sha256=meta['boot_disk_sha256'], stages=checks)
        (BASE/'analysis/loader-verification.json').write_text(json.dumps(result,indent=2),encoding='utf-8')
        print(json.dumps(result,indent=2),flush=True)
    except Exception as error:
        if r:
            try:
                if isinstance(error, TimeoutError):
                    r.s.sendall(b'\x03')
                    r.recv()
                (BASE/'analysis/loader-diagnostic.json').write_text(json.dumps(dict(registers=r.cmd('g'), state=r.read(0x900,32).hex(), fdc=r.read(0xff48,4).hex()),indent=2))
            except Exception:
                pass
        raise
    finally:
        if r:
            r.s.close()
        p.terminate()
        p.wait(timeout=10)
        log.close()


if __name__ == '__main__':
    main()
