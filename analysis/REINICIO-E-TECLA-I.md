> Memória e carregador atualizados: veja [OTIMIZACAO-MEMORIA.md](OTIMIZACAO-MEMORIA.md). Medidas de memória e hashes antigos neste documento são históricos.

# Reinício pela abertura KONAMI e invencibilidade de teste

Revisão de 31/10/2026. Corrige o fim das vidas na primeira fase e acrescenta a tecla I ao jogo nativo. As duas variantes continuam em 128 KiB, com efeitos sonoros e sem música.

## Causa e correção do fim de jogo

O código anterior deixava `combat_status=2` após a contagem da última morte. Nesse estado, a lógica retornava sem progredir para outra cena, parecendo um travamento; o usuário precisava pressionar R. Agora, o fim da última vida solicita uma transição pendente, processada pelo chamador depois da atualização do combate.

`game_over_to_intro`, em `src/intro.asm`, reinicializa o combate, os itens, a pausa e a liberação de EXIT da partida, preserva o recorde e chama a abertura. O caminho retorna diretamente ao laço de cenas, evitando desenhar HUD/inimigos sobre a imagem recém-limpa da KONAMI. A reprodução de efeitos pendentes é encerrada pela inicialização da abertura; o IRQ de vídeo continua funcionando.

Ao acabarem as vidas, termina a contagem da morte e volta a animação KONAMI com fundo azul. Em seguida vem a tela de título. Espaço passa pelas telas e inicia uma partida com três vidas, pontuação zerada e modo de teste desligado. Manter o disparo pressionado durante a transição não cria uma nova borda de tecla nem pula imediatamente a abertura.

## Tecla I

Durante uma partida, **pressione e solte I para alternar a proteção de teste**. A borda GIME fica azul enquanto a proteção está ligada e volta ao preto ao desligar. Segurar I não alterna repetidamente. A tecla é ignorada durante a abertura/título e durante a morte.

O estado `test_invincible` é separado do inventário e das habilidades originais. Ele impede perda de vida por contato com inimigos, projéteis e o chefe, sem ativar a habilidade original que proíbe disparos. O jogador mantém os comandos e armas usuais. Pressionar I novamente restaura o dano normal. Uma nova partida, inclusive por R ou após o fim das vidas, sempre começa com essa proteção desligada.

A tecla é lida pela mesma matriz PIA do teclado existente, coluna 1/linha de máscara 2. A ativação usa apenas o evento de pressionamento. Os testes fornecem os sinais externos nos pontos reais de leitura do 6809; não escrevem o flag para simular um acionamento da tecla.

## Memória

Foi removida a tabela `combat_art` obsoleta, de 104 bytes, que duplicava descritores já presentes em `all_art` e não tinha consumidores no programa. O gerador `prepare_combat.py` também deixou de emiti-la, mantendo a recompilação reproduzível. Os gráficos e tabelas efetivamente usados foram preservados.

A mudança usa um byte adicional de estado. O estado direto termina em `$0CEC`; o programa termina em `$7FB5` no DAC6 e `$7FBC` no Orchestra-90, dentro da janela fixa anterior a `$8000`. O limite de 128 KiB não foi ampliado.

## Verificação

Os dois discos foram testados no XRoar configurado como CoCo 3 de 128 KiB, incluindo:

- Leitura real da matriz de I, ativação, tecla mantida e nova pressão para desligar.
- Proteção sustentada com disparos do jogador habilitados e colisões controladas de inimigo/projétil.
- Perda efetiva das três vidas pela rotina normal de colisão/morte, sem proteção de teste.
- Retorno à KONAMI azul, comparação independente dos pixels, laço de vídeo ainda ativo e pilha sem vazamento.
- Disparo mantido durante a última morte, passagem ao título e início de outra partida.
- Limpeza do modo de teste em R, inventário/estado de partida reiniciado e recorde preservado.
- Percurso integrado até o chefe com a tecla I acionada pela matriz, seguido da derrota por flechas nativas.
- Regressões de IA contra o Z80, checkpoints, controles, itens, combate e áudio.

O registrador GIME `$FF9A` é de escrita. O teste da cor de borda verifica o valor do acumulador no STA real, em vez de interpretar uma leitura de `$FF9A` como retorno da cor. O fundo azul da KONAMI também é verificado pela paleta e pelo framebuffer produzidos pelo jogo.

Os resultados estão em `DAC6-gameover-testmode-verification.json` e `ORCH90-gameover-testmode-verification.json`, com o SHA-256 do disco executado. As imagens `*-gameover-konami.png` e `*-gameover-new-game.png` foram obtidas dos pixels calculados pelo 6809, sem injeção de imagem esperada na RAM.

O percurso até o chefe usa a proteção deliberadamente e suprime tokens flutuantes para isolar a integração; não é uma partida sem intervenção. Não houve teste em hardware físico. A campanha permanece limitada à primeira fase, com as adaptações e pendências descritas em `IA-E-CHECKPOINTS.md` e `STATUS.json`.

Para repetir o teste específico:

```powershell
python tools/verify_gameover_testmode.py --xroar C:\caminho\xroar.exe
```
