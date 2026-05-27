#include "sala_de_aula.h"
#include "prof.h"
#include "submain.h"

// Variáveis internas (visíveis apenas dentro deste arquivo)
static Texture2D backgroundAula;
static Professora professora;
static RenderTexture2D telaSubJogo;

static Vector2 posicaoSubJogo = { 320, 320 };
static Vector2 posicaoBase = { 320, 320 };
static Vector2 posicaoAbaixado = { 320, 985 };

static bool tabletLevantado = true;
static bool gameOverPrincipal = false;

void InitSalaDeAula(int dificuldadeProfessora) {
    // 1. Carrega o fundo da sala
    backgroundAula = LoadTexture("GAME/assets/images/background_sala_de_aula.png");
    
    // 2. Inicializa a professora (ex: posição X=900, Y=400)
    InitProfessora(&professora, dificuldadeProfessora, (Vector2){ 900, 400 });
    
    // 3. Inicializa o subjogo virtual do Gnomo/Abelha
    telaSubJogo = LoadRenderTexture(1280, 720);
    InitSubJogo();
    
    gameOverPrincipal = false;
    tabletLevantado = true;
}

void UpdateSalaDeAula(float deltaTime) {
    if (gameOverPrincipal) {
        if (IsKeyPressed(KEY_R)) { 
            InitSalaDeAula(professora.dificuldade); 
        }
        return; 
    }

    UpdateProfessora(&professora, deltaTime);
    UpdateSubJogo(); 

    if (IsKeyDown(KEY_SPACE)) {
        posicaoSubJogo = posicaoAbaixado; 
        tabletLevantado = false;
    } else {
        posicaoSubJogo = posicaoBase;     
        tabletLevantado = true;
    }

    if (professora.estadoAtual == PROFE_OLHANDO && tabletLevantado) {
        gameOverPrincipal = true;
    }

    // ==========================================
    // MUDANÇA AQUI: Renderiza o tablet ANTES de desenhar a tela principal!
    // ==========================================
    BeginTextureMode(telaSubJogo);
        ClearBackground(BLACK);
        DrawSubJogo(); 
    EndTextureMode();
}

void DrawSalaDeAula(void) {
    
    // Proteção caso a textura falhe
    if (backgroundAula.width > 0) {
        DrawTexturePro(
            backgroundAula,
            (Rectangle){ 0, 0, (float)backgroundAula.width, (float)backgroundAula.height },
            (Rectangle){ 0, 0, 1920.0f, 1080.0f },
            (Vector2){ 0, 0 },
            0.0f, WHITE
        );
    } else {
        DrawRectangle(0, 0, 1920, 1080, DARKGRAY);
    }

    // Desenha a Professora (muda de cor dependendo do estado)
    DrawProfessora(&professora);

    // Desenha a tela do Subjogo (o tablet deslizando)
    Rectangle orig = { 0, 0, 1280.0f, -720.0f };
    Rectangle dest = { posicaoSubJogo.x, posicaoSubJogo.y, 1280.0f, 720.0f };
    DrawTexturePro(telaSubJogo.texture, orig, dest, (Vector2){ 0, 0 }, 0.0f, WHITE);

    // Se deu Game Over na sala de aula
    if (gameOverPrincipal) {
        DrawRectangle(0, 0, 1920, 1080, Fade(BLACK, 0.8f)); // Escurece a tela
        DrawText("VOCE FOI PEGO PELA PROFESSORA!", 450, 450, 50, RED);
        DrawText("Pressione 'R' para tentar novamente", 600, 550, 30, LIGHTGRAY);
    }
}

void UnloadSalaDeAula(void) {
    UnloadTexture(backgroundAula);
    UnloadSubJogo();
    UnloadRenderTexture(telaSubJogo);
}