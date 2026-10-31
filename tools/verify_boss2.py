from pathlib import Path
import argparse,json,sys,hashlib
OUT=Path(__file__).resolve().parents[1];ROOT=OUT.parents[1];sys.path.insert(0,str(OUT/'tools'))
from verify_ai import boot,shim
from verify_probe import symbols
from verify_controls_scroll import call,enter_game
from prepare_ai import machine
ap=argparse.ArgumentParser();ap.add_argument('--variant',default='DAC6',choices=['DAC6','ORCH90']);ap.add_argument('--rom',type=Path,default=ROOT/'work/knightmare.rom');ap.add_argument('--lwasm',type=Path,default=Path('C:/Users/Anderson/Documents/Codex/2026-10-25/ol/work/tools/lwtools/lwtools-4.25-win64/lwasm.exe'));ap.add_argument('--xroar',type=Path,default=Path('C:/Users/Anderson/Downloads/xroar-1.12-w64/xroar.exe'));a=ap.parse_args();name=a.variant;sy=symbols(name);lw=a.lwasm
raw=a.rom.read_bytes();z=machine(raw);z.mem[0xe600]=2;z.mem[0xe3b0]=4;z.mem[0xe3b3]=64;z.mem[0xe3b4]=1;z.mem[0xe3b7]=32;z.mem[0xe3b8]=120;z.mem[0xe3d3]=32;z.mem[0xe3d5]=120;z._escribir16(0xe3d6,0x100);z._escribir16(0xe3d8,0x180);z._escribir16(0xe3b1,3600)
expected=[]
for t in range(240):
 for sub in range(2):
  z.mem[0xe003]=(2*t+sub+1)&255;z.mem[0xe090]=(2*t+sub+1)&255;z.corre(0x8e34)
 drops=[]
 for i in range(5):
  q=z.mem[0xe200+i*16:0xe210+i*16]
  drops.append(dict(state=q[0],x=q[5]*256+q[4],y=q[3]*256+q[2],vx=q[9]*256+q[8],vy=q[7]*256+q[6],ty=q[12],tx=q[13],phase=q[14]))
 expected.append(dict(x=z._leer16(0xe3d4),y=z._leer16(0xe3d2),vx=z._leer16(0xe3d8),vy=z._leer16(0xe3d6),shot=z.mem[0xe3cf],life=z._leer16(0xe3b1),drops=drops))
p,log,r=boot(a.xroar,name)
try:
 call(r,sy,'main_loop');enter_game(r,sy)
 for field,value in [('campaign_stage',1),('boss_state',1),('boss_phase',0),('boss_x',120),('boss_y',32),('boss_hp',30)]:r.write(sy[field],bytes([value]))
 r.write(sy['B2_XFRAC'],bytes(2));r.write(sy['B2_SHOT'],bytes([0]));r.write(sy['B2_VX'],(0x180).to_bytes(2,'big'));r.write(sy['B2_VY'],(0x100).to_bytes(2,'big'));r.write(sy['boss_lifetime'],(3600).to_bytes(2,'big'));r.write(0xbe4,bytes(2))
 for batch in range(6):
  n=40;body=f'''        orcc #$50
        lda #12
        sta $ffa4
        ldy #$8000
        sty $0be0
        clr $0be2
boss2_trace_loop
        lda $0be5
        lsla
        inca
        sta ${sy['ai_clock']:04x}
        jsr ${sy['boss2_update']:04x}
        ldy $0be0
        lda ${sy['boss_x']:04x}
        ldb ${sy['B2_XFRAC']:04x}
        std ,y++
        lda ${sy['boss_y']:04x}
        ldb ${sy['B2_YFRAC']:04x}
        std ,y++
        ldd ${sy['B2_VX']:04x}
        std ,y++
        ldd ${sy['B2_VY']:04x}
        std ,y++
        lda ${sy['B2_SHOT']:04x}
        sta ,y+
        ldd ${sy['boss_lifetime']:04x}
        std ,y++
        ldx #$2840
        ldb #160
boss2_copy_drops
        lda ,x+
        sta ,y+
        decb
        bne boss2_copy_drops
        sty $0be0
        inc $0be5
        inc $0be2
        lda $0be2
        cmpa #40
        lblo boss2_trace_loop
'''
  shim(r,sy,lw,'boss2_trace',body);data=r.read(0x8000,n*171)
  for i in range(n):
   q=data[i*171:(i+1)*171];want=expected[batch*n+i];got={k:int.from_bytes(q[j:j+2],'big') for k,j in [('x',0),('y',2),('vx',4),('vy',6),('life',9)]};got['shot']=q[8]
   assert all(got[k]==want[k] for k in got),dict(tick=batch*n+i,got=got,want=want)
   for j,w in enumerate(want['drops']):
    s=q[11+j*32:43+j*32];d=dict(state=s[0],x=int.from_bytes(s[2:4],'big'),y=int.from_bytes(s[4:6],'big'),vx=int.from_bytes(s[6:8],'big'),vy=int.from_bytes(s[8:10],'big'),ty=s[24],tx=s[25],phase=s[26])
    assert d['state']==w['state'] and (not w['state'] or d==w),dict(tick=batch*n+i,slot=j,got=d,want=w,previous_native=data[(i-1)*171+11+j*32:(i-1)*171+43+j*32].hex(),previous_original=expected[batch*n+i-1]['drops'][j],boss=got)
  print('boss2',batch*n,'..',(batch+1)*n,'passed',flush=True)
 (OUT/f'analysis/{name}-phase2-boss-verification.json').write_text(json.dumps(dict(tested_disk_sha256=hashlib.sha256((OUT/f'bin/{name}.DSK').read_bytes()).hexdigest(),tested_levels_sha256=hashlib.sha256((OUT/'bin/LEVELS.DSK').read_bytes()).hexdigest(),status='phase2 boss motion and five dropped pieces passed',native_updates=240,original_frames=480),indent=2))
finally:r.s.close();p.terminate();p.wait(timeout=10);log.close()
