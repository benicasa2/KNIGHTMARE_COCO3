"""Differential tests: actual Z80 reference vs actual native 6809 in XRoar.

Harness code only calls game routines and records their output in an unused
tile-cache bank (only within this isolated AI test process). It never supplies expected positions/velocities to the DUT.
"""
from pathlib import Path
import argparse,json,socket,subprocess,time,hashlib,struct
from verify_probe import RSP,symbols
from verify_controls_scroll import call,enter_game,val,word
from prepare_ai import machine
ROOT=Path(__file__).resolve().parents[1]
REF=json.loads((ROOT/'analysis/ai-rom-reference.json').read_text())

def boot(exe,name):
    with socket.socket() as s:s.bind(('127.0.0.1',0));port=s.getsockname()[1]
    log=(ROOT/f'analysis/{name}-ai-xroar.log').open('wb')
    args=[str(exe),'-machine','coco3','-ram','128','-no-bas','-extbas',str(exe.parent/'coco3.rom'),'-cart','rsdos','-cart-rom',str(exe.parent/'disk11.rom')]
    if name=='ORCH90':args+=['-cart','mpi','-mpi-slot','3','-mpi-load-cart','0=orch90','-mpi-load-cart','3=rsdos']
    args+=['-load-fd0',str(ROOT/f'bin/{name}.DSK'),'-load-fd1',str(ROOT/'bin/LEVELS.DSK'),'-type','RUN"START"\r','-no-ratelimit','-tv-input','rgb','-ui','null','-ao','null','-gdb','-gdb-port',str(port),'-no-disk-write-back']
    p=subprocess.Popen(args,stdout=log,stderr=log)
    try:
        for _ in range(100):
            try:return p,log,RSP(port)
            except OSError:time.sleep(.1)
        raise RuntimeError('XRoar debugger unavailable')
    except Exception:
        p.terminate();p.wait(timeout=10);log.close();raise

def shim(r,sy,lwasm,name,body):
    base=ROOT/'analysis/ai-harness';base.mkdir(exist_ok=True)
    source='        org $0b00\n'+body+'\nharness_done\n        bra harness_done\n'
    (base/f'{name}.asm').write_text(source,encoding='ascii')
    subprocess.run([str(lwasm),'--format=raw','--output='+str(base/f'{name}.bin'),'--symbol-dump='+str(base/f'{name}.sym'),str(base/f'{name}.asm')],check=True,capture_output=True)
    code=(base/f'{name}.bin').read_bytes();assert len(code)<0xe0
    done=0xb00+len(code)-2
    r.write(0xb00,code);r.cmd('P7=2fff');r.cmd('P8=0b00');r.bp(done);r.go();r.unbp(done)
    return done

def verify_boss(r,sy,lwasm):
    for offset in range(0,19*32,256):r.write(0x2700+offset,bytes(min(256,19*32-offset)))
    for field,value in [('boss_state',1),('boss_hp',20),('boss_x',112),('boss_y',32),('boss_phase',0),('boss_wait',0),('satellite_side',0)]:r.write(sy[field],bytes([value]))
    r.write(sy['boss_clock'],b'\0\0');r.write(sy['boss_lifetime'],(3600).to_bytes(2,'big'));r.write(sy['satellite_clocks'],bytes(3));r.write(sy['player_x'],b'\x78');r.write(sy['player_y'],b'\x87')
    shim(r,sy,lwasm,'target',f'        jsr ${sy["boss_choose_target"]:04x}\n')
    r.write(0xbe4,b'\0\0')
    hashes=[]
    for batch in range(5):
        body=f'''        lda #12
        sta $ffa4
        ldy #$8000
        sty $0be0
        clr $0be2
boss_sample_loop
        lda $0be5
        lsla
        inca
        sta ${sy['ai_clock']:04x}
        lda $0be5
        anda #127
        adda #64
        sta ${sy['player_x']:04x}
        jsr ${sy['update_boss']:04x}
        inc ${sy['ai_clock']:04x}
        jsr ${sy['move_enemies']:04x}
        dec ${sy['ai_clock']:04x}
        ldy $0be0
        ldx #${sy['boss_state']:04x}
        ldb #8
boss_copy
        lda ,x+
        sta ,y+
        decb
        bne boss_copy
'''
        for label,size in [('boss_phase',1),('boss_lifetime',2),('boss_target',1),('boss_velocity',1),('satellite_clocks',3)]:
            for i in range(size):body+=f'        lda ${sy[label]+i:04x}\n        sta ,y+\n'
        body+='''        ldx #$2700
        ldb #96
sat_copy
        lda ,x+
        sta ,y+
        decb
        bne sat_copy
        sty $0be0
        ldd $0be4
        addd #1
        std $0be4
        inc $0be2
        lda $0be2
        cmpa #64
        lblo boss_sample_loop
'''
        shim(r,sy,lwasm,'boss',body)
        data=r.read(0x8000,64*112);hashes.append(hashlib.sha256(data).hexdigest())
        for i in range(64):
            expected=REF['boss_trace'][batch*64+i];q=data[i*112:(i+1)*112]
            actual=dict(x=q[2],y=q[3],wait=q[4],target=q[11],velocity=q[12],lifetime=int.from_bytes(q[9:11],'big'),satellite_clocks=list(q[13:16]))
            assert all(actual[k]==expected[k] for k in actual),dict(tick=batch*64+i,actual=actual,expected=expected)
            for sat in range(3):
                z=q[16+sat*32:48+sat*32];want=expected['satellites'][sat]
                got=dict(state=z[12] if z[0] else 0,x=int.from_bytes(z[2:4],'big'),y=int.from_bytes(z[4:6],'big'),vx=int.from_bytes(z[6:8],'big'),vy=int.from_bytes(z[8:10],'big'),hp=z[10],target_y=z[24],fraction=z[25])
                if not want['state']:assert got['state']==0,(batch,i,sat,got,want)
                else:assert got==want,(batch,i,sat,got,want)
    return dict(native_samples=320,original_boss_frames=640,three_satellites_compared=True,sha256_batches=hashes)

def verify_waves(r,sy,lwasm):
    for offset in range(0,19*32,256):r.write(0x2700+offset,bytes(min(256,19*32-offset)))
    r.write(sy['wave_index'],b'\0');r.write(sy['player_x'],b'\x78');r.write(sy['player_y'],b'\x87')
    body=f'''        jsr ${sy['ai_init']:04x}
        lda #12
        sta $ffa4
        ldy #$8000
        sty $0be0
        clr $0be4
        clr $0be5
wave_sample_loop
        lda $0be5
        lsla
        inca
        sta ${sy['ai_clock']:04x}
        ldd $0be4
        lsra
        rorb
        lsra
        rorb
        std ${sy['scroll_steps']:04x}
        jsr ${sy['spawn_waves']:04x}
        ldx #$2700
        clr $0be6
wave_log_slot
        lda ,x
        cmpa #1
        bne wave_log_next
        lda 12,x
        cmpa #1
        bne wave_log_next
        ldy $0be0
        ldd $0be4
        std ,y++
        lda $0be6
        sta ,y+
        lda 26,x
        sta ,y+
        lda 15,x
        sta ,y+
        ldd 2,x
        std ,y++
        ldd 4,x
        std ,y++
        sty $0be0
wave_log_next
        leax 32,x
        inc $0be6
        lda $0be6
        cmpa #7
        blo wave_log_slot
        jsr ${sy['move_enemies']:04x}
        ldd $0be4
        addd #1
        std $0be4
        cmpd #{REF['wave_trace']['ticks']}
        lblo wave_sample_loop
'''
    shim(r,sy,lwasm,'waves',body)
    end=int.from_bytes(r.read(0xbe0,2),'big');assert 0x8000<=end<0xa000
    data=r.read(0x8000,end-0x8000);events=[]
    for i in range(0,len(data),9):
        q=data[i:i+9];events.append(dict(tick=int.from_bytes(q[:2],'big'),slot=q[2],raw_type=q[3],member=q[4],x=int.from_bytes(q[5:7],'big'),y=int.from_bytes(q[7:9],'big')))
    want=REF['wave_trace']['events']
    for i,(actual,expected) in enumerate(zip(events,want)):
        assert actual==expected,dict(event=i,actual=actual,expected=expected)
    assert len(events)==len(want),(len(events),len(want))
    assert r.read(sy['wave_index'],1)[0]==REF['wave_trace']['next_wave']
    return dict(native_updates=REF['wave_trace']['ticks'],births_compared=len(events),next_wave=REF['wave_trace']['next_wave'],sha256=hashlib.sha256(data).hexdigest())

def verify_shots(r,sy,lwasm,raw):
    items=json.loads((ROOT/'analysis/items-assets.json').read_text())
    configs=[]
    for kind in [1,2,9,14]:
        for flags in [0,128,192]:
            for member in [255,0,1]:configs.append((kind,flags,member,64,40))
        for x,y in [(64,192),(64,120),(105,40),(64,119),(104,40)]:configs.append((kind,128,255,x,y))
    rows=[]
    for kind,flags,member,x,y in configs:
        clock=REF['shot_schedules'][kind][0]-1
        z=machine(raw);z.ix=0xe100
        for offset,value in [(1,kind|flags),(3,y),(5,x),(15,clock),(17,member)]:z.mem[0xe100+offset]=value
        z.corre(0x7633)
        state=bytearray(32);state[0]=1;state[1]=kind;state[2]=x;state[4]=y;state[13]=clock;state[15]=member;state[26]=kind|flags
        r.write(0x2700,state);r.write(0x2840,bytes(256));r.write(sy['player_x'],b'\x78');r.write(sy['player_y'],b'\x87')
        shim(r,sy,lwasm,'fire',f'        ldx #$2700\n        jsr ${sy["ai_enemy_fire"]:04x}\n')
        bullets=r.read(0x2840,256);actual=[];expected=[]
        for i in range(8):
            q=bullets[i*32:(i+1)*32]
            if q[0]:actual.append(dict(kind=q[1],x=int.from_bytes(q[2:4],'big'),y=int.from_bytes(q[4:6],'big'),vx=int.from_bytes(q[6:8],'big'),vy=int.from_bytes(q[8:10],'big')))
            q=z.mem[0xe200+i*16:0xe210+i*16]
            if q[0]:expected.append(dict(kind=q[1],x=q[5]*256+q[4],y=q[3]*256+q[2],vx=q[9]*256+q[8],vy=q[7]*256+q[6]))
        assert actual==expected,dict(kind=kind,flags=flags,member=member,x=x,y=y,actual=actual,expected=expected)
        if actual:
            shot=actual[0]['kind'];art=bullets[14]
            if shot in [2,3]:assert art==items['small_enemy_bullet_art']
            else:assert art in items['directional_enemy_bullet_art']
        rows.append(dict(enemy_kind=kind,flags=flags,member=member,source=[x,y],projectiles=actual))
    return rows

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--xroar',type=Path,required=True);ap.add_argument('--lwasm',type=Path,required=True);ap.add_argument('--rom',type=Path,required=True);ap.add_argument('--variant',choices=['DAC6','ORCH90']);a=ap.parse_args()
    raw=a.rom.read_bytes();assert hashlib.sha256(raw).hexdigest()==REF['source_rom_sha256']
    for name in ([a.variant] if a.variant else ['DAC6','ORCH90']):
        p,log,r=boot(a.xroar,name)
        try:
            sy=symbols(name);call(r,sy,'main_loop');r.cmd(f'P8={sy["start"]:04x}');call(r,sy,'main_loop');enter_game(r,sy)
            checks=[]
            for case in REF['trajectories']:
                state=bytearray(32);state[0]=1;state[1]=case['kind'];state[2]=case['x'];state[4]=case['y'];state[12]=1;state[15]=255
                r.write(0x2700,state);r.write(sy['player_x'],b'\x78');r.write(sy['player_y'],b'\x87')
                body=f'''        lda #12
        sta $ffa4
        ldy #$8000
        sty $0be0
        clr $0be2
sample_loop
        lda $0be2
        lsla
        inca
        sta ${sy['ai_clock']:04x}
        ldx #$2700
        lda ,x
        cmpa #1
        bne sample_copy
        jsr ${sy['ai_enemy_tick']:04x}
sample_copy
        ldx #$2700
        ldy $0be0
        ldb #32
copy_loop
        lda ,x+
        sta ,y+
        decb
        bne copy_loop
        sty $0be0
        inc $0be2
        lda $0be2
        cmpa #{len(case['rows'])}
        blo sample_loop
'''
                shim(r,sy,a.lwasm,'enemies',body)
                data=r.read(0x8000,len(case['rows'])*32)
                for t,expected in enumerate(case['rows']):
                    row=data[t*32:(t+1)*32]
                    actual=dict(state=row[12] if row[0] else 0,x=int.from_bytes(row[2:4],'big'),y=int.from_bytes(row[4:6],'big'),vx=int.from_bytes(row[6:8],'big'),vy=int.from_bytes(row[8:10],'big'),hp=row[10],target_x=row[16],target_y=row[24],clock=row[27],shot_clock=row[13])
                    # Only type 9 uses +14 as a state timer; other types use it
                    # for unrelated counters cleared by their initializers.
                    fields=['state','x','y','vx','vy','hp','target_x','target_y','shot_clock']
                    if case['kind']==9:fields+=['clock']
                    assert all(actual[k]==expected[k] for k in fields),dict(kind=case['kind'],x=case['x'],tick=t,actual=actual,expected=expected)
                checks.append(dict(kind=case['kind'],initial_x=case['x'],samples=len(case['rows']),sha256=hashlib.sha256(data).hexdigest()))
            print(f'{name}: 5400 native enemy samples match executed Z80',flush=True)
            angles=[]
            for y,x,py,px in [(y,x,135,120) for y in [0,40,100,151,240] for x in [16,60,128,192,240]]+[(151,136,135,120),(151,120,135,120),(100,136,135,120)]:
                z=machine(raw);z.ix=0xe100;z.mem[0xe103]=y;z.mem[0xe105]=x;z.mem[0xe602]=py;z.mem[0xe604]=px;z.corre(0x84e0)
                r.write(sy['aim_x'],bytes([x]));r.write(sy['aim_y'],bytes([y]));r.write(sy['player_x'],bytes([px]));r.write(sy['player_y'],bytes([py]))
                shim(r,sy,a.lwasm,'aim',f'        jsr ${sy["ai_aim_angle"]:04x}\n        sta $0be4\n')
                actual=r.read(0xbe4,1)[0];assert actual==z.a,(y,x,actual,z.a)
                angles.append(dict(source=[x,y],angle=actual))
            boss=verify_boss(r,sy,a.lwasm)
            print(f'{name}: 640 original boss frames and three satellite traces match',flush=True)
            waves=verify_waves(r,sy,a.lwasm)
            print(f'{name}: {waves["births_compared"]} wave births match over 6000 updates',flush=True)
            shots=verify_shots(r,sy,a.lwasm,raw)
            print(f'{name}: {len(shots)} firing condition/type/velocity cases match',flush=True)
            result=dict(status='native AI differential scenarios passed',variant=name,ram_kib=128,
                        tested_disk_sha256=hashlib.sha256((ROOT/f'bin/{name}.DSK').read_bytes()).hexdigest(),
                        source_rom_sha256=REF['source_rom_sha256'],enemy_trajectories=checks,aiming=angles,boss=boss,waves=waves,firing=shots,
                        scope='actual native enemy routines vs executed Z80; fixtures, not a complete gameplay equivalence test')
            (ROOT/f'analysis/{name}-ai-verification.json').write_text(json.dumps(result,indent=2),encoding='utf-8')
            print(f'{name}: {len(angles)} native aiming scenarios match executed Z80',flush=True)
        finally:
            r.s.close();p.terminate();p.wait(timeout=10);log.close()

if __name__=='__main__':main()
