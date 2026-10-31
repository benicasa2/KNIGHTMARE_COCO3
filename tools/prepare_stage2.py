from pathlib import Path
import argparse,hashlib,json,sys
ROOT=Path(__file__).resolve().parents[1]
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--rom',type=Path,required=True);a=ap.parse_args()
 sys.path.insert(0,str(ROOT/'third_party/knightmare'));import sprites as S,mapas as M
 r=M.Rom(str(a.rom),0x4000);meta=json.loads((ROOT/'analysis/combat-assets.json').read_text());items=json.loads((ROOT/'analysis/items-assets.json').read_text())
 # Run after prepare_combat + prepare_items; preserve all existing IDs.
 desc=meta['descriptors'][:];images=meta['indexed'][:];ids=meta['enemy_ids'];fixed=S.vram_con(r,[0])
 def add(name,parts):
  pix={}
  for im,dx,dy in parts:
   for y,row in enumerate(im):
    for x,c in enumerate(row):
     if c is not None:pix[x+dx,y+dy]=M.PALETA.index(c) or 1
  x0=min(x for x,y in pix);y0=min(y for x,y in pix);w=max(x for x,y in pix)-x0+1;h=max(y for x,y in pix)-y0+1
  px=[[pix.get((x+x0,y+y0),0) for x in range(w)] for y in range(h)];n=(w+2)//2
  for i,(d,im) in enumerate(zip(desc,images)):
   if (d['dx'],d['dy'],d['width'],d['height'],im['pixels'])==(x0,y0,w,h,px):return i
  ident=len(desc);desc.append(dict(name=name,span=n,height=h,dx=x0,dy=y0,width=w));images.append(dict(name=name,pixels=px,dx=x0,dy=y0));return ident
 type_frames={}
 for kind,frames in S.TIPOS:
  if kind not in [0,1,2,3,4,7,9,11,12,14]:continue
  v=S.vram_del_tipo(r,kind,fixed);row=[]
  for frame in frames:
   ptr=r.w(0x7772+frame*2);dys=[(r.b(ptr+4*k)+128)%256-128 for k in range(2)];dxs=[(r.b(ptr+4*k+1)+128)%256-128 for k in range(2)]
   row.append(add(f'enemy {kind} original frame {frame}',[(S.dibuja_un_enemigo(r,v,ptr,S.BASE_RANURA_0),min(dxs),min(dys))]))
  type_frames[kind]=row;ids[str(kind)]=row[:2] if len(row)>1 else row*2
 v=S.vram_con(r,[0]);dest=r.w(0xad69);part=M.descomprime(r,0xad69,con_palabra=True);v[dest-0x1800:dest-0x1800+len(part)]=part
 part=M.descomprime(r,0xabdc);v[0x780:0x780+len(part)]=part
 body=[]
 for i in range(7):
  p=0x902d+i*4;dy=(r.b(p)+128)%256-128;dx=(r.b(p+1)+128)%256-128
  body.append((S.patron(v,r.b(p+2)//4,r.b(p+3)),dx,dy))
 boss2=add('original second guardian body',body)
 drops=[add(f'original second guardian dropped piece {i}',[(S.patron(v,(0x7c+4*i)//4,15),-8,-8)]) for i in range(4)]
 balls=[add(f'original second guardian attached ball {i}',[(S.patron(v,0xf8//4,1),0,0)]) for i in range(3)]
 bullet6=add('original enemy projectile 6',[(S.patron(fixed,0x64//4,15),-8,-8)])
 bullet4=add('original enemy projectile 4 and 5',[(S.patron(fixed,r.b(0x8304)//4,15),-8,-8)])
 oldbank=(ROOT/'assets/combat-bank.bin').read_bytes();boss1=b''.join(oldbank[p-0xc000:p-0xc000+800] for p in meta['boss_offsets'])
 banks={14:bytearray(boss1),15:bytearray()};patches=[];inv=items['invisibility_pointer']-0xc000
 invdata=bytes.fromhex(next(p['data'] for p in json.loads((ROOT/'assets/items-bank13-patches.json').read_text()) if p['offset']==inv))
 patches.append(dict(offset=inv,data=invdata.hex()));gaps=[[14,len(boss1),8192],[15,0,0x1ef4],[13,1152,inv],[13,inv+len(invdata),4096],[13,6144,8192]]
 artbanks=[]
 for d,im in zip(desc,images):
  data=bytearray();n=d['span'];w=d['width']
  for shift in (0,1):
   for row in im['pixels']:
    line=[0]*shift+row+[0]*(n*2-w-shift);data+=bytes(line[i]*16+line[i+1] for i in range(0,n*2,2))
  gap=next((g for g in gaps if g[1]+len(data)<=g[2]),None);assert gap is not None,('graphics overflow',d['name'],len(data),gaps)
  bank,pos,end=gap;gap[1]+=len(data);artbanks.append(bank);d['even']=0xc000+pos;d['odd']=d['even']+len(data)//2
  if bank==13:patches.append(dict(offset=pos,data=data.hex()))
  else:
   if len(banks[bank])<pos:banks[bank]+=bytes(pos-len(banks[bank]))
   banks[bank]+=data
 (ROOT/'assets/combat-bank.bin').write_bytes(banks[14]);(ROOT/'assets/items-bank15.bin').write_bytes(banks[15]);(ROOT/'assets/items-bank13-patches.json').write_text(json.dumps(patches))
 meta.update(descriptors=desc,indexed=images,art_banks=artbanks,enemy_ids=ids,boss_offsets=[0xc000,0xc320],raw_color_art=True,stage2_type_frames=type_frames,boss2_art=boss2,boss2_drop_art=drops,boss2_ball_art=balls,enemy_bullet4_art=bullet4)
 (ROOT/'analysis/combat-assets.json').write_text(json.dumps(meta,indent=2),encoding='utf-8')
 p=ROOT/'src/items_data.inc';s=p.read_text();start=s.index('all_art:');end=s.index('weapon_art_ids:');lines=['all_art:']
 for d in desc:lines+=['        fdb $%04x,$%04x'%(d['even'],d['odd']),'        fcb '+','.join(str(d[k]&255) for k in ['span','height','dx','dy'])]
 lines+=['all_art_banks:','        fcb '+','.join(map(str,artbanks))];p.write_text(s[:start]+'\n'.join(lines)+'\n'+s[end:])
 p=ROOT/'src/combat_data.inc';s=p.read_text();start=s.index('enemy_art_ids:');end=s.index('original_waves:');lines=['enemy_art_ids:']
 for k in range(16):lines+=['        fcb '+','.join(map(str,ids.get(str(k),ids['1'])))]
 p.write_text(s[:start]+'\n'.join(lines)+'\n'+s[end:])
 import re
 p=ROOT/'src/combat_data.inc';p.write_text(re.sub(r'(boss_pointers:\n)        fdb [^\n]+',r'\1        fdb $c000,$c320',p.read_text()))
 lines=[]
 for k in [0,7]:lines+=['phase2_art_'+str(k)+':','        fcb '+','.join(map(str,type_frames[k]))]
 for name,vals in [('boss2_drop_art',drops),('boss2_ball_art',balls)]:lines += [name+':','        fcb '+','.join(map(str,vals))]
 lines+=['BOSS2_ART equ '+str(boss2),'BULLET4_ART equ '+str(bullet4),'BULLET6_ART equ '+str(bullet6)]
 raw=a.rom.read_bytes();b=lambda p:raw[p-0x4000];w=lambda p:b(p)+256*b(p+1)
 ptr=w(0x704d);count=b(0x7044);lines+=['phase2_waves:','        fcb '+','.join(str(b(ptr+i)) for i in range(count*2)),'PHASE2_WAVE_COUNT equ '+str(count),'phase2_fixed_items:']
 for o in items['all_stage_objects'][1]:lines+=['        fdb '+str(o['world_y']),'        fcb '+','.join(str(o[k]) for k in ['x','type','hp'])+',0']
 lines+=['PHASE2_FIXED_COUNT equ '+str(len(items['all_stage_objects'][1])),'phase2_tokens:']
 for t in items['tokens'][1]:lines+=['        fdb '+str(t['world_y']),'        fcb '+str(t['family'])]
 lines+=['PHASE2_TOKEN_COUNT equ '+str(len(items['tokens'][1]))]
 (ROOT/'src/phase2_data.inc').write_text('\n'.join(lines)+'\n',encoding='ascii')
 # Native loader reads only the current 8 KiB tile bank from drive 1.
 tiledata=bytearray()
 for stage in [1,2]:
  pats=(ROOT/f'assets/stages/stage{stage}-patterns.bin').read_bytes();cols=(ROOT/f'assets/stages/stage{stage}-colors.bin').read_bytes()
  for tile in range(256):
   for y in range(8):
    bits=pats[tile*8+y];color=cols[tile*8+y];row=[color>>4 if bits&(128>>x) else color&15 for x in range(8)];tiledata+=bytes(row[x]*16+row[x+1] for x in range(0,8,2))
  (ROOT/f'assets/stage{stage}-packed-tiles.bin').write_bytes(tiledata[-8192:])
 (ROOT/'bin/LEVELS.DSK').write_bytes(tiledata+bytes(161280-len(tiledata)))
 from prepare_ai import machine
 mathlines=['; Exact original tangent and velocity tables.','cutscene_tangents:','        fdb '+','.join(str(w(0x6262+2*i)) for i in range(64)),'cutscene_vectors:']
 for angle in range(129):
  z=machine(raw);z.a=angle;z.corre(0x83e1);mathlines+=['        fdb $%04x,$%04x'%(z.de,z.bc)]
 for kind in [5,6]:
  mathlines+=['phase2_vectors_'+str(kind)+':']
  for angle in range(0,256,8):
   z=machine(raw);z.iy=0xe200;z.mem[0xe201]=kind;z.mem[0xe0c2]=angle;z.corre(0x86b5);mathlines+=['        fdb $%04x,$%04x'%(z.de,z.bc)]
 (ROOT/'src/phase2_math.inc').write_text('\n'.join(mathlines)+'\n',encoding='ascii')
 report=dict(source_rom_sha256=hashlib.sha256(raw).hexdigest(),art_frames=len(desc),art_bank14_bytes=len(banks[14]),art_bank15_bytes=len(banks[15]),art_bank13_patch_bytes=sum(len(bytes.fromhex(p['data'])) for p in patches),stage2_waves=count,stage2_fixed_objects=len(items['all_stage_objects'][1]),stage2_tokens=len(items['tokens'][1]),boss2_art=boss2,graphics_format='two packed-color alignments; transparency inferred from zero nibble',all_banks_within_128k=True)
 (ROOT/'analysis/phase2-assets.json').write_text(json.dumps(report,indent=2),encoding='utf-8');print(json.dumps(report,indent=2))
if __name__=='__main__':main()
