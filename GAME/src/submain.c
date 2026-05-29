#include "raylib.h"
#include "submain.h"
#include "abelha.h"
#include "inimigo.h"
#include "constantes.h"
#include "ranking.h"
#include "sala_de_aula.h" // <-- IMPORTANTE: Traz as funções de tempo e dificuldade da sala!

#include <stdio.h>
#include <string.h>

// ============================================================================
// VARIÁVEIS GLOBAIS DO SUBJOGO (Visíveis apenas neste arquivo graças ao 'static')
// ============================================================================
static const int screenWidth = 1280;
static const int screenHeight = 720;
static const float tempoParaLinhaChegada = 60.0f;
static const float velocidadeLinhaChegada = 8.0f;

static bool jogoComecou = false;
static bool jogoVencido = false;
static bool linhaChegadaLiberada = false;
static bool aguardandoNome = false;
static bool rankingSalvo = false;
static bool solicitarRetornoMenu = false;
static bool retornoLiberadoAposSalvar = false;

static float tempoDecorrido = 0.0f;
static float linhaChegadaX = 1280 + 200; // 1280 é o screenWidth fixo aqui
static char nomeJogador[50] = { 0 };
static int nomeJogadorTamanho = 0;
static Ranking *rankingLista = NULL;

static int totalDerrotasAbelha = 0; // <-- NOSSA VARIÁVEL RESTAURADA!

static Texture2D background;
static Texture2D texturaAbelha1;
static Texture2D texturaAbelha2;
static Texture2D texturaAbelha3;
static Texture2D et1;
static Texture2D et2;
static Texture2D et3;
static Texture2D frameEt; // Guardará o frame atual da animação

static int frameAbelhaAtual = 0;
static int timerAbelhaAnimacao = 0;
static int frameEtAtual = 0;
static int timerEtAnimacao = 0;

static Abelha abelha;
static Inimigo inimigos[MAX_INIMIGOS];

static Rectangle botaoVoltarMenu = { 470.0f, 605.0f, 340.0f, 40.0f };

static Vector2 MouseParaCoordenadaSubJogo(void)
{
    const float virtualWidth = 1920.0f;
    const float virtualHeight = 1080.0f;
    const float subJogoOffsetX = 320.0f;
    const float subJogoOffsetY = 320.0f;

    float scale = (float)GetScreenWidth() / virtualWidth;
    float scaleAltura = (float)GetScreenHeight() / virtualHeight;
    if (scaleAltura < scale)
    {
        scale = scaleAltura;
    }

    float offsetX = ((float)GetScreenWidth() - (virtualWidth * scale)) * 0.5f;
    float offsetY = ((float)GetScreenHeight() - (virtualHeight * scale)) * 0.5f;

    Vector2 mouseTela = GetMousePosition();
    Vector2 mouseVirtual = {
        (mouseTela.x - offsetX) / scale,
        (mouseTela.y - offsetY) / scale
    };

    return (Vector2){
        mouseVirtual.x - subJogoOffsetX,
        mouseVirtual.y - subJogoOffsetY
    };
}

// ============================================================================
// 1. INICIALIZAÇÃO DO SUBJOGO
// ============================================================================
void InitSubJogo(void)
{
    if (rankingLista != NULL)
    {
        LiberarRanking(rankingLista);
        rankingLista = NULL;
    }

    CarregarRankingTXT(&rankingLista);

    // Carrega o Background
    background = LoadTexture("GAME/assets/images/background.png");
    SetTextureFilter(background, TEXTURE_FILTER_POINT);

    // Carrega o Gnomo (Abelha)
    texturaAbelha1 = LoadTexture("GAME/assets/images/gnomo_jetpack1.png");
    texturaAbelha2 = LoadTexture("GAME/assets/images/gnomo_jetpack2.png");
    texturaAbelha3 = LoadTexture("GAME/assets/images/gnomo_jetpack3.png");
    SetTextureFilter(texturaAbelha1, TEXTURE_FILTER_POINT);
    SetTextureFilter(texturaAbelha2, TEXTURE_FILTER_POINT);
    SetTextureFilter(texturaAbelha3, TEXTURE_FILTER_POINT);
    InitAbelha(&abelha, texturaAbelha1, 1);

    // Carrega os ETs (Inimigos)
    et1 = LoadTexture("GAME/assets/images/et1.png");
    et2 = LoadTexture("GAME/assets/images/et2.png");
    et3 = LoadTexture("GAME/assets/images/et3.png");
    SetTextureFilter(et1, TEXTURE_FILTER_POINT);
    SetTextureFilter(et2, TEXTURE_FILTER_POINT);
    SetTextureFilter(et3, TEXTURE_FILTER_POINT);

    InitInimigos(inimigos, 0, screenWidth, GetDificuldadeSala());
    
    // Garante que o jogo comece resetado
    jogoComecou = false;
    jogoVencido = false;
    linhaChegadaLiberada = false;
    aguardandoNome = false;
    rankingSalvo = false;
    solicitarRetornoMenu = false;
    retornoLiberadoAposSalvar = false;
    tempoDecorrido = 0.0f;
    linhaChegadaX = screenWidth + 200;
    nomeJogador[0] = '\0';
    nomeJogadorTamanho = 0;

    totalDerrotasAbelha = 0; // <-- ZERA AS MORTES QUANDO INICIA PARTIDA NOVA!
}

// ============================================================================
// 2. LÓGICA DO SUBJOGO (Roda a cada Frame)
// ============================================================================
void UpdateSubJogo(void)
{
    int teclaPressionada = GetKeyPressed();

    if (jogoVencido)
    {
        aguardandoNome = true;

        int caractere = GetCharPressed();

        while (caractere > 0)
        {
            if (caractere >= 32 && caractere <= 125 && nomeJogadorTamanho < 49)
            {
                nomeJogador[nomeJogadorTamanho] = (char)caractere;
                nomeJogadorTamanho++;
                nomeJogador[nomeJogadorTamanho] = '\0';
            }

            caractere = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && nomeJogadorTamanho > 0)
        {
            nomeJogadorTamanho--;
            nomeJogador[nomeJogadorTamanho] = '\0';
        }

        if (!rankingSalvo && IsKeyPressed(KEY_ENTER))
        {
            char nomeFinal[50];

            if (nomeJogadorTamanho == 0)
            {
                strcpy(nomeFinal, "ANONIMO");
            }
            else
            {
                strcpy(nomeFinal, nomeJogador);
            }

            // <-- NOSSO INSERIR RANKING PERFEITO RESTAURADO AQUI!
            InserirRanking(
                &rankingLista,
                nomeFinal,
                GetTempoSobrevivencia(),
                GetDificuldadeSala(),
                GetDerrotasAbelha()
            );

            SalvarRankingTXT(rankingLista);
            rankingSalvo = true;
            aguardandoNome = false;
            retornoLiberadoAposSalvar = false;
        }

        if (rankingSalvo)
        {
            if (!retornoLiberadoAposSalvar)
            {
                retornoLiberadoAposSalvar = true;
            }
            else
            {
                Vector2 mouseSubJogo = MouseParaCoordenadaSubJogo();
                bool cliqueBotao = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouseSubJogo, botaoVoltarMenu);
                bool qualquerTecla = (teclaPressionada != 0);

                if (cliqueBotao || qualquerTecla)
                {
                    solicitarRetornoMenu = true;
                }
            }
        }

        return;
    }

    // --- COMEÇAR JOGO ---
    if (!jogoComecou && teclaPressionada != 0)
    {
        jogoComecou = true;
        jogoVencido = false;
        linhaChegadaLiberada = false;
        tempoDecorrido = 0.0f;
        linhaChegadaX = screenWidth + 200;

        abelha.x = 120;
        abelha.lane = 1;

        InitInimigos(inimigos, 3, screenWidth, GetDificuldadeSala());
    }

    // --- ATUALIZAR GNOMO/ABELHA ---

    timerAbelhaAnimacao++;
    
    // O número 10 define a velocidade da animação (A cada 10 frames do jogo, ele troca a imagem)
    // Se quiser que ele bata as asas mais rápido, diminua para 5. Mais devagar, aumente para 15.
    if (timerAbelhaAnimacao >= 10) 
    {
        timerAbelhaAnimacao = 0; // Zera o relógio
        frameAbelhaAtual++;      // Pula pro próximo frame
        
        if (frameAbelhaAtual > 2) frameAbelhaAtual = 0; // Volta pro começo se passar do frame 3

        // Troca a textura que a abelha está segurando no momento
        if (frameAbelhaAtual == 0) abelha.textura = texturaAbelha1;
        else if (frameAbelhaAtual == 1) abelha.textura = texturaAbelha2;
        else if (frameAbelhaAtual == 2) abelha.textura = texturaAbelha3;
    }

    if (jogoComecou && !jogoVencido)
    {
        tempoDecorrido += GetFrameTime();

        if (tempoDecorrido >= tempoParaLinhaChegada)
            linhaChegadaLiberada = true;

        if (linhaChegadaLiberada)
        {
            linhaChegadaX -= velocidadeLinhaChegada * 60 * GetFrameTime();
        }

        AtualizarAbelha(&abelha, GetFrameTime(), screenWidth);
    }

    // --- INIMIGOS ---
    if (jogoComecou && !jogoVencido)
    {
        AtualizarInimigos(inimigos, screenWidth);
    }

    // --- COLISÃO ---
    // <-- RESTAURADA NOSSA PROTEÇÃO PARA NÃO CONTAR 60 MORTES POR SEGUNDO!
    if (jogoComecou && !jogoVencido) 
    {
        if (ChecarColisaoInimigos(inimigos, GetHitboxAbelha(&abelha))) 
        {
            totalDerrotasAbelha++; // Soma +1 na nossa variável

            jogoComecou = false;
            jogoVencido = false;
            linhaChegadaLiberada = false;
            tempoDecorrido = 0.0f;
            linhaChegadaX = screenWidth + 200;
        }
    }

    // --- VITÓRIA ---
    if (jogoComecou && !jogoVencido && linhaChegadaLiberada && linhaChegadaX <= abelha.x + 20)
    {
        jogoVencido = true;
        jogoComecou = false;
        aguardandoNome = true;
        nomeJogador[0] = '\0';
        nomeJogadorTamanho = 0;
    }

    // --- ANIMAÇÃO ET ---
    if (jogoComecou && !jogoVencido)
        timerEtAnimacao++;

    if (timerEtAnimacao > 10)
    {
        frameEtAtual++;
        timerEtAnimacao = 0;

        if (frameEtAtual > 2)
            frameEtAtual = 0;
    }

    if (frameEtAtual == 0)
        frameEt = et1;
    else if (frameEtAtual == 1)
        frameEt = et2;
    else
        frameEt = et3;
}

// ============================================================================
// 3. RENDERIZAÇÃO DO SUBJOGO (Desenho Puro)
// ============================================================================
void DrawSubJogo(void)
{
    ClearBackground(BLACK);

    // Trava de segurança do Background
    if (background.width > 0) {
        DrawTexturePro(
            background,
            (Rectangle){0, 0, (float)background.width, (float)background.height},
            (Rectangle){0, 0, (float)screenWidth, (float)screenHeight},
            (Vector2){0, 0}, 0, WHITE
        );
    } else {
        // Se falhar, desenha um fundo cinza escuro
        DrawRectangle(0, 0, screenWidth, screenHeight, DARKGRAY);
        DrawText("FALTA BACKGROUND", 10, 10, 20, RED);
    }

    if (!jogoComecou) {
        DrawText("APERTE QUALQUER TECLA PARA COMEÇAR", 320, 80, 30, WHITE);
        DrawText("SOBREVIVA 1 MINUTO PARA LIBERAR A LINHA DE CHEGADA", 250, 120, 24, LIGHTGRAY);
    }

    if (linhaChegadaLiberada) {
        DrawLine((int)linhaChegadaX, 0, (int)linhaChegadaX, screenHeight, YELLOW);
        DrawText("CHEGADA", (int)linhaChegadaX - 30, 20, 22, YELLOW);
    }

    DesenharAbelha(&abelha);

    if (jogoComecou || jogoVencido) {
        float tempoRestante = tempoParaLinhaChegada - tempoDecorrido;
        if (tempoRestante < 0.0f) tempoRestante = 0.0f;
        DrawText(TextFormat("TEMPO: %02d", (int)tempoRestante), 30, 30, 28, WHITE);
    }

    DesenharInimigos(inimigos, frameEt);

    if (jogoVencido) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.92f));

        int tituloLargura = MeasureText("PARABENS VOCE VENCEU O JOGO", 30);
        int instrucaoLargura = MeasureText("DIGITE SEU NOME E APERTE ENTER PARA SALVAR NO RANKING", 24);
        char nomeTexto[80];
        snprintf(nomeTexto, sizeof(nomeTexto), "NOME: %s", nomeJogadorTamanho > 0 ? nomeJogador : "");
        int nomeLargura = MeasureText(nomeTexto, 28);

        DrawText("PARABENS VOCE VENCEU O JOGO", (screenWidth - tituloLargura) / 2, screenHeight / 2 - 70, 30, YELLOW);
        DrawText("DIGITE SEU NOME E APERTE ENTER PARA SALVAR NO RANKING", (screenWidth - instrucaoLargura) / 2, screenHeight / 2 - 20, 24, WHITE);
        DrawText(nomeTexto, (screenWidth - nomeLargura) / 2, screenHeight / 2 + 25, 28, GREEN);

        if (rankingSalvo)
        {
            int rankingLargura = MeasureText("RANKING ATUALIZADO", 22);
            DrawText("RANKING ATUALIZADO", (screenWidth - rankingLargura) / 2, screenHeight / 2 + 72, 22, LIME);

            DrawRectangleRec(botaoVoltarMenu, Fade(WHITE, 0.12f));
            DrawRectangleLinesEx(botaoVoltarMenu, 1.0f, LIGHTGRAY);

            const char *textoBotao = "VOLTAR AO MENU PRINCIPAL";
            int textoLargura = MeasureText(textoBotao, 20);
            DrawText(textoBotao, (int)(botaoVoltarMenu.x + (botaoVoltarMenu.width - textoLargura) * 0.5f), (int)botaoVoltarMenu.y + 10, 20, WHITE);
        }
    }
}

// ============================================================================
// 4. DESCARREGAR ARQUIVOS DA MEMÓRIA
// ============================================================================
void UnloadSubJogo(void)
{
    UnloadAbelha(&abelha);
    UnloadTexture(background);
    UnloadTexture(texturaAbelha1);
    UnloadTexture(texturaAbelha2);
    UnloadTexture(texturaAbelha3);
    UnloadTexture(et1);
    UnloadTexture(et2);
    UnloadTexture(et3);

    LiberarRanking(rankingLista);
    rankingLista = NULL;

}

bool SubJogoFoiVencido(void)
{
    return jogoVencido;
}

bool SubJogoConsumirSolicitacaoRetornoMenu(void)
{
    if (solicitarRetornoMenu)
    {
        solicitarRetornoMenu = false;
        return true;
    }

    return false;
}

// ============================================================================
// FUNÇÃO EXTRATORA DE MORTES QUE O GIT TINHA APAGADO!
// ============================================================================
int GetDerrotasAbelha(void) {
    return totalDerrotasAbelha;
}