"""Extract first-stage combat art and constants from the supplied cartridge.
The emitted metadata separates ROM observations from native timing adaptations.
"""
from pathlib import Path
import argparse,hashlib,json,struct,sys
ROOT=Path(__file__).resolve().parents[1]
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--rom',type=Path,required=True);args=ap.parse_args();d=args.rom.read_bytes()
 assert hashlib.sha256(d).hexdigest()==json.loads((ROOT/'analysis/rom.json').read_text())['sha256']
 sys.path.insert(0,str(ROOT/'third_party/knightmare'));import sprites,mapas
 r=mapas.Rom(str(args.rom),0x4000);fixed=sprites.vram_con(r,[0]);bank=bytearray();desc=[];indexed=[]
 def add(name,pixels,ox=0,oy=0):
  used=[(x,y) for y,row in enumerate(pixels) for x,c in enumerate(row) if c is not None]
  x0=min(x for x,y in used);x1=max(x for x,y in used)+1;y0=min(y for x,y in used);y1=max(y for x,y in used)+1
  w=x1-x0;h=y1-y0;n=(w+2)//2
  image=[[0 if c is None else mapas.PALETA.index(c) or 1 for c in row[x0:x1]] for row in pixels[y0:y1]]
  pointers=[]
  for shift in range(2):
   pointers.append(0xc000+len(bank))
   for row in image:
    line=[0]*shift+row+[0]*(n*2-w-shift)
    for x in range(0,n*2,2):
     a,b=line[x:x+2];bank.extend([(0 if a else 240)|(0 if b else 15),a*16+b])
  desc.append(dict(name=name,even=pointers[0],odd=pointers[1],span=n,height=h,dx=ox+x0,dy=oy+y0,width=w))
  indexed.append(dict(name=name,pixels=image,dx=ox+x0,dy=oy+y0))
  return len(desc)-1
 ids={}
 ids['arrow']=add('original arrow weapon 0',sprites.patron(fixed,r.b(0x63fb)//4,r.b(0x63fc)))
 ids['bullet']=add('original enemy shot type 8',sprites.patron(fixed,r.b(0x8308)//4,15),-8,-8)
 enemy_ids={}
 for kind,frames in [(1,[9,10]),(9,[24,25]),(14,[32,33]),(12,[28,29])]:
  v=sprites.vram_del_tipo(r,kind,fixed);result=[]
  for frame in frames:
   ptr=r.w(0x7772+frame*2)
   dys=[(r.b(ptr+4*k)+128)%256-128 for k in range(2)]
   dxs=[(r.b(ptr+4*k+1)+128)%256-128 for k in range(2)]
   result.append(add(f'enemy {kind} frame {frame}',sprites.dibuja_un_enemigo(r,v,ptr,sprites.BASE_RANURA_0),min(dxs),min(dys)))
  enemy_ids[kind]=result
 enemy_ids[2]=enemy_ids[1]
 for frame in [1,2,3]:
  ptr=r.w(0x7772+frame*2);dys=[(r.b(ptr+4*k)+128)%256-128 for k in range(2)];dxs=[(r.b(ptr+4*k+1)+128)%256-128 for k in range(2)]
  ids[f'explosion{frame}']=add(f'original explosion {frame}',sprites.dibuja_un_enemigo(r,fixed,ptr),min(dxs),min(dys))
 boss_offsets=[];tiles=(ROOT/'assets/stage1-packed-tiles.bin').read_bytes();boss_tile_frames=[]
 table=r.w(0xbecc)
 for frame in range(2):
  ptr=r.w(table+frame*2);tileids=[r.b(ptr+i) for i in range(25)];boss_tile_frames.append(tileids);boss_offsets.append(0xc000+len(bank))
  for y in range(40):
   for x in range(5):
    tile=tileids[(y//8)*5+x];p=tile*32+(y&7)*4;bank+=tiles[p:p+4]
 assert len(bank)<=8192,f'Combat bank overflow: {len(bank)}'
 (ROOT/'assets/combat-bank.bin').write_bytes(bank)
 # Remove the baked-in boss face; the native boss is now a moving object.
 original=bytearray((ROOT/'assets/stages/stage1-map.bin').read_bytes());native=bytearray(original);removed=[]
 for y in range(20):
  for x in range(28):
   if sum(original[(y+dy)*32+x+dx]!=boss_tile_frames[0][dy*5+dx] for dy in range(5) for dx in range(5))<=1:
    removed.append([x,y])
    for dy in range(5):native[(y+dy)*32+x:(y+dy)*32+x+5]=bytes([original[(y+dy)*32+10]])*5
 assert removed,'Original first boss tile block was not located in map'
 (ROOT/'assets/stage1-combat-map.bin').write_bytes(native)
 waveptr=r.w(0x704b);waves=[list(d[waveptr-0x4000+i*2:waveptr-0x4000+i*2+2]) for i in range(r.b(0x7043))]
 def b(a,n):return list(d[a-0x4000:a-0x4000+n])
 boxes=[b(0x87c4+i*4,4) for i in range(16)]
 meta=dict(source_sha256=hashlib.sha256(d).hexdigest(),bank_bytes=len(bank),descriptors=desc,indexed=indexed,ids=ids,enemy_ids=enemy_ids,boss_offsets=boss_offsets,boss_hp=r.b(0x9992),boss_tile_frames=boss_tile_frames,boss_removed_at_tiles=removed,waves=waves,spawn_gaps=b(0x740d,16),spawn_x=b(0x7405,8),enemy_boxes=boxes,points=b(0x75ba,16),boss_target_offsets=b(0x8c25,3),original_player_hitbox=[3,3,10,10],terrain_blocked_ranges=[[0x43,0x7f],[0xa3,0xdf]],adaptations=['native renderer and event PCM retain platform adaptations','enemy/boss AI and segment checkpoints are translated separately in ai.asm, boss_ai.asm and checkpoints.asm','see analysis/IA-E-CHECKPOINTS.md for differential evidence and remaining limits','campaign remains first-stage only'])
 (ROOT/'analysis/combat-assets.json').write_text(json.dumps(meta,indent=2),encoding='utf-8')
 # The runtime uses all_art from items_data.inc. Do not emit the obsolete
 # duplicate 13-descriptor table: reclaim 104 bytes in the fixed code window.
 lines=['; Generated from the user ROM by prepare_combat.py','enemy_art_ids:']
 for kind in range(16):lines+=['        fcb '+','.join(str(x) for x in enemy_ids.get(kind,enemy_ids[1]))]
 for name,values in [('original_waves',[a for pair in waves for a in pair]),('original_spawn_gaps',b(0x740d,16)),('original_spawn_x',b(0x7405,8)),('original_enemy_boxes',[a for row in boxes for a in row]),('original_enemy_points',b(0x75ba,16))]:lines += [name+':','        fcb '+','.join(str(v) for v in values)]
 lines+=['boss_pointers:','        fdb '+','.join('$%04x'%p for p in boss_offsets)]
 lines+=['combat_points:','        fdb '+','.join(str(((p>>4)*10+(p&15))*10) for p in b(0x75ba,16))]
 lines+=['ARROW_ART equ %d'%ids['arrow'],'BULLET_ART equ %d'%ids['bullet'],'EXPLOSION_ART equ %d'%ids['explosion1'],'BOSS_HITS equ %d'%meta['boss_hp'],'WAVE_COUNT equ %d'%len(waves)]
 (ROOT/'src/combat_data.inc').write_text('\n'.join(lines)+'\n',encoding='ascii')
 print('combat bank',len(bank),'art frames',len(desc),'original waves',len(waves),'boss HP',meta['boss_hp'],'map replacement',removed)
if __name__=='__main__':main()
