from pathlib import Path
import subprocess
root=Path(__file__).resolve().parents[1]
with (root/'work/openmsx-console.log').open('wb') as log:
 p=subprocess.Popen([r'C:\Program Files\openMSX\openmsx.exe','-control','stdio','-machine','C-BIOS_MSX1','-cart',str(root/'work/knightmare.rom'),'-script',str(root/'work/capture_msx.tcl')],stdin=subprocess.PIPE,stdout=log,stderr=subprocess.STDOUT,cwd=root)
 p.stdin.write(b'<openmsx-control>\n<command>set pause off</command>\n');p.stdin.flush()
 try:p.wait(timeout=28)
 except subprocess.TimeoutExpired:p.terminate();p.wait()
print('exit',p.returncode)
print((root/'work/openmsx-console.log').read_text(errors='replace')[-6000:])
