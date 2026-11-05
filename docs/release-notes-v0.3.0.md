# Knightmare CoCo 3 v0.3.0 — Tandy Speech/Sound Cartridge

Nova variante `SSC.DSK` para o cartucho Tandy Speech/Sound **original sem modificação**, em CoCo 3 de 128 KiB. Música A/B e efeitos C no AY do cartucho, usando as sequências extraídas do MSX. M liga/desliga música e mantém a redução de sete para cinco inimigos comuns. Sem cartucho ou com BUSY preso, a partida continua silenciosa.

CPU a 0,895 MHz, inclusive durante a carga das fases. O leitor de cenários foi ajustado para esse clock. **Há perda de fluidez:** aproximadamente 15–19 atualizações/s nos dois trechos medidos com música; os resultados variam conforme a cena. A comunicação sequencial com o firmware pode atrasar/combinar mudanças rápidas de registros. Não houve teste em hardware físico.

Validado no MAME 0.289: quatro músicas e seus loops (10.414 quadros), seis efeitos com música ligada/desligada, emissão de áudio, controles, limites de inimigos, tiles completos de ambas as fases, transição, game over e falhas do cartucho. O teste usou firmware SSC/TMS7040; a ROM SP0256 disponível gerou aviso de checksum, sem uso da fala.

O pacote de jogo inclui os três discos de áudio, LEVELS, lançadores, o manual ilustrado v0.2.1 e o suplemento `docs/SSC.md`. **SSC usa MAME, não XRoar 1.12.** ROMs e emuladores não estão incluídos. No CoCo, monte SSC no drive 0 e LEVELS no drive 1 e use `RUN"START"`; M ativa a música.

Os discos DAC6 e Orchestra-90 permanecem idênticos aos da v0.2.1. A campanha continua com fases 1 e 2; fases 3–8 pendentes.
