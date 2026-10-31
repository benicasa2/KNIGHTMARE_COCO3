> Histórico de revisão anterior. Estado atual: [IA-E-CHECKPOINTS.md](IA-E-CHECKPOINTS.md). A IA da primeira fase e a restauração dos trechos foram substituídas; o inventário agora é removido ao perder uma vida. Valores de memória/desempenho abaixo pertencem à revisão anterior.

# Otimização e áudio somente por eventos — 30/10/2026

O pedido desta revisão foi retirar a música, manter efeitos e melhorar a resposta sem acelerar artificialmente a ação. A versão anterior sintetizava continuamente três vozes e ruído em aproximadamente 3.925 interrupções por segundo, mesmo sem eventos de combate. Seus efeitos ainda não eram acionados pela lógica nativa. Nesta revisão a música e essa síntese foram retiradas; efeitos foram integrados ao combate.

## Mudanças aplicadas

1. Relógio de jogo separado do áudio: IRQ vertical do GIME em `$FF92`, vetor RAM `$FEF7`. O divisor mantém aproximadamente 29,96 atualizações/s e os mesmos passos de movimento, flechas e cadências da versão anterior. A rolagem nominal permanece em 14,98 pixels/s. Não foram reduzidas contagens de inimigos ou alteradas trajetórias para obter o ganho.
2. Efeitos PCM por evento: FIRQ do timer somente enquanto há amostras. Ao terminar, desativa sua fonte em `$FF93` e deixa o DAC em repouso. A reprodução copia uma amostra pronta, em vez de sintetizar três canais e ruído em cada interrupção. O timer não determina mais o avanço do jogo.
3. Cópias curtas de fundo em palavras de 16 bits, tratando explicitamente larguras ímpares. A cópia de linhas completas conserva o caminho desenrolado de 128 bytes.
4. Popolon é composto diretamente no framebuffer, preservando seu fundo, sem o antigo passe intermediário de saída.
5. Composição de sprites com entradas desenroladas para as larguras de 1 a 9 bytes. Os caminhos compartilham instruções consecutivas; não há laço e comparação por pixel compactado.
6. Ponteiro de vídeo avança por linha; a MMU é atualizada quando cruza o limite de 8 KiB. Desenho e apagamento de sprites e desenho do HUD deixam de recalcular o endereço completo a cada linha.

A ordem de apagar/desenhar os objetos, máscaras, alinhamento ímpar e fundo salvo foram preservados. A correção da colisão do cenário e das setas também permanece. Nada exige 6309, 512 KiB ou aceleração do XRoar para funcionar.

## Efeitos e limites de reprodução

O script `capture_sfx.tcl` executa isoladamente as rotinas Z80 da ROM do usuário: solicitação em `$490C` e atualização em `$49C0`, registrando os 16 registros PSG a cada quadro. O estado do motor de som é limpo entre efeitos. A ROM não é alterada. As capturas têm 180 quadros cada, incluindo silêncio final.

| Evento nativo | Efeito original | Duração PCM aproximada |
| --- | --- | --- |
| Flecha básica | `$03`, tabela da arma em `$60D5` | 84 ms |
| Impacto | `$0B`, usado no dano do chefe em `$9954` | 601 ms |
| Explosão de inimigo | `$4D`, rotina de morte em `$75A5` | 401 ms |
| Morte de Popolon | Reutilização adaptada de `$4D` | 401 ms |
| Derrota do primeiro chefe | `$47`, efeito usado no remate visual do chefe em `$8CD9` | 200 ms |

`prepare_sfx.py` renderiza os registros para PCM com oito subamostras por amostra e mistura de tom/ruído. A taxa é 3.924,94 Hz. Não é uma reprodução bit a bit do AY: há limite de banda, quantização e normalização de nível por efeito. Há somente uma voz de efeitos, com prioridade: chefe > jogador > explosão > impacto > flecha. Um efeito prioritário pode suprimir flechas durante sua reprodução. Orchestra envia o mesmo sinal aos dois canais, coerente com a origem mono.

As quatro amostras distintas somam 5.044 bytes. O código e os dados fixos terminam em `$5556` (limite exclusivo) na variante DAC6 e sete bytes adiante na Orchestra; antes terminavam em `$68FC`/`$68FD`. O estado da página direta termina em `$0C94`. Os bancos de mapa e gráficos e o anel de vídeo permanecem na organização anterior. O arquivo antigo de parâmetros musicais não é carregado nem incluído no binário atual.

## Medição reproduzível

`benchmark_combat.py` lê `monitor cycles` do XRoar: 14.318.180 ticks de hardware/s. Mede da entrada de `apply_input` até `combat_frame_complete`, incluindo lógica, desenho e interrupções. Exclui a espera até a próxima atualização. O tempo do computador hospedeiro e as pausas no depurador não contam. Foram feitas 12 amostras por cenário em CoCo 3/6809, 128 KiB.

| Cenário | Antes, média | Depois, média | Redução de tempo |
| --- | ---: | ---: | ---: |
| Sem entidades ativas, jogador/HUD/rolagem | 27,43 ms | 9,74 ms | 64,5% |
| Sete inimigos, três flechas e oito tiros hostis | 179,43 ms | 58,99 ms | 67,1% |

São configurações controladas, sem efeitos disparados durante essas amostras; antes a música permanecia ativa. As posições, tipos e contagens são preparados pelo teste; não são médias de uma campanha jogada. As 12 amostras incluem trabalho diferente em atualizações com e sem rolagem e com o jogador piscando. O cenário cheio ainda excede o orçamento de cerca de 33,38 ms por atualização: portanto **não há promessa de 30 FPS constantes**. O ganho elimina muito trabalho, mas o compositor completo de muitos objetos ainda pode ser lento. Os dados brutos estão em `performance.json`.

## Verificação final

- As duas variantes passaram nos cenários de combate em XRoar com 128 KiB: setas/WASD com combate ativo, colisão sólida, flechas, dano, vidas, sobreposição, fundo restaurado e primeiro chefe.
- Os eventos de explosão, morte do jogador e chefe foram verificados pelas prioridades de áudio acionadas no código nativo.
- As primeiras 128 escritas DAC de uma flecha foram comparadas exatamente com seu PCM, aplicando a quantização DAC6 quando necessário. A taxa efetiva foi medida pelos ticks emulados, próxima de 3.924,94 Hz.
- Depois do efeito, o ponteiro PCM permanece parado enquanto o contador de vídeo continua avançando. Não há reprodução contínua de música no intervalo.
- Nove imagens de combate por variante têm os mesmos hashes do framebuffer da versão anterior corrigida. A comparação com o renderizador independente também passou.
- A regressão de controles atravessou os 1.776 pixels da câmera, incluindo voltas do anel e limites MMU, com combate isolado/desativado pelo depurador. Os testes específicos de setas executam o caminho com combate ativo.

Os relatórios identificam seus limites: sinais PIA e posições de cenários são preparados pelo depurador. Não houve partida completa sem intervenção nem teste físico de teclado, joystick, cartucho Orchestra ou CoCo real. A nova reprodução de efeitos foi verificada pelos dados escritos no DAC; não foi apresentada como gravação de áudio do hardware físico.

## Próximos gargalos

O cenário cheio ainda exige desenho e restauração de muitos sprites. Redesenho por regiões sobrepostas, rotinas especializadas para cada sprite e redução de escrita do HUD podem ser próximos passos, mas precisam preservar corretamente fundo, máscaras e ordem das camadas. Esta entrega aplica as otimizações já verificadas; não altera resolução, arte, número de objetos ou velocidades para esconder o custo.
