"""Extract original title, token/weapon art, item scripts and effect constants."""
from pathlib import Path
import argparse,json,hashlib,sys
ROOT=Path(__file__).resolve().parents[1]
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--rom',required=True,type=Path);a=ap.parse_args()
 raw=a.rom.read_bytes();assert hashlib.sha256(raw).hexdigest()==json.loads((ROOT/'analysis/rom.json').read_text())['sha256']
 sys.path.insert(0,str(ROOT/'third_party/knightmare'));import mapas as M,sprites as S,pantallas as P
 from render_msx import png
 r=M.Rom(str(a.rom),0x4000);meta=json.loads((ROOT/'analysis/combat-assets.json').read_text())
 # Always start from the thirteen first-stage combat frames, even on repeat runs.
 desc=meta['descriptors'][:13];indexed=meta['indexed'][:13];banks=[14]*13
 # Bank 15 is also visible at $E000: reserve RAM interrupt trampolines $FEF4+.
 bank15=bytearray();patches=[];gaps=[[15,0,0x1ef4],[13,1152,4096],[13,6144,8192]]
 def allocate(b,preferred_bank=None):
  for gap in sorted(gaps,key=lambda g:preferred_bank is not None and g[0]!=preferred_bank):
   bank,pos,end=gap
   if pos+len(b)<=end:
    gap[1]+=len(b)
    if bank==15:
     if len(bank15)<pos:bank15.extend(bytes(pos-len(bank15)))
     bank15.extend(b)
    else:patches.append(dict(offset=pos,data=b.hex()))
    return bank,0xc000+pos
  raise AssertionError('Item graphics exceed unused 128 KiB banks')
 art_cache={}
 def art(name,parts):
  used={}
  for image,dx,dy in parts:
   for y,row in enumerate(image):
    for x,c in enumerate(row):
     if c is not None:used[x+dx,y+dy]=M.PALETA.index(c) or 1
  x0=min(x for x,y in used);y0=min(y for x,y in used);w=max(x for x,y in used)-x0+1;h=max(y for x,y in used)-y0+1;n=(w+2)//2
  assert n<=17 and n*h<=256,(name,n,h)
  px=[[used.get((x+x0,y+y0),0) for x in range(w)] for y in range(h)];data=bytearray()
  for shift in (0,1):
   for row in px:
    line=[0]*shift+row+[0]*(n*2-w-shift)
    for x in range(0,n*2,2):
     aa,bb=line[x:x+2];data.extend([(0 if aa else 240)|(0 if bb else 15),aa*16+bb])
  key=(w,h,x0,y0,bytes(data))
  if key in art_cache:return art_cache[key]
  bank,p=allocate(data);ident=len(desc);art_cache[key]=ident
  desc.append(dict(name=name,even=p,odd=p+len(data)//2,span=n,height=h,dx=x0,dy=y0,width=w));indexed.append(dict(name=name,pixels=px,dx=x0,dy=y0));banks.append(bank)
  return ident
 v=S.vram_con(r,[0]);weapon_art=[]
 for weapon in range(12):
  k=12 if weapon==5 else weapon&14;p=r.b(0x63fb+k)//4;c=r.b(0x63fc+k)
  weapon_art.append(art(f'weapon {weapon}',[(S.patron(v,p,c),0,0)]))
 boomerang=[]
 for frame in range(4):boomerang.append(art(f'boomerang frame {frame}',[(S.patron(v,r.b(0x6401)//4+frame,1),0,0)]))
 base=S.patron(v,0x94//4,1);token_weapon={}
 for weapon in [0,2,4,5,6,8,10]:
  ptr=r.w(0x6756+weapon*2);dy,dx,pat,col=[r.b(ptr+i) for i in range(4)]
  token_weapon[str(weapon)]=art(f'weapon token {weapon}',[(base,0,0),(S.patron(v,pat//4,15),dx,dy)])
 token_power=[]
 for selection in range(7):
  # Index 14 marks the flashing foreground only; native code substitutes $674B.
  token_power.append(art(f'ability token {selection}',[(S.patron(v,0x94//4,r.b(0x674f+selection)),0,0),(S.patron(v,0x38//4,14),5,3)]))
 player_modes={}
 for anim,index,data in S.fotogramas_del_jugador(r):
  if anim in (1,2):player_modes[anim,index]=S.dibuja_al_jugador(r,v,data,S.ATRIBUTOS[anim])
 def masked_player(frames):
  blob=bytearray();images=[]
  for frame in frames:
   image=[[0 if c is None else M.PALETA.index(c) or 1 for c in row] for row in frame];images.append(image)
   for shift in (0,1):
    for row in image:
     line=[0]*shift+row+[0]*(2-shift)
     for x in range(0,18,2):
      aa,bb=line[x:x+2];blob.extend([(0 if aa else 240)|(0 if bb else 15),aa*16+bb])
  return blob,images
 invis,invis_images=masked_player([player_modes[2,i][10:26] for i in (0,1)])
 inv_bank,inv_ptr=allocate(invis,preferred_bank=13);assert inv_bank==13
 shield,shield_images=masked_player([player_modes[1,0][1:10]])
 (ROOT/'assets/player-shield-mask.bin').write_bytes(shield)
 # Original first-stage hostile projectiles: tiny types 2/3 and type-1
 # directional patterns, including the literal $0C entries in the ROM.
 small_bullet=art('enemy projectile types 2 and 3',[(S.patron(v,0x58//4,15),-8,-8)])
 directional_bullets={}
 for pat in sorted({r.b(0x8626+i) for i in range(16)}):
  directional_bullets[pat]=art(f'enemy projectile type 1 pattern {pat:02X}',[(S.patron(v,pat//4,15),-8,-8)])
 objects=[];all_objects=[]
 for stage in range(8):
  lo=r.w(0x6a98+12*stage);hi=r.w(0x6a98+12*(stage+1)) if stage<7 else 0x6f24
  records=[]
  for p in range(lo,hi,3):
   typ,row,x=[r.b(p+i) for i in range(3)];hp=r.b(0x6a7f+(typ&15))
   rec=dict(type=typ,source_row=row,x=x,world_y=(220-row)*8,hp=hp+(1 if typ&32 else 0),visible=bool(typ&32),kind=typ&7,passage=(typ&31)>=23)
   records.append(rec)
  all_objects.append(records)
  if stage==0:objects=records
 tokens=[]
 for stage in range(8):
  p=r.w(0x65e6+stage*2);seq=[]
  while r.b(p)!=255:
   b=r.b(p);p+=1;seq.append(dict(raw=b,world_y=(220-(b&127)*2)*8,family=b>>7))
  tokens.append(seq)
 def packed(px):
  ix=[M.PALETA.index(c) or 0 for row in px for c in row]
  return bytes(ix[i]*16+ix[i+1] for i in range(0,len(ix),2)),bytes(ix)
 tv,nt=P.pantalla_del_titulo(r);title,title_ix=packed(P.pinta(tv,nt))
 pv,nn=P.pantalla_de_la_presentacion(r);present,pres_ix=packed(P.pinta(pv,nn))
 logo=bytes(present[y*128+40:y*128+88][i] for y in range(56,80) for i in range(48))
 def rle(data):
  out=bytearray();i=0
  while i<len(data):
   n=1
   while i+n<len(data) and data[i+n]==data[i] and n<255:n+=1
   out.extend([n,data[i]]);i+=n
  return out
 (ROOT/'assets/title.4bpp').write_bytes(title);(ROOT/'assets/intro-logo.4bpp').write_bytes(logo)
 (ROOT/'assets/title.rle').write_bytes(rle(title));png(ROOT/'analysis/original-title.png',title_ix);png(ROOT/'analysis/original-konami.png',pres_ix,palette=[M.PALETA[4]]+[M.PALETA[i] for i in range(1,16)])
 (ROOT/'assets/items-bank15.bin').write_bytes(bank15);(ROOT/'assets/items-bank13-patches.json').write_text(json.dumps(patches))
 reward_weapon=[r.b(0x6839+i) for i in range(8)];reward_power=[r.b(0x6841+i) for i in range(7)]
 cosine=[]
 for angle in range(256):
  q=0 if angle<=64 else 1 if angle<128 else 2 if angle<192 else 3
  k=angle if q==0 else 128-angle if q==1 else angle-128 if q==2 else 256-angle
  c=r.b(0x844e+k);c=-c if q in (1,2) else c;cosine.append((c*32//256)&255)
 details=dict(source_sha256=hashlib.sha256(raw).hexdigest(),weapon_art=weapon_art,boomerang_art=boomerang,token_weapon_art=token_weapon,token_power_art=token_power,weapon_rewards=reward_weapon,power_rewards=reward_power,objects=objects,all_stage_objects=all_objects,tokens=tokens,token_x_offsets=cosine,bank15_bytes=len(bank15),bank13_patch_bytes=sum(len(bytes.fromhex(p['data'])) for p in patches),title_rle_bytes=len(rle(title)),token_color_cycle=[r.b(0x674b+i) for i in range(4)],bridge_tiles=[r.b(0x9a9c+i) for i in range(16)],item_tiles=[[r.b(0x6f24+k*4+i) for i in range(4)] for k in range(8)],speed_pairs=[r.b(0x62e2+i) for i in range(10)],boomerang_steps=[r.b(0x6442+i) for i in range(22)],spawning='scripted original row tables, not random drops')
 details.update(invisibility_images=invis_images,invisibility_pointer=inv_ptr,shield_image=shield_images[0],exit_jumps=[[r.b(0x6a8c+i*2),r.b(0x6a8d+i*2)] for i in range(6)],token_step_8_8=159,token_angle_step=64,token_update_hz='approximately 30 Hz, original alternate-frame branch at $5831/$583D')
 details.update(small_enemy_bullet_art=small_bullet,directional_enemy_bullet_art=[directional_bullets[r.b(0x8626+i)] for i in range(16)],deduplicated_art=True)
 (ROOT/'analysis/items-assets.json').write_text(json.dumps(details,indent=2),encoding='utf-8')
 meta.update(descriptors=desc,indexed=indexed,art_banks=banks);(ROOT/'analysis/combat-assets.json').write_text(json.dumps(meta,indent=2),encoding='utf-8')
 lines=['; Generated from the supplied ROM.','all_art:']
 for s in desc:lines+=['        fdb $%04x,$%04x'%(s['even'],s['odd']),'        fcb '+','.join(str(v&255) for v in [s['span'],s['height'],s['dx'],s['dy']])]
 def table(label,vals):lines.extend([label+':','        fcb '+','.join(str(v&255) for v in vals)])
 table('all_art_banks',banks);table('weapon_art_ids',weapon_art);table('boomerang_art_ids',boomerang)
 table('enemy_bullet1_art',details['directional_enemy_bullet_art']);lines+=['SMALL_BULLET_ART equ '+str(small_bullet)]
 table('token_weapon_ids',[token_weapon[str(w)] for w in [0,0,0,2,4,5,6,6,8,8,10,10]])
 table('token_power_ids',token_power);table('weapon_reward_table',reward_weapon);table('power_reward_table',reward_power)
 table('token_flash_colors',details['token_color_cycle']);table('token_cosine',cosine)
 table('fixed_item_tiles',[b for block in details['item_tiles'] for b in block]);table('bridge_item_tiles',details['bridge_tiles'])
 table('weapon_slot_counts',[r.b(0x60bd+2*i) for i in range(12)]);table('weapon_x_offsets',[r.b(0x60be+2*i) for i in range(12)]);table('original_weapon_boxes',[r.b(0x88bb+i) for i in range(24)]);table('original_speed_pairs',details['speed_pairs']);table('original_boomerang_steps',details['boomerang_steps'])
 lines+=['fixed_item_table:']
 for o in objects:lines+=['        fdb '+str(o['world_y']),'        fcb '+','.join(str(o[k]) for k in ['x','type','hp'])+',0']
 lines+=['invisibility_pointers:','        fdb '+','.join('$%04x'%(inv_ptr+i*288) for i in range(4)),'shield_mask:','        includebin "../assets/player-shield-mask.bin"','FIXED_COUNT equ '+str(len(objects)),'floating_item_table:']
 for o in tokens[0]:lines+=['        fdb '+str(o['world_y']),'        fcb '+str(o['family'])]
 lines+=['TOKEN_COUNT equ '+str(len(tokens[0]))]
 (ROOT/'src/items_data.inc').write_text('\n'.join(lines)+'\n',encoding='ascii')
 print('art frames',len(desc),'bank15',len(bank15),'bank13 patches',details['bank13_patch_bytes'],'title RLE',details['title_rle_bytes'],'fixed objects',len(objects),'token dimensions',[(s['name'],s['span'],s['height']) for s in desc if 'token' in s['name']])
if __name__=='__main__':main()
