> Revisão posterior: [MUSICA-TECLA-M.md](MUSICA-TECLA-M.md) acrescenta música opcional e limite de cinco inimigos quando ligada. As medidas abaixo documentam a revisão anterior, sem música.

# Redução da cintilação sem aumentar o tempo das atualizações

Revisão de 31/10/2026, CoCo 3 de 128 KiB, DAC6 e Orchestra-90 CC.

## Causa e alteração

O laço anterior apagava o jogador e todos os inimigos/projéteis, calculava movimento, IA e colisões, e só então redesenhava. Durante esse cálculo, o buffer que o GIME exibia já estava sem os sprites. A rolagem e as atualizações dos itens também ocorriam no meio dessa sequência.

Agora o último desenho completo permanece em memória durante a lógica. A atualização das posições e colisões ocorre primeiro; somente depois começa a sequência de apagar e redesenhar. As informações do desenho anterior (+17..22 de cada entidade) continuam separadas dos campos de movimento e permitem apagar corretamente até uma entidade eliminada ou um slot reutilizado nessa atualização. A ordem de sobreposição e a ordem inversa de restauração foram preservadas.

As mudanças visuais dos objetos fixos são enfileiradas durante a lógica e aplicadas depois da restauração dos sprites. A fila usa 36 bytes de scratch RAM e dois indicadores de um byte. O desenho do jogador usa o compositor desenrolado de nove bytes já existente, eliminando o controle do laço por pixel. Não foi acrescentado outro framebuffer.

O teste novo também expôs um erro anterior na atualização dos itens: somar a posição visível ao início do anel podia ultrapassar 255, perder o carry e atualizar outra linha. A soma agora é feita com 16 bits e convertida para a linha canônica do anel de 192 linhas, atualizando as duas cópias do fundo. Isso evita resíduos visuais ao pegar ou destruir itens perto da volta do buffer.

## Medições

Comparação com o pacote anterior “Mapas Compactos e Memória”, usando ciclos emulados no XRoar, não tempo de execução no computador hospedeiro. Cada cenário tem doze atualizações. O caso carregado contém 18 objetos controlados; não é uma medição de FPS da campanha inteira.

| Medida média | Antes | Agora |
| --- | ---: | ---: |
| Atualização completa, cenário leve | 9,84 ms | 9,63 ms |
| Atualização completa, cenário carregado | 60,62 ms | 60,41 ms |
| Início do apagamento até fim do redesenho, leve | 9,80 ms | 6,79 ms |
| Mesmo intervalo, carregado | 60,58 ms | 53,82 ms |

O intervalo de apagamento/redesenho ficou aproximadamente 30% menor no caso leve e 11% menor no carregado. Esse intervalo é uma medida conservadora da janela em que o desenho está sendo reconstruído; não significa que todos os pixels de todos os sprites ficam ausentes durante todo esse tempo. Não houve aumento do tempo total nos cenários medidos; o ganho de velocidade foi pequeno.

**A alteração reduz a causa da cintilação, mas não garante eliminá-la.** A composição ainda acontece no buffer visível. O teste carregado continua acima dos 33,4 ms disponíveis para uma atualização a aproximadamente 30 Hz. Portanto, ainda pode haver cintilação ou cortes durante o desenho em situações pesadas.

Um framebuffer duplo com apresentação somente depois de pronto é uma alternativa para eliminar a exposição do quadro intermediário. Ele não foi implementado aqui: o anel de rolagem atual já usa 48 KiB, e um esquema duplo compatível com essa rolagem exige redistribuir memória e/ou copiar mais pixels. O custo dessa alternativa não foi medido nesta revisão; não se afirma que toda solução possível seria necessariamente mais lenta. Evitou-se introduzir esse custo sem validação.

Os efeitos sonoros, os dados compactos dos oito mapas e os controles foram mantidos. O desenho intencionalmente intermitente usado por estados do jogo não foi removido. A campanha jogável continua na primeira fase; esta revisão não integra a fase 2.

## Evidência

`tools/verify_flicker_pipeline.py` verifica nas duas variantes que o bitmap exibido fica byte a byte intacto durante a lógica, antes de `render_begin`, inclusive quando há movimento, tiros, avanço da câmera e coleta de itens. Depois da composição, compara o bitmap com um renderizador independente. São onze quadros por variante, incluindo a posição do anel que revelou o erro de carry na atualização de itens.

As regressões de controles/joystick, rolagem completa, IA, disparos, chefe, checkpoints, itens, áudio, tecla I, fim das vidas e oito mapas também foram executadas. Os resultados são vinculados aos hashes dos discos finais em `flicker-release-verification.json`. Não houve captura de vídeo para quantificar a percepção da cintilação nem teste em CoCo físico.

- `analysis/flicker-timing-before.json` e `flicker-timing.json`: tempos antes e depois.
- `analysis/*-flicker-verification.json`: comparação do quadro antes da composição e do resultado final.
- `tools/benchmark_flicker.py`: medição em ciclos emulados.
- `src/gameplay.asm`: lógica antes do apagamento e desenho desenrolado do jogador.
- `src/items.asm`: fila de atualizações visuais e correção do endereço circular.
- `src/items_state.inc`: dois indicadores novos.

Para executar, extraia o pacote em uma pasta nova e use `EXECUTAR-DAC6.cmd` ou `EXECUTAR-ORCH90.cmd`. Confira a identificação “REDUCAO DE CINTILACAO” em `VERSAO.txt`.
