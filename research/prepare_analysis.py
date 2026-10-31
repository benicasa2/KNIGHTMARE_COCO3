from pathlib import Path
import hashlib,struct,json,re
root=Path(__file__).resolve().parents[1]
out=root/'outputs/Knightmare-CoCo3'
for p in ['analysis','tools','src','assets','bin','reference']:(out/p).mkdir(parents=True,exist_ok=True)
source=Path(r'C:\Users\Anderson\Pictures\Knightmare - Majyo Densetsu - Konami (1986) [RC-739] [1409].rom')
d=source.read_bytes()
(root/'work/knightmare.rom').write_bytes(d)
header=dict(signature=d[:2].decode('ascii'),init=f'0x{struct.unpack_from("<H",d,2)[0]:04x}',statement=struct.unpack_from('<H',d,4)[0],device=struct.unpack_from('<H',d,6)[0],text=struct.unpack_from('<H',d,8)[0])
manifest=dict(source=str(source),size=len(d),sha256=hashlib.sha256(d).hexdigest(),header=header,load_address='0x4000',end_exclusive='0xc000',status='reverse engineering in progress; no completed port')
(out/'analysis/rom.json').write_text(json.dumps(manifest,indent=2),encoding='utf-8')
(root/'work/seeds.tsv').write_text('0x407e\tfunction\tcartridge_init\n',encoding='ascii')
print(json.dumps(manifest,indent=2))
