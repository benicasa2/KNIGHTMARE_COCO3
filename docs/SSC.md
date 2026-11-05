# Tandy Speech/Sound Cartridge original — v0.3.0

Esta variante usa o **AY-3-8913 do Tandy Speech/Sound Cartridge original, sem modificação**. Música nos canais A/B e efeitos no canal C. Não usa o sintetizador de fala. CoCo 3 de **128 KiB**, CPU a **0,895 MHz** durante todo o jogo, inclusive carregamento de cenários. Não exige a adaptação do cartucho para 1,79 MHz.

## Jogar no CoCo 3

Use Multi-Pak compatível com CoCo 3 para conectar simultaneamente o SSC e a interface de disco. Selecione o slot da interface de disco para iniciar o Disk BASIC. A configuração validada no emulador usa SSC no slot 1 e FDC no slot 4; a instalação física deve seguir as instruções dos seus equipamentos.

| Unidade | Imagem |
| --- | --- |
| Drive 0 | `bin/SSC.DSK` |
| Drive 1 | `bin/LEVELS.DSK` |

Digite `RUN"START"` e aguarde. Mantenha LEVELS no drive 1 durante a partida. Espaço inicia e dispara; setas/WASD movem; J/L selecionam joystick direito/esquerdo e K seleciona teclado. **M liga/desliga a música**. Com música, o limite de inimigos comuns é cinco; sem música, sete. Efeitos permanecem ativos. I liga/desliga a invencibilidade de teste, P pausa e R inicia nova partida. A música começa desligada, como nas outras variantes.

O som do SSC sai pelo caminho de áudio do cartucho/CoCo. A seleção desse caminho é restabelecida depois da leitura do joystick. Sem cartucho reconhecido, ou se BUSY ficar preso, o jogo continua silencioso e a música fica indisponível. Não há retorno automático para o áudio DAC6 nesta variante.

## Emular

Use **MAME com suporte ao dispositivo `ssc`**; validado no MAME 0.289. XRoar 1.12 não emula este cartucho. As ROMs de sistema, FDC e SSC devem ser fornecidas pelo usuário na organização esperada pelo MAME; não estão no pacote.

```powershell
python tools/run_ssc.py --mame C:\emuladores\mame\mame.exe --rompath C:\emuladores\mame\roms
```

Ou passe esses mesmos argumentos a `EXECUTAR-SSC.cmd`. Também são aceitas as variáveis `MAME_EXE` e `MAME_ROMPATH`. O lançador monta os dois discos, configura 128 KiB/Multi-Pak e digita o comando de partida. A carga por Disk BASIC pode levar aproximadamente um minuto em velocidade real.

## Fidelidade, velocidade e escopo

Foram mantidos os períodos AY de 12 bits, volumes de quatro bits e sequências extraídos da ROM MSX: quatro músicas das fases 1 e 2 e de seus chefes, além de disparo, acerto, explosão, chefe, ciclo de token e coleta. O efeito de dano ao jogador reutiliza a explosão, como na versão anterior. O AY é compatível com os registros musicais utilizados pelo original e funciona com clock próximo ao PSG MSX.

O transporte passa pelo microcontrolador do cartucho. Os registros chegam sequencialmente, sujeitos a BUSY e ao tempo de processamento do firmware. A fila mantém o estado desejado mais recente e pode combinar mudanças rápidas sob carga; **não há garantia de temporização idêntica, amostra por amostra, ao MSX**. A comparação integral das sequências do decodificador e a conferência do AY após estabilização estão registradas nos testes.

O cartucho original exige clock lento nesta adaptação, com perda de fluidez. Em janelas de 600 quadros de vídeo, com jogo nativo em execução contínua e proteção I:

| Trecho medido | SSC, 0,895 MHz | DAC6, 1,79 MHz |
| --- | ---: | ---: |
| Fase 1, música ligada | 19,38 atualizações/s | 25,67 atualizações/s |
| Fase 1, música desligada | 14,58 atualizações/s | 26,97 atualizações/s |
| Fase 2, música ligada | 14,98 atualizações/s | 19,08 atualizações/s |

Os trechos são sequenciais, com populações e situações diferentes. A linha sem música não é uma medição isolada do custo musical; desligar música também permite mais inimigos. Estes valores não representam toda a campanha nem garantem FPS mínimo em hardware físico. A cadência nominal continua sendo aproximadamente 30 atualizações/s, mas o processamento pode não alcançá-la.

A campanha continua com **duas fases**. Depois do segundo chefe, retorna à introdução. O manual ilustrado v0.2.1 acompanha a distribuição como referência dos controles, história e itens; este suplemento descreve o novo áudio SSC. As variantes DAC6 e Orchestra-90 mantêm os mesmos discos da v0.2.1.

## Implementação e evidências

- `src/audio_ssc.asm`: driver 6809, protocolo direto `$AF`, controle de BUSY e efeitos prioritários. A FIRQ transporta no máximo um byte por disparo, a aproximadamente 1 kHz, sem espera ocupada dentro da interrupção.
- `tools/prepare_ssc.py`: partituras sem perdas e RLE dos efeitos, 4.442 bytes no banco 7. O programa completo permanece dentro de 128 KiB.
- `src/level_disk.asm`: caminho de leitura a clock lento, contando 256 bytes em B e usando timeout em X. O caminho de byte pronto leva 26 ciclos; a espera vazia, 18 ciclos. Isso respeita a janela do disco de dupla densidade, sem acelerar o SSC.
- `analysis/SSC-music-verification.json`: 10.414 quadros musicais comparados, loops, seis efeitos com música desligada/ligada, registradores estabilizados e WAV efetivamente não silencioso no MAME.
- `analysis/SSC-game-verification.json`: inicialização pelo disco, teclas M/I/J/K, redução de inimigos, leitura integral dos 8.192 bytes de tiles de cada fase, transição entre fases, retorno após game over, ausência de cartucho e BUSY preso.

Os testes usam CPU 6809, firmware TMS7040, AY e controlador de disco emulados. Não houve validação no cartucho físico. A ROM SP0256 disponível para o teste produziu aviso de checksum; a síntese de fala não foi acionada. O firmware TMS7040 passou pela identificação esperada. Os testes de áudio isolam a reprodução nativa depois da inicialização; não são uma gravação de campanha inteira.

Para reproduzir os testes:

```powershell
python tools/build.py --lwasm C:\ferramentas\lwasm.exe
python tools/verify_ssc_mame.py --mame C:\emuladores\mame\mame.exe --rompath C:\emuladores\mame\roms --output C:\temp\knightmare-audio
python tools/verify_ssc_game.py --mame C:\emuladores\mame\mame.exe --rompath C:\emuladores\mame\roms --output C:\temp\knightmare-jogo
```

Referências: [manual Tandy](https://colorcomputerarchive.com/repo/Documents/Manuals/Hardware/Speech-Sound%20Cartridge%20(Tandy).pdf), [manual de serviço Tandy](https://colorcomputerarchive.com/repo/Documents/Manuals/Hardware/Speech-Sound%20Cartridge%20Service%20Manual%20%28Tandy%29.pdf), [implementação SSC no MAME](https://github.com/mamedev/mame/blob/master/src/devices/bus/coco/coco_ssc.cpp) e [manual XRoar](https://www.6809.org.uk/xroar/doc/xroar.shtml).
