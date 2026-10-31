"""Extract AI tables and execute the supplied Z80 ROM as a reference oracle.

Only sound requests are stubbed in trajectory tests. No movement instruction is
replaced. The CoCo game never contains or executes this Python/Z80 interpreter.
"""
from pathlib import Path
import argparse, hashlib, json, sys
ROOT = Path(__file__).resolve().parents[1]
SHA = '589b2fd276d84827bb1b033f43a0cd5ab0aa9da3c8784cac2f62474fed00aae3'
sys.path.insert(0,str(ROOT/'third_party/knightmare'))
from z80run import Z80

def machine(raw):
    z=Z80();z.carga(raw,0x4000)
    # Sound requests only: preserve flags/registers, no VDP/audio in this oracle.
    for p in [0x490c,0x4901]: z.mem[p]=0xc9
    z.mem[0xe02e]=1
    z.mem[0xe061]=1
    z.mem[0xe602]=135;z.mem[0xe604]=120
    z.mem[0xe620:0xe650:16]=bytes([0x80]*3)
    return z

def trajectory(raw,kind,x=80,y=240,flags=0,member=255,ticks=180):
    z=machine(raw);z.ix=0xe100
    z.mem[0xe100]=1;z.mem[0xe101]=kind|flags
    z.mem[0xe103]=y;z.mem[0xe105]=x;z.mem[0xe111]=member
    rows=[]
    for t in range(ticks):
        z.mem[0xe003]=(2*t+1)&255
        z.corre(0x75e5)
        q=z.mem[0xe100:0xe120]
        rows.append(dict(state=q[0],x=q[5]*256+q[4],y=q[3]*256+q[2],
                         vx=q[9]*256+q[8],vy=q[7]*256+q[6],hp=q[11],
                         frame=q[10],target_x=q[13],target_y=q[12],
                         clock=q[14],shot_clock=q[15]))
    return rows

def boss_reference(raw,ticks=320):
    z=machine(raw);z.mem[0xe600]=2
    z.mem[0xe3b0]=3;z.mem[0xe3d3]=32;z.mem[0xe3d5]=112
    z._escribir16(0xe3b1,3600)
    z.corre(0x8bd7)
    rows=[]
    for t in range(ticks):
        z.mem[0xe604]=64+(t&127)
        for sub in range(2):
            z.mem[0xe003]=(2*t+sub+1)&255;z.mem[0xe090]=(2*t+sub+1)&255
            # The real caller has C'=$98 (VDP port). Satellite sprite LDI
            # decrements BC' inside a DJNZ loop; a zero fixture would violate
            # that call context and could underflow B'. Movement is untouched.
            z.bc_=0x0098
            z.corre(0x8b1b)
        satellites=[]
        for i in range(3):
            q=z.mem[0xe590+i*32:0xe590+(i+1)*32]
            satellites.append(dict(state=q[0],x=q[5]*256+q[4],y=q[3]*256+q[2],vx=q[9]*256+q[8],vy=q[7]*256+q[6],hp=q[11],target_y=q[12],fraction=q[18]))
        rows.append(dict(x=z.mem[0xe3d5],y=z.mem[0xe3d3],wait=z.mem[0xe3db],
                         target=z.mem[0xe3da],velocity=z.mem[0xe3d9],lifetime=z._leer16(0xe3b1),
                         satellite_clocks=[z.mem[0xe5a8+i*32] for i in range(3)],satellites=satellites))
    return rows

def wave_reference(raw,ticks=6000,stage=0):
    z=machine(raw);z.mem[0xe062]=stage;z.mem[0xe061]=stage+1;events=[]
    for t in range(ticks):
        z.mem[0xe003]=(2*t+1)&255;z.mem[0xe091]=t//32
        z.mem[0xe09e]=int(t>=96)
        z.corre(0x6f75);z.corre(0x729b)
        for i in range(7):
            p=0xe100+i*32;q=z.mem[p:p+32]
            if q[0]==1:
                events.append(dict(tick=t,slot=i,raw_type=q[1],member=q[17],x=q[5]*256+q[4],y=q[3]*256+q[2]))
            z.ix=p;z.corre(0x75e5)
        # The even VBL after this update services these pattern requests.
        # Native art is preloaded, but retains the same one-update delay.
        z.mem[0xe0a8]=0;z.mem[0xe0b8]=0
    return dict(ticks=ticks,events=events,next_wave=z.mem[0xe09f])

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--rom',type=Path,required=True);a=ap.parse_args()
    raw=a.rom.read_bytes();assert hashlib.sha256(raw).hexdigest()==SHA
    def b(p):return raw[p-0x4000]
    def w(p):return b(p)+256*b(p+1)
    tables={}
    for name,p,n in [('ai_spawn_y',0x73f5,16),('ai_burst_size',0x7033,16),
                     ('ai_burst_gap',0x73ce,16),('ai_launch_x',0x7991,6),
                     ('ai_enemy_burst_kind',0x76bd,16),
                     ('ai_enemy_shot_boxes',0x8871,16),
                     ('ai_launch_vx_hi',0x7997,6),('ai_shot_schedule_counts',0x76ed,16),
                     ('ai_boss_target_offsets',0x8c25,9)]:
        tables[name]=[b(p+i) for i in range(n)]
    schedules=[]
    for k in range(16):
        p=w(0x76cd+k*2);n=b(0x76ed+k)
        schedules.append([b(p+i) for i in range(n)])
    tables['ai_satellite_thresholds']=[b(0x8cbf+3-i) for i in range(3)]
    tables['ai_boss_weapon_boxes']=[b(0x99b5+i) for i in range(24)]
    vectors={}
    for kind in [1,2,3,4,8]:
        v=[]
        for angle in range(0,256,8):
            z=machine(raw);z.iy=0xe200;z.mem[0xe201]=kind;z.mem[0xe0c2]=angle
            z.corre(0x86b5)
            v.append([z.de,z.bc]) # native X, Y
        vectors[kind]=v
    lines=['; ROM-derived constants. Angles/velocities executed by the Z80 oracle.']
    for name,values in tables.items():
        lines += [name+':','        fcb '+','.join(map(str,values))]
    lines += ['ai_shot_schedules:','        fdb '+','.join('ai_schedule_'+str(k) for k in range(16))]
    for k,schedule in enumerate(schedules):
        lines += [f'ai_schedule_{k}:','        fcb '+','.join(map(str,schedule))]
    for kind,rows in vectors.items():
        if kind!=8:continue
        lines += [f'ai_vectors_{kind}:']
        lines += ['        fdb '+','.join(f'${n:04x}' for n in row) for row in rows]
    auxiliary=[]
    for angle in range(0,256,8):
        z=machine(raw);z.a=angle;z.corre(0x83e1);auxiliary.append(z.mem[0xe0cf])
    tables['ai_auxiliary_x']=auxiliary
    lines += ['ai_auxiliary_x:','        fcb '+','.join(map(str,auxiliary))]
    lines += ['ai_tangents:','        fdb '+','.join(str(w(0x84d0+2*i)) for i in range(8))]
    (ROOT/'src/ai_data.inc').write_text('\n'.join(lines)+'\n',encoding='ascii')
    cases=[]
    for kind in [1,2,9,14,12]:
        for x in [40,80,127,128,176,216]:
            cases.append(dict(kind=kind,x=x,y=16 if kind==12 else 240,
                              rows=trajectory(raw,kind,x=x,y=16 if kind==12 else 240)))
    # Actual mount header proves equipment reset on a lost life, retention when
    # mounting a NEW phase. Graphics calls occur after the stop address $5464.
    inventory=[]
    for mounted in [0,1]:
        z=machine(raw);z.mem[0xe073]=mounted
        before=bytes([9,4,0,2,12,0x45,23]);z.mem[0xe609:0xe610]=before
        # Suppress music requests at the public entry, outside checkpoint logic.
        z.mem[0x568c]=0xc9;z.mem[0x569d]=0xc9
        z.corre_hasta(0x53d3,0x5464)
        inventory.append(dict(stage_already_mounted=bool(mounted),before=list(before),after=list(z.mem[0xe609:0xe610])))
    rewinds=[]
    for index in range(b(0x7043)):
        z=machine(raw);z.mem[0xe073]=1;z.mem[0xe09f]=index
        z.mem[0x568c]=0xc9;z.mem[0x569d]=0xc9
        z.corre_hasta(0x53d3,0x54b5)
        rewinds.append(z.mem[0xe09f])
    z=machine(raw);z.mem[0xe073]=1;z.mem[0xe092]=9;z.mem[0xe3b0]=3
    z.mem[0x568c]=0xc9;z.mem[0x569d]=0xc9
    for i,n in enumerate([9,88,201]):z.mem[0xe5a8+i*32]=n;z.mem[0xe590+i*32]=2
    z.mem[0xe3d1]=13;z.corre_hasta(0x53d3,0x5464)
    boss_checkpoint=dict(satellite_clocks=[z.mem[0xe5a8+i*32] for i in range(3)],satellite_states=[z.mem[0xe590+i*32] for i in range(3)],boss_state=z.mem[0xe3b0],accumulated_boss_damage=z.mem[0xe3d1])
    report=dict(source_rom_sha256=SHA,method='actual Z80 instructions, sound calls stubbed; no hardware timing model',
                tables=tables,shot_schedules=schedules,vectors=vectors,trajectories=cases,
                inventory_mount_observations=inventory,checkpoint_wave_rewinds=rewinds,boss_checkpoint_observation=boss_checkpoint,
                handlers={str(k):f'${w(0x75f3+2*k):04X}' for k in range(16)},
                boss_handlers=[f'${w(0x88f2+2*k):04X}' for k in range(8)],boss_trace=boss_reference(raw),wave_trace=wave_reference(raw))
    (ROOT/'analysis/ai-rom-reference.json').write_text(json.dumps(report,indent=2),encoding='utf-8')
    print(json.dumps(dict(trajectories=len(cases),samples=sum(len(c['rows']) for c in cases),
                         satellite_thresholds=tables['ai_satellite_thresholds'],inventory=inventory),indent=2))

if __name__=='__main__':main()
