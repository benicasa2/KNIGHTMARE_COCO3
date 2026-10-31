# Música opcional na tecla M e limite reduzido de inimigos

Revisão de 31/10/2026. CoCo 3 de 128 KiB, versões DAC6 e Orchestra-90 CC.

## Usar

Durante a partida, pressione **M** para ligar ou desligar a música. A tecla precisa ser solta antes de alternar novamente. A música começa desligada; iniciar outra partida ou voltar à abertura também a desliga. M na abertura/título é ignorado.

Com a música ligada, o limite de inimigos comuns simultâneos passa de **sete para cinco**: redução de 28,57%, aproximadamente os 30% pedidos, considerando que não existe fração de inimigo. Ao desligar, o limite volta a sete. Se os dois últimos slots estiverem ocupados ao ligar, os inimigos neles são retirados sem pontos, com restauração do fundo pelo renderizador. A redução se refere ao limite simultâneo; não representa uma promessa de exatamente 30% menos nascimentos durante toda a fase.

O chefe e seus três satélites permanecem completos. O limite de projéteis e as armas do jogador são mantidos. Os efeitos continuam tocando junto da música. A pausa do jogo não interrompe a trilha; M permite silenciá-la.

## Música original e adaptação de som

Foram extraídas as músicas da primeira fase (pedido original $9D, tabela $56A2) e do chefe ($A3, tabela $56AA). O programa troca de trilha quando entra o chefe. As outras fases ainda não são jogáveis nesta campanha; esta revisão não acrescenta as outras trilhas nem todas as vinhetas do original.

`prepare_music.py` executa as instruções originais em $490C/$49C0 da ROM fornecida e registra as escritas PSG. A única interceptação é a BIOS WRTPSG, que recebe os valores escritos pelo motor. A descoberta do ciclo inclui todo o estado de áudio e os contadores de repetição, preservando as repetições finitas da composição. Não foi executada uma nova sessão Ghidra nesta revisão.

As duas músicas usam duas vozes de tom. Frequências, volumes e duração dos eventos são convertidos para um dicionário e sequências compactadas com literais e referências a eventos anteriores. Os dados ocupam 1.336 bytes de sequências e 468 bytes de dicionário. A reprodução no CoCo é assembly 6809 nativo; o intérprete Z80 só é usado para preparar/verificar os dados.

O sintetizador soma duas ondas quadradas com acumuladores de fase de 16 bits e amplitudes derivadas dos volumes originais. A frequência das interrupções musicais cai para aproximadamente 1.962 Hz quando as fundamentais permitem, com os incrementos de fase dobrados para manter a afinação. Notas mais altas e todos os efeitos usam aproximadamente 3.925 Hz. O relógio de eventos musicais continua ligado ao IRQ de vídeo, independente da demora da renderização.

Os efeitos PCM anteriores são somados à música, respeitando a prioridade entre efeitos existente. A soma permanece dentro da faixa do DAC. Sem música e sem efeito ativo, o FIRQ de áudio fica desligado. Na Orchestra-90 CC, o mesmo sinal mono é enviado aos canais esquerdo e direito.

Este é som adaptado ao CoCo: não há emulação integral do AY/PSG nem promessa de timbre idêntico ao MSX. A troca da taxa musical altera a largura de banda; as notas e os eventos são preservados. Os efeitos permanecem na sua taxa anterior.

## Custo de desempenho

A primeira implementação de música contínua a 3.925 Hz levou aproximadamente 82 ms por atualização no teste carregado, mesmo com cinco inimigos. A seleção de taxa reduziu esse valor para aproximadamente 65 ms.

| Cenário controlado | Música desligada | Música ligada |
| --- | ---: | ---: |
| Leve, média por atualização | 9,51 ms | 11,50 ms |
| Carregado, sete/cinco inimigos respectivamente | 60,56 ms | 65,28 ms |

O modo musical ficou cerca de **7,8% mais lento no teste carregado**, apesar da redução de inimigos. No cenário leve, os dois modos permaneceram abaixo do orçamento de aproximadamente 33,4 ms. Esses valores medem ciclos emulados no XRoar, incluindo lógica, desenho e interrupções, em doze atualizações por cenário. O caso carregado também contém três flechas e oito projéteis hostis; não é uma medição de FPS de toda a campanha.

Por isso a música é opcional e começa desligada. M permite recuperar o modo sem síntese contínua. As alterações anteriores para reduzir cintilação permanecem, mas ainda pode haver cintilação em cenas carregadas. Não houve teste em CoCo físico.

## Validação e memória

O teste executa o decodificador 6809 em XRoar de 128 KiB e compara eventos com a ROM durante um ciclo completo e outro trecho completo de repetição: 2.305 quadros da música da fase e 1.691 da música do chefe, por variante. Também confere os incrementos do sintetizador nas duas taxas.

A leitura de M é testada pela matriz PIA real, com sinais externos controlados: início desligado, M ignorado na abertura, alternância, tecla mantida, limite cinco/sete, remoção dos excedentes, restauração de slots de nascimento e reinício desligado. A composição após remover os inimigos é comparada por pixels. Amostras efetivamente enviadas aos DACs verificam música audível e a soma da música com o PCM de disparo.

As regressões de IA, chefe, checkpoints, controles/joystick, rolagem, itens, fim das vidas, tecla I, mapas e desenho diferido também foram executadas nas duas variantes. Os discos são vinculados aos resultados por SHA-256 e devem ser reproduzidos pelos geradores e compilador.

A memória permanece em 128 KiB e o buffer de vídeo em 48 KiB. O histórico de descompactação usa duas páginas de RAM de 256 bytes, em $0500 e $0600, fora dos bancos de vídeo e gráficos. O estado de vozes usa $0D80..$0DA5, separado da fila de itens e do cache de linha; o restante fica na página direta. Essa RAM pertence ao jogo, que não chama BASIC/BIOS durante a partida.

Arquivos principais: `src/music.asm`, `src/music_data.inc`, `tools/prepare_music.py`, `tools/verify_music.py`, `tools/benchmark_music.py`, `analysis/music-assets.json`, `analysis/music-performance.json` e `analysis/*-music-verification.json`.

Para regenerar a trilha a partir da ROM: `python tools/prepare_music.py --rom CAMINHO_DA_ROM`. Depois compile normalmente com `tools/build.py --lwasm CAMINHO_DO_LWASM`.
