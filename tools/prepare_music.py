from pathlib import Path
import argparse,hashlib,json,struct,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'third_party/knightmare'));from z80run import Z80
RATE=3579545/912
class MusicOracle(Z80):
 def _una(self):
  if self.pc==0x93:self.maq.psg[self.a&15]=self.e;self.pc=self._pop()
  else:super()._una()
def frames(raw,track):
 z=MusicOracle();z.carga(raw,0x4000);z.sp=0xf000;z.mem[0xe03a]=0xb8;z.corre(0x49c0);z.a=track;z.corre(0x490c);seen={};out=[]
 for t in range(30000):
  state=bytes(z.mem[0xe010:0xe03b])+bytes(z.maq.psg[:14])
  if state in seen:return out,seen[state]
  seen[state]=t;z.corre(0x49c0);out.append(bytes(z.maq.psg[:14]))
 raise RuntimeError('No exact music-engine cycle')
def events(vals):
 out=[];i=0
 while i<len(vals):
  n=1
  while i+n<len(vals) and vals[i+n]==vals[i] and n<255:n+=1
  out.append((n,vals[i]));i+=n
 return out
def compress(seq):
 out=bytearray();i=0
 while i<len(seq):
  best=0;dist=0
  for d in range(1,min(i,255)+1):
   n=0
   while n<255 and i+n<len(seq) and seq[i+n]==seq[i+n-d]:n+=1
   if n>best:best=n;dist=d
  if best>=3:out+=bytes([0,best,dist]);i+=best
  else:out+=bytes(seq[i]);i+=1
 return out
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--rom',type=Path,required=True);a=ap.parse_args();raw=a.rom.read_bytes()
 assert hashlib.sha256(raw).hexdigest()=='589b2fd276d84827bb1b033f43a0cd5ab0aa9da3c8784cac2f62474fed00aae3'
 dictionary=[];catalogs=[];tracks=[];lines=['; Original music oracle: ROM $490C/$49C0, two PSG tone voices.','music_tracks:']
 for k in range(4):lines+=['        fdb '+','.join(f'music_{k}_{ch}_{p}' for p in ['start','end','loop'] for ch in range(2))]
 for k,track in enumerate([0x9d,0xa3,0x9f,0xa1]):
  if k==2:catalogs.append(dictionary);dictionary=[]
  fr,loop=frames(raw,track);entries=[]
  (ROOT/f'reference/music-{track:02x}-psg.bin').write_bytes(b''.join(fr))
  for ch in range(2):
   ids=[]
   for q in fr:
    pair=(int.from_bytes(q[ch*2:ch*2+2],'little'),q[8+ch])
    assert q[7]&(8<<ch),'Noise in music requires another synthesizer'
    if pair not in dictionary:dictionary.append(pair)
    ids.append(dictionary.index(pair))
   assert len(dictionary)<=256
   prefix=compress(events(ids[:loop]));body=compress(events(ids[loop:]));data=prefix+body
   path=ROOT/f'assets/music-{k}-{ch}.lz';path.write_bytes(data)
   lines += [f'music_{k}_{ch}_start:',f'        includebin "../assets/{path.name}"',f'music_{k}_{ch}_end:',f'music_{k}_{ch}_loop equ music_{k}_{ch}_start+{len(prefix)}']
   entries.append(dict(channel=ch,bytes=len(data),prefix_bytes=len(prefix),frames=len(fr),loop_frame=loop))
  tracks.append(dict(original_id=track,frames=len(fr),loop_frame=loop,channels=entries,psg_sha256=hashlib.sha256(b''.join(fr)).hexdigest()))
 catalogs.append(dictionary)
 native_catalogs=[]
 for bank,table in enumerate(catalogs):
  lines+=['music_dictionary'+('2' if bank else '')+':'];native=[]
  for period,vol in table:
   step=round((1789772.5/(16*max(1,period)))/RATE*65536)&65535
   amp=0 if not vol else round(12*10**((vol-15)*3/20))
   lines+=['        fdb '+str(step),'        fcb '+str(amp)];native.append(dict(period=period,volume=vol,step=step,amplitude=amp))
  native_catalogs.append(native)
 (ROOT/'src/music_data.inc').write_text('\n'.join(lines)+'\n',encoding='ascii')
 report=dict(source_rom_sha256=hashlib.sha256(raw).hexdigest(),sample_rate=RATE,tracks=tracks,dictionary=native_catalogs[0],dictionaries=native_catalogs,dictionary_bytes=sum(len(x) for x in native_catalogs)*3,compressed_event_bytes=sum(c['bytes'] for t in tracks for c in t['channels']),method='execute original Z80 audio engine; trap BIOS WRTPSG only; exact full engine-state cycle including repeat counters; lossless event LZ; native two-voice DDS square-wave adaptation')
 (ROOT/'analysis/music-assets.json').write_text(json.dumps(report,indent=2),encoding='utf-8');print('music events',report['compressed_event_bytes'],'dictionary',report['dictionary_bytes'])
if __name__=='__main__':main()
