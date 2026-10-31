> Histórico de revisão anterior. Estado atual: [IA-E-CHECKPOINTS.md](IA-E-CHECKPOINTS.md). A IA da primeira fase e a restauração dos trechos foram substituídas; o inventário agora é removido ao perder uma vida. Valores de memória/desempenho abaixo pertencem à revisão anterior.

# Etapa 2 — controles e rolagem nativos

O protótipo agora desenha a primeira fase a partir dos tiles e do mapa extraídos da ROM. Popolon pode ser movido por teclado ou joystick. A rolagem é automática, o HUD permanece fixo e o áudio das duas variantes continua ativo. Esta etapa atende à adição de controles e rolagem; o jogo completo continua incompleto.

## Uso

| Controle | Função |
| --- | --- |
| Setas ou W/A/S/D | Movimentação, inclusive diagonal |
| K | Somente teclado; modo inicial |
| J | Teclado e joystick analógico direito |
| L | Teclado e joystick analógico esquerdo |
| P | Pausar/retomar movimento e rolagem; áudio continua |
| R | Reiniciar a travessia do mapa e reposicionar Popolon |
| Espaço ou botões do joystick selecionado | Entrada de disparo reconhecida; projéteis ainda não implementados |

O teclado continua disponível quando um joystick é selecionado. Eixos opostos se cancelam. A posição é limitada a X=0–239 e Y=0–160, mantendo o sprite fora do HUD. Um joystick desconectado pode apresentar leituras indefinidas; selecione K para usar só o teclado.

## Implementação

`src/gameplay.asm` contém a matriz de teclado, seleção dos joysticks, movimento, animação original de caminhada, restauração do fundo, renderização de tiles e controle da câmera. `src/game_state.inc` contém o estado de execução.

As entradas são lidas diretamente nas PIAs, sem chamadas à ROM BASIC. O teclado permite teclas simultâneas. As linhas que já estavam baixas com todas as colunas inativas são removidas da leitura da matriz, evitando que botões de joystick compartilhados com o teclado criem letras falsas.

O joystick usa o comparador de `$FF00`, os seletores em `$FF01/$FF03` e os limiares 24/40 da faixa de 0–63. A leitura cria uma zona central de repouso. O FIRQ é mascarado apenas durante a seção curta de leitura analógica. O valor anterior do DAC e a seleção da saída sonora são restaurados ao terminar. No Orchestra-90 a mistura continua saindo pelos DACs externos; o DAC interno serve à leitura analógica.

O campo de jogo tem 176 linhas e o HUD, 16. Cada linha usa 128 bytes em 4 bpp. O buffer circular tem 256 linhas, com uma cópia adicional das primeiras 192: 448×128 = 57.344 bytes. Isso permite ao GIME exibir uma janela de 192 linhas mesmo quando o endereço atravessa o fim do anel. Só a nova linha de fundo é desenhada a cada passo de rolagem. O HUD e o jogador são atualizados na janela visível; o fundo sob o jogador é restaurado antes da mudança de posição ou câmera.

O mapa tem 244×32 tiles. A câmera começa em Y=1.776 e termina em zero, mostrando o mapa de baixo para cima. O ritmo nominal é de aproximadamente 29,96 atualizações de controle/s e 14,98 pixels de rolagem/s. Esses valores são configurações desta etapa, não uma declaração de que a velocidade original inteira foi reproduzida exatamente. Ainda falta um confronto temporal quadro a quadro com o MSX.

Popolon usa dois quadros originais, compostos a partir de seus planos de sprite; apenas as bordas transparentes foram recortadas. As versões pré-deslocadas permitem X par e ímpar sem perder um pixel na borda.

## Memória física em 128 KiB

| Intervalo | Uso |
| --- | --- |
| `$00000–$0DFFF` | Buffer circular e extensão de vídeo, 56 KiB |
| `$0E000–$0FFFF` | Banco do mapa; 7.808 bytes úteis |
| `$10C00…` | Estado e pequenos buffers, na metade superior da RAM |
| `$13000…` | Programa 6809 e trecho de áudio; endereço lógico inicial `$3000` |
| `$18000–$19FFF` | 256 tiles em 4 bpp, 8 KiB |
| `$1A000–$1BFFF` | Sprites com máscaras e HUD |
| `$1FEF4` | Salto de FIRQ do sistema |

A janela lógica `$8000` é usada para o framebuffer, `$A000` para os tiles e `$C000` para mapa ou sprites/HUD. O programa executa abaixo de `$8000`. O build e os testes continuam usando CPU 6809 e **128 KiB**, sem exigir 6309 ou expansão de 512 KiB.

## Verificação realizada

Os relatórios `DAC6-controls-verification.json` e `ORCH90-controls-verification.json` foram produzidos por execução real dos binários no XRoar 1.12:

- Boot pelo Disk BASIC em 128 KiB.
- Comparação byte a byte com um renderizador independente do mapa, tiles e sprite no início, em X ímpar, após movimento, na volta do anel, no fim do mapa e após reiniciar.
- Movimento diagonal por WASD e setas, limites de posição, entrada de Espaço, seleção K/J/L, zona central dos joysticks, pausa e reinício.
- Conferência dos valores do DAC e dos seletores de porta/eixo durante as leituras analógicas.
- Travessia dos 1.776 pixels de câmera, incluindo várias voltas do anel e parada no limite do mapa.
- Escritas de áudio não constantes em `$FF20` ou `$FF7B` durante a execução. O teste Orchestra-90 inclui o cartucho e o controlador de disco em Multi-Pak emulado.

Nos testes automatizados de entrada, valores externos de teclado e comparador foram fornecidos nas fronteiras das instruções `LDA $FF00`; o decodificador, a seleção das PIAs, o movimento e o desenho executaram no 6809 emulado. Isso não equivale a testar um joystick físico. A tentativa adicional de validar uma tecla enviada pela janela do Windows não foi concluída: a ferramenta de controle de janela falhou e não há resultado aprovado desse teste. Os hashes das imagens referem-se aos testes automatizados descritos acima.

## Limites desta etapa

Não há inimigos ativos, colisões com o cenário, projéteis, itens coletáveis, chefes com lógica ou passagem para outra fase. Alguns desenhos no mapa, inclusive a figura ao final, são apenas dados gráficos de fundo. Os valores do HUD continuam estáticos. O áudio continua sendo o trecho capturado de dez segundos com síntese aproximada; o sequenciador original ainda não foi convertido.

Esta versão é uma demonstração interativa de controles e travessia do mapa, não a conversão completa de Knightmare.

As bases de I/O usadas foram conferidas com a [referência técnica do CoCo](https://www.bighole.nl/pub/mirror/homepage.ntlworld.com/kryten_droid/coco/coco_tm_s3.htm) e o código local do XRoar. O relatório da etapa 1 permanece como histórico da análise da ROM.
