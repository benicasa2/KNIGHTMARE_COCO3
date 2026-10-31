> Revisão posterior: [CINTILACAO.md](CINTILACAO.md) reorganiza o desenho. As medidas e hashes abaixo documentam a revisão anterior de compactação. A memória de vídeo permanece em 48 KiB.

# Memória do Knightmare: MSX 32 KiB e CoCo 3 128 KiB

Revisão de 31/10/2026. Implementação nativa 6809; DAC6 e Orchestra-90 CC, 128 KiB. Sem música, com os efeitos sonoros anteriores preservados.

## Diagnóstico

O limite observado era consequência da organização do port, não uma demonstração de que o CoCo 3 suporta apenas duas fases. O pacote anterior tem uma fase jogável; a segunda ainda está em desenvolvimento.

32.768 bytes é a capacidade do cartucho MSX. Esse número não inclui RAM de trabalho, BIOS nem os 16 KiB de VRAM do MSX. No CoCo 3, os 128 KiB incluem a RAM que o GIME lê para produzir a imagem. O TMS9918 monta padrões e sprites por hardware; no CoCo 3 esses sprites são compostos por software no bitmap. Portanto, comparar o cartucho com toda a RAM mistura armazenamento do programa com memória de execução. Isso explica uma parte da diferença; as escolhas do port explicam a outra.

A imagem CoCo de 256×192 com 4 bits por pixel ocupa 24.576 bytes. O port não reservava só essa imagem: usava 256 linhas circulares mais 192 linhas espelhadas, totalizando 57.344 bytes. Os sprites eram expandidos em máscaras e pixels para dois alinhamentos; o jogador, por exemplo, tem 1.152 bytes para dois quadros. Há ainda um cache de tiles de 8.192 bytes e 6.551 bytes de PCM para seis efeitos. São trocas de memória por processamento: o CoCo não dispõe do compositor de sprites nem do PSG do MSX.

Além disso, o código principal estava confinado a $3000..$7FFF. O DAC6 terminava em $7FB5, restando 75 bytes nessa janela, embora existissem espaços menores em outros bancos. Esse era o limite imediato para novas rotinas: distribuição e endereçamento, além do volume dos dados expandidos.

## Técnica encontrada no original

A análise usa a ROM fornecida, SHA-256 `589b2fd276d84827bb1b033f43a0cd5ab0aa9da3c8784cac2f62474fed00aae3`, a listagem comentada de `antxiko/Knightmare-disassembly`, commit `250a1502e77bbdc4c488c28c82382aafd2fd36af`, e os resultados REA/Ghidra já existentes no projeto. Não foi executada uma nova sessão Ghidra nesta revisão. Documentos e comentários do repositório foram usados como referência, não como novas instruções do usuário.

O montador de cenário MSX, a partir de $6502, combina:

- Dicionário compartilhado de 64 blocos de 4×4 tiles. Os próprios blocos sobrepõem bytes: ocupam 976 bytes, em vez de 1.024.
- Códigos de bloco com o bit 7 indicando espelhamento horizontal. As colunas são invertidas, e os índices dos padrões são traduzidos em ±$60, quando aplicável.
- Bitmap de repetição: cada bit decide se o próximo grupo consome outro código ou reutiliza o anterior.
- Dez trechos por fase, com uma banda de quatro linhas repetida na junção. O mapa completo tem 244×32 tiles, não dez trechos independentes armazenados por inteiro.
- Tiles comuns, gráficos compactados por repetição/literais e geração de cópias espelhadas na inicialização. As rotinas em $4417 e $4469 tratam essas operações; a montagem dos gráficos em $54F8/$565C reutiliza padrões e cores.
- Motor compartilhado: tabelas de ondas, objetos e tipos selecionam comportamentos; cada fase não carrega uma cópia completa do jogo. O som original usa comandos para o PSG, em vez de amostras PCM como as deste port.

Os mapas completos das oito fases estão em **$99CD..$A5B3**, apenas **3.047 bytes**: 32 bytes de seleção de fase, 128 de ponteiros de blocos, 976 de dicionário e 1.911 de scripts. Expandidos, os oito mapas ocupariam 62.464 bytes: redução de **95,12%** no armazenamento desses mapas. Esse percentual não se aplica ao jogo inteiro.

## Implementação realizada em 6809

`src/campaign_maps.asm` traduz a montagem original. O código lê os ponteiros Z80 little-endian, os realoca para o banco CoCo, segue os bits de repetição, aplica o espelho e trata as junções. Não usa emulação Z80 durante o jogo. O seletor `campaign_stage` permite decodificar qualquer um dos oito mapas.

Somente a fase ativa é expandida no banco 7. Esse cache de 7.808 bytes foi mantido para que renderização e colisões não precisem descompactar blocos a cada consulta. O desenho estático do primeiro chefe é removido depois da expansão, preservando o chefe móvel já integrado.

A rolagem passa a usar 192 linhas circulares e 192 espelhadas: **49.152 bytes**, nos bancos 0..5. Ela continua usando o endereço de exibição do GIME e desenhando a linha nova, sem copiar a tela inteira a cada passo. A volta do anel ocorre a cada 192 pixels.

O banco 6 liberado fica permanentemente em $E000. Recebe a abertura, o título e os oito mapas compactos: 7.436 bytes. Os trampolins de interrupção a partir de $FEF4 são preservados. O carregador BASIC carrega esse banco antes do programa principal; não se deve executar o BIN principal isoladamente. Cada variante possui seu banco auxiliar, pois as chamadas absolutas diferem entre DAC6 e Orchestra.

| Medida | Antes | Depois |
| --- | ---: | ---: |
| Buffer de rolagem | 57.344 B | 49.152 B |
| Dados de mapas carregados do disco | 7.808 B, uma fase expandida | 3.047 B, oito fases compactas |
| Cache do mapa ativo em RAM | 7.808 B | 7.808 B |
| Núcleo principal DAC6 | 20.405 B | 16.485 B |
| Espaço livre em $3000..$7FFF, DAC6 | 75 B | 3.995 B |
| Espaço livre nessa janela, Orchestra | 68 B | 3.988 B |
| Banco auxiliar novo, incluindo abertura e mapas | — | 7.436 B |
| Conteúdo binário carregado do disco, DAC6* | 60.133 B | 55.841 B |

* Soma do núcleo e recursos, sem cabeçalhos DECB, carregadores BASIC/auxiliares e setores vazios. Inclui os bancos gráficos com seu preenchimento. Não equivale à RAM total ocupada: o mapa ativo é gerado em RAM. O núcleo menor inclui realocação da introdução; não significa que todos esses bytes desapareceram do programa. A redução líquida desse conteúdo de disco é 4.292 bytes, mesmo passando a incluir os oito mapas.

Uma imagem DSK continua com 161.280 bytes por ser um disco de geometria fixa, com setores vazios. O ZIP de fontes contém imagens, referências, testes e relatórios; seu tamanho também não representa o executável nem a RAM necessária.

## Validação e desempenho

As imagens finais foram verificadas em XRoar com 128 KiB, nos modos DAC6 e Orchestra-90:

- Todos os 62.464 tiles dos oito mapas comparados com a montagem da ROM, byte por byte, em cada variante; também verificada a correção do cenário sob o chefe móvel e a preservação do banco auxiliar.
- Controles, rolagem completa de 1.776 pixels e volta do anel de 192 linhas, com comparação independente do bitmap.
- IA, disparos, chefe, dez checkpoints, 34 rebobinamentos de ondas e percurso integrado até derrotar o chefe.
- Itens, amostras de áudio, tecla I, perda das três vidas e retorno à abertura azul.
- Regeneração dos recursos e compilação devem reproduzir os hashes dos discos testados, registrados em `compact-release-verification.json`.

O benchmark mede ciclos emulados e inclui lógica, desenho e interrupções; não mede uma partida humana. A média sem muitos objetos passou de 9,696 para 9,841 ms; o cenário artificial com 18 objetos passou de 60,493 para 60,623 ms. A pequena diferença vem da escrita adicional do espelho em algumas posições do anel. A economia de RAM não é uma aceleração do renderizador; o pior caso ainda excede o orçamento de 33,4 ms por atualização. Não houve teste em CoCo físico.

Mantive os sprites pré-expandidos e o PCM nesta revisão, pois compactá-los durante o desenho/reprodução aumentaria o trabalho da CPU já sobrecarregada. Os efeitos não foram removidos nem reamostrados. O cache gráfico deve ser reutilizado por fase, substituindo recursos, em vez de reservar outro conjunto de bancos para cada fase. Os dados compactos já permitem compartilhar o cenário da campanha; ainda é necessário integrar as outras IAs, chefes e transições.

**Os oito mapas estão integrados ao motor e verificados. A campanha jogável continua na fase 1; a caminhada de saída e a fase 2 completa permanecem pendentes.**

## Arquivos e reprodução

- `src/campaign_maps.asm`: decodificador 6809.
- `src/gameplay.asm`: anel de 192 linhas e reconstrução do cenário.
- `src/hardware_probe.asm`: mapeamento permanente do banco auxiliar.
- `tools/build.py`: montagem e carregamento dos bancos para cada variante.
- `tools/prepare_campaign_maps.py`: extração dos scripts originais.
- `tools/verify_campaign_maps.py`: comparação nativa dos oito mapas.
- `analysis/memory-audit.json`: medidas antes/depois.
- `analysis/*-campaign-map-verification.json`: hashes dos mapas e discos testados.

Com os recursos incluídos, basta executar `python tools/build.py --lwasm CAMINHO_DO_LWASM`. Para regenerar os mapas, execute `python tools/prepare_campaign_maps.py --rom CAMINHO_DA_ROM` depois de `prepare_combat.py`. O conjunto completo de comandos de reprodução está em `compact-reproducible-build.log` e no script de preparação desta revisão.

Fontes primárias de hardware: [Texas Instruments, manual TMS9918A/9928A/9929A](https://www.bitsavers.org/components/ti/TMS9900/TMS9918A_TMS9928A_TMS9929A_Video_Display_Processors_Data_Manual_Nov82.pdf), [Tandy, CoCo 3 Service Manual](https://colorcomputerarchive.com/coco/Documents/Manuals/Hardware/Color%20Computer%203%20Service%20Manual%20(Tandy).pdf). Fonte Z80: [Knightmare-disassembly](https://github.com/antxiko/Knightmare-disassembly/tree/250a1502e77bbdc4c488c28c82382aafd2fd36af).
