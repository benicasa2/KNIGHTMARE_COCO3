# Knightmare CoCo 3 — fases 1 e 2, menos cintilação

**v0.3.0: Tandy Speech/Sound Cartridge original.** Use `bin/SSC.DSK` no drive 0 e `bin/LEVELS.DSK` no drive 1, com CoCo 3 de 128 KiB e SSC conectado. `RUN"START"` carrega o jogo; M liga/desliga a música. Esta variante roda a **0,895 MHz**, com perda de fluidez (aproximadamente 15–19 atualizações/s nos trechos medidos com música). Para emular, use MAME com suporte a SSC, validado na versão 0.289. XRoar 1.12 não emula esse cartucho. Leia [docs/SSC.md](docs/SSC.md) para instalação, execução e testes. O manual PDF v0.2.1 acompanha este suplemento; DAC6 e Orchestra-90 continuam com os mesmos discos da v0.2.1.

Revisão de renderização de **31/10/2026**: restauração rápida dos sprites e retenção de quadros parados. As mudanças reduzem o intervalo de apagamento e o tempo de processamento nos cenários testados. Detalhes e comparação: [CINTILACAO-DUAS-FASES.md](analysis/CINTILACAO-DUAS-FASES.md). Ainda pode haver cintilação residual; o piscar intencional após dano foi mantido.

Revisão da campanha de **31/10/2026: saída automática pela passagem e segunda fase jogável**, em Assembly 6809, usando CoCo 3 de 128 KiB. Inclui variantes DAC6 e Orchestra-90 CC.

Depois do primeiro chefe, o personagem caminha sozinho até a passagem e segue para a fase 2. Mantém pontuação, armas e habilidades, e recebe uma vida. A fase 2 usa mapa, tiles, 41 entradas de ondas, 42 objetos fixos, nove tokens e segundo chefe extraídos/analisados da ROM fornecida. A caminhada, as novas trajetórias e a física do chefe foram comparadas com instruções Z80 executadas.

## Executar

**Extraia em uma pasta nova.** Execute `EXECUTAR-DAC6.cmd` ou `EXECUTAR-ORCH90.cmd`. Requer Python e uma instalação existente do XRoar. Os scripts montam automaticamente os dois discos necessários:

| Drive | Disco |
| --- | --- |
| 0 | `bin/DAC6.DSK` ou `bin/ORCH90.DSK` |
| 1 | **`bin/LEVELS.DSK`**, necessário para os cenários |

Em configuração manual, use CoCo 3 de **128 KiB**, saída RGB, monte os dois discos e digite `RUN"START"`. Aguarde o carregamento. Espaço pula a abertura e inicia a partida no título. O disco LEVELS permanece montado durante toda a partida.

Para indicar outra instalação:

```powershell
python tools/run_demo.py --variant DAC6 --xroar C:\caminho\xroar.exe
python tools/run_demo.py --variant ORCH90 --xroar C:\caminho\xroar.exe
```

As ROMs de sistema `coco3.rom` e `disk11.rom` devem estar ao lado do emulador; não são incluídas. Orchestra-90 usa Multi-Pak com cartucho no slot 0 e disco no slot 3, com o slot 3 selecionado.

## Controles

| Tecla | Ação |
| --- | --- |
| Setas ou W/A/S/D | Mover, incluindo diagonais |
| Espaço / botão do joystick | Iniciar ou disparar |
| K | Teclado |
| J / L | Joystick analógico direito / esquerdo, mantendo teclado |
| M | Música opcional: cinco inimigos comuns em vez de sete |
| I | Proteção de teste; borda azul quando ativa |
| P | Pausar/retomar |
| R | Nova partida na fase 1; desliga música e proteção de teste |

A música começa desligada e os efeitos continuam ativos. M também funciona na segunda fase e no segundo chefe. Orchestra-90 recebe a mistura mono nos dois canais. Ao acabar as vidas, o jogo volta à tela azul KONAMI para permitir nova partida.

## Análise, fontes e testes

Leia [FASE-2-E-SAIDA.md](analysis/FASE-2-E-SAIDA.md) para endereços da ROM, comportamento portado, organização da memória, carregador de cenários e procedimento de regeneração. O código novo está em `src/cutscene.asm`, `src/ai_phase2.asm`, `src/boss2.asm`, `src/phases.asm` e `src/level_disk.asm`.

O pacote de fontes contém os recursos gerados, fontes 6809, ferramentas Python, licenças, histórico REA/Ghidra e relatórios. Os relatórios `*-phase2-*-verification.json`, `*-cutscene-verification.json` e `*-two-stage-flow-verification.json` registram os testes da segunda fase. A evidência consolidada e os hashes estão em `analysis/phase2-release-verification.json`. A revisão atual de renderização também tem `analysis/flicker-two-stage-release-verification.json` e os testes `*-sprite-retention-verification.json`.

Para compilar usando os recursos incluídos:

```powershell
python tools/build.py --lwasm C:\caminho\lwasm.exe
```

Para regenerar da ROM, use a ordem documentada em FASE-2-E-SAIDA.md; `prepare_stage2.py` deve executar depois das preparações antigas de combate/itens.

## Escopo e limitações

**Duas fases jogáveis.** Depois de vencer o segundo chefe e concluir a caminhada, esta revisão volta à abertura KONAMI. As fases 3 a 8 ainda precisam de integração; o visualizador dos oito mapas é separado da campanha.

Os testes rodam no XRoar de 128 KiB, incluindo comparações Z80/6809, pixels, controles, áudio e sequência das duas fases. O teste integrado usa proteção I, preparação de combate e flechas controladas; não é uma partida completa sem intervenção. Não houve teste em CoCo físico.

A morte do segundo chefe usa animação simplificada; o sinal sonoro da passagem reutiliza um efeito curto existente. Música de duas vozes e paleta RGB são adaptações. A cintilação residual e a lentidão em cenas carregadas ainda podem ocorrer, principalmente com música. Os relatórios de desempenho medem processamento em cenários controlados, não FPS de uma campanha inteira.

Os documentos antigos de revisões anteriores são históricos. Consulte esta revisão e os hashes dos relatórios para identificar o comportamento atual.
