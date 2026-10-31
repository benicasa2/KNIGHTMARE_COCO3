from pathlib import Path
import json,shutil
from PIL import Image as PIL
from reportlab.pdfgen import canvas
from reportlab.platypus import SimpleDocTemplate,Paragraph,Spacer,Image,Table,TableStyle,PageBreak,Flowable,KeepTogether
from reportlab.lib.styles import ParagraphStyle
from reportlab.lib import colors
from reportlab.lib.pagesizes import A4
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
ROOT=Path(__file__).resolve().parents[1]; REPO=ROOT;DOC=REPO/'docs/manual';IMG=DOC/'images'
for name,file in [('Arial','arial.ttf'),('ArialB','arialbd.ttf'),('ArialI','ariali.ttf')]:pdfmetrics.registerFont(TTFont(name,'C:/Windows/Fonts/'+file))
pdfmetrics.registerFontFamily('Arial',normal='Arial',bold='ArialB',italic='ArialI',boldItalic='ArialB')
W,H=A4; CW=W-84; navy=colors.HexColor('#112848');blue=colors.HexColor('#245ca8');muted=colors.HexColor('#566477');ink=colors.HexColor('#182333')
styles={'p':ParagraphStyle('p',fontName='Arial',fontSize=11,leading=15.6,textColor=ink,spaceAfter=8),'small':ParagraphStyle('small',fontName='Arial',fontSize=9,leading=12,textColor=muted,spaceAfter=7),'h':ParagraphStyle('h',fontName='ArialB',fontSize=23,leading=28,textColor=navy,spaceAfter=13),'h2':ParagraphStyle('h2',fontName='ArialB',fontSize=14,leading=18,textColor=ink,spaceBefore=7,spaceAfter=8),'k':ParagraphStyle('k',fontName='ArialB',fontSize=9,leading=12,textColor=blue,spaceAfter=8),'td':ParagraphStyle('td',fontName='Arial',fontSize=10,leading=13,textColor=ink),'th':ParagraphStyle('th',fontName='ArialB',fontSize=10,leading=13,textColor=colors.white)}
meta=json.loads((REPO/'analysis/combat-assets.json').read_text());items=json.loads((REPO/'analysis/items-assets.json').read_text());pal=json.loads((REPO/'analysis/build.json').read_text())['palette']
rgb=[((((v>>5)&1)*2+((v>>2)&1))*85,(((v>>4)&1)*2+((v>>1)&1))*85,(((v>>3)&1)*2+(v&1))*85) for v in pal]
def icon(i,name):
 pix=meta['indexed'][i]['pixels'];im=PIL.new('RGBA',(len(pix[0]),len(pix)));im.putdata([(*rgb[c],255 if c else 0) for row in pix for c in row]);im=im.resize((im.width*5,im.height*5),PIL.Resampling.NEAREST);im.save(IMG/(name+'.png'));return name
weapon_icons=[icon(i,'weapon-'+str(k)) for k,i in enumerate([items['weapon_art'][0],items['weapon_art'][2],items['weapon_art'][4],items['weapon_art'][6],items['weapon_art'][8],items['weapon_art'][10]])]
for k,i in enumerate(items['token_power_art'][3:]):icon(i,'power-'+str(k))
for k in [0,3,4,7,11,12]:icon(meta['enemy_ids'][str(k)][0],'enemy-'+str(k))
pages=[]
def page(k,title,blocks):pages.append(dict(kicker=k,title=title,blocks=blocks))
def p(t):return ('p',t)
def h(t):return ('h2',t)
def photo(name,caption,width=360):return ('photo',name,caption,width)
def pair(a,b,ca,cb):return ('pair',a,b,ca,cb)
def table(headers,rows,widths):return ('table',headers,rows,widths)
page('MANUAL DO JOGADOR  |  VERSÃO 0.2.1','Knightmare no CoCo 3',[
 p('<b>Uma aventura de Popolon em Assembly 6809.</b> História, primeiros passos, armas, habilidades e dicas para explorar as duas fases desta versão.'),
 pair('manual-stage1-action','manual-stage2-action','A estrada da primeira fase','A trilha da segunda fase'),
 p('Este manual acompanha a edição com menos cintilação, para <b>CoCo 3 com 128 KiB</b>. Há duas opções de áudio: DAC de 6 bits e Orchestra-90 CC. Os comandos e as regras descritos aqui correspondem ao port atual.'),
 photo('DAC6-intro-title','O título conserva a apresentação do Knightmare original.',260),
 p('<b>Comece por aqui</b>  História na página 2; instalação na 3; teclas na 4; combate na 5; armas e poderes nas 6 e 7; fases e chefes nas 8 a 10; dicas e consulta nas 11 e 12.'),
 ('small','Projeto de adaptação não oficial. Knightmare e seus elementos originais pertencem aos respectivos titulares. Imagens obtidas do jogo e de seus próprios sprites; não são ilustrações geradas por IA.')])
page('01  |  A AVENTURA','A busca de Popolon',[
 p('Aphrodite está prisioneira no castelo do mundo das trevas. Popolon parte em sua busca, mas ainda não conhece o caminho. Em um sonho, Hera revela a direção da montanha sagrada e o incentiva a aproveitar os cristais e as joias mágicas que encontrará. Ao despertar, o cavaleiro retoma a jornada. <i>Este é um resumo livre do enredo do manual original.</i>'),
 photo('manual-stage1-action','Popolon avança pela estrada enquanto as primeiras criaturas se aproximam.',384),
 h('Uma estrada que não espera'),p('No jogo, a história se transforma em movimento: a paisagem desce continuamente e os inimigos tentam fechar os espaços livres. Cada cristal exige uma escolha. Cada chefe guarda o caminho para o trecho seguinte.'),
 p('Nesta adaptação, você já pode atravessar as <b>fases 1 e 2</b>. O objetivo desta edição é vencer seus guardiões e concluir a caminhada de saída. O resgate final da campanha original ainda depende da integração das fases seguintes.'),
 ('small','A narrativa acima resume a apresentação do manual Konami de 1986. As orientações de jogo deste livreto descrevem o código atual do CoCo 3.')])
page('02  |  PRIMEIROS PASSOS','Preparar e iniciar',[
 p('Extraia o pacote em uma <b>pasta nova</b>. No computador, execute <b>EXECUTAR-DAC6.cmd</b> ou <b>EXECUTAR-ORCH90.cmd</b>. Os scripts usam Python e uma instalação existente do XRoar e montam os dois discos automaticamente.'),
 pair('DAC6-intro-konami','DAC6-intro-title','Abertura azul KONAMI','Título e início da partida'),
 table(['Unidade','Disco que deve permanecer montado'],[['Drive 0','DAC6.DSK ou ORCH90.DSK'],['Drive 1','LEVELS.DSK']], [90,CW-90]),
 p('Em configuração manual, selecione <b>CoCo 3 de 128 KiB</b>, saída RGB e controlador de disco. Monte os dois discos antes de digitar <b>RUN"START"</b>. O carregamento inicial pode levar alguns instantes.'),
 p('Pressione <b>Espaço</b> para pular a abertura e novamente para começar no título. A primeira partida inicia com três vidas, música desligada e proteção de teste desligada.'),
 p('As ROMs de sistema <b>coco3.rom</b> e <b>disk11.rom</b> devem estar ao lado do XRoar. Elas não acompanham o projeto. Para Orchestra-90 no emulador, os scripts configuram Multi-Pak, áudio no slot 0 e disco no slot 3.'),
 ('small','Se precisar indicar outra instalação: python tools/run_demo.py --variant DAC6 --xroar C:\\caminho\\xroar.exe. Troque DAC6 por ORCH90 para a outra versão.')])
page('03  |  COMANDOS','As teclas da aventura',[
 p('Use as setas ou W A S D. Duas direções ao mesmo tempo permitem movimento diagonal. O disparo acompanha a direção da arma; a posição do personagem é que define a linha de ataque.'),
 ('keys',),
 table(['Tecla ou controle','Função'],[['Setas ou W A S D','Mover Popolon em oito direções'],['Espaço ou botão do joystick','Pular abertura, iniciar e disparar'],['K','Selecionar teclado'],['J','Selecionar joystick analógico direito'],['L','Selecionar joystick analógico esquerdo'],['P','Pausar e continuar'],['M','Ligar ou desligar a música'],['I','Alternar invencibilidade de teste'],['R','Reiniciar uma partida na fase 1']], [145,CW-145]),
 p('<b>Segure Espaço</b> para disparar continuamente. Com o joystick, pressione J ou L para escolher a porta correspondente; o teclado permanece disponível.'),
 p('<b>R reinicia a partida</b> e desliga música e proteção de teste. A tecla I existe para experimentação: quando ativa, a borda fica azul. Ela não é um item coletado durante a aventura.')])
page('04  |  COMBATE','Ler o campo de batalha',[
 photo('DAC6-combat-initial','A tela de jogo tem uma área de ação e o placar na faixa inferior.',384),
 table(['Indicação','Como interpretar'],[['SCORE','Pontos da partida atual'],['HI SCORE','Melhor pontuação conservada nesta sessão'],['REST','Vidas de reserva além do personagem atual'],['STAGE','Número da fase em andamento']], [105,CW-105]),
 p('Procure uma faixa de fuga antes de se aproximar de um grupo. Os disparos inimigos e o contato com os corpos podem custar uma vida. O cenário também limita os deslocamentos; insistir contra uma borda não abre espaço.'),
 p('Quando Popolon perde uma vida, o jogo retoma o início do <b>trecho de 192 pixels</b> correspondente. Arma, velocidade e habilidades são reiniciadas. A pontuação e as vidas restantes são preservadas.'),
 p('Ao perder todas as vidas, o jogo volta à abertura azul KONAMI. A partida seguinte pode ser iniciada novamente pelo título. O recorde da sessão permanece enquanto o programa continua carregado.')])
page('05  |  ARMAMENTO','Cristais e armas',[
 p('Os tokens flutuantes aparecem em posições previstas pelo roteiro da fase. <b>Atingi-los muda sua seleção.</b> Observe o símbolo antes de encostar neles: o disparo escolhe o prêmio e o contato o recolhe.'),
 photo('DAC6-items-token','O token está em seleção durante uma demonstração de coleta.',290),
 ('weapons',),
 p('As famílias de armas incluem flechas, lâminas, bumerangue, ataque em três direções e flecha de fogo. O motor mantém <b>doze formas de armamento</b>, combinando as versões básicas e reforçadas.'),
 p('Recolher novamente uma arma da mesma família pode elevar sua forma. O bumerangue tem uma trajetória de retorno; acompanhe seu espaço de ataque. O disparo em três direções ajuda a cobrir inimigos que vêm pelos lados.'),
 p('As seleções de bônus dão pontos em vez de uma arma nova. Não dispare sem olhar quando um token já mostra o prêmio que você deseja: outro acerto pode mudar a escolha.')])
page('06  |  PODERES','Habilidades e joias',[
 pair('DAC6-items-shield','DAC6-items-invisibility','Proteção frontal com escudo','Aparência da habilidade temporária'),
 table(['Recompensa','Efeito nesta versão'],[['Velocidade','Aumenta o deslocamento até quatro níveis'],['Escudo','Absorve tiros frontais; possui resistência limitada'],['Transparência','Proteção temporária contra contato e projéteis'],['Explosão de contato','Destrói inimigos ao encostar; impede o disparo enquanto ativa']], [135,CW-135]),
 p('O escudo começa com <b>30 unidades de resistência</b>. Ataques mais fortes podem consumir mais de uma unidade. Por isso, continue desviando mesmo protegido.'),
 p('As habilidades temporárias terminam sozinhas. Se havia um escudo conservado, ele pode voltar depois do efeito. O poder de contato é útil para atravessar uma formação, mas você fica sem atirar durante sua duração.'),
 h('Prêmios escondidos'),p('Alguns blocos revelam recompensas ao serem atingidos. Há bônus, vida, bomba e congelamento. <b>Congelar não torna os inimigos inofensivos:</b> mantenha distância dos corpos parados.'),
 ('small','Itens, pontes e obstáculos dependem do roteiro de cada fase. Nem todo objeto demonstrado pelos testes aparece naturalmente em ambas as fases. A tecla I é uma ferramenta de teste separada desses poderes.')])
page('07  |  PRIMEIRA FASE','A estrada e o guardião',[
 photo('manual-stage1-action','As formações entram em momentos definidos pelo roteiro.',270),
 p('Aprenda a cadência das entradas antes de correr para o alto da tela. Movimentos curtos costumam deixar mais opções de fuga do que atravessar a estrada inteira. Use os tokens para chegar ao final com um armamento conhecido.'),
 photo('DAC6-combat-boss-active','O primeiro guardião e seus satélites protegem a passagem.',270),
 p('O primeiro chefe começa com <b>20 pontos de vida</b> e possui três satélites. Observe o deslocamento do corpo e o caminho dos acompanhantes. Atire quando a linha estiver livre e reserve uma lateral para escapar.'),
 p('Evite subir para dentro do corpo do chefe. No trecho final, perder uma vida preserva o estado do combate conforme o checkpoint; o equipamento do jogador continua sendo perdido.')])
page('08  |  ENTRE AS FASES','A caminhada pela passagem',[
 p('Depois da derrota do primeiro guardião, o controle da caminhada passa ao jogo. Popolon se alinha à passagem central, avança e sai pela parte superior. Você não precisa manter uma tecla de direção pressionada.'),
 ('sequence', ['manual-exit-0','manual-exit-24','manual-exit-48']),
 p('A sequência é executada pelo código 6809 do port, com trajetória e animação comparadas à rotina original do MSX. As três imagens mostram momentos reais da mesma saída.'),
 photo('DAC6-stage2-start','A chegada à fase 2 conserva o equipamento e a pontuação.',270),
 p('Ao entrar na segunda fase, a pontuação, as armas e as habilidades são mantidas, e o jogador recebe <b>uma vida adicional</b>. O disco de cenários é lido nesse intervalo: deixe <b>LEVELS.DSK no drive 1</b>.'),
 p('Não pressione R para continuar: essa tecla cria uma partida nova na fase 1. Basta aguardar a conclusão da caminhada e do carregamento.')])
page('09  |  SEGUNDA FASE','A trilha vermelha',[
 photo('manual-stage2-action','A segunda fase troca o cenário e amplia os padrões de ataque.',270),
 p('A nova estrada traz investidas, cruzamentos e inimigos que mudam de comportamento ao receber dano. Um dos tipos resiste a quatro impactos e passa por uma animação de expansão e recuperação. Não confunda essa reação com uma eliminação imediata.'),
 photo('DAC6-stage2-boss','O segundo guardião lança peças com movimento próprio.',270),
 p('O segundo chefe começa com <b>30 pontos de vida</b>. Seu corpo se desloca continuamente e ele lança peças que também precisam ser acompanhadas. Desvie pensando no movimento seguinte, não apenas na posição atual do chefe.'),
 p('Depois da vitória e da caminhada final, esta edição volta à abertura KONAMI. <b>As fases 3 a 8 ainda não estão integradas à campanha jogável.</b> O visualizador de oito mapas é uma ferramenta separada.')])
page('10  |  ESTRATÉGIA','Sobreviver por mais tempo',[
 ('enemies',),
 h('Durante o caminho'),
 p('<b>Guarde uma saída.</b> Evite ficar entre um inimigo e a borda. Quando um grupo entra, observe de qual lado sobra espaço antes de disparar e avançar.'),
 p('<b>Troque distância por tempo.</b> Ficar mais abaixo permite acompanhar a trajetória de alguns projéteis. Subir demais reduz o tempo para reagir aos nascimentos.'),
 p('<b>Escolha o token.</b> Pare de atirar quando o símbolo desejado aparecer e aproxime-se por uma linha segura. O prêmio errado pode trocar sua arma atual.'),
 h('Nos chefes e nos retornos'),
 p('<b>Conheça o alcance.</b> Flechas, bumerangue e ataque espalhado ocupam áreas diferentes. A melhor posição depende do armamento que você realmente tem.'),
 p('<b>Recomece o trecho com calma.</b> Depois de morrer, o equipamento é reiniciado. Reavalie a velocidade e a arma antes de repetir um movimento que dependia de um poder perdido.'),
 p('<b>Use o teste para estudar.</b> I permite observar trajetórias com proteção. Ao voltar à partida normal, desligue o modo e confirme a borda sem o indicador azul.'),
 ('small','Os sprites acima são imagens reais dos recursos do jogo. Eles ajudam a reconhecer silhuetas e não indicam a ordem exata de aparecimento das formações.')])
page('11  |  SOM E CONSULTA','Áudio e informações finais',[
 table(['Opção','O que muda'],[['DAC6','Efeitos e música pelo DAC de 6 bits do CoCo'],['Orchestra-90 CC','Mistura mono enviada aos dois canais do cartucho'],['Música desligada','Efeitos ativos e até sete inimigos comuns'],['Música ligada com M','Até cinco inimigos comuns; chefes preservados']], [145,CW-145]),
 p('A música começa desligada. M alterna as faixas da fase e do chefe nas duas etapas jogáveis. Com música, o limite de inimigos comuns cai de sete para cinco, uma redução de aproximadamente 29%.'),
 p('A revisão de desenho reduz cópias e evita apagar quadros sem mudanças. Ela foi mais rápida nos cenários medidos, mas ainda pode haver lentidão e cintilação residual em situações carregadas. O piscar intencional após dano foi mantido.'),
 h('O que esta edição entrega'),
 p('Duas fases, dois chefes, controles por teclado e joystick, armas, habilidades, checkpoints, caminhada automática e reinício pela abertura KONAMI. Som de duas vozes, paleta RGB e morte do segundo chefe ainda têm adaptações. Os testes foram feitos no XRoar com 128 KiB; não houve teste em hardware físico.'),
 h('Créditos e referências'),
 p('Jogo original: <b>Knightmare</b>, Konami, 1986. Adaptação e documentação deste projeto: reengenharia do MSX para CoCo 3, preparada para o repositório <b>benicasa2/KNIGHTMARE_COCO3</b>.'),
 ('small','Enredo: manual original Konami, preservado em <link href="https://manuals.plus/m/e0b612d28cc1ab4fc591a86587b876cd3f2fae274e11ebab61bfd5c6e69275a2" color="#245ca8">Knightmare How to play</link>. Ferramentas e pesquisa: <link href="https://github.com/benicasa2/REA-RETRO-9" color="#245ca8">REA RETRO 9</link>, Ghidra, LWASM e XRoar. As licenças dos componentes externos estão em third_party.'),
 ('small','As capturas foram obtidas na versão DAC6 do port. Algumas cenas usam preparação controlada ou proteção I para documentação, sem alterar a arte renderizada. As instruções também valem para Orchestra-90. ROMs originais e ROMs de sistema não são distribuídas.'),
 ('small','Consulta técnica: LEIA-ME.md, STATUS.json, analysis/FASE-2-E-SAIDA.md e analysis/CINTILACAO-DUAS-FASES.md. A pontuação desta edição satura em 65.535; não suponha que todas as regras de pontuação do cartucho original já estejam reproduzidas.')])

class Pictures(Flowable):
 def __init__(self,names,captions,width=None):Flowable.__init__(self);self.names=names;self.captions=captions;self.width=CW;self.cell=CW/len(names);self.iw=min(width or self.cell-12,self.cell-12);self.ih=self.iw*.75;self.height=self.ih+36
 def draw(self):
  for i,(name,cap) in enumerate(zip(self.names,self.captions)):
   x=i*self.cell+(self.cell-self.iw)/2;self.canv.drawImage(str(IMG/(name+'.png')),x,32,self.iw,self.ih,mask='auto');q=Paragraph(cap,styles['small']);w,h=q.wrap(self.cell-16,32);q.drawOn(self.canv,i*self.cell+8,29-h)
class Keys(Flowable):
 def __init__(self):Flowable.__init__(self);self.width=CW;self.height=106
 def draw(self):
  c=self.canv
  for off,label in [(55,'W A S D'),(285,'Setas')]:
   c.setFillColor(muted);c.setFont('ArialB',9);c.drawString(off,94,label)
   for x,y,t in [(off+30,52,'W' if off==55 else '↑'),(off,15,'A' if off==55 else '←'),(off+36,15,'S' if off==55 else '↓'),(off+72,15,'D' if off==55 else '→')]:
    c.setFillColor(navy);c.roundRect(x,y,32,30,4,fill=1,stroke=0);c.setFillColor(colors.white);c.setFont('ArialB',14);c.drawCentredString(x+16,y+8,t)
class Sprites(Flowable):
 def __init__(self,names,labels):Flowable.__init__(self);self.names=names;self.labels=labels;self.width=CW;self.height=146 if len(names)>4 else 92
 def draw(self):
  c=self.canv;cols=3 if len(self.names)>4 else len(self.names);rows=(len(self.names)+cols-1)//cols;ch=self.height/rows
  for i,(name,label) in enumerate(zip(self.names,self.labels)):
   x=(i%cols)*CW/cols;y=self.height-(i//cols+1)*ch;c.setFillColor(colors.HexColor('#edf2f7'));c.roundRect(x+4,y+7,CW/cols-8,ch-10,5,fill=1,stroke=0)
   c.setFillColor(colors.HexColor('#607890'));c.roundRect(x+CW/cols/2-43,y+23,86,ch-30,3,fill=1,stroke=0);im=PIL.open(IMG/(name+'.png'));scale=min((ch-34)/im.height,76/im.width);iw=im.width*scale;ih=im.height*scale;c.drawImage(str(IMG/(name+'.png')),x+(CW/cols-iw)/2,y+22,iw,ih,mask='auto');c.setFillColor(ink);c.setFont('Arial',9);c.drawCentredString(x+CW/cols/2,y+12,label)
def header(c,d):
 c.setFillColor(navy);c.rect(0,H-13,W,13,stroke=0,fill=1);c.setFillColor(muted);c.setFont('Arial',8);c.drawString(42,H-33,'KNIGHTMARE  /  COCO 3 128 KiB');c.setStrokeColor(colors.HexColor('#d7dfe8'));c.line(42,35,W-42,35);c.drawString(42,22,'Manual do jogador   •   v0.2.1');c.drawRightString(W-42,22,f'{d.page:02d}')
story=[];md=['# Knightmare no CoCo 3\n\nManual ilustrado do jogador | v0.2.1\n']
for num,pg in enumerate(pages):
 if num:story.append(PageBreak())
 story.extend([Paragraph(pg['kicker'],styles['k']),Paragraph(pg['title'],styles['h'])]);md.append('\n## '+pg['title']+'\n')
 for b in pg['blocks']:
  typ=b[0]
  if typ in ['p','small','h2']:
   story.append(Paragraph(b[1],styles[typ]));md.append(b[1]+'\n')
  elif typ=='photo':story.extend([Spacer(1,5),Pictures([b[1]],[b[2]],b[3]),Spacer(1,6)]);md.append(f'![{b[2]}](images/{b[1]}.png)\n\n{b[2]}\n')
  elif typ=='pair':story.extend([Pictures([b[1],b[2]],[b[3],b[4]]),Spacer(1,6)]);md.extend([f'![{b[3]}](images/{b[1]}.png)\n',f'![{b[4]}](images/{b[2]}.png)\n'])
  elif typ=='sequence':story.extend([Pictures(b[1],['Alinhar','Avançar','Entrar']),Spacer(1,6)]);md.extend(f'![Caminhada {i+1}](images/{n}.png)\n' for i,n in enumerate(b[1]))
  elif typ=='keys':story.append(Keys());md.append('Movimento: W cima, A esquerda, S baixo, D direita. As setas também funcionam.\n')
  elif typ=='weapons':story.append(Sprites(weapon_icons,['Flecha','Flechas reforçadas','Lâmina','Bumerangue','Três direções','Flecha de fogo']));md.extend(f'![Arma {i+1}](images/{n}.png)\n' for i,n in enumerate(weapon_icons))
  elif typ=='enemies':story.append(Sprites(['enemy-'+str(k) for k in [0,3,4,7,11,12]],['Mergulho','Cruzamento','Investida','Quatro impactos','Dardo','Ataque veloz']));md.append('Silhuetas dos inimigos\n');md.extend(f'![Inimigo {k}](images/enemy-{k}.png)\n' for k in [0,3,4,7,11,12])
  elif typ=='table':
   cells=[[Paragraph(t,styles['th']) for t in b[1]]]+[[Paragraph(t,styles['td']) for t in row] for row in b[2]];tb=Table(cells,colWidths=b[3],hAlign='LEFT');tb.setStyle(TableStyle([('BACKGROUND',(0,0),(-1,0),navy),('VALIGN',(0,0),(-1,-1),'MIDDLE'),('GRID',(0,0),(-1,-1),.4,colors.HexColor('#d9dfe8')),('ROWBACKGROUNDS',(0,1),(-1,-1),[colors.white,colors.HexColor('#f2f5f8')]),('LEFTPADDING',(0,0),(-1,-1),10),('RIGHTPADDING',(0,0),(-1,-1),10),('TOPPADDING',(0,0),(-1,-1),7),('BOTTOMPADDING',(0,0),(-1,-1),7)]));story.extend([tb,Spacer(1,10)]);md.append('| '+' | '.join(b[1])+' |\n| '+' | '.join(['---']*len(b[1]))+' |\n'+'\n'.join('| '+' | '.join(row)+' |' for row in b[2])+'\n')
DOC.mkdir(parents=True,exist_ok=True);(DOC/'MANUAL.md').write_text(__import__('re').sub(r'<link href="([^"]+)"[^>]*>(.*?)</link>',r'[\2](\1)','\n'.join(md)),encoding='utf-8');(DOC/'manual-content.json').write_text(json.dumps(pages,ensure_ascii=False,indent=2),encoding='utf-8')
path=DOC/'KNIGHTMARE_COCO3_Manual_v0.2.1.pdf';doc=SimpleDocTemplate(str(path),pagesize=A4,leftMargin=42,rightMargin=42,topMargin=53,bottomMargin=48,title='Knightmare no CoCo 3 Manual do jogador',author='Projeto KNIGHTMARE_COCO3',pageCompression=1);doc.build(story,onFirstPage=header,onLaterPages=header)
print(path)
