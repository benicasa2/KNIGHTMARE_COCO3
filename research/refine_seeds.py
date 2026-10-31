from pathlib import Path
import struct,json
root=Path(__file__).resolve().parents[1];d=(root/'work/knightmare.rom').read_bytes()
seeds={0x407e:('code','cartridge_init'),0x402e:('code','interrupt_hook'),0x49c0:('function','audio_tick'),0x406c:('function','inline_dispatch')}
tables=[];p=0
while (p:=d.find(bytes.fromhex('cd6c40'),p))>=0:
 start=p+3;targets=[]
 while start+2*len(targets)+2<=len(d):
  x=struct.unpack_from('<H',d,start+2*len(targets))[0]
  if not 0x4000<=x<0xc000:break
  targets.append(x);seeds[x]=('function',f'state_{x:04x}')
 tables.append(dict(call=f'0x{p+0x4000:04x}',table=f'0x{start+0x4000:04x}',targets=[f'0x{x:04x}' for x in targets],classification='candidate inline jump table, targets require dynamic confirmation'))
 p+=3
(root/'work/seeds.tsv').write_text(''.join(f'0x{a:04x}\t{k}'+(f'\t{n}' if k!='code' else '')+'\n' for a,(k,n) in sorted(seeds.items())),encoding='ascii',newline='\n')
(root/'outputs/Knightmare-CoCo3/analysis/dispatch-tables.json').write_text(json.dumps(tables,indent=2),encoding='utf-8')
# One-shot breakpoints provide independent executed-address evidence.
with (root/'work/observed_breakpoints.tcl').open('w') as f:
 for a in sorted(seeds):
  f.write(f'debug breakpoint create -address 0x{a:04x} -once true -command {{puts $::hits "[machine_info time] {a:04x}"; flush $::hits}}\n')
print('seeds',len(seeds),'tables',len(tables))
