"""Create verified runtime, source and manual assets for the GitHub Release."""
from pathlib import Path
import hashlib,json,shutil,zipfile
ROOT=Path(__file__).resolve().parents[1]
VERSION=(ROOT/'VERSION').read_text(encoding='utf-8').strip()
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 dest=ROOT/'release';dest.mkdir(exist_ok=True)
 evidence=json.loads((ROOT/'analysis/flicker-two-stage-release-verification.json').read_text(encoding='utf-8'))
 for name,expected in evidence['disks'].items():assert digest(ROOT/f'bin/{name}.DSK')==expected
 assert digest(ROOT/'bin/LEVELS.DSK')==evidence['levels_disk_sha256']
 ssc=json.loads((ROOT/'analysis/SSC-music-verification.json').read_text())
 game=json.loads((ROOT/'analysis/SSC-game-verification.json').read_text())
 assert digest(ROOT/'bin/SSC.DSK')==ssc['tested_disk_sha256']
 for name in ['SSC-normal','SSC-absent','SSC-busy']:
  assert game['results'][name]['passed'] and game['results'][name]['disk_sha256']==ssc['tested_disk_sha256']
 files=sorted(p for p in ROOT.rglob('*') if p.is_file() and not ({'.git','__pycache__','release'} & set(p.relative_to(ROOT).parts)) and p.name!='MANIFEST.json')
 assert not any(p.suffix.lower()=='.rom' for p in files),'Original ROMs must not be included'
 # Keep working-tree bytes consistent with .gitattributes on Windows/Linux.
 for p in files:
  if p.suffix.lower() in ['.asm','.inc','.py','.md','.json','.yml','.ps1','.cmd']:
   original=p.read_bytes();data=original.replace(b'\r\n',b'\n')
   if p.suffix.lower() in ['.ps1','.cmd']:data=data.replace(b'\n',b'\r\n')
   if data!=original:p.write_bytes(data)
 manifest={p.relative_to(ROOT).as_posix():dict(bytes=p.stat().st_size,sha256=digest(p)) for p in files}
 (ROOT/'MANIFEST.json').write_text(json.dumps(manifest,indent=2),encoding='utf-8',newline='\n')
 # The illustrated manual describes the existing two-stage campaign. Keep
 # its real version; the new SSC supplement documents hardware and timing.
 manual=ROOT/'docs/manual/KNIGHTMARE_COCO3_Manual_v0.2.1.pdf';assert manual.is_file()
 runtime=dest/f'KNIGHTMARE_COCO3_v{VERSION}_Jogo.zip';sources=dest/f'KNIGHTMARE_COCO3_v{VERSION}_Fontes.zip'
 runtime_files=[ROOT/n for n in ['EXECUTAR-DAC6.cmd','EXECUTAR-ORCH90.cmd','EXECUTAR-SSC.cmd','LEIA-ME.md','VERSAO.txt','VERSION','bin/DAC6.DSK','bin/ORCH90.DSK','bin/SSC.DSK','bin/LEVELS.DSK','tools/run_demo.py','tools/run_ssc.py','docs/SSC.md','analysis/SSC-music-verification.json','analysis/SSC-game-verification.json','analysis/CINTILACAO-DUAS-FASES.md','analysis/FASE-2-E-SAIDA.md','analysis/flicker-two-stage-release-verification.json']]+[manual]
 for target,entries in [(runtime,runtime_files),(sources,files+[ROOT/'MANIFEST.json'])]:
  with zipfile.ZipFile(target,'w',zipfile.ZIP_DEFLATED,compresslevel=9) as z:
   for p in entries:z.write(p,Path('KNIGHTMARE_COCO3')/p.relative_to(ROOT))
  with zipfile.ZipFile(target) as z:
   assert z.testzip() is None
   for p in entries:assert hashlib.sha256(z.read('KNIGHTMARE_COCO3/'+p.relative_to(ROOT).as_posix())).hexdigest()==digest(p)
 shutil.copy2(manual,dest/manual.name)
 shutil.copy2(ROOT/'docs/SSC.md',dest/'SSC.md')
 assets=[runtime,sources,dest/manual.name,dest/'SSC.md']
 (dest/'SHA256SUMS.txt').write_text(''.join(digest(p)+'  '+p.name+'\n' for p in assets),encoding='ascii')
 print(json.dumps(dict(version=VERSION,assets={p.name:dict(bytes=p.stat().st_size,sha256=digest(p)) for p in assets},verified_project_files=len(files)+1,disks=evidence['disks']),indent=2))
if __name__=='__main__':main()
