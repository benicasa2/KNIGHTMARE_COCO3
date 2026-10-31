# Correção do movimento — 30/10/2026

O bloqueio das teclas direcionais foi reproduzido no disco anterior: com combate ativo, a seta para cima era decodificada, mas a posição permanecia em (120,136). O chão inicial usa os tiles `$80..$82`.

A rotina MSX em `$620C–$6215` devolve carry ligado para os intervalos **sólidos** `$43..$7F` e `$A3..$DF`. A primeira implementação nativa interpretava esses intervalos como transitáveis e bloqueava o restante, incluindo a grama. A correção em `src/combat.asm`, `terrain_tile`, inverte esse resultado. O teclado e o joystick compartilham a verificação de terreno corrigida.

O teste novo em `tools/verify_combat.py` falhou no disco anterior já na primeira seta. Nos discos corrigidos ele verifica cima, baixo, esquerda, direita e W/S/A/D na posição inicial, com combate ativo, além de confirmar que obstáculos verdadeiros continuam bloqueando o movimento. Também são repetidos os cenários de projéteis, sobreposição de sprites, vidas, chefe e áudio nas duas variantes de 128 KiB.

Os testes antigos de controles isolavam o caminho sem combate e, portanto, não detectaram a inversão da colisão introduzida nesta etapa. Agora a regressão cobre explicitamente o caminho com combate. Os testes de entrada simulam sinais externos nas leituras da PIA; não constituem um teste físico do teclado ou joystick. Os resultados estão nos arquivos `DAC6-combat-verification.json` e `ORCH90-combat-verification.json`.

Para usar a correção, encerre a instância antiga do jogo, extraia o pacote **Combate-Corrigido** e execute `EXECUTAR-DAC6.cmd` ou `EXECUTAR-ORCH90.cmd` da pasta nova. Reiniciar com R na instância antiga não carrega o disco atualizado.
