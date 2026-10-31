"""Reproduce Ghidra static exports with the bundled REA seed script."""
from pathlib import Path
import argparse,subprocess,tempfile,hashlib,json
ROOT=Path(__file__).resolve().parents[1]
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--rom',type=Path,required=True);ap.add_argument('--ghidra',type=Path,required=True);ap.add_argument('--java',default='java');args=ap.parse_args()
 digest=hashlib.sha256(args.rom.read_bytes()).hexdigest()
 assert digest==json.loads((ROOT/'analysis/rom.json').read_text())['sha256'],'Wrong ROM revision for these seeds'
 with tempfile.TemporaryDirectory(prefix='knightmare-ghidra-') as tmp:
  w=Path(tmp)
  for n in ['projects','settings','cache','tmp']:(w/n).mkdir()
  script_path=str(ROOT/'tools/ghidra')+';'+str(ROOT/'third_party/rea')
  cmd=[args.java,'-Xmx2G','-XX:ParallelGCThreads=2','-Djava.system.class.loader=ghidra.GhidraClassLoader',f'-Dapplication.settingsdir={w}/settings',f'-Dapplication.cachedir={w}/cache',f'-Djava.io.tmpdir={w}/tmp','-Djava.awt.headless=true','-cp',str(args.ghidra/'Ghidra/Framework/Utility/lib/Utility.jar'),'ghidra.GhidraLauncher','ghidra.app.util.headless.AnalyzeHeadless',str(w/'projects'),'Knightmare','-import',str(args.rom.resolve()),'-loader','BinaryLoader','-loader-baseAddr','0x4000','-processor','z80:LE:16:default','-cspec','default','-scriptPath',script_path,'-preScript','PrepareKnightmare.java','-preScript','ReaGhidraApplySeeds.java',str(ROOT/'analysis/seeds.tsv'),'-postScript','ExportKnightmare.java',str(ROOT/'analysis'),'-analysisTimeoutPerFile','180','-deleteProject']
  subprocess.run(cmd,check=True)
 assert hashlib.sha256(args.rom.read_bytes()).hexdigest()==digest
if __name__=='__main__':main()
