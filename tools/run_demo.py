"""Launch the interactive prototype in an existing XRoar installation."""
from pathlib import Path
import argparse,os,shutil,subprocess
ROOT=Path(__file__).resolve().parents[1]
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--variant',choices=['DAC6','ORCH90'],default='DAC6');ap.add_argument('--xroar',type=Path);args=ap.parse_args()
 exe=args.xroar
 if exe is None:
  found=os.environ.get('XROAR_EXE') or shutil.which('xroar.exe') or shutil.which('xroar')
  exe=Path(found) if found else Path.home()/'Downloads/xroar-1.12-w64/xroar.exe'
 if not exe.is_file():ap.error('Indique o emulador existente com --xroar C:\\caminho\\xroar.exe')
 for rom in ['coco3.rom','disk11.rom']:
  if not (exe.parent/rom).is_file():ap.error(f'Falta {rom} ao lado do XRoar')
 cmd=[str(exe),'-machine','coco3','-ram','128','-no-bas','-extbas',str(exe.parent/'coco3.rom'),'-cart','rsdos','-cart-rom',str(exe.parent/'disk11.rom')]
 if args.variant=='ORCH90':cmd+=['-cart','mpi','-mpi-slot','3','-mpi-load-cart','0=orch90','-mpi-load-cart','3=rsdos']
 cmd+=['-tv-input','rgb','-load-fd0',str(ROOT/f'bin/{args.variant}.DSK'),'-load-fd1',str(ROOT/'bin/LEVELS.DSK'),'-type','RUN"START"\r','-no-disk-write-back']
 print('REVISAO 2026-10-31: FASES 1 E 2 - MENOS CINTILACAO; disco de cenarios no drive 1. M: musica; I: teste.',flush=True)
 print('Carregando o disco. Setas/WASD movem; Espaco/botao inicia ou dispara; K teclado; J joystick direito; L esquerdo; P pausa; R reinicia.',flush=True)
 subprocess.run(cmd,check=True)
if __name__=='__main__':main()
