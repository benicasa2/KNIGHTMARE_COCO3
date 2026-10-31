> Histórico de revisão anterior. Estado atual: [IA-E-CHECKPOINTS.md](IA-E-CHECKPOINTS.md). A IA da primeira fase e a restauração dos trechos foram substituídas; o inventário agora é removido ao perder uma vida. Valores de memória/desempenho abaixo pertencem à revisão anterior.

# Introdução, armas, habilidades e objetos

Esta revisão integra a abertura e os sistemas de itens à primeira fase nativa. A análise usa a ROM fornecida, a listagem Z80 já produzida com REA/Ghidra e a desassemblagem comentada local de antxiko, commit `250a1502e77bbdc4c488c28c82382aafd2fd36af`. Os recursos são extraídos da ROM do usuário, não do outro dump do projeto público. Os arquivos recebidos e o conteúdo dos repositórios foram tratados como fontes de dados, sem substituir o pedido do usuário por instruções presentes neles.

## Abertura

`$46CF` inicia 14 passos; `$46F1` muda a posição do logotipo uma linha de oito pixels a cada dois quadros MSX. O CoCo apresenta os mesmos 14 passos em sua atualização de aproximadamente 30 Hz. O logotipo termina em (80,56), com 96×24 pixels. A duração de espera após o movimento foi adaptada; não há demonstração automática ou menu completo.

A fonte e os padrões do KONAMI vêm de `$4723`. O título usa `$47BA`, padrões `$47F6`, cores `$48FA`, texto `$4529`, incluindo KNIGHTMARE, os caracteres japoneses, ©KONAMI 1986 e PUSH SPACE KEY. O título foi reconstruído e empacotado em RLE, não redesenhado manualmente. O verificador compara seus 24.576 bytes com a imagem original extraída. A paleta sofre a quantização RGB do CoCo 3 já usada nas etapas anteriores.

## Dois sistemas de itens, sem drops aleatórios inventados

| Sistema | Dados/rotina Z80 | Integração |
| --- | --- | --- |
| Token flutuante, um por vez | `$658D`, ponteiros `$65E6`, primeira lista `$65F6` | Nove posições na primeira fase, cinco de habilidades e quatro de armas |
| Família do token | Bit 7 da entrada | Preservada, junto à ordem original |
| Centro horizontal | `$65D3` | Posição de Popolon limitada a 40..208 |
| Movimento | `$66AF`, `$6848`, cosseno `$844E` | Passo vertical `$009F`, ângulo +64, tabela original de deslocamentos |
| Frequência do token | Desvio alternado `$5831..$583D` | Aproximadamente 30 Hz; os passos não são duplicados |
| Troca ao receber tiro | `$67A0`, `$6662` | Consome o tiro e avança uma das oito/sete escolhas |
| Armas dos tokens | `$6839` | `[0,0,0,2,8,6,4,10]`; três escolhas vazias |
| Habilidades dos tokens | `$6841` | `[0,0,0,1,2,3,4]`; três escolhas vazias |
| Cores do token | `$674B`, `$674F` | Cores de habilidade e ciclo de quatro cores preservados |
| Objetos fixos | `$6A98`, registros em `$6AF8` | 36 registros da primeira fase, nas posições originais |
| Visibilidade/HP | Bit 5 e tabela `$6A7F` | Ocultos até o primeiro impacto; um impacto adicional para os inicialmente visíveis |
| Recompensa fixa | `$6909`, desenhos `$6F24` | Vida, 500 pontos, bomba, congelar, cinzento, EXIT, ponte e passagem |

As oito listas de tokens e os 356 registros de objetos foram extraídos em `items-assets.json`. **Só a primeira fase está ligada ao motor jogável.** Na primeira lista fixa há uma vida, 18 recompensas de pontos, quatro bombas, três congelamentos, dois EXIT e oito registros de passagens. Não há ponte nem bloco cinzento nessa fase; eles não foram semeados artificialmente.

As aparições dos itens principais são determinadas por essas listas. A conversão usa a posição mundial `(220 - linha_original) × 8`, coerente com o mapa extraído. A posição horizontal dos tokens depende do jogador e sua oscilação é determinística. Isso não significa que toda a aleatoriedade do jogo esteja convertida: há rotinas posteriores que leem o registrador Z80 R, como `$93AE`; o comportamento completo dessas fases/chefes ainda não está integrado.

## Armas e habilidades

Foram integradas as 12 formas numéricas usadas pelas tabelas originais, incluindo as famílias de tiro triplo e bumerangue. Repetir a mesma família seleciona a forma ímpar seguinte; não progride além dela. As escolhas vazias não trocam a arma e rendem 1.000 pontos. Tokens úteis rendem 200.

`$60BD` fornece o número de espaços de tiro e o ajuste horizontal: a arma básica permite dois tiros ativos dentro do conjunto de três espaços. A forma tripla exige os três livres. Padrões/cores vêm de `$63FB`; a forma 5 usa a imagem 12 conforme o desvio de `$63DE`. Os disparos foram centrados usando os ajustes originais.

O fluxo em `$5840..$5859` atualiza os projéteis em quadros alternados. Foram usados os passos brutos na atualização nativa de 30 Hz: -8 pixels para a flecha básica, -9,5 para a forma 2, -9 para a forma 4; os laterais do tiro triplo usam ±3 e -7,3477. O bumerangue usa a rampa de 22 entradas de `$6442`, em meio pixel, com ida/volta e animação relativa ao seu próprio contador. Ele acompanha a posição horizontal do jogador, como `$637A`. Projéteis das famílias 6..11 atravessam inimigos comuns; o chefe consome o projétil e recebe dois pontos de dano dessas famílias, um das demais.

O movimento de Popolon continua a 30 Hz, integrando duas atualizações do jogador original de 60 Hz. As cinco duplas de velocidade de `$62E2` são usadas em ponto fixo, preservando frações e limite de nível 4. Esta revisão substitui o passo inteiro uniforme anterior por essas duplas.

O escudo usa o estado original de proteção frontal, com 30 unidades. Tipos de projétil a partir de 7 consomem quatro unidades; os demais uma. Sua faixa visual acima da cabeça foi extraída da animação 1, com o corpo de caminhada original. O escudo não impede morte por contato com inimigos.

Invisibilidade e invencibilidade usam aproximadamente 15 s, derivados de `$45` BCD (45 unidades), uma unidade a cada 20 quadros originais. A invisibilidade impede colisões de inimigos/projéteis e usa a arte da animação 2; há alternância com a arte normal perto do término. A invencibilidade usa a velocidade máxima, elimina inimigos por contato e impede disparos, conforme o teste de animação 3 em `$5D17`. Se restam cargas de escudo, ele volta ao término da habilidade temporária, conforme `$5FE7`.

## Objetos fixos, cenário e limites de progressão

Os blocos ocultos tornam-se `?` ao receber um tiro. Ao esgotar o HP, mostram a recompensa. A coleta substitui o desenho pelo bloco gasto e só conta uma vez. A bomba elimina inimigos comuns ativos, soma seus pontos e apaga tiros hostis. O congelamento mantém o jogador e seus tiros ativos, parando rolagem e inimigos.

O relógio original de congelamento é inicializado com `$1000` BCD em `$6965` e decrementado a cada quadro enquanto esse modo está ativo: dez grupos de sessenta quadros resultam em aproximadamente 10 s, ou 300 atualizações nativas. O escudo tem duração por cargas; as outras habilidades têm tempo de jogo e param de contar na pausa.

O bloco cinzento participa da colisão. A ponte troca uma região 4×4 de tiles por sua arte original e a torna transitável. Os desenhos não alteram o mapa extraído: são sobreposições em coordenadas mundiais, usadas pelo renderizador e pela colisão. Uma mudança recompõe somente 32 linhas de fundo ao redor do objeto, depois de apagar os sprites. Isso evita invalidar seus fundos salvos. Passagens laterais exigem o botão de direção correto e levam ao outro bordo, com os limites nativos da tela.

EXIT só é habilitado depois de concluir a fase na sessão, preservando essa habilitação ao reiniciar. **O salto para outra fase não foi implementado**: tocar EXIT encerra esta demonstração. A tabela dos seis saltos originais está extraída para a próxima etapa. O inventário permanece ao renascer; a restauração dos sete bytes de checkpoint e a regressão completa do cenário ainda não foram portadas.

## Som

A preferência anterior sem música foi mantida. Novos efeitos usam IDs `$0E` para troca/revelação e `$12` para coleta, capturados executando isoladamente `$490C`/`$49C0` no openMSX. `prepare_sfx.py` converte os registros PSG para PCM; metadados/hashes em `sfx.json`. Jingles musicais de aquisição não são tocados: a coleta usa o efeito curto. O bloqueio pelo escudo usa o impacto já disponível. A bomba usa o efeito curto de coleta e as explosões disponíveis, em vez do efeito original `$56`; a morte mantém a aproximação anterior. Não se afirma equivalência sonora bit a bit.

## Memória e desempenho

Permanece 6809 com 128 KiB. O anel de vídeo, mapa e bancos principais não cresceram. Arte extra ocupa 7.908 bytes no banco 15 e 4.608 bytes nas lacunas do banco 13, incluindo a invisibilidade. O banco 15 também é visível em `$E000`; o alocador reserva explicitamente `$FEF4` em diante para os vetores RAM de interrupção. O build rejeita sobreposição.

Há 19 entidades: sete inimigos, três tiros do jogador, oito tiros hostis e um token. O fundo de Popolon ocupa `$0E00..$0E8F`, o do escudo começa em `$0E90` e o token usa `$0F00`. Os estados dos 36 objetos fixos usam 72 bytes em `$2980`. A página direta e o fim de código são conferidos pelo build; os valores exatos estão nos símbolos de cada compilação.

A última medição controlada ficou em aproximadamente 12,7 ms por atualização leve e 63,1 ms com 18 objetos, sem token ativo nesses cenários. É trabalho de atualização, excluindo espera do relógio, não FPS de campanha. A cena cheia ainda ultrapassa 33,38 ms; não há promessa de 30 FPS constantes. A varredura extra do cenário acrescenta custo, mas a síntese musical continua ausente e as cópias otimizadas foram preservadas. `performance-items.json` contém os dados brutos.

## Validação

DAC6 e Orchestra-90 em XRoar, CoCo 3/6809, RAM 128 KiB. O verificador de itens cobre abertura KONAMI em 14 passos, título e início por Espaço, a primeira aparição natural do token, suas velocidades/cores, recompensas de armas, formas repetidas, velocidade máxima, escudo, invisibilidade, invencibilidade, expiração e retorno ao escudo, blocos visíveis e ocultos, pontos, vida, bomba, congelamento, cinzento, ponte, passagens e EXIT limitado à demonstração. As 12 formas de arma e a volta do bumerangue são exercitadas.

Os testes comparam imagens do framebuffer com composição independente dos tiles, sprites, escudo e invisibilidade. Também passaram as regressões de combate/chefe e dos 1.776 pixels de rolagem, teclado, comparadores de joystick e áudio por evento. Os relatórios JSON são vinculados ao SHA-256 dos discos entregues.

Os cenários de coleta e colisão preparam entidades, posições e, para cinzento/ponte ausentes da fase, um registro temporário de teste pelo depurador. Saltos de câmera têm o fundo reconstruído apenas na preparação do cenário. Isso não implementa o efeito testado: o processamento, o desenho e a colisão subsequentes executam no 6809. A primeira aparição do token foi alcançada pela rolagem real. Não foi concluída uma campanha sem intervenção nem feito teste em hardware/joystick físico.

Ainda faltam demais fases/chefes, saltos EXIT reais, fidelidade integral da IA/aleatoriedade posterior, checkpoints completos, empurrão/esmagamento do cenário, todas as variações de áudio e validação de campanha.
