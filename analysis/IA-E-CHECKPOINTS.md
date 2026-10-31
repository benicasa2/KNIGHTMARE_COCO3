> Memória e carregador atualizados: veja [OTIMIZACAO-MEMORIA.md](OTIMIZACAO-MEMORIA.md). Medidas de memória e hashes antigos neste documento são históricos.

> Atualização de 31/10/2026: [REINICIO-E-TECLA-I.md](REINICIO-E-TECLA-I.md) documenta o retorno automático à abertura e o modo de teste na tecla I.

# IA extraída da ROM e checkpoints da primeira fase

A primeira fase agora utiliza rotinas 6809 traduzidas da IA Z80 dos seus inimigos e do primeiro chefe. A revisão substitui trajetórias aproximadas, o disparo genérico e a órbita inventada dos satélites. Também integra a reconstrução dos trechos após perder uma vida. As variantes DAC6 e Orchestra-90 CC continuam em **128 KiB, sem música e com efeitos sonoros**.

O trabalho cobre os inimigos da primeira fase, seu chefe e seus checkpoints. As fases 2..8 continuam fora da campanha jogável. A extração inclui tabelas para os demais tipos e endereços dos demais chefes, mas não traduz suas rotinas automaticamente. Ainda existem adaptações visuais, de som e desempenho; não se afirma equivalência completa do jogo.

## Origem e método

ROM fornecida: SHA-256 `589b2fd276d84827bb1b033f43a0cd5ab0aa9da3c8784cac2f62474fed00aae3`, preservada sem alterações. Foram consultadas a análise REA/Ghidra existente e a listagem comentada local de `antxiko/Knightmare-disassembly`, commit `250a1502e77bbdc4c488c28c82382aafd2fd36af`. Esta revisão não executou uma nova sessão Ghidra.

`tools/prepare_ai.py` extrai bytes dessa ROM e executa suas instruções Z80 com a ferramenta de referência `third_party/knightmare/z80run.py`. O intérprete rejeita opcodes não implementados; não é um MSX completo nem um modelo de ciclos. Chamadas de som são substituídas por retorno somente na cópia de memória do verificador. A montagem dos checkpoints suprime as chamadas de música. O código do jogo CoCo 3 permanece 6809 nativo e não contém esse intérprete.

A referência do chefe inicializa C' em `$98`, contexto do chamador original que usa o porto VDP. Isso é necessário porque a composição dos satélites usa LDI dentro de DJNZ. Na referência das ondas, pedidos de gráficos são atendidos entre atualizações, preservando a espera de uma atualização; a transferência de padrões não participa da comparação da IA. No CoCo 3, os gráficos da primeira fase já estão carregados.

As instruções encontradas em arquivos/repositórios são tratadas como material de referência; não substituem o pedido do usuário.

## Rotinas portadas

| Sistema | Endereços Z80 principais | Implementação 6809 |
| --- | --- | --- |
| Gerador de ondas, duas filas e formações | `$6F75`, `$729B..$741C` | `src/ai.asm` |
| Inimigo 1, curva em 8.8 | `$795F..$7A31` | `ai_enemy_init`, `ai_pull_full` |
| Inimigo 2, entrada lateral e curva parcial | `$7A81..$7AC3` | `ai_init_two`, `ai_pull_half` |
| Inimigo 9, aproximação e arrancada lateral | `$7EB9..$7F1C` | `ai_init_nine`, `ai_enemy_tick` |
| Inimigo 14, queda lenta | `$8114..$8126` | `ai_init_fourteen` |
| Tipo 12 usado pelos satélites | `$8014..$8075` | `ai_init_satellite`, `ai_satellite_move` |
| Condições e relógios de disparo | `$7633..$7729` | `ai_enemy_fire`, `ai_data.inc` |
| Mira e velocidades | `$83E1..$857E`, `$86B5..$872A` | `ai_aim_angle`, `aimed_shot` |
| Primeiro chefe e satélites | `$8A95..$8CBE` | `src/boss_ai.asm` |
| Caixa/dano do chefe | `$98DB..$997A`, `$9999`, `$99B5` | `hit_boss`, tabelas extraídas |
| Reinício de trecho e rebobinamento de ondas | `$53D3..$55EA` | `src/checkpoints.asm` |

O movimento dos inimigos conserva posições e velocidades em ponto fixo 8.8, inicializadores, destinos, mudanças de estado e descarte ao sair da tela. A curva parcial preserva o comportamento do SRA de oito bits do original. O tipo 9 conserva três impactos das armas fracas, sua espera de 48 atualizações e a passagem para queda de 1,5 pixel por atualização.

O gerador conserva as duas filas, bit `$20` de sobreposição, bit `$40` de formação, quantidades de grupos, intervalos entre grupos/membros, posição inicial e identificação de formação. A retirada de gráficos do caminho de execução não antecipa o nascimento: a espera original foi mantida. Eliminar os cinco membros de uma formação válida rende o bônus de 1.000 pontos.

## Disparos

Os tipos 1/2 de inimigo usam a família 2 de projétil; o inimigo 9 usa a família 1; o 14 usa a família 3. O primeiro chefe usa a família 8. Os relógios de disparo vêm de `$76CD/$76ED`, com um evento a menos nas três primeiras fases, conforme o original. Na primeira fase, é necessário o bit `$80` do tipo; membros alternados das formações não disparam. A rotina também verifica as distâncias mínimas ao jogador e a posição vertical.

A mira usa a divisão inteira, a tabela de tangentes e 32 direções do original, em lugar das oito direções aproximadas anteriores. As velocidades das famílias 1, 2, 3 e 8 foram comparadas executando a ROM. A família 2 preserva inclusive a soma auxiliar unsigned feita pela rotina original. A arte pequena das famílias 2/3, os desenhos direcionais da família 1 e as caixas dos projéteis foram integrados. As entradas literais `$0C` da tabela de desenhos foram preservadas.

## Chefe

O corpo executa duas atualizações originais por atualização nativa; os inimigos e satélites executam uma. O chefe escolhe um destino a partir da coluna do jogador arredondada a quatro, move-se com o passo original e espera 80 quadros originais ao chegar, com limites `$20..$BF`. Conserva 20 unidades de resistência, dano por arma, ataque periódico e limite de luta de 3.600 quadros originais. A derrota por disparos concede 10.000 pontos, corrigindo a recompensa adaptada anterior.

A entrada começa na câmera 120; a rolagem termina em 48. O corpo sobe em passos de oito pixels segundo a máscara de 64 quadros da ROM. A animação alterna pelo bit 4 do relógio. Os satélites não orbitam: usam a trajetória serpenteante do tipo 12 e entram por lados alternados.

Um detalhe do código foi confirmado por execução: a rotina lê os limites dos satélites como **58, 255 e 170**. Apesar de a pequena tabela conter `$55,$AA,$FF`, o índice usado na primeira leitura alcança o byte `$3A` da instrução seguinte. A tradução preserva essa leitura, sem substituí-la pela interpretação sugerida pelo comentário.

A duração de saída conserva os 208 quadros das etapas `$40,$30,$30,$30`. A representação visual continua simplificada: não há ainda a composição completa dos dez fragmentos da morte do chefe, e sua entrada parcialmente fora da tela usa o recorte do renderizador nativo.

## Checkpoints

O retorno ocorre no começo do trecho atual de **24 linhas de tiles, 192 pixels**, e não no mesmo ponto de morte. O cenário é redesenhado pelo próprio 6809. Inimigos, projéteis, objetos temporários e estados dos blocos são reconstruídos; os tokens anteriores à câmera restaurada são pulados, permitindo reaparecimento dos que pertencem ao trecho reiniciado.

O inventário foi corrigido segundo a execução de `$53D3`: **perder uma vida remove arma, velocidade, escudo e habilidades**. Os sete bytes copiados para `$E080` são devolvidos quando se monta uma fase nova; com a fase já montada, essa devolução é pulada. A interpretação anterior de que o equipamento deveria permanecer após uma morte estava incorreta. Pontuação, recorde e vidas restantes são preservados.

O índice de onda é rebobinado conforme `$549A..$54B4`, até a entrada seguinte à anterior que termina com nibble de espera zero. Todas as 34 posições de índice da primeira fase foram confrontadas com a ROM executada.

No trecho 9, preservam-se posição, dano acumulado, destino, espera e relógio do chefe. Os satélites ficam inativos, mas seus contadores e frações de posição são preservados, como os registros originais fora da área apagada. Como o motor nativo compartilha o conjunto de entidades, uma pequena área em `$0A40..$0A45` guarda essas frações. A alternância lateral é reiniciada porque sua variável original está na área apagada.

A contagem de morte usa 48 atualizações nativas, equivalentes a 96 quadros MSX. A imunidade artificial de dois segundos após reaparecer foi removida. A apresentação da morte e a tela transitória de STAGE/vidas ainda utilizam a apresentação simplificada do port; a mecânica de reconstrução do trecho está integrada.

## Testes executados em 128 KiB

Os números abaixo se aplicam **a cada variante**, DAC6 e Orchestra-90 CC:

| Verificação | Cobertura aprovada |
| --- | --- |
| Trajetórias dos inimigos/satélites | 30 cenários × 180 atualizações = 5.400 amostras contra Z80 executado |
| Corpo do chefe e três satélites | 640 quadros originais, com jogador mudando de coluna |
| Ondas e formações | 91 nascimentos em 6.000 atualizações, incluindo posição fracionária e identificação de membro |
| Condições/tipos/velocidades de disparo | 56 cenários contra a rotina Z80 |
| Mira | 28 cenários com quadrantes, coincidência e divisão por zero |
| Retorno de trecho | Dez reconstruções completas com comparação independente do framebuffer |
| Rebobinamento de ondas | Todas as 34 posições contra a rotina Z80 |
| Perda de vida | Impacto real de projétil e retorno pela rotina normal do jogo |
| Integração da primeira fase | Rolagem completa até o chefe e derrota por flechas nativas |
| Regressões | Abertura azul, título, inventário, objetos, combate, teclado, joystick, rolagem e efeitos sonoros |

O percurso integrado não injeta câmera, inimigos, HP, impactos ou pixels: usa a proteção acionada pela tecla I e suprime tokens flutuantes para proteger o jogador e isolar a integração. O chefe foi derrotado após 21 disparos, com 20 acertos, e a recompensa registrada foi de 10.000 pontos. **Não é uma partida completa sem intervenção nem teste em hardware físico.**

Os testes unitários usam pequenos programas 6809 auxiliares que chamam as rotinas reais e registram suas saídas em um banco de vídeo não exibido. O verificador não escreve posições ou velocidades esperadas durante as trajetórias. Os casos de checkpoints usam câmeras/estados preparados; a reconstrução e os pixels resultantes são sempre calculados pelo jogo.

Relatórios por binário: `*-ai-verification.json`, `*-checkpoints-verification.json`, `*-stage1-flow-verification.json`, `*-combat-verification.json`, `*-items-verification.json` e `*-controls-verification.json`. Os SHA-256 dos discos testados constam nos relatórios. As referências Z80 e tabelas reutilizáveis estão em `ai-rom-reference.json` e `src/ai_data.inc`.

## Memória, ritmo e limites

A deduplicação de desenhos permitiu incluir os projéteis originais sem ampliar a RAM. São 44 descritores de arte; o banco 15 ocupa 7.900 bytes e continua respeitando os trampolins de interrupção em `$FEF4` em diante. A invisibilidade permanece no banco 13. O estado direto termina em `$0CEC`; os geradores/formações usam `$0A00..$0A2E` e as frações de satélites usam `$0A40..$0A45`.

O código termina em `$7FB5` no DAC6 e `$7FBC` no Orchestra-90. Há somente 75/68 bytes livres antes da janela MMU `$8000`; adicionar as rotinas das outras fases exige reorganizar código/dados ou carregá-los por bancos. O laboratório de carregamento das oito fases continua disponível separadamente e ainda não faz a progressão do jogo.

A rolagem foi corrigida para oito pixels por 64 quadros MSX: aproximadamente **7,49 pixels/s**, um pixel a cada quatro atualizações nativas. A revisão anterior usava aproximadamente 14,98 pixels/s. Essa diferença é ritmo do cenário, não uma redução da velocidade do processador.

O custo medido no DAC6 foi de aproximadamente 9,69 ms por atualização no cenário vazio e 60,48 ms no cenário preparado com 18 objetos. A cadência de rolagem mudou entre revisões; esses valores não são um ensaio isolado de otimização. Sob carga, o renderizador ainda ultrapassa o intervalo nominal de 33,38 ms, e não se promete 30 FPS constantes.

Ainda faltam a IA dos tipos das outras fases e suas variantes de chefe, transições/EXIT de campanha, efeitos visuais completos, reprodução sonora mais exata, empurrão/esmagamento pelo cenário e validação de campanha. A pontuação continua limitada ao contador nativo de 16 bits, em vez dos seis dígitos BCD completos do original. A paleta RGB e o renderizador CoCo 3 também mantêm suas adaptações.

## Regenerar e verificar

```powershell
python tools/prepare_combat.py --rom C:\caminho\Knightmare.rom
python tools/prepare_items.py --rom C:\caminho\Knightmare.rom
python tools/prepare_ai.py --rom C:\caminho\Knightmare.rom
python tools/build.py --lwasm C:\caminho\lwasm.exe
python tools/verify_ai.py --rom C:\caminho\Knightmare.rom --lwasm C:\caminho\lwasm.exe --xroar C:\caminho\xroar.exe
python tools/verify_checkpoints.py --lwasm C:\caminho\lwasm.exe --xroar C:\caminho\xroar.exe
python tools/verify_stage1_flow.py --xroar C:\caminho\xroar.exe
```

Os recursos prontos estão incluídos. A ROM original só é necessária para regeneração e comparação Z80. As ROMs de sistema do CoCo 3/Disk BASIC não estão incluídas.
