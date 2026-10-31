"""Build original eight-stage resource packs and native 6809 loader laboratory.

This does not translate the later enemy or boss handlers. Those are explicitly
identified in the generated manifest, never replaced with the first-stage boss.
"""
from pathlib import Path
import argparse, hashlib, json, struct, subprocess
from build import disk_image

ROOT = Path(__file__).resolve().parents[1]
SHA = '589b2fd276d84827bb1b033f43a0cd5ab0aa9da3c8784cac2f62474fed00aae3'


def pack(data):
    out = bytearray()
    i = 0
    while i < len(data):
        n = 1
        while i+n < len(data) and data[i+n] == data[i] and n < 128:
            n += 1
        if n >= 3:
            out.extend([0x80 | (n-1), data[i]])
            i += n
        else:
            begin = i
            i += n
            while i < len(data) and i-begin < 128:
                if data[i:i+3] == data[i:i+1]*3:
                    break
                i += 1
            out.append(i-begin-1)
            out.extend(data[begin:i])
    return bytes(out)


def unpack(data, size):
    out = bytearray()
    i = 0
    while len(out) < size:
        ctl = data[i]
        i += 1
        n = (ctl & 127)+1
        if ctl & 128:
            out.extend(data[i:i+1]*n)
            i += 1
        else:
            out.extend(data[i:i+n])
            i += n
    assert len(out) == size
    return bytes(out), i


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--rom', required=True, type=Path)
    ap.add_argument('--lwasm', required=True, type=Path)
    a = ap.parse_args()
    rom = a.rom.read_bytes()
    assert hashlib.sha256(rom).hexdigest() == SHA
    def b(addr): return rom[addr-0x4000]
    def w(addr): return b(addr) | b(addr+1) << 8
    items = json.loads((ROOT/'analysis/items-assets.json').read_text())
    assert items['source_sha256'] == SHA
    base = ROOT/'campaign'
    for d in ['assets', 'bin', 'analysis', 'src']:
        (base/d).mkdir(parents=True, exist_ok=True)
    disk = bytearray()
    profiles = []
    directory = []
    boss_names = ['winged first guardian', 'three-ball guardian',
                  'mouth guardian', 'fragment guardian',
                  'first guardian variant 5', 'first guardian variant 6',
                  'six-segment serpent', 'six-gate final encounter']
    for stage in range(8):
        wave_ptr = w(0x704b+stage*2)
        count = b(0x7043+stage)
        waves = bytes(b(wave_ptr+i) for i in range(count*2))
        boss = w(0x88f2+stage*2)
        hp = b(0x9992+stage) if stage < 7 else None
        objects = items['all_stage_objects'][stage]
        tokens = items['tokens'][stage]
        script = bytearray(b'STGP'+bytes([1, stage, count, len(tokens), len(objects)]))
        script += struct.pack('>HB', boss, hp or 0)
        script += waves
        for t in tokens:
            script += struct.pack('>HB', t['world_y'], t['family'])
        for o in objects:
            script += struct.pack('>HBBBB', o['world_y'], o['x'], o['type'], o['hp'], o['kind'])
        patterns = (ROOT/f'assets/stages/stage{stage+1}-patterns.bin').read_bytes()
        colors = (ROOT/f'assets/stages/stage{stage+1}-colors.bin').read_bytes()
        tiles = bytearray()
        for tile in range(256):
            for y in range(8):
                bits = patterns[tile*8+y]
                col = colors[tile*8+y]
                row = [col >> 4 if bits & (128 >> x) else col & 15 for x in range(8)]
                tiles.extend(row[x]*16+row[x+1] for x in range(0, 8, 2))
        tilemap = (ROOT/f'assets/stages/stage{stage+1}-map.bin').read_bytes()
        resource_data = [(7, 'map', tilemap), (12, 'tiles', bytes(tiles)), (255, 'script', bytes(script))]
        pack_data = bytearray(b'KMST'+bytes([1, stage, 3]))
        resources = []
        for bank, name, raw in resource_data:
            encoded = pack(raw)
            assert unpack(encoded, len(raw)) == (raw, len(encoded))
            pack_data += bytes([bank])+struct.pack('>H', len(raw))+encoded
            filename = f'assets/stage{stage+1}-{name}.bin'
            (base/filename).write_bytes(raw)
            resources.append(dict(bank=bank, path=filename, bytes=len(raw),
                                  packed_bytes=len(encoded), sha256=hashlib.sha256(raw).hexdigest()))
        directory.append(len(disk)//256)
        disk += pack_data
        disk += bytes((-len(disk)) % 256)
        (base/f'assets/stage{stage+1}.kmst').write_bytes(pack_data)
        profiles.append(dict(stage=stage+1, start_lsn=directory[-1], packed_bytes=len(pack_data),
                             wave_pointer=f'${wave_ptr:04X}', wave_count=count,
                             enemy_types=sorted({waves[i] & 15 for i in range(0,len(waves),2) if waves[i]}),
                             fixed_objects=len(objects), tokens=len(tokens),
                             boss_handler=f'${boss:04X}', boss_family=boss_names[stage], boss_hp=hp,
                             final_gates_hp=[64,20,20,20,20,20] if stage == 7 else None,
                             native_gameplay_complete=False, resources=resources))
    used = len(disk)
    assert used <= 35*18*256
    disk += bytes(35*18*256-len(disk))
    (base/'bin/CAMPAIGN.DSK').write_bytes(disk)
    (base/'src/stage_directory.inc').write_text('stage_directory\n        fdb '+','.join(map(str, directory))+'\n')
    subprocess.run([str(a.lwasm), '--format=decb', '--output='+str(base/'bin/LOADER.BIN'),
                    '--symbol-dump='+str(base/'analysis/LOADER.sym'),
                    '--list='+str(base/'analysis/LOADER.lst'), 'stage_loader.asm'],
                   cwd=base/'src', check=True)
    basic = b'10 CLEAR 200,&H2BFF\r20 PRINT "LABORATORIO DE RECURSOS - 8 FASES"\r30 PRINT "DISCO CAMPAIGN NO DRIVE 1"\r40 LOADM"LOADER":EXEC\r'
    boot = disk_image([('START.BAS',0,255,basic),('LOADER.BIN',2,0,(base/'bin/LOADER.BIN').read_bytes())])
    (base/'bin/BOOT.DSK').write_bytes(boot)
    report = dict(source_rom_sha256=SHA, status='resource streaming laboratory; NOT playable campaign',
                  resource_disk_used_bytes=used, resource_disk_capacity_bytes=len(disk),
                  resource_disk_sha256=hashlib.sha256(disk).hexdigest(),
                  boot_disk_sha256=hashlib.sha256(boot).hexdigest(), ram_kib=128,
                  format='KMST v1, PackBits, big-endian native words; stage scripts STGP v1', stages=profiles)
    (base/'analysis/stages.json').write_text(json.dumps(report, indent=2), encoding='utf-8')
    print(json.dumps({k:v for k,v in report.items() if k != 'stages'}, indent=2))


if __name__ == '__main__':
    main()
