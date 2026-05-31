
# Accurate School Simulator

**Accurate School Simulator** é um jogo de sobrevivência e agilidade onde você deve terminar um joguinho no seu tablet sem ser pego pela professora no meio da aula! 

Desenvolvido inteiramente em **C** utilizando a biblioteca **Raylib** para gráficos, texturas e áudio, o projeto foi estruturado utilizando um `Makefile`, tornando o processo de download, compilação e execução no Linux extremamente rápido e direto ao ponto.

---

## 🐧 1. Pré-requisitos (Dependências do Linux)

Para conseguir compilar e rodar o jogo, seu sistema precisará das ferramentas básicas de compilação em C e da biblioteca Raylib. Abra o seu terminal e rode os comandos abaixo:

**Atualize a lista de pacotes:**

```bash
sudo apt update

```

**Instale o compilador, o Git e a Raylib:**

```bash
sudo apt install build-essential libraylib-dev git

```

*(Nota: O gerenciador de pacotes baixará e instalará tudo o que é necessário automaticamente).*

---

## 🚀 2. Como Baixar e Jogar

Com as ferramentas instaladas, você só precisa clonar o repositório, entrar na pasta do jogo e mandar o código rodar. Tudo isso direto pelo terminal!

**Passo 1:** Clone o repositório do jogo para o seu computador:

```bash
git clone https://github.com/LuisHigino/accurate_school_simulator_pif.git

```

**Passo 2:** Entre na pasta que acabou de ser baixada:

```bash
cd accurate_school_simulator_pif/

```

**Passo 3:** Compile e execute o jogo magicamente com um único comando:

```bash
make run

```

*(O comando `make run` cuida de limpar resíduos antigos, compilar o executável mais recente e abrir o jogo imediatamente para você).*

---

## ⌨️ 3. Controles do Jogo

* **No Menu Principal:**
* ⬆️ / ⬇️ **Setas para Cima/Baixo:** Navegar pelas opções (*Iniciar*, *Dificuldade*, *Ranking*, *Sair*).
* ↩️ **ENTER:** Confirmar a opção ou alterar a dificuldade.


* **Durante o Jogo (Gnomo Jetpack):**
* ⬆️ / ⬇️ **Setas para Cima/Baixo:** Mudar de linha rapidamente para desviar dos ETs.


* **Fim de Jogo (Salvando o Ranking):**
* **Teclado:** Digite o seu nome.
* ⌫ **Backspace:** Apaga letras caso erre.
* ↩️ **ENTER (1º toque):** Salva o seu nome e tempo no arquivo `ranking.txt`.
* ↩️ **ENTER (2º toque):** Retorna ao Menu Principal.

*(Também é possível usar W/S para movimentação em menus e controle do Gnomo)*

---
# Vídeo do Jogo:

[![Vídeo do Jogo no Youtube](https://youtube.com)](https://www.youtube.com/watch?v=Ei_swrwTkoI)

# Explicação das Funções:

### 🐝 1. `abelha.c` (Mecânicas do Minigame)

* **`InitAbelha`**: Prepara o personagem (Gnomo/Abelha) no início da partida, definindo sua textura, posição X fixa e a faixa (lane) inicial.
* **`AtualizarAbelha`**: Lê as setas do teclado (ou W/S) e muda a `lane` do personagem (0 = Cima, 1 = Meio, 2 = Baixo). A posição Y é travada automaticamente dependendo da faixa.
* **`DesenharAbelha`**: Imprime a textura do Gnomo na tela. Se a textura falhar, tem um fallback seguro desenhando um quadrado azul.
* **`GetHitboxAbelha`**: Cria um "retângulo invisível" (Hitbox) um pouco menor que a textura do Gnomo, essencial para colisões mais justas.
* **`UnloadAbelha`**: Libera a memória da textura.

*(O motivo pelo qual se chama de "Abelha" é por que, durante o desenvolvimento, a idea original era uma abelha tentando não ser pega por sapos! Mas, agora é um Gnomo numa jetpack desviando de ETs.)*

### 👽 2. `inimigo.c` (Mecânicas dos Obstáculos)

* **`VelocidadePorDificuldade`** *(Static)*: Define a velocidade dos ETs baseada na dificuldade escolhida (Fácil = 8, Médio = 10, Difícil = 11).
* **`SortearFaixa`**: Rola um dado de 0 a 2 para decidir em qual linha o ET vai nascer.
* **`InitInimigos`**: Prepara o "pool" (vetor) de inimigos. Ativa apenas a quantidade necessária e os espalha para fora da tela (à direita).
* **`AtualizarInimigos`**: Move os ETs para a esquerda. Quando um ET sai da tela pela esquerda, ele é "reciclado" e volta para a direita numa faixa aleatória.
* **`DesenharInimigos`**: Desenha a textura dos ETs (ou um quadrado verde de segurança) na tela.
* **`ChecarColisaoInimigos`**: Pega a Hitbox do Gnomo e verifica se ela está sobrepondo a Hitbox de algum ET ativo. Retorna `true` se bater.

### 👩‍🏫 3. `prof.c` (IA da Professora)

* **`InitProfessora`**: Define a professora começando de costas (escrevendo), carrega as texturas e ajusta a dificuldade.
* **`UpdateProfessora`**: O coração da IA! É uma **Máquina de Estados**.
* Atualiza os timers.
* Baseado na dificuldade, define o tempo de "escrever" e "vigiar".
* No estado `PROFE_ESCREVENDO`, rola um dado: se cair na chance (aumentada pela dificuldade), ela entra em `PROFE_ALERTA`.
* No estado `PROFE_ALERTA` (Aviso), espera 1 segundo e vira.
* No estado `PROFE_OLHANDO`, pune o jogador e volta a escrever.


* **`DrawProfessora`**: Desenha a textura correta (idle, alerta, vigiando) baseada no estado atual.
* **`UnloadProfessora`**: Limpa as texturas da memória.

### 🏆 4. `ranking.c` (Sistema de Arquivos e Lista Encadeada)

* **`DeveFicarNaFrente`** *(Static)*: O Juiz. Compara dois jogadores e retorna quem ganha baseado na ordem de prioridade: Dificuldade Maior -> Tempo Menor -> Mortes Menores utilizando Lista Encadeada.
* **`InserirRanking`**: Cria um novo "Nó" na Lista Encadeada e usa o juiz para fazer uma **Inserção Ordenada**, colocando o jogador já na posição correta do pódio.
* **`SalvarRankingTXT` / `CarregarRankingTXT**`: Lida com a persistência de dados. O `Carregar` lê linha por linha e joga na `InserirRanking` para auto-organizar a lista.
* **`MostrarRanking` / `LiberarRanking**`: Imprime no terminal (para debug) e limpa a memória dinâmica (`malloc/free`) para evitar vazamentos (*memory leaks*).

### 🏫 5. `sala_de_aula.c` (Gerenciador do Ambiente Principal)

* **`InitSalaDeAula`**: Carrega tudo: Fundo, Jumpscare, Sons (com controle de volume individual). Inicia a Professora e também manda inicializar o SubJogo.
* **`UpdateSalaDeAula`**: A lógica de risco/recompensa.
* Se o jogo acabou, espera a animação de Jumpscare e a tecla ESPAÇO.
* Se estiver rodando, monitora a tecla ESPAÇO para "levantar/abaixar o tablet".
* Compara o estado do tablet com o estado da Professora (Se tablet = Levantado E Professora = Olhando -> Game Over).


* **`DrawSalaDeAula`**: Desenha o fundo, o aluno e a professora. A grande sacada aqui é desenhar o `telaSubJogo` (RenderTexture2D) em cima do tablet, simulando uma tela dentro da tela!
* **`UnloadSalaDeAula`**: Descarrega todos os arquivos gigantes de mídia e chama o Unload do Subjogo e da Professora.

### 📱 6. `submain.c` (Gerenciador do Minigame)

* **`InitSubJogo`**: Carrega as texturas do minigame e chama o `Init` da abelha e dos inimigos.
* **`UpdateSubJogo`**: Controla o fluxo da partida interna. Conta o tempo até 60 segundos para liberar a "Linha de Chegada". Trata a digitação do nome do jogador char por char na tela de vitória e gerencia a lógica de salvar e voltar ao menu.
* **`DrawSubJogo`**: Renderiza os ETs, o Gnomo, os Textos de HUD (tempo) e a tela final de vitória com o nome do jogador.
* **`UnloadSubJogo`**: Limpa as memórias específicas do minigame.

### 🖥️ 7. `main.c` (O Motor Central)

* **`DesenharRankingNaTela`**: Pega a lista encadeada e desenha o Top 8 perfeitamente alinhado no Menu Principal.
* **`main`**: Inicializa a Raylib (`InitWindow`, `InitAudioDevice`). Cria um Canvas virtual de 1920x1080.
* Controla a **Tela Atual** (Menu vs Jogo).
* No Menu, escuta as setas para navegar entre as opções (Iniciar, Dificuldade, Ranking) com feedback sonoro.
* No final, redimensiona o Canvas virtual matematicamente para caber na tela física (monitor) do jogador mantendo a proporção.

---

# ✨ Destaques da Implementação (Highlights)

### 1. Efeito "Tela dentro da Tela" (RenderTexture2D)

A mecânica principal do jogo exige que o jogador preste atenção em duas coisas ao mesmo tempo: o minigame do Gnomo no tablet e a professora no ambiente físico.
Para criar esse efeito visual sem misturar os códigos, utilizamos o conceito de **RenderTexture2D** (Canvas Virtuais).

* Todo o código de `submain.c` (O jogo da abelha) é desenhado em uma "textura invisível" em vez da tela principal.
* Depois, em `sala_de_aula.c`, essa textura do minigame é colada em cima do desenho do tablet.
* Quando o jogador aperta "ESPAÇO" para esconder o tablet, nós simplesmente abaixamos a textura do minigame na tela para uma posição que vai além da tela, sem precisar pausar a lógica do Gnomo ou dos ETs e dar o efeito de estar abaixado!

### 2. Oportunidade de Movimento (Estilo FNAF)

A IA da professora foi inspirada nas mecânicas clássicas de *Five Nights at Freddy's*.

* **`PROFE_ESCREVENDO`**: Fica de costas. A cada 6/5/4,5 segundos (dependendo da dificuldade), ela rola um dado de 0 à 40. Esse número é comparado ao nível de dificuldade da professora (entre 1 a 3) vezes 10. Se o numéro rolado for menor ou igual a dificuldade vezes 10, a professora irá entrar no estado de alerta
* **`PROFE_ALERTA`**: Uma janela curta de tempo (ex: 1 segundo no modo difícil) onde a animação muda, avisando o jogador que ela vai virar.
* **`PROFE_OLHANDO`**: Se o tablet estiver levantado durante este estado, é *Game Over*.
Isso garante que a professora não vire em tempos aleatórios mas sim siga um rítimo. Apenas a condição de movimentação é aleatória, fazendo com que até na dificuldade mais difícil, ainda exista a chance dela não virar num intervalo de tempo

---

## 🐛 Desafios e Soluções

Durante o desenvolvimento na linguagem C, lidamos com alguns obstáculos técnicos interessantes:

### 1. O Desafio do Redimensionamento de Janela

**O Problema:** Como permitir que o jogador jogue em tela cheia (FullHD, 4K) ou redimensione a janela com o mouse sem quebrar as coordenadas do jogo, os cliques ou distorcer as pixel-arts?
**A Solução:** Em `main.c`, criamos uma resolução base virtual (1920x1080). Em vez de desenhar os objetos diretamente no monitor, desenhamos nesse "Canvas". No final de cada frame da `main`, calculamos uma variável `scale` (A menor proporção entre a largura e a altura). Assim, imprimimos o Canvas na tela esticando-o de forma proporcional (Letterboxing), mantendo as barras pretas perfeitas e o jogo intacto, independente do tamanho da janela.

Ah, entendi perfeitamente! Esse é um erro clássico de variáveis de escopo e estado global em C (principalmente quando usamos variáveis `static` ou dividimos o jogo em vários arquivos).

A correção de vocês foi na verdade muito mais focada em **sincronização de estados** e **formatação de dados** do que apenas na leitura do arquivo. Isso é um excelente ponto técnico para destacar!

Aqui está a versão reescrita e muito mais precisa do desafio do Ranking para você substituir no seu `README.md`:

---

### 2. Sincronização de Estados e Formatação no Ranking

**O Problema:** O sistema de ranking apresentava três inconsistências graves na hora de salvar os dados do jogador:

1. A dificuldade registrada não mudava (ficava sempre a mesma);
2. O tempo salvo era apenas o do minigame do Gnomo e não o da sessão inteira da sala de aula, além de ser exibido em formato bruto (ex: "62" em vez de "1:02");
3. O número de derrotas (mortes) na abelha era cumulativo, ou seja, não zerava quando o jogador iniciava uma nova partida.

**A Solução:** Fizemos uma varredura para garantir que os módulos do jogo se comunicassem corretamente e limpassem a memória residual:

* **Reset de Memória Estática:** Criamos a função `ReiniciarEstadoSubJogo()` no `submain.c`, garantindo que variáveis como `totalDerrotasAbelha` fossem zeradas rigidamente toda vez que a função `InitSubJogo` fosse chamada.
* **Comunicação entre Arquivos:** Em vez do minigame isolar suas próprias regras, fizemos ele puxar os dados da `sala_de_aula.c` utilizando getters como `GetDificuldadeSala()` e enviando o tempo de sobrevivência da sessão como um todo, não apenas os 60 segundos do minigame.
* **Matemática de Formatação:** Para exibir o tempo como um relógio real, utilizamos operações de módulo no `ranking.c` e `main.c`. Em vez de salvar strings complexas, salvamos o total de segundos inteiros e, na hora de mostrar ou escrever no arquivo, fazemos a separação em minutos e segundos com máscara de formatação: `(tempoTotal / 60)` para os minutos e `(tempoTotal % 60)` para os segundos exibidos com `%02d` (garantindo o zero à esquerda, transformando 62s em `1:02`).

### **Implementação dos sons do jogo:** 
Na teoria, os sons já estão funcionando, porém em caso de erro, tente baixar os sons pelo comando abaixo. Esse comando irá permitir que os sons em que escolhemos pro nosso jogo apareçam. 

```bash
sudo apt update && sudo apt install pulseaudio alsa-utils -y
pulseaudio --start.

```

### Integrantes da equipe: 

Eduardo Boxwell Simões - ebs2@cesar.school

Luís Felipe Higino Moura - lfhm@cesar.school

Rafael Pimentel Borba - rpb@cesar.school

---
