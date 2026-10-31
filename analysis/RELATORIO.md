> Histórico de revisão anterior. Estado atual: [IA-E-CHECKPOINTS.md](IA-E-CHECKPOINTS.md). A IA da primeira fase e a restauração dos trechos foram substituídas; o inventário agora é removido ao perder uma vida. Valores de memória/desempenho abaixo pertencem à revisão anterior.

# Análise de Knightmare e estado da conversão

## Resultado e alcance

Há análise estática e dinâmica, recursos extraídos e duas provas nativas de vídeo/áudio. **Não há uma conversão jogável completa.** A prova não implementa o motor de Knightmare e não deve ser confundida com ele.

## Identidade da entrada

- Arquivo fornecido: `Knightmare - Majyo Densetsu - Konami (1986) [RC-739] [1409].rom`.
- Tamanho: 32.768 bytes.
- SHA-256: `589b2fd276d84827bb1b033f43a0cd5ab0aa9da3c8784cac2f62474fed00aae3`.
- SHA-1: `c484f44af0ab1bcaf46ae5f14a3b38a53ee77f31`.
- Cabeçalho `AB`; inicialização em `$407E`; carga em `$4000–$BFFF`.
- A base local de identificação do openMSX reconhece esse SHA-1 como uma variante de Knightmare RC-739. Isso não comprova que seja o dump original de fabricação.

O projeto público de desassemblagem consultado usa outro SHA-256. Nesta ROM há NOPs, por exemplo, nos intervalos `$4028–$402A`, `$4055–$4057` e `$40CE–$40CF`, onde o listado público tem instruções de escrita/modificação em ROM. São observações locais desses intervalos, não uma comparação integral das duas revisões.

## Ferramentas e proveniência

- [REA-RETRO-9](https://github.com/benicasa2/REA-RETRO-9), commit `28ee8a3bc2a133fa584de38f14e86ecbc2a349b8`, baixado para esta tarefa; seu `ReaGhidraApplySeeds.java` aplicou as sementes da análise headless.
- A consulta completa REA de `$49C0` usou a instalação local previamente compilada da mesma linha REA 6.7.0 com suporte Z80. O JSON conserva o perfil, hash, limitações e identidade do provedor. Não foi compilado novamente todo o checkout recém-baixado.
- Ghidra 12.1.4, linguagem `z80:LE:16:default`; JDK Temurin 25.
- openMSX 21, configuração `C-BIOS_MSX1`, entradas de teclado programadas, sem alteração dos bytes do cartucho.
- LWASM 4.25; XRoar 1.12, CPU 6809 e RAM de 128 KiB.
- Extração complementar: [Knightmare-disassembly](https://github.com/antxiko/Knightmare-disassembly), commit `250a1502e77bbdc4c488c28c82382aafd2fd36af`, aplicada à ROM do usuário. Ferramentas e licença estão em `third_party/knightmare/`.

## Achados confirmados e derivados

| Endereço original | Evidência / interpretação |
| --- | --- |
| `$407E` | Inicialização declarada pelo cabeçalho e observada em breakpoint |
| `$40A3–$40A9` | Instala salto em `$FD9A`, apontando para `$402E` |
| `$402E` | Gancho de interrupção; chama áudio antes do processamento principal |
| `$40C9` | Laço vazio; a execução do jogo depende do gancho de interrupção |
| `$406C` | Retira da pilha o endereço da tabela inline e despacha por salto indireto |
| `$490C` | Entrada de solicitação de som, com seleção/prioridade por voz |
| `$49C0` | Atualização de três blocos de voz, cada um com 14 bytes |
| `$E010/$E01E/$E02C` | Blocos de estado das três vozes, derivados da rotina |
| `$E03A` | Cópia do mixer PSG usada pelo código |
| `$E002`, bit 6 | Habilita pedidos musicais durante a partida; a execução inicial estava em demonstração silenciosa |

O processo headless terminou com 86 sementes aplicadas: 83 funções criadas, uma já existente e duas sementes de código decodificadas. O Ghidra reconheceu 334 funções, 8.858 instruções e 16.097 bytes de instruções. Esses números descrevem sua classificação automática; não significam 100% de entendimento da ROM, nem 334 rotinas já convertidas.

`dispatch-tables.json` registra 15 tabelas candidatas encontradas por inspeção de bytes. O limite inferido apenas pela faixa dos ponteiros pode incluir entradas extras. `executed-addresses.tsv` confirma somente os destinos efetivamente percorridos nessa partida curta. Fluxos indiretos adicionais continuam exigindo revisão.

## Vídeo e dados

Os registradores observados configuram SCREEN 2, com nomes em `$3800`, atributos de sprites em `$3B00`, padrões de sprites em `$1800`, cores em `$0000` e padrões de fundo em `$2000`. As capturas completas da VRAM usam o tamanho exposto pelo depurador do openMSX; somente os primeiros 16 KiB são relevantes para esse modo MSX1.

Foram reconstruídos mapas gráficos das oito fases e extraídos sprites e telas. A primeira fase foi verificada contra `reference/action-VRAM.bin`: **zero diferenças nos 6.144 bytes de padrões e nos 6.144 bytes de cores**. O mapa gráfico extraído não contém por si só o escalonamento dos inimigos, os objetos interativos ou as colisões.

O decoder local da captura trata os sprites de 16×16, prioridade, early-clock, ampliação e limite de quatro sprites por linha. A cena usada na prova é uma captura estática da fase em execução.

## Prova CoCo 3

O programa é Assembly 6809, sem instruções 6309 e sem emulação de Z80. O carregador instala uma imagem de 24.576 bytes na RAM física `$00000–$05FFF`. O código e o trecho de áudio ficam na metade superior da RAM. A janela MMU `$8000–$9FFF` permite copiar o framebuffer sem sobrepor o programa.

O vídeo usa 256×192×16, `$FF98=$80`, `$FF99=$1A`. O endereço GIME de vídeo `$60000` espelha a RAM física `$00000` na máquina com 128 KiB testada. Isso não é uma exigência de 512 KiB; o teste foi executado com 128 KiB.

O FIRQ passa pelo salto em RAM `$FEF4`, pois `$FFF6` permanece em ROM no CoCo 3. O timer é programado com o byte baixo primeiro. No GIME 1986 emulado, 910+2 ticks dão aproximadamente 3.924,94 amostras/s. Uma divisão alternada de 65/66 amostras atualiza os parâmetros em aproximadamente 59,9227 Hz.

As duas compilações compartilham a síntese de três ondas quadradas e um gerador de ruído de 17 bits. DAC6 escreve os seis bits superiores em `$FF20`. ORCH90 escreve a mistura de oito bits em `$FF7A` e `$FF7B`. O áudio dos dois canais externos é idêntico.

O trecho atual contém 600 quadros de registradores capturados, convertido em parâmetros do sintetizador. Ele **não é o sequenciador original convertido**. A baixa taxa de amostragem causa aliasing em tons altos, a curva de volume é aproximada e a implementação geral não cobre envelopes de hardware. O trecho selecionado não usa esses envelopes. A síntese de ruído tem limitação de taxa, registrada em `build.json` quando aplicável.

## Verificação executada

Os arquivos `DAC6-verification.json` e `ORCH90-verification.json` registram a execução real no XRoar: carregamento pelo Disk BASIC, RAM de 128 KiB, comparação byte a byte do framebuffer e da paleta, avanço do timer e 256 escritas DAC observadas por watchpoint. ORCH90 usa um Multi-Pak emulado com cartucho Orchestra-90 e controlador de disco.

A verificação também percorre os 600 quadros do trecho até o ponto de repetição, sem alterar os parâmetros de áudio em execução.

Isso verifica as provas técnicas. Não comprova fidelidade auditiva, conclusão de fase, taxa de quadros de um motor completo, funcionamento em monitor composto ou funcionamento em hardware físico. As imagens `*-actual-xroar.png` foram reconstruídas dos bytes lidos na RAM e dos registros de paleta do emulador.

## Engenharia que ainda falta

1. Converter a máquina de estados e as estruturas de entidades, preservando a ordem de atualização e a aritmética do Z80.
2. Implementar rolagem e sprites em software no 6809; medir ciclos nos quadros de maior carga com áudio ativo.
3. Converter os dados de surgimento, armas, itens, colisões, oito fases, chefes e final; confrontar execuções determinísticas com o MSX.
4. Converter o sequenciador de `$490C/$49C0`, suas prioridades e comandos; reproduzir todas as músicas e efeitos por evento, em ambos os backends.
5. Fechar o mapa de memória do jogo inteiro e executar uma campanha completa em 128 KiB. O consumo pequeno da prova não estabelece o consumo final.
6. Validar visual, áudio, controles e temporização no hardware alvo.

Não existe, neste pacote, evidência que permita declarar esses itens concluídos.

## Referências de hardware

- [Referência GIME de John Kowalski](https://www.6809.org.uk/twilight/sock/gime.html).
- [XRoar — código do Orchestra-90](https://www.6809.org.uk/git/xroar.git/tree/src/orch90.c).
- Manuais locais do openMSX: depuração, captura de VRAM e gravação de áudio.
