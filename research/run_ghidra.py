from pathlib import Path
import subprocess
root=Path(__file__).resolve().parents[1]
gh=Path(r'C:\Users\Anderson\Documents\Codex\2026-10-23\https-github-com-morluto-rea-https\tools\ghidra_12.1.4_PUBLIC')
java=Path(r'C:\Program Files\Eclipse Adoptium\jdk-25.0.4.101-hotspot\bin\java.exe')
for name in ['ghidra','ghidra-settings','ghidra-cache','ghidra-tmp']:(root/'work'/name).mkdir(exist_ok=True)
args=[str(java),'-Xmx2G','-XX:ParallelGCThreads=2',f'-Dapplication.settingsdir={root}/work/ghidra-settings',f'-Dapplication.cachedir={root}/work/ghidra-cache',f'-Djava.io.tmpdir={root}/work/ghidra-tmp','-Djava.awt.headless=true','-cp',str(gh/'Ghidra/Framework/Utility/lib/Utility.jar'),'ghidra.GhidraLauncher','ghidra.app.util.headless.AnalyzeHeadless',str(root/'work/ghidra'),'Knightmare','-import',str(root/'work/knightmare.rom'),'-loader','BinaryLoader','-loader-baseAddr','0x4000','-processor','z80:LE:16:default','-cspec','default','-scriptPath',str(root/'work')+';'+str(root/'work/REA-RETRO-9/bridge/ghidra'),'-preScript','ReaGhidraApplySeeds.java',str(root/'work/seeds.tsv'),'-postScript','ExportKnightmare.java',str(root/'outputs/Knightmare-CoCo3/analysis'),'-analysisTimeoutPerFile','180','-overwrite','-log',str(root/'work/ghidra.log')]
args.insert(2,'-Djava.system.class.loader=ghidra.GhidraClassLoader')
idx=args.index('-preScript')
args[idx:idx]=['-preScript','PrepareKnightmare.java']
with (root/'work/ghidra-console.log').open('w') as log:
 p=subprocess.run(args,stdout=log,stderr=subprocess.STDOUT)
print('Ghidra exit:',p.returncode)
print((root/'work/ghidra-console.log').read_text(errors='replace')[-5000:])
raise SystemExit(p.returncode)
