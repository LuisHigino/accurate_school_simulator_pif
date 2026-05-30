#include "raylib.h"
#include "sala_de_aula.h"
#include "submain.h"
#include "ranking.h"

#include <stddef.h>

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

typedef enum {
    TELA_MENU = 0,
    TELA_JOGO = 1
} TelaAtual;

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
    DrawRectangle(posX - 40, posY, 540, 520, Fade(BLACK, 0.78f));
    DrawRectangleLines(posX - 40, posY, 540, 520, GOLD);

    DrawText("RANKING AO VIVO", posX - 14, posY + 20, 34, YELLOW);
    
    DrawText("NOME      TEMPO      PERDAS      DIF", posX - 14, posY + 70, 22, LIGHTGRAY);
    int linha = 0;
    while (lista != NULL && linha < 8)
    {
        DrawText(TextFormat("%d. %s", linha + 1, lista->nome), posX - 14, posY + 110 + (linha * 48), 24, WHITE);
        
        int min = lista->dados[0][0] / 60;
        int seg = lista->dados[0][0] % 60;
        DrawText(TextFormat("%d:%02d", min, seg), posX + 150, posY + 110 + (linha * 48), 24, SKYBLUE);
        
        DrawText(TextFormat("%d", lista->dados[1][0]), posX + 275, posY + 110 + (linha * 48), 24, RED);
        
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
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);

    InitWindow(1280, 720, "Jogo Principal - Sala de Aula");
    InitAudioDevice();
    SetTargetFPS(60);

    int virtualWidth = 1920;
    int virtualHeight = 1080;
    RenderTexture2D canvas = LoadRenderTexture(virtualWidth, virtualHeight);
    Texture2D menuTexture = LoadTexture("GAME/assets/images/Mainscreen.png");
    SetTextureFilter(menuTexture, TEXTURE_FILTER_POINT);

    Sound somMover = LoadSound("GAME/assets/sounds/move.mp3");
    Sound somConfirmar = LoadSound("GAME/assets/sounds/enter.mp3");


    TelaAtual telaAtual = TELA_MENU;
    int dificuldadeSelecionada = 1;
    bool mostrarRanking = false;
    bool sairDoJogo = false;
    
    int opcaoSelecionada = 0; 

    while (!WindowShouldClose() && !sairDoJogo) {
        
        if (telaAtual == TELA_MENU)
        {
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                opcaoSelecionada++;
                if (opcaoSelecionada > 3) opcaoSelecionada = 0;
                PlaySound(somMover);
            }
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                opcaoSelecionada--;
                if (opcaoSelecionada < 0) opcaoSelecionada = 3;
                PlaySound(somMover);
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                PlaySound(somConfirmar);

                if (opcaoSelecionada == 0)
                {
                    InitSalaDeAula(dificuldadeSelecionada);
                    telaAtual = TELA_JOGO;
                    mostrarRanking = false;
                }
                else if (opcaoSelecionada == 1)
                {
                    dificuldadeSelecionada++;
                    if (dificuldadeSelecionada > 3) dificuldadeSelecionada = 1;
                }
                else if (opcaoSelecionada == 2)
                {
                    mostrarRanking = !mostrarRanking;
                }
                else if (opcaoSelecionada == 3)
                {
                    sairDoJogo = true;
                }
            }
        }
        else
        {
            UpdateSalaDeAula(GetFrameTime());

            if (SubJogoConsumirSolicitacaoRetornoMenu())
            {
                PlaySound(somConfirmar);
                
                telaAtual = TELA_MENU;
                mostrarRanking = false;
                UnloadSalaDeAula();
            }
        }

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

                DrawText(opcaoSelecionada == 0 ? "> INICIAR <" : "INICIAR", 60, 280, 28, opcaoSelecionada == 0 ? YELLOW : WHITE);
                DrawText(opcaoSelecionada == 1 ? "> DIFICULDADE <" : "DIFICULDADE", 60, 400, 28, opcaoSelecionada == 1 ? YELLOW : WHITE);
                DrawText(opcaoSelecionada == 2 ? "> RANKING <" : "RANKING", 60, 520, 28, opcaoSelecionada == 2 ? YELLOW : WHITE);
                DrawText(opcaoSelecionada == 3 ? "> SAIR <" : "SAIR", 60, 640, 28, opcaoSelecionada == 3 ? YELLOW : WHITE);

                DrawText(TextFormat("%s", TextoDificuldade(dificuldadeSelecionada)), 300, 400, 28, YELLOW);

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

    UnloadRenderTexture(canvas);
    UnloadTexture(menuTexture);
    
    UnloadSound(somMover);
    UnloadSound(somConfirmar);

    if (telaAtual == TELA_JOGO) {
        UnloadSalaDeAula();
    }
    
    CloseAudioDevice();
    CloseWindow();

    return 0;
}