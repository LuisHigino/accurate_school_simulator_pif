#include "sala_de_aula.h"
#include "prof.h"
#include "submain.h"

// Variáveis internas (visíveis apenas dentro deste arquivo)
static Texture2D backgroundAula;
static Texture2D tabletTexture;
static Professora professora;
static RenderTexture2D telaSubJogo;


//AJEITAR POSIÇÃO DO TABLET
static Vector2 posicaoSubJogo = { 320, 320 };
static Vector2 posicaoBase = { 320, 320 };
static Vector2 posicaoAbaixado = { 320, 985 };
static Vector2 posicaoTablet = { 10, 30 }; // posição separada do Tablet.png (ajustada para direita)
                            //horizontal/vertical
                        // ++ = direita / ++ = baixo
static bool tabletLevantado = true;
static bool gameOverPrincipal = false;

void InitSalaDeAula(int dificuldadeProfessora) {
    // 1. Carrega o fundo da sala
    backgroundAula = LoadTexture("GAME/assets/images/backgroundAula.png");
    tabletTexture = LoadTexture("GAME/assets/images/Tablet.png");
    
    // 2. Inicializa a professora em posição maior e um pouco mais à esquerda
    InitProfessora(&professora, dificuldadeProfessora, (Vector2){ 30, 50 });
    
    // 3. Inicializa o subjogo virtual do Gnomo/Abelha
    telaSubJogo = LoadRenderTexture(1280, 720);
    InitSubJogo();
    
    gameOverPrincipal = false;
    tabletLevantado = true;
}

void UpdateSalaDeAula(float deltaTime) {
    if (gameOverPrincipal) {
        if (IsKeyPressed(KEY_SPACE)) { 
            InitSalaDeAula(professora.dificuldade); 
        }
        return; 
    }

    UpdateProfessora(&professora, deltaTime);
    UpdateSubJogo(); 

    if (IsKeyDown(KEY_SPACE)) {
        tabletLevantado = true;
        posicaoSubJogo = posicaoBase;     
    } else {
        tabletLevantado = false;
        posicaoSubJogo = posicaoAbaixado; 
        posicaoSubJogo = posicaoBase;     
        tabletLevantado = true;
    } else {
        posicaoSubJogo = posicaoAbaixado; 
        tabletLevantado = false;
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
    
    // Desenha o background da sala de aula cobrindo toda a tela
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

    // Desenha o tablet de fundo exatamente atrás do subjogo, sem mudar o tamanho
    Rectangle orig = { 0, 0, 1280.0f, -720.0f };
    Rectangle dest = { posicaoSubJogo.x, posicaoSubJogo.y, 1280.0f, 720.0f };

    if (tabletLevantado && tabletTexture.width > 0) {
        Rectangle destTablet = { posicaoTablet.x, posicaoTablet.y, 1700.0f, 1050.0f };
            destTablet.width = 1900.0f; // Increase tablet width from 2000 to 2200
            DrawTexturePro(
                tabletTexture,
                (Rectangle){ 0.0f, 0.0f, (float)tabletTexture.width, (float)tabletTexture.height },
                destTablet,
                (Vector2){ 0.0f, 0.0f },
                0.0f,
                WHITE
            );
    }

    DrawTexturePro(telaSubJogo.texture, orig, dest, (Vector2){ 0, 0 }, 0.0f, WHITE);

    // Se deu Game Over na sala de aula
    if (gameOverPrincipal) {
        DrawRectangle(0, 0, 1920, 1080, Fade(BLACK, 0.8f)); // Escurece a tela
        DrawText("VOCE FOI PEGO PELA PROFESSORA!", 450, 450, 50, RED);
        DrawText("Pressione 'ESPAÇO' para tentar novamente", 600, 550, 30, LIGHTGRAY);
    }
}

void UnloadSalaDeAula(void) {
    UnloadTexture(backgroundAula);
    UnloadTexture(tabletTexture);
    UnloadProfessora(&professora);
    UnloadSubJogo();
    UnloadRenderTexture(telaSubJogo);
}