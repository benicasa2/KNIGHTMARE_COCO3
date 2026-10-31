# Menos cintilação nas duas fases

Revisão de 31/10/2026. As alterações foram aplicadas ao motor compartilhado das fases 1 e 2, nas versões DAC6 e Orchestra-90 CC, mantendo 128 KiB.

## O que mudou

- Quadros parados são mantidos quando não há inimigos/projéteis visíveis, movimento, rolagem, alteração de itens/HUD ou habilidade ativa. A lógica e os relógios continuam rodando. Se algo muda, o redesenho normal é retomado.
- A restauração dos fundos dos sprites usa cópias desenroladas de 0 a 32 bytes, escolhidas uma vez por objeto. Jogador, escudo, inimigos e chefes passam menos tempo entre apagar e redesenhar.
- O primeiro chefe deixou de recalcular o endereço do framebuffer a cada linha; os dois chefes usam a cópia rápida de pares de bytes na composição opaca.
- Os sprites compactos deixam de executar um teste redundante depois de ler cada byte. Transparência e cores são preservadas.
- O cálculo do jogo continua ocorrendo antes do apagamento, com atualização adiada dos itens e a mesma ordem de sobreposição. Não foram alteradas IA, velocidades, colisões, população ou ritmo da rolagem.

## Comparação com o pacote anterior

Médias de 12 atualizações controladas por cenário, medidas com o relógio emulado do XRoar. **Tempo de processamento menor significa menor custo; não é uma alteração da velocidade programada do personagem.**

| Cenário | Antes (ms) | Agora (ms) | Redução do processamento | Redução do intervalo do jogador apagado |
| --- | ---: | ---: | ---: | ---: |
| Fase 1, parado | 13.67 | 9.54 | 30.2% | 36.2% |
| Fase 1, carregada | 73.29 | 66.64 | 9.1% | 9.8% |
| Fase 1, carregada com música | 79.44 | 72.46 | 8.8% | 9.4% |
| Primeiro chefe | 38.95 | 23.87 | 38.7% | 42.7% |
| Sprite largo, cenário 1 | 28.93 | 25.87 | 10.6% | 10.9% |
| Fase 2, parado | 14.17 | 10.04 | 29.1% | 35.7% |
| Fase 2, carregada | 73.90 | 67.17 | 9.1% | 9.9% |
| Fase 2, carregada com música | 83.44 | 76.64 | 8.2% | 8.8% |
| Segundo chefe | 58.34 | 51.96 | 10.9% | 11.1% |
| Sprite largo, cenário 2 | 29.31 | 26.26 | 10.4% | 10.9% |

O cenário parado manteve o quadro sem apagamento em 75% das atualizações medidas; as restantes redesenharam devido à rolagem. Nos casos carregados, foram usados sete inimigos sem música ou cinco com música, três flechas e oito projéteis hostis. Nos casos de chefe/sprite largo, a lógica dos inimigos foi congelada para isolar o custo do desenho. O caso de sprite largo no cenário 1 é um teste de estresse do motor, não uma nova formação da fase 1.

O intervalo do jogador apagado é medido do fim da restauração do seu fundo até o fim do compositor. É uma aproximação temporal da exposição ao apagamento, **não uma medição perceptual da cintilação nem captura de vídeo**. Nenhuma das dez médias apresentou regressão de processamento. As cenas carregadas ainda podem exceder o orçamento normal de uma atualização.

## Verificação e limites

As duas variantes passaram nas regressões de controles, combate, itens, música, checkpoints, chefes, caminhada de saída e fluxo das duas fases. Os testes novos comparam os pixels finais com um compositor independente, incluindo inimigos sobrepostos, sprites largos, aparecimento/remoção de projéteis, movimento, rolagem, escudo e expiração da invisibilidade. Também conferem as 33 larguras da cópia rápida e sentinelas de memória contra escrita fora dos limites.

A comparação usa a cópia intacta da entrega anterior. Os hashes e resultados estão em `sprite-commit-baseline.json`, `sprite-commit-timing.json` e `flicker-two-stage-release-verification.json`. Os testes de retenção estão em `*-sprite-retention-verification.json`. A regeneração da ROM e recompilação reproduziu os discos testados byte a byte.

Ainda pode haver cintilação residual: o jogo continua usando um único framebuffer de rolagem, sem buffer duplo completo. O piscar intencional de invulnerabilidade após dano, as animações de dano dos inimigos e a morte dos chefes foram mantidos. Não houve teste em CoCo físico nem avaliação por captura de vídeo.

O escopo continua sendo duas fases jogáveis. Controles, discos e limitações de áudio/campanha estão no LEIA-ME. Para executar, extraia em uma pasta nova e use EXECUTAR-DAC6.cmd ou EXECUTAR-ORCH90.cmd; ambos montam LEVELS.DSK no drive 1.

Para repetir a medição: `python tools/benchmark_sprite_commit.py --xroar CAMINHO_DO_XROAR --output resultado.json`. Para os testes novos: `python tools/verify_sprite_retention.py --xroar CAMINHO_DO_XROAR --lwasm CAMINHO_DO_LWASM`.
