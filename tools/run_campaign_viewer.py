"""Launch the separate eight-stage resource viewer, not the playable game."""
from pathlib import Path
import argparse, os, shutil, subprocess
ROOT = Path(__file__).resolve().parents[1]

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--xroar',type=Path)
    args = ap.parse_args()
    found = os.environ.get('XROAR_EXE') or shutil.which('xroar.exe') or shutil.which('xroar')
    exe = args.xroar or (Path(found) if found else Path.home()/'Downloads/xroar-1.12-w64/xroar.exe')
    if not exe.is_file():
        ap.error('Indique --xroar C:\\caminho\\xroar.exe')
    for name in ['coco3.rom','disk11.rom']:
        if not (exe.parent/name).is_file():
            ap.error(f'Falta {name} ao lado do XRoar')
    print('Visualizador de mapas, sem combate. Setas cima/baixo: 8 pixels por toque. Espaco: proxima fase. A oitava permanece na tela.',flush=True)
    subprocess.run([str(exe),'-machine','coco3','-ram','128','-no-bas',
                    '-extbas',str(exe.parent/'coco3.rom'),'-cart','rsdos',
                    '-cart-rom',str(exe.parent/'disk11.rom'),'-tv-input','rgb',
                    '-load-fd0',str(ROOT/'campaign/bin/BOOT.DSK'),
                    '-load-fd1',str(ROOT/'campaign/bin/CAMPAIGN.DSK'),
                    '-type','RUN"START"\r','-no-disk-write-back'],check=True)

if __name__ == '__main__':
    main()
