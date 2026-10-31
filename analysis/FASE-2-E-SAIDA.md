# Fases 1 e 2 — saída pela passagem e segundo guardião

Revisão de 31/10/2026. Código nativo 6809, CoCo 3 de 128 KiB, DAC de 6 bits ou Orchestra-90 CC. Esta revisão implementa duas fases jogáveis; as fases 3 a 8 continuam pendentes.

## Análise do original

A referência principal é a ROM fornecida, SHA-256 `589b2fd276d84827bb1b033f43a0cd5ab0aa9da3c8784cac2f62474fed00aae3`, mapeada em $4000–$BFFF. O disassembly comentado e os artefatos anteriores do REA-RETRO-9/Ghidra ajudam a localizar as rotinas. Nesta revisão, as comparações novas executaram as instruções Z80 da ROM; não foi feita uma nova sessão gráfica do Ghidra. Comentários dos fontes consultados são material de referência, não instruções do usuário.

- **$5D95–$5DF7:** sequência automática depois do chefe. Calcula o vetor até (120,32), usa as frações de posição e, ao alcançar a passagem, fixa a velocidade vertical em $FF50. A conclusão ocorre quando Y fica abaixo de 5. A animação usa o relógio original. Tradução em `src/cutscene.asm`, tabelas de tangente/vetores em `src/phase2_math.inc`.
- **$53D3, $549A, $555C:** montagem da fase, restauração por trechos de 192 pixels e rebobinamento das ondas. O equipamento é retido ao montar uma nova fase e removido ao perder uma vida. A transição mantém pontuação, recorde, armas e habilidades e acrescenta uma vida.
- **$7044/$704D:** quantidade e ponteiro das **41 entradas de ondas da fase 2**. Objetos fixos: **42 registros**; tokens posicionais: **9 registros**. Dados em `src/phase2_data.inc`. O motor de ondas, formações, colisões e itens é compartilhado entre as duas fases.
- **$7AD2/$7BC7/$7C33/$7DC5/$7FE5/$8076:** estados adicionais usados pelos inimigos da fase 2. Incluem mergulho com espera/disparo, deslocamento até o centro, investida lateral, inimigo de quatro impactos com expansão/recuperação e dardos. Tradução em `src/ai_phase2.asm`. O tipo 12 comum reage ao impacto; os satélites do primeiro chefe mantêm seu tratamento próprio.
- **$857F/$863F/$86B5:** rajadas 5 e 6 e vetores fracionários. A emissão depende de haver vagas para a rajada inteira.
- **$8E34–$9090:** segundo chefe. Entrada, 30 impactos, aceleração em direção aos alvos, cinco peças lançadas, retorno das peças e três ornamentos pretos. São executados dois passos do relógio MSX por atualização do CoCo. Implementação em `src/boss2.asm`.
- **$AD69/$ABDC e $902D:** arte e composição do segundo chefe; padrões de cenário e mapas extraídos da fase 2. Música normal $9F e chefe $A1, além de $9D/$A3 da fase 1.

Há uma divergência relevante no disassembly comentado: em $8FD6 ele indica `LD B,8`, mas os bytes da ROM fornecida são `04 08` (`INC B` / `EX AF,AF'`). O retorno das peças no port segue os bytes efetivos, conferidos também no arquivo original do usuário. Não substituímos esse comportamento pelo comentário.

## Memória e carregamento

O jogo continua usando 16 bancos de 8 KiB, sem exigir 512 KiB. A rolagem ocupa 48 KiB; o banco 6 mantém o código auxiliar, introdução e os oito mapas compactos. O mapa é decodificado por bandas em um cache de **128 bytes**, em vez de ocupar um banco inteiro expandido. O banco 7 armazena 6.551 bytes de efeitos PCM.

Os sprites comuns usam duas versões de alinhamento em 4 bpp, com transparência inferida do nibble zero. Isso elimina a máscara separada desses sprites e permite manter a arte das duas fases nos bancos 13–15. Jogador, escudo e invisibilidade conservam o desenho com máscara. Há buffers maiores para os sprites expandidos de dano do inimigo de quatro impactos e para o segundo chefe.

Apenas os 8 KiB de tiles da fase atual ficam no banco 12. Um carregador 6809 WD2793 lê o banco seguinte de `LEVELS.DSK`, no **drive 1**, durante a passagem de fase. O disco de programa fica no **drive 0**. Não há troca de disco durante a ação. O carregador é somente leitura. Se faltar o disco de cenários, a partida não inicia; após montá-lo, R tenta novamente.

Os scripts EXECUTAR montam os dois discos automaticamente. Em configuração manual, monte ambos antes de `RUN"START"`. O disco de cenários não é um disco BASIC de arquivos: é um banco de setores para o carregador nativo. O banco 15 deixa reservada a área usada pelos vetores do Disk BASIC durante o bootstrap.

## Verificação

Os relatórios desta revisão vinculam os resultados aos hashes dos discos testados:

- `*-cutscene-verification.json`: vinte posições iniciais comparadas com $5D95 até a conclusão, incluindo posição 8.8, velocidade, ângulo, estado e animação.
- `*-phase2-ai-verification.json`: 2.700 amostras por variante dos cinco tipos adicionais, comparadas com execução Z80.
- `*-phase2-boss-verification.json`: 480 quadros originais de movimento do chefe e das cinco peças.
- `*-two-stage-flow-verification.json`: derrota do primeiro chefe, saída automática, preservação do equipamento, vida adicional, carregamento do cenário 2, ondas/rolagem da segunda fase, derrota do segundo chefe e conclusão.
- `*-music-verification.json`: quatro faixas, ciclo completo e repetição, eventos de notas, tecla M, limite de inimigos e mistura real dos efeitos no DAC.
- `*-campaign-map-verification.json`: os oito mapas compactos decodificados pelo 6809 contra a referência.

O fluxo integrado usa uma preparação controlada do primeiro chefe, flechas inseridas nos cenários de teste, proteção I e supressão de tokens durante o percurso longo. **Não é um playthrough sem intervenção.** Os testes verificam código 6809 real no XRoar configurado com 128 KiB; não houve teste em hardware físico.

## Limites desta entrega

Depois do segundo chefe e da caminhada, esta revisão volta à abertura azul KONAMI. Não apresenta uma fase 3 substituta. Os oito mapas extraídos e seu visualizador separado não significam oito fases jogáveis.

O som é uma adaptação: duas vozes tonais sintetizadas e efeitos PCM; não equivale ao PSG AY completo. Orchestra-90 recebe a mistura mono nos dois canais. A caminhada usa o efeito curto de coleta existente como sinal de conclusão; o efeito musical específico $B7 ainda não foi portado. A animação de morte do segundo chefe reaproveita o piscar temporizado do motor, sem reproduzir toda a decomposição original. Cores RGB são aproximações da paleta MSX.

A renderização continua com um único framebuffer de rolagem e pode apresentar cintilação residual. Cenas carregadas podem exceder o orçamento de 33,4 ms por atualização, sobretudo com música. Consulte `performance-ai.json` e `music-performance.json` para as medições desta compilação. A música começa desligada; M a ativa com cinco inimigos comuns em vez de sete (28,57% menos), mantendo os efeitos e os chefes.

## Recompilar e repetir

Os assets já estão prontos. Execute `python tools/build.py --lwasm CAMINHO_DO_LWASM`. Para regenerar a partir da ROM, execute nesta ordem `prepare_combat.py`, `prepare_items.py`, `prepare_ai.py`, `prepare_campaign_maps.py`, `prepare_music.py`, **`prepare_stage2.py`**, todos com `--rom CAMINHO_DA_ROM`, e depois `build.py`. A última preparação é necessária para o formato compacto de sprites desta revisão.

Os verificadores novos aceitam `--xroar`, `--lwasm` e `--rom`; os quatro verificadores de trajetória/chefe/saída/fluxo também aceitam `--variant DAC6` ou `ORCH90`. Os scripts `verify_phase2_edges.py`, `verify_campaign_maps.py` e `verify_music.py` verificam as duas variantes. ROMs de sistema e o arquivo completo do cartucho não são incluídos.

Desempenho atualizado após a revisão de desenho: consulte [CINTILACAO-DUAS-FASES.md](CINTILACAO-DUAS-FASES.md). Os números da entrega anterior foram substituídos pelos relatórios vinculados aos discos atuais.
