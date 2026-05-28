#include "raylib.h"
#include "sala_de_aula.h"
#include "submain.h"
#include "ranking.h"

#include <stddef.h>

// Funções matemáticas simples para descobrirmos o tamanho do monitor
#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

typedef enum {
    TELA_MENU = 0,
    TELA_JOGO = 1
} TelaAtual;

static Vector2 ConverterMouseParaVirtual(Vector2 mouse, int screenWidth, int screenHeight, int virtualWidth, int virtualHeight)
{
    float scale = MIN((float)screenWidth / virtualWidth, (float)screenHeight / virtualHeight);
    float offsetX = (screenWidth - (virtualWidth * scale)) * 0.5f;
    float offsetY = (screenHeight - (virtualHeight * scale)) * 0.5f;

    return (Vector2){
        (mouse.x - offsetX) / scale,
        (mouse.y - offsetY) / scale
    };
}

static bool CliqueNoRetanguloVirtual(Rectangle retanguloVirtual, Vector2 mouseVirtual)
{
    return CheckCollisionPointRec(mouseVirtual, retanguloVirtual);
}

static const char *TextoDificuldade(int dificuldade)
{
    switch (dificuldade)
    {
        case 1: return "FACIL";
        case 2: return "MEDIO";
        case 3: return "DIFICIL";
        default: return "FACIL";
    }
}

static void DesenharRankingNaTela(Ranking *lista, int posX, int posY)
{
    // Aumentamos a largura para 540 para caber a nova coluna confortavelmente
    DrawRectangle(posX - 40, posY, 540, 520, Fade(BLACK, 0.78f));
    DrawRectangleLines(posX - 40, posY, 540, 520, GOLD);

    DrawText("RANKING AO VIVO", posX - 14, posY + 20, 34, YELLOW);
    
    // Cabeçalho das colunas atualizado
    DrawText("NOME      TEMPO      PERDAS      DIF", posX - 14, posY + 70, 22, LIGHTGRAY);

    int linha = 0;
    while (lista != NULL && linha < 8)
    {
        // 1. Nome do Jogador
        DrawText(TextFormat("%d. %s", linha + 1, lista->nome), posX - 14, posY + 110 + (linha * 48), 24, WHITE);
        
        // 2. Tempo Formatado (M:SS)
        int min = lista->dados[0][0] / 60;
        int seg = lista->dados[0][0] % 60;
        DrawText(TextFormat("%d:%02d", min, seg), posX + 150, posY + 110 + (linha * 48), 24, SKYBLUE);
        
        // 3. NOVA COLUNA: Quantidade de Perdas (dados[1][0])
        DrawText(TextFormat("%d", lista->dados[1][0]), posX + 275, posY + 110 + (linha * 48), 24, RED);
        
        // 4. Dificuldade em Texto
        DrawText(TextoDificuldade(lista->dados[0][1]), posX + 370, posY + 110 + (linha * 48), 20, ORANGE);
        
        lista = lista->prox;
        linha++;
    }

    if (linha == 0)
    {
        DrawText("Nenhum resultado salvo ainda.", posX - 14, posY + 130, 22, LIGHTGRAY);
    }
}


int main(void) {
    // 1. Avisa a Raylib que a janela pode ser redimensionada e deve abrir maximizada
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);

    // 2. Inicia a janela com um tamanho seguro.
    InitWindow(1280, 720, "Jogo Principal - Sala de Aula");
    InitAudioDevice();
    SetTargetFPS(60);

    // 3. A MÁGICA: O "Canvas" Virtual
    int virtualWidth = 1920;
    int virtualHeight = 1080;
    RenderTexture2D canvas = LoadRenderTexture(virtualWidth, virtualHeight);
    Texture2D menuTexture = LoadTexture("GAME/assets/images/Mainscreen.png");

    TelaAtual telaAtual = TELA_MENU;
    int dificuldadeSelecionada = 1; // Melhor começar no Fácil (1)
    bool mostrarRanking = false;
    bool sairDoJogo = false;

    SetTextureFilter(menuTexture, TEXTURE_FILTER_POINT);

    while (!WindowShouldClose() && !sairDoJogo) {
        // --- ATUALIZAÇÃO (UPDATE) ---
        if (telaAtual == TELA_MENU)
        {
            Vector2 mouseVirtual = ConverterMouseParaVirtual(GetMousePosition(), GetScreenWidth(), GetScreenHeight(), virtualWidth, virtualHeight);

            Rectangle botaoIniciar = { 0, 240, 360, 90 };
            Rectangle botaoDificuldade = { 0, 360, 360, 90 };
            Rectangle botaoRanking = { 0, 480, 360, 90 };
            Rectangle botaoSair = { 0, 600, 360, 90 };

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (CliqueNoRetanguloVirtual(botaoIniciar, mouseVirtual))
                {
                    InitSalaDeAula(dificuldadeSelecionada);
                    telaAtual = TELA_JOGO;
                    mostrarRanking = false;
                }
                else if (CliqueNoRetanguloVirtual(botaoDificuldade, mouseVirtual))
                {
                    dificuldadeSelecionada++;
                    if (dificuldadeSelecionada > 3)
                    {
                        dificuldadeSelecionada = 1;
                    }
                }
                else if (CliqueNoRetanguloVirtual(botaoRanking, mouseVirtual))
                {
                    mostrarRanking = !mostrarRanking;
                }
                else if (CliqueNoRetanguloVirtual(botaoSair, mouseVirtual))
                {
                    sairDoJogo = true;
                }
            }
        }
        else
        {
            UpdateSalaDeAula(GetFrameTime());

            // A NOVIDADE DO SEU GRUPO MANTIDA AQUI!
            if (SubJogoConsumirSolicitacaoRetornoMenu())
            {
                telaAtual = TELA_MENU;
                mostrarRanking = false;
                UnloadSalaDeAula(); // Descarrega o jogo da memória para não dar erro de áudio quando tentar iniciar de novo
            }
        }

        // --- DESENHO NO CANVAS VIRTUAL (1920x1080) ---
        BeginTextureMode(canvas);
            ClearBackground(BLACK);

            if (telaAtual == TELA_MENU)
            {
                if (menuTexture.width > 0)
                {
                    DrawTexturePro(
                        menuTexture,
                        (Rectangle){ 0, 0, (float)menuTexture.width, (float)menuTexture.height },
                        (Rectangle){ 0, 0, (float)virtualWidth, (float)virtualHeight },
                        (Vector2){ 0, 0 },
                        0.0f,
                        WHITE
                    );
                }
                else
                {
                    DrawRectangle(0, 0, virtualWidth, virtualHeight, DARKBLUE);
                    DrawText("MENU NAO ENCONTRADO", 120, 120, 40, RED);
                }

                DrawText("INICIAR", 60, 280, 28, WHITE);
                DrawText("DIFICULDADE", 60, 400, 28, WHITE);
                DrawText("RANKING", 60, 520, 28, WHITE);
                DrawText("SAIR", 60, 640, 28, WHITE);

                // Dificuldade sempre visível
                DrawText(TextFormat("DIFICULDADE: %s", TextoDificuldade(dificuldadeSelecionada)), 620, 900, 30, YELLOW);

                if (mostrarRanking)
                {
                    Ranking *rankingMenu = NULL;
                    CarregarRankingTXT(&rankingMenu);
                    DesenharRankingNaTela(rankingMenu, 1410, 220);
                    LiberarRanking(rankingMenu);
                }
            }
            else
            {
                DrawSalaDeAula();
            }
        EndTextureMode();

        // --- DESENHO FINAL NA TELA DO JOGADOR ---
        BeginDrawing();
            ClearBackground(BLACK); 

            float scale = MIN((float)GetScreenWidth() / virtualWidth, (float)GetScreenHeight() / virtualHeight);
            Rectangle areaOriginal = { 0.0f, 0.0f, (float)canvas.texture.width, -(float)canvas.texture.height };
            
            Rectangle areaDestino = {
                (GetScreenWidth() - (virtualWidth * scale)) * 0.5f,
                (GetScreenHeight() - (virtualHeight * scale)) * 0.5f,
                virtualWidth * scale,
                virtualHeight * scale
            };

            DrawTexturePro(canvas.texture, areaOriginal, areaDestino, (Vector2){ 0, 0 }, 0.0f, WHITE);

        EndDrawing();
    }

    // Limpeza da memória
    UnloadRenderTexture(canvas);
    UnloadTexture(menuTexture);
    
    // Descarrega apenas se o jogo fechou enquanto a fase estava rodando
    if (telaAtual == TELA_JOGO) {
        UnloadSalaDeAula();
    }
    
    CloseAudioDevice();
    CloseWindow();

    return 0;
}