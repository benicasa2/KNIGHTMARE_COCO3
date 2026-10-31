from pathlib import Path
import argparse,json,sys,hashlib
OUT=Path(__file__).resolve().parents[1];ROOT=OUT.parents[1];sys.path.insert(0,str(OUT/'tools'))
from verify_ai import boot,shim
from verify_probe import symbols
from verify_controls_scroll import call,enter_game
from prepare_ai import machine
ap=argparse.ArgumentParser();ap.add_argument('--variant',default='DAC6');ap.add_argument('--rom',type=Path,default=ROOT/'work/knightmare.rom');ap.add_argument('--lwasm',type=Path,default=Path('C:/Users/Anderson/Documents/Codex/2026-10-25/ol/work/tools/lwtools/lwtools-4.25-win64/lwasm.exe'));ap.add_argument('--xroar',type=Path,default=Path('C:/Users/Anderson/Downloads/xroar-1.12-w64/xroar.exe'));a=ap.parse_args();name=a.variant
raw=a.rom.read_bytes();sy=symbols(name);lw=a.lwasm
p,log,r=boot(a.xroar,name)
try:
 call(r,sy,'main_loop');enter_game(r,sy);cases=[]
 for kind in [0,3,4,7,11]:
  for x in [40,128,216]:
   y=136 if kind==3 else 1 if kind==11 else 240
   z=machine(raw);z.ix=0xe100;z.mem[0xe100]=1;z.mem[0xe101]=kind;z.mem[0xe103]=y;z.mem[0xe105]=x;z.mem[0xe111]=255
   expected=[]
   for t in range(180):
    z.mem[0xe003]=(2*t+1)&255;z.corre(0x75e5);q=z.mem[0xe100:0xe120]
    expected.append(dict(state=q[0],x=q[5]*256+q[4],y=q[3]*256+q[2],vx=q[9]*256+q[8],vy=q[7]*256+q[6],clock=q[14],shot=q[15],hp=q[20] if kind==7 else q[11]))
   st=bytearray(32);st[0]=1;st[1]=kind;st[2]=x;st[4]=y;st[12]=1;st[15]=255;r.write(0x2700,st)
   body=f'''        orcc #$50
        lda #12
        sta $ffa4
        ldy #$8000
        sty $0be0
        clr $0be2
phase2_sample
        lda $0be2
        lsla
        inca
        sta ${sy['ai_clock']:04x}
        ldx #$2700
        lda ,x
        cmpa #1
        bne phase2_copy
        jsr ${sy['ai_enemy_tick']:04x}
phase2_copy
        ldx #$2700
        ldy $0be0
        ldb #32
phase2_copy_byte
        lda ,x+
        sta ,y+
        decb
        bne phase2_copy_byte
        sty $0be0
        inc $0be2
        lda $0be2
        cmpa #180
        blo phase2_sample
'''
   shim(r,sy,lw,'phase2_ai',body);data=r.read(0x8000,180*32)
   for t,want in enumerate(expected):
    q=data[t*32:(t+1)*32];got=dict(state=q[12] if q[0] else 0,x=int.from_bytes(q[2:4],'big'),y=int.from_bytes(q[4:6],'big'),vx=int.from_bytes(q[6:8],'big'),vy=int.from_bytes(q[8:10],'big'),clock=q[27],shot=q[13],hp=q[10])
    fields=['state','x','y','vx','vy','hp']+(['clock'] if kind==0 else [])+(['shot'] if kind in [3,4,7,11] else [])
    assert all(got[k]==want[k] for k in fields),dict(kind=kind,x=x,tick=t,got=got,want=want)
   print(name,kind,x,'passed',flush=True);cases.append(dict(kind=kind,x=x,samples=180,sha256=hashlib.sha256(data).hexdigest()))
 (OUT/f'analysis/{name}-phase2-ai-verification.json').write_text(json.dumps(dict(tested_disk_sha256=hashlib.sha256((OUT/f'bin/{name}.DSK').read_bytes()).hexdigest(),tested_levels_sha256=hashlib.sha256((OUT/'bin/LEVELS.DSK').read_bytes()).hexdigest(),status='phase2 AI trajectories passed',cases=cases),indent=2))
finally:r.s.close();p.terminate();p.wait(timeout=10);log.close()
