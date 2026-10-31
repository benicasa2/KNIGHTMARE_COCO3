> Histórico de revisão anterior. Estado atual: [IA-E-CHECKPOINTS.md](IA-E-CHECKPOINTS.md). A IA da primeira fase e a restauração dos trechos foram substituídas; o inventário agora é removido ao perder uma vida. Valores de memória/desempenho abaixo pertencem à revisão anterior.

# Combate nativo — primeira fase

Esta etapa acrescenta combate funcional à base de controles e rolagem. Implementa somente a primeira fase e seu chefe; não constitui a conversão integral solicitada inicialmente.

## Dados do original e decisões da conversão

A análise REA/Ghidra anterior foi mantida, incluindo listagem Z80 e tabelas de despacho. Nesta etapa a leitura da ROM fornecida foi complementada pela desassemblagem comentada de antxiko, commit `250a1502e77bbdc4c488c28c82382aafd2fd36af`. A revisão do cartucho desse projeto difere da ROM do usuário; o extrator usa e verifica o hash da ROM do usuário, não substitui o arquivo pelo outro dump. Os metadados reproduzíveis estão em `combat-assets.json`.

Endereços abaixo são endereços Z80 no espaço `$4000–$BFFF`.

| Evidência do MSX | Uso no CoCo |
| --- | --- |
| Ponteiro de oleadas `$704B` → `$705B`; 34 pares | Ordem, tipos e contagens lidos da ROM. Agendamento e formações simplificados; tabela se repete enquanto a câmera não chega à arena. Entradas nulas representam pausas. |
| Tipos da primeira fase: 1, 2, 9 e 14 | Arte original e comportamentos nativos aproximados. Tipo 9 requer três impactos; os demais um. |
| Sete inimigos, três tiros do jogador e oito tiros hostis | 18 registros de 32 bytes; não há alocação dinâmica. |
| Caixas dos inimigos em `$87C4`; jogador em `$888F` | Retângulos do jogador com margem 3, área 10×10 e caixas dos inimigos extraídas; flechas usam área varrida para compensar passos maiores. |
| Pontos em `$75BA` | Tipos 1/2: 50, tipo 9: 100, tipo 14: 10. HUD e recorde atualizados. Pontuação interna de 16 bits. |
| Terreno sólido `$43..$7F` e `$A3..$DF` (carry indica bloqueio) | Teste de dois pontos dos pés bloqueia movimento manual para tiles sólidos. Ainda não reproduz empurrão/esmagamento pelo cenário durante a rolagem. |
| Resistência do primeiro chefe em `$9992`: 20 | Chefe recebe 20 impactos de flechas. Barra visual de energia acrescentada na versão CoCo. |
| Forma de tiles `$BECC` → `$BED4`, duas formas de 5×5 tiles | Chefe de 40×40 pixels animado. Não foi usado o chefe de sprites de `$902D`, que pertence à fase seguinte. |
| Disparo mirado do chefe, cadência original de 32 quadros | A cada 16 atualizações nativas, direção aproximada em oito setores; órbitas dos satélites foram adaptadas. |
| Três satélites do tipo 12, escalonamento `$55/$AA/$FF` | Arte original, órbita nativa e surgimento escalonado. Caixa vulnerável do chefe é aproximada. |
| Velocidade da flecha básica: 8 pixels por quadro original | 16 pixels por atualização nativa; limite de três e intervalo de quatro atualizações entre disparos. |
| Atraso de morte: `$90` quadros originais | 72 atualizações nativas. Renascimento no mesmo ponto da fase, três vidas totais e imunidade de 60 atualizações são escolhas desta versão. |

Há explosões de três formas originais, tiros hostis, colisão de contato, fim das vidas e reinício. Derrotar o chefe limpa projéteis/inimigos, dá bônus adaptado de 1.000 pontos e encerra a demonstração. R mantém o recorde da sessão e reinicia pontos e vidas.

A figura do chefe antes embutida no mapa foi removida apenas no mapa nativo `stage1-combat-map.bin`, em tiles (14,8), e substituída por fundo adjacente; o mapa extraído original permanece intacto. O extrator confere o bloco de 25 tiles, permitindo uma diferença de animação.

## Implementação e memória

O motor é 6809 nativo, sem emular Z80. Duas compilações usam a mesma lógica; muda a saída DAC6 ou os dois canais Orchestra-90. A música é o trecho de 600 quadros de parâmetros PSG da etapa anterior, com síntese aproximada a 3.924,94 amostras/s. Não há sequenciador integral nem efeitos de disparo, impacto ou chefe acionados pelos eventos desta etapa.

| Área | Uso |
| --- | --- |
| RAM física `$00000–$0DFFF` | Anel de vídeo de 57.344 bytes: 256 linhas + 192 duplicadas |
| Banco físico 7, `$0E000` | Mapa nativo da primeira fase |
| Banco 12, `$18000` | 256 tiles compactados, 8 KiB |
| Banco 13, `$1A000` | Popolon e HUD |
| Banco 14, `$1C000` | Arte de combate, 7.636 bytes |
| CPU `$0C00–$0CAD` | Estado, incluindo áudio e combate |
| CPU `$0E00`, `$1000` | Fundo salvo de Popolon e linha temporária |
| CPU `$1100–$22FF` | Fundos salvos dos 18 objetos |
| CPU `$2300–$266F` | Fundo salvo do chefe e barra |
| CPU `$2700–$293F` | 18 registros de entidades |
| CPU `$3000–$68FB` | Código e dados fixos na compilação DAC6; Orchestra ocupa um byte adicional |

O build verifica os limites da página direta e da janela fixa de código. As duas variantes foram executadas com 128 KiB; não exigem 6309 ou 512 KiB. A pilha começa em `$2FFF`. Os endereços CPU baixos representam RAM física superior pelo mapeamento MMU, separados do framebuffer.

Objetos salvam fundo antes de desenhar e são apagados em ordem inversa, incluindo o chefe. O compositor aplica máscara diretamente no framebuffer e trata alinhamento ímpar. Sprites parcialmente fora dos limites são ocultados integralmente nesta versão. A atualização nominal é 29,96 Hz, com rolagem nominal de 14,98 pixels/s; desempenho constante sob carga máxima não foi certificado.

## Verificação dos discos atuais

XRoar 1.12, CPU 6809, RAM 128 KiB. DAC6 e Orchestra-90 passaram nos mesmos cenários; a segunda usa cartucho Orchestra e disco em Multi-Pak emulado.

- Disparo mantido, limite de três flechas e aparecimento da primeira oleada.
- Movimento bloqueado por um tile sólido do mapa original.
- Sobreposição de inimigos, projétil e jogador; imagem resultante e restauração do fundo comparadas byte a byte.
- Flecha atinge inimigo, soma pontos e desencadeia explosão com duração limitada; tipo 9 exige três impactos.
- Contato e projétil hostil retiram vida; atraso de morte, renascimento, imunidade, game over e R.
- Arena do chefe, disparos e satélite ativos, redução de energia em 20 impactos preparados e encerramento após a explosão.
- Imagens do framebuffer comparadas com renderizador independente de mapa, tiles, arte e camadas. O oráculo lê o HUD em RAM; ele não prova de forma independente todos os glifos numéricos do HUD.
- 128 escritas de áudio por variante, com valores não constantes. Isso prova atividade da saída; não prova equivalência musical nem qualidade sonora.

Os testes são cenários controlados: o depurador fornece entradas, reposiciona entidades e prepara impactos. Para chegar rapidamente à arena, salta a câmera e recompõe o fundo de referência como preparação; o código nativo desenha e processa as colisões em seguida. Não foi vencida uma partida completa sem intervenção. As imagens documentam o código CoCo, não são capturas do MSX apresentadas como conversão.

A regressão de controles executa o decodificador de teclado/joystick com sinais externos simulados nas leituras da PIA, e atravessa os 1.776 pixels da câmera no caminho de rolagem conservado, com combate desativado pelo depurador. Essa regressão não prova a travessia completa com inimigos ativos. Não houve validação em CoCo real ou joystick físico.

## Trabalho ainda necessário

Conversão exata das máquinas de estado Z80, dos gatilhos/formações e da dificuldade; física do cenário durante a rolagem; itens e armas; demais fases e chefes; efeitos e sequenciador de áudio; telas de abertura/transições; desempenho sob carga; partidas completas e teste em hardware. Os documentos RELATORIO e CONTROLES-E-ROLAGEM são históricos das etapas anteriores.
