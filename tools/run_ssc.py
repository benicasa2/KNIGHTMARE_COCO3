"""Launch the factory Speech/Sound Cartridge variant in an existing MAME."""
from pathlib import Path
import argparse,os,shutil,subprocess,tempfile
ROOT=Path(__file__).resolve().parents[1]

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--mame',type=Path);ap.add_argument('--rompath',type=Path);a=ap.parse_args()
 found=a.mame or os.environ.get('MAME_EXE') or shutil.which('mame.exe') or shutil.which('mame')
 if not found:ap.error('Informe --mame C:\\caminho\\mame.exe ou configure MAME_EXE.')
 exe=Path(found).resolve()
 if not exe.is_file():ap.error('Executavel MAME nao encontrado: '+str(exe))
 rompath=(a.rompath or Path(os.environ.get('MAME_ROMPATH',str(exe.parent/'roms')))).resolve()
 if not rompath.is_dir():ap.error('Informe --rompath com suas ROMs do CoCo 3, FDC e SSC.')
 # MAME 0.289 supports the actual TMS7040/AY cartridge. XRoar 1.12 does not.
 with tempfile.TemporaryDirectory(prefix='knightmare-ssc-') as folder:
  temp=Path(folder);lua=temp/'start.lua'
  lua.write_text('local n=0\nemu.register_frame_done(function() n=n+1;if n==180 then manager.machine.natkeyboard:post(\'RUN"START"\\n\') end end)\n')
  cmd=[str(exe),'coco3','-ram','128k','-rompath',str(rompath),'-ext','multi','-ext:multi:slot1','ssc','-ext:multi:slot4','fdc','-flop1',str(ROOT/'bin/SSC.DSK'),'-flop2',str(ROOT/'bin/LEVELS.DSK'),'-skip_gameinfo','-autoboot_script',str(lua)]
  for opt in ['cfg','nvram','diff','snapshot']:cmd+=['-'+opt+'_directory',str(temp/opt)]
  print('SSC ORIGINAL: CoCo 3 128 KiB, 0,895 MHz. M liga/desliga musica; I ativa teste.',flush=True)
  print('Espaco inicia/dispara. Setas/WASD movem. P pausa. R reinicia. Aguarde a carga.',flush=True)
  subprocess.run(cmd,check=True)
if __name__=='__main__':main()
