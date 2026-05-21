#include "raylib.h"
#include "submain.h"
#include "abelha.h"
#include "inimigo.h"
#include "constantes.h"

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

static float tempoDecorrido = 0.0f;
static float linhaChegadaX = 1280 + 200; // 1280 é o screenWidth fixo aqui

static Texture2D background;
static Texture2D texturaAbelha;
static Texture2D et1;
static Texture2D et2;
static Texture2D et3;
static Texture2D frameEt; // Guardará o frame atual da animação

static int frameEtAtual = 0;
static int timerEtAnimacao = 0;

static Abelha abelha;
static Inimigo inimigos[MAX_INIMIGOS];

// ============================================================================
// 1. INICIALIZAÇÃO DO SUBJOGO
// ============================================================================
void InitSubJogo(void)
{
    // Carrega o Background
    background = LoadTexture("GAME/assets/images/background.png");
    SetTextureFilter(background, TEXTURE_FILTER_POINT);

    // Carrega o Gnomo (Abelha)
    texturaAbelha = LoadTexture("GAME/assets/images/gnomo_jetpack1.png");
    SetTextureFilter(texturaAbelha, TEXTURE_FILTER_POINT);
    InitAbelha(&abelha, texturaAbelha, 1);

    // Carrega os ETs (Inimigos)
    et1 = LoadTexture("GAME/assets/images/et1.png");
    et2 = LoadTexture("GAME/assets/images/et2.png");
    et3 = LoadTexture("GAME/assets/images/et3.png");
    SetTextureFilter(et1, TEXTURE_FILTER_POINT);
    SetTextureFilter(et2, TEXTURE_FILTER_POINT);
    SetTextureFilter(et3, TEXTURE_FILTER_POINT);

    InitInimigos(inimigos, 0, screenWidth);
    
    // Garante que o jogo comece resetado
    jogoComecou = false;
    jogoVencido = false;
    linhaChegadaLiberada = false;
    tempoDecorrido = 0.0f;
    linhaChegadaX = screenWidth + 200;
}

// ============================================================================
// 2. LÓGICA DO SUBJOGO (Roda a cada Frame)
// ============================================================================
void UpdateSubJogo(void)
{
    int teclaPressionada = GetKeyPressed();

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

        InitInimigos(inimigos, 3, screenWidth);
    }

    // --- ATUALIZAR GNOMO/ABELHA ---
    if (jogoComecou && !jogoVencido)
    {
        tempoDecorrido += GetFrameTime();

        if (tempoDecorrido >= tempoParaLinhaChegada)
            linhaChegadaLiberada = true;

        if (linhaChegadaLiberada)
        {
            if (linhaChegadaX > abelha.x + 40)
                linhaChegadaX -= velocidadeLinhaChegada * 60 * GetFrameTime();
            else
                linhaChegadaX = abelha.x + 40;
        }

        AtualizarAbelha(&abelha, GetFrameTime(), screenWidth);
    }

    // --- INIMIGOS ---
    if (jogoComecou && !jogoVencido)
    {
        AtualizarInimigos(inimigos, screenWidth);
    }

    // --- COLISÃO ---
    if (ChecarColisaoInimigos(inimigos, GetHitboxAbelha(&abelha))) 
    {
        jogoComecou = false;
        jogoVencido = false;
        linhaChegadaLiberada = false;
        tempoDecorrido = 0.0f;
        linhaChegadaX = screenWidth + 200;
    }

    // --- VITÓRIA ---
    if (jogoComecou && !jogoVencido && linhaChegadaLiberada && abelha.x >= linhaChegadaX - 20)
    {
        jogoVencido = true;
        jogoComecou = false;
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
    // NOTA: BeginDrawing() e EndDrawing() FORAM REMOVIDOS DAQUI!
    
    ClearBackground(BLACK);

    DrawTexturePro(
        background,
        (Rectangle){0, 0, (float)background.width, (float)background.height},
        (Rectangle){0, 0, (float)screenWidth, (float)screenHeight},
        (Vector2){0, 0},
        0,
        WHITE
    );

    if (!jogoComecou)
    {
        DrawText("APERTE QUALQUER TECLA PARA COMEÇAR", 320, 80, 30, WHITE);
        DrawText("SOBREVIVA 1 MINUTO PARA LIBERAR A LINHA DE CHEGADA", 250, 120, 24, LIGHTGRAY);
    }

    if (linhaChegadaLiberada)
    {
        DrawLine((int)linhaChegadaX, 0, (int)linhaChegadaX, screenHeight, YELLOW);
        DrawText("CHEGADA", (int)linhaChegadaX - 30, 20, 22, YELLOW);
    }

    DesenharAbelha(&abelha);

    if (jogoComecou || jogoVencido)
    {
        float tempoRestante = tempoParaLinhaChegada - tempoDecorrido;
        if (tempoRestante < 0.0f) tempoRestante = 0.0f;

        DrawText(TextFormat("TEMPO: %02d", (int)tempoRestante), 30, 30, 28, WHITE);
    }

    DesenharInimigos(inimigos, frameEt);

    if (jogoVencido)
    {
        DrawRectangle(0, screenHeight / 2 - 60, screenWidth, 120, Fade(BLACK, 0.75f));
        DrawText("PARABENS VOCE VENCEU O JOGO", 360, screenHeight / 2 - 15, 30, YELLOW);
        DrawText("PRESSIONE QUALQUER TECLA PARA JOGAR NOVAMENTE", 280, screenHeight / 2 + 25, 22, WHITE);
    }
}

// ============================================================================
// 4. DESCARREGAR ARQUIVOS DA MEMÓRIA
// ============================================================================
void UnloadSubJogo(void)
{
    UnloadAbelha(&abelha);
    UnloadTexture(background);
    UnloadTexture(et1);
    UnloadTexture(et2);
    UnloadTexture(et3);

}