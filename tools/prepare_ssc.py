"""Pack lossless original AY note events and channel-C effects in bank 7."""
from pathlib import Path
import hashlib,json,struct
ROOT=Path(__file__).resolve().parents[1]
def main():
 music=json.loads((ROOT/'analysis/music-assets.json').read_text(encoding='utf-8'))
 bank=bytearray(48);labels={'music_tracks':0x8000};tracks=[]
 for k,t in enumerate(music['tracks']):
  ptrs={}
  for ch in range(2):
   q=t['channels'][ch];data=(ROOT/f'assets/music-{k}-{ch}.lz').read_bytes();start=0x8000+len(bank);bank+=data
   ptrs[ch]=(start,start+len(data),start+q['prefix_bytes'])
  for field in range(3):
   for ch in range(2):struct.pack_into('>H',bank,k*12+field*4+ch*2,ptrs[ch][field])
  raw=(ROOT/f'reference/music-{t["original_id"]:02x}-psg.bin').read_bytes()
  frames=[raw[i:i+14] for i in range(0,len(raw),14)]
  assert all(q[10]==0 and q[7]&0x3f==0x38 and not(q[8]&16 or q[9]&16) for q in frames)
  tracks.append(dict(original_id=t['original_id'],frames=t['frames'],loop_frame=t['loop_frame'],psg_sha256=hashlib.sha256(raw).hexdigest()))
 for k,ds in enumerate(music['dictionaries']):
  labels['music_dictionary'+('2' if k else '')]=0x8000+len(bank)
  for d in ds:bank+=struct.pack('>HB',d['period'],d['volume'])
 effects=[]
 for name,ident in [('shot',3),('hit',11),('explosion',77),('boss',71),('cycle',14),('pickup',18)]:
  raw=(ROOT/f'reference/sfx-{ident}-psg.bin').read_bytes();fr=[raw[i:i+16] for i in range(0,len(raw),16)]
  assert all(q[8]==q[9]==0 and q[10]<16 for q in fr)
  n=max(i for i,q in enumerate(fr) if q[10])+2;rows=[bytes([q[4],q[5]&15,q[6]&31,q[7]&0x24,q[10]]) for q in fr[:n]]
  start=0x8000+len(bank);i=0
  while i<len(rows):
   count=1
   while i+count<len(rows) and rows[i+count]==rows[i] and count<255:count+=1
   bank+=bytes([count])+rows[i];i+=count
  labels['ssc_sfx_'+name]=start;labels['ssc_sfx_'+name+'_end']=0x8000+len(bank)
  effects.append(dict(name=name,original_id=ident,frames=n,bytes=0x8000+len(bank)-start,source_sha256=hashlib.sha256(raw).hexdigest()))
 assert len(bank)<=8192
 (ROOT/'assets/ssc-bank7.bin').write_bytes(bank)
 (ROOT/'src/ssc_data.inc').write_text('; Bank 7 mapped at $8000 while decoding, never during screen writes.\n'+'\n'.join(f'{k} equ ${v:04x}' for k,v in labels.items())+'\n',encoding='ascii')
 (ROOT/'analysis/ssc-assets.json').write_text(json.dumps(dict(bank=7,bytes=len(bank),ay_clock_hz=1789772.5,music=tracks,effects=effects,format='original 12-bit periods and 4-bit volumes; existing lossless two-voice LZ; channel-C effect RLE',sha256=hashlib.sha256(bank).hexdigest()),indent=2),encoding='utf-8')
 print('SSC bank 7:',len(bank),'bytes')
if __name__=='__main__':main()
