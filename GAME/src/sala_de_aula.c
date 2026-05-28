#include "sala_de_aula.h"
#include "prof.h"
#include "submain.h"

// Variáveis internas (visíveis apenas dentro deste arquivo)
static Texture2D backgroundAula;
static Texture2D tabletTexture;
static Texture2D jumpscareTexture1;
static Texture2D jumpscareTexture2;
static Professora professora;
static RenderTexture2D telaSubJogo;
static float gameOverJumpscareTimer = 0.0f;
static bool gameOverJumpscareFrame = false;


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
    jumpscareTexture1 = LoadTexture("GAME/assets/images/professora_jumpscare1.png");
    jumpscareTexture2 = LoadTexture("GAME/assets/images/professora_jumpscare2.png");
    
    // 2. Inicializa a professora em posição maior e um pouco mais à esquerda
    InitProfessora(&professora, dificuldadeProfessora, (Vector2){ -40, 40 });
    gameOverJumpscareTimer = 0.0f;
    gameOverJumpscareFrame = false;
                                                            //horizontal/vertical
                                                        // ++ = direita / ++ = cima                                
    // 3. Inicializa o subjogo virtual do Gnomo/Abelha
    telaSubJogo = LoadRenderTexture(1280, 720);
    InitSubJogo();
    
    gameOverPrincipal = false;
    tabletLevantado = true;
}

void UpdateSalaDeAula(float deltaTime) {
    if (gameOverPrincipal) {
        gameOverJumpscareTimer += deltaTime;
        if (gameOverJumpscareTimer >= 0.35f) {
            gameOverJumpscareTimer = 0.0f;
            gameOverJumpscareFrame = !gameOverJumpscareFrame;
        }

        if (IsKeyPressed(KEY_SPACE)) { 
            InitSalaDeAula(professora.dificuldade); 
        }
        return; 
    }

    UpdateSubJogo(); 

    if (!SubJogoFoiVencido())
    {
        UpdateProfessora(&professora, deltaTime);
    if (IsKeyDown(KEY_SPACE)) {
        tabletLevantado = true;
        posicaoSubJogo = posicaoBase;     
    } else{
        tabletLevantado = false;
        posicaoSubJogo = posicaoAbaixado;
    }

        if (IsKeyDown(KEY_SPACE)) {
            posicaoSubJogo = posicaoBase;     
            tabletLevantado = true;
        } else {
            posicaoSubJogo = posicaoAbaixado; 
            tabletLevantado = false;
        }

        if (professora.estadoAtual == PROFE_OLHANDO && tabletLevantado) {
            gameOverPrincipal = true;
        }
    }
    else
    {
        tabletLevantado = true;
        posicaoSubJogo = posicaoBase;
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
        DrawRectangle(0, 0, 1920, 1080, Fade(BLACK, 1.0f));
        Texture2D jumpscare = gameOverJumpscareFrame ? jumpscareTexture2 : jumpscareTexture1;
        if (jumpscare.width > 0) {
            float scale = 12.0f;
            float destWidth = jumpscare.width * scale;
            float destHeight = jumpscare.height * scale;
            float posX = 960.0f - destWidth / 2.0f;
            float posY = 0.0f;
            DrawTexturePro(
                jumpscare,
                (Rectangle){ 0.0f, 0.0f, (float)jumpscare.width, (float)jumpscare.height },
                (Rectangle){ posX, posY, destWidth, destHeight },
                (Vector2){ 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        }
        DrawText("VOCE FOI PEGO PELA PROFESSORA!", 450, 25, 50, RED);
        DrawText("Pressione 'ESPAÇO' para tentar novamente", 600, 75, 30, RED);
    }
}

void UnloadSalaDeAula(void) {
    UnloadTexture(backgroundAula);
    UnloadTexture(tabletTexture);
    UnloadTexture(jumpscareTexture1);
    UnloadTexture(jumpscareTexture2);
    UnloadProfessora(&professora);
    UnloadSubJogo();
    UnloadRenderTexture(telaSubJogo);
}