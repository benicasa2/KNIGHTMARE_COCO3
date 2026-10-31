from pathlib import Path
import argparse,json,sys,hashlib
OUT=Path(__file__).resolve().parents[1];ROOT=OUT.parents[1];sys.path.insert(0,str(OUT/'tools'))
from verify_ai import boot,shim
from verify_probe import symbols
from verify_controls_scroll import call,enter_game
from prepare_ai import machine
ap=argparse.ArgumentParser();ap.add_argument('--variant',default='DAC6',choices=['DAC6','ORCH90']);ap.add_argument('--rom',type=Path,default=ROOT/'work/knightmare.rom');ap.add_argument('--lwasm',type=Path,default=Path('C:/Users/Anderson/Documents/Codex/2026-10-25/ol/work/tools/lwtools/lwtools-4.25-win64/lwasm.exe'));ap.add_argument('--xroar',type=Path,default=Path('C:/Users/Anderson/Downloads/xroar-1.12-w64/xroar.exe'));a=ap.parse_args();name=a.variant;raw=a.rom.read_bytes();sy=symbols(name);lw=a.lwasm
p,log,r=boot(a.xroar,name)
try:
 call(r,sy,'main_loop');enter_game(r,sy);cases=[]
 for x,y in [(x,y) for y in [32,100,135,160] for x in [16,60,120,180,224]]:
  z=machine(raw);z.ix=0xe600;z.mem[0xe600]=3;z.mem[0xe602]=y;z.mem[0xe604]=x;z.mem[0x6112]=0xc9;z.mem[0x5e02]=0xc9;want=[]
  for t in range(160):
   for sub in range(2):
    z.mem[0xe003]=(2*t+sub+1)&255
    if not z.mem[0xe00d]:z.corre(0x5d95)
   q=z.mem[0xe600:0xe620];want.append(dict(state=3 if z.mem[0xe00d] else q[11],x=q[4]*256+q[3],y=q[2]*256+q[1],vx=z._leer16(0xe678),vy=z._leer16(0xe676),angle=z.mem[0xe675],frame=q[6]))
   if z.mem[0xe00d]:break
  assert z.mem[0xe00d],(x,y,'original exit did not finish')
  for field,value in [('player_x',x),('player_y',y),('player_fx',0),('player_fy',0),('player_frame',0),('EXIT_STATE',0)]:r.write(sy[field],bytes([value]))
  n=len(want);body=f'''        orcc #$50
        lda #12
        sta $ffa4
        ldy #$8000
        sty $0be0
        clr $0be2
exit_trace_loop
        lda $0be2
        lsla
        inca
        sta ${sy['ai_clock']:04x}
        jsr ${sy['cutscene_tick']:04x}
        ldy $0be0
        lda ${sy['EXIT_STATE']:04x}
        sta ,y+
        lda ${sy['player_x']:04x}
        ldb ${sy['player_fx']:04x}
        std ,y++
        lda ${sy['player_y']:04x}
        ldb ${sy['player_fy']:04x}
        std ,y++
        ldd ${sy['EXIT_VX']:04x}
        std ,y++
        ldd ${sy['EXIT_VY']:04x}
        std ,y++
        lda ${sy['EXIT_ANGLE']:04x}
        sta ,y+
        lda ${sy['player_frame']:04x}
        sta ,y+
        sty $0be0
        inc $0be2
        lda $0be2
        cmpa #{n}
        lblo exit_trace_loop
'''
  shim(r,sy,lw,'cutscene_trace',body);data=r.read(0x8000,n*11)
  for t,e in enumerate(want):
   q=data[t*11:(t+1)*11];a=dict(state=q[0],x=int.from_bytes(q[1:3],'big'),y=int.from_bytes(q[3:5],'big'),vx=int.from_bytes(q[5:7],'big'),vy=int.from_bytes(q[7:9],'big'),angle=q[9],frame=q[10]);assert a==e,dict(x=x,y=y,tick=t,got=a,want=e)
  print(x,y,n,'passed',flush=True);cases.append(dict(x=x,y=y,native_updates=n))
 (OUT/f'analysis/{name}-cutscene-verification.json').write_text(json.dumps(dict(tested_disk_sha256=hashlib.sha256((OUT/f'bin/{name}.DSK').read_bytes()).hexdigest(),tested_levels_sha256=hashlib.sha256((OUT/'bin/LEVELS.DSK').read_bytes()).hexdigest(),status='original exit trajectory and animation passed',cases=cases),indent=2))
finally:r.s.close();p.terminate();p.wait(timeout=10);log.close()
