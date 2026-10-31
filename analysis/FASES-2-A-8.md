> Histórico de revisão anterior. Estado atual: [IA-E-CHECKPOINTS.md](IA-E-CHECKPOINTS.md). A IA da primeira fase e a restauração dos trechos foram substituídas; o inventário agora é removido ao perder uma vida. Valores de memória/desempenho abaixo pertencem à revisão anterior.

# Fundo azul e preparação da campanha de oito fases

Esta entrega é **parcial em relação ao pedido das sete fases restantes**. O jogo principal continua com a primeira fase e seu chefe adaptado. Foram corrigidas as cores da apresentação e implementados, em um programa 6809 separado, carregamento e visualização dos recursos das oito fases. Os novos scripts ainda não comandam a IA do jogo principal. Não há progressão jogável da fase 1 até a 8.

## Apresentação corrigida

A tabela original em `$44C5..$44CC` escreve `$E4` no registrador 7 do TMS9918: o fundo da animação KONAMI é azul, índice MSX 4. A rotina de título em `$47BA` passa a `$E0`, fundo preto. `src/intro.asm` agora muda tanto a cor transparente da paleta GIME quanto a borda para o azul mais próximo disponível no CoCo 3. O título e o início da partida restauram o preto, inclusive ao pular a abertura com R.

As duas variantes foram recompiladas e testadas em 128 KiB. `analysis/DAC6-intro-konami.png` e `ORCH90-intro-konami.png` foram geradas da memória de vídeo e paleta realmente produzidas pelo 6809 no XRoar. A quantização da paleta RGB de 64 cores do CoCo 3 impede equivalência RGB exata com a paleta do MSX.

## Parâmetros extraídos da ROM

Fonte: ROM do usuário, SHA-256 `589b2fd276d84827bb1b033f43a0cd5ab0aa9da3c8784cac2f62474fed00aae3`, listagem REA/Ghidra preservada das etapas anteriores e desassemblagem comentada local de antxiko, commit `250a1502e77bbdc4c488c28c82382aafd2fd36af`. Esta etapa não executou uma nova análise Ghidra. Foram usados os endereços e bytes da ROM; comentários com numeração ambígua das fases não substituem a tabela real de despacho.

| Fase | Entradas de ondas | Objetos fixos | Rotina Z80 do chefe | HP genérico inicial |
| --- | ---: | ---: | --- | ---: |
| 1 | 34 | 36 | `$8A95` | 20 |
| 2 | 41 | 42 | `$8E34` | 30 |
| 3 | 41 | 42 | `$9090` | 30 |
| 4 | 38 | 46 | `$9201` | 45 |
| 5 | 36 | 52 | `$8A95`, variante da fase | 45 |
| 6 | 33 | 50 | `$8A95`, variante da fase | 45 |
| 7 | 30 | 45 | `$941D`, seis segmentos | 60 |
| 8 | 35 | 43 | `$95E4`, encontro com seis portões | sistema próprio |

Contagens de ondas: `$7043`. Ponteiros das listas: `$704B`. Despacho dos chefes: `$88F2`. HP genérico: sete bytes a partir de `$9992`. O byte seguinte é o começo de outra tabela: **não é HP 242 para o chefe final**. A fase 8 inicializa registros próprios em `$E400`, com valores de HP `$40` no primeiro registro inicialmente desativado e `$14` nos outros cinco; ativação e dano precisam da lógica específica original.

A fase 2 usa um composto de sete sprites e três bolas; a fase 3 tem abertura de boca, espelhamento e janelas de disparo; a fase 4 solta fragmentos e lê o registrador Z80 R em `$93AE`; a fase 7 gera os seis segmentos com contadores defasados. As fases 5 e 6 compartilham o endereço do primeiro chefe, mas dependem do número da fase para arte e comportamento. Reutilizar a rotina adaptada da primeira fase não reproduz essas diferenças.

Os mapas têm 244 linhas de 32 tiles, 1.952 pixels de altura. Cada pacote inclui o mapa de 7.808 bytes, 8.192 bytes de tiles CoCo 3 e um script nativo. As cores herdadas entre fases foram preservadas pelo extrator existente. Os tipos de inimigos, tokens, objetos e endereços de cada fase estão em `campaign/analysis/stages.json`. As oito listas somam 356 objetos fixos. As aparições dos tokens vêm das sequências posicionais originais, sem novos sorteios de drops.

## Carregador e visualizador 6809

Execute `VISUALIZAR-8-MAPAS.cmd`. O programa abre **outro programa de laboratório**, com `campaign/bin/BOOT.DSK` no drive 0 e `CAMPAIGN.DSK` no drive 1. São necessárias as ROMs de sistema já usadas pelo jogo. Cima/baixo percorrem o mapa em passos de oito pixels, um toque por passo. Espaço carrega a fase seguinte. Depois da oitava, a última imagem permanece na tela. Feche e execute novamente para voltar à primeira.

Não há jogador, inimigos, chefe ou áudio nesse visualizador. Os comandos `EXECUTAR-DAC6.cmd` e `EXECUTAR-ORCH90.cmd` continuam iniciando a versão jogável da primeira fase, agora com a abertura azul, sem música e com efeitos sonoros.

`campaign/src/stage_loader.asm` implementa leitura WD2793, seleção do drive, busca de trilha, espera de DRQ/BUSY com limites, verificação de erros do controlador, fluxo entre setores, descompactação e escrita pelos bancos MMU. O NMI do disco usa um RTI no trampolim RAM correto do CoCo 3, `$FEFD`. A memória não contém as oito fases simultaneamente.

O disco de recursos ocupa 100.352 dos 161.280 bytes disponíveis. O carregamento não depende de retornar ao Disk BASIC durante a execução. O programa principal ainda não chama esse carregador; sua integração exigirá coordenar interrupções de áudio/vídeo, bancos de arte, estado do jogador e transições.

| Região | Uso no visualizador |
| --- | --- |
| CPU `$0900` | Estado do carregador e câmera |
| CPU `$1000..$10FF` | Um setor de entrada |
| CPU `$2000..$27FF` | Janela reservada ao script da fase |
| CPU `$3000` em diante | Programa 6809 fixo |
| Bancos físicos 0..2 | Tela 256×192 em 16 cores |
| Banco físico 7 | Mapa da fase atual |
| Banco físico 12 | Tiles da fase atual |

Formato KMST v1: assinatura `KMST`, versão 1, índice de fase 0..7 e três recursos. Cada recurso começa com banco e tamanho descompactado big endian. Banco `$FF` indica destino CPU `$2000`; os outros destinos usam a janela `$8000`. Controles 0..127 copiam 1..128 bytes literais; 128..255 repetem o byte seguinte 1..128 vezes. Recursos terminam pelo tamanho descompactado. Scripts STGP v1 contêm contagens, endereço Z80 do chefe apenas como referência, HP e listas de ondas/tokens/objetos com palavras convertidas para big endian.

## Validação e limites

`campaign/analysis/loader-verification.json` comprova que o 6809 leu os três recursos de cada uma das oito fases pelo controlador de disco. O verificador não escreve os recursos nem os pixels na RAM do emulador. Compara os bytes carregados e a imagem nativa em três posições de câmera por fase: início, meio e fim do percurso. As posições e o avanço automático são fixtures do depurador, não uma partida. São 24 comparações de imagem, com PNGs na pasta `campaign/analysis/`.

Isso valida o transporte e o renderizador CoCo 3 contra os recursos extraídos. Não é comparação dinâmica das fases 2..8 com uma partida MSX, nem validação dos seus inimigos, chefes, checkpoints ou encerramento. A checagem anterior de VRAM no openMSX cobre a primeira fase. Não houve teste em hardware físico.

As regressões do jogo principal passaram para DAC6 e Orchestra-90: abertura/itens, combate e controles/rolagem. O desempenho permanece o da revisão anterior: média de aproximadamente 12,74 ms no cenário vazio e 63,09 ms no cenário preparado com 18 objetos. Esta entrega não elimina a lentidão sob carga.

Para concluir o pedido ainda faltam as rotinas 6809 dos tipos de inimigos posteriores, cache de arte suficiente, chefes específicos, integração dos scripts, transições e EXIT, restauração de checkpoints e verificação de uma campanha completa. Esses pontos permanecem marcados como incompletos em `STATUS.json`.

## Reproduzir o laboratório

```powershell
python tools/prepare_campaign.py --rom C:\caminho\Knightmare.rom --lwasm C:\caminho\lwasm.exe
python tools/verify_campaign_loader.py --xroar C:\caminho\xroar.exe
python tools/run_campaign_viewer.py --xroar C:\caminho\xroar.exe
```

O pacote contém os recursos prontos. A ROM é necessária apenas para regenerar os pacotes. O código do laboratório foi escrito para 6809; os endereços Z80 nos metadados documentam a origem e não são executados pelo CoCo 3.
