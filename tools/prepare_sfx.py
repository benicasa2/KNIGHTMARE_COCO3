"""Render isolated original PSG-register traces to inexpensive event PCM."""
from pathlib import Path
import math,json,hashlib
ROOT=Path(__file__).resolve().parents[1]
RATE=3579545/912;FPS=3579545/(228*262)
def main():
 result={}
 for name,ident in [('shot',3),('hit',11),('explosion',77),('boss',71),('cycle',14),('pickup',18)]:
  source=ROOT/f'reference/sfx-{ident}-psg.bin';raw=source.read_bytes();assert len(raw)==180*16
  frames=[raw[i:i+16] for i in range(0,len(raw),16)]
  active=[i for i,r in enumerate(frames) if any(r[8:11])];assert active
  count=math.ceil((active[-1]+2)*RATE/FPS);phase=[0.0]*3;noise_phase=0.;lfsr=1;pcm=bytearray()
  scale=max(sum(10**((r[8+c]-15)*3/20) if r[8+c] else 0 for c in range(3)) for r in frames)
  for n in range(count):
   mixed=0.
   for k in range(8):
    r=frames[min(179,int((n+(k+.5)/8)*FPS/RATE))]
    noise_phase+=1789772.5/(16*max(1,r[6]&31))/(RATE*8)
    while noise_phase>=1:
     noise_phase-=1;lfsr=(lfsr>>1)|(((lfsr^(lfsr>>3))&1)<<16)
    for c in range(3):
     period=max(1,r[c*2]|((r[c*2+1]&15)<<8));phase[c]=(phase[c]+1789772.5/(16*period)/(RATE*8))%1
     volume=r[8+c];assert not volume&16,'AY hardware envelope needs a separate decoder'
     amp=10**((volume-15)*3/20) if volume else 0
     gate=(phase[c]>=.5 or r[7]&(1<<c)) and (lfsr&1 or r[7]&(8<<c))
     mixed+=(amp if gate else -amp)/8
   pcm.append(max(0,min(255,round(128+72*mixed/scale))))
  pcm[-1]=128
  (ROOT/f'assets/sfx-{name}.pcm').write_bytes(pcm)
  result[name]=dict(original_effect_id=ident,trace_sha256=hashlib.sha256(raw).hexdigest(),active_psg_frames=active[-1]+1,samples=len(pcm),duration_seconds=len(pcm)/RATE,pcm_sha256=hashlib.sha256(pcm).hexdigest())
 (ROOT/'analysis/sfx.json').write_text(json.dumps(dict(sample_rate=RATE,music_enabled=False,method='isolated user-ROM routines $490C/$49C0 in openMSX; PSG trace rendered offline with 8x oversampling, quantized to PCM; approximate AY output, one prioritized sound at a time',effects=result),indent=2),encoding='utf-8')
 print(json.dumps(result,indent=2))
if __name__=='__main__':main()
