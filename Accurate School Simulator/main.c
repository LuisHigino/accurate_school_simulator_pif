#include "raylib.h"

int main(void)
{
    const int screenWidth = 1390;
    const int screenHeight = 797;

    InitWindow(screenWidth, screenHeight, "Gnomo VS Aliens");

    SetTargetFPS(60);

    // =========================
    // FAIXAS
    // =========================

    int cima = 250;
    int meio = 400;
    int baixo = 550;

    // =========================
    // PLAYER
    // =========================

    int faixa = 1;

    int playerX = 120;
    int playerY = meio;

    // =========================
    // JOGO
    // =========================

    bool jogoComecou = false;

    // =========================
    // BACKGROUND
    // =========================

    Texture2D background = LoadTexture("background.jpg");

    SetTextureFilter(background, TEXTURE_FILTER_POINT);

    // =========================
    // INIMIGOS
    // =========================

    int inimigo1X = screenWidth + 100;
    int inimigo1Y = meio;

    int inimigo2X = screenWidth + 500;
    int inimigo2Y = baixo;

    int velocidadeInimigo = 15;

    // =========================
    // GNOMO
    // =========================

    Texture2D gnomo1 = LoadTexture("gnomo_jetpack1.png");
    Texture2D gnomo2 = LoadTexture("gnomo_jetpack2.png");
    Texture2D gnomo3 = LoadTexture("gnomo_jetpack3.png");

    SetTextureFilter(gnomo1, TEXTURE_FILTER_POINT);
    SetTextureFilter(gnomo2, TEXTURE_FILTER_POINT);
    SetTextureFilter(gnomo3, TEXTURE_FILTER_POINT);

    int frameAtual = 0;
    int timerAnimacao = 0;

    // =========================
    // ETS
    // =========================

    Texture2D et1 = LoadTexture("et1.png");
    Texture2D et2 = LoadTexture("et2.png");
    Texture2D et3 = LoadTexture("et3.png");

    SetTextureFilter(et1, TEXTURE_FILTER_POINT);
    SetTextureFilter(et2, TEXTURE_FILTER_POINT);
    SetTextureFilter(et3, TEXTURE_FILTER_POINT);

    int frameEtAtual = 0;
    int timerEtAnimacao = 0;

    while (!WindowShouldClose())
    {
        // =========================
        // COMEÇAR JOGO
        // =========================

        if (!jogoComecou && GetKeyPressed() != 0)
        {
            jogoComecou = true;
        }

        // =========================
        // MOVIMENTO PLAYER
        // =========================

        if (jogoComecou)
        {
            if (IsKeyPressed(KEY_UP))
            {
                if (faixa > 0)
                {
                    faixa--;
                }
            }

            if (IsKeyPressed(KEY_DOWN))
            {
                if (faixa < 2)
                {
                    faixa++;
                }
            }
        }

        // =========================
        // POSIÇÃO PLAYER
        // =========================

        if (faixa == 0)
        {
            playerY = cima;
        }
        else if (faixa == 1)
        {
            playerY = meio;
        }
        else
        {
            playerY = baixo;
        }

        // =========================
        // MOVIMENTO INIMIGOS
        // =========================

        if (jogoComecou)
        {
            inimigo1X -= velocidadeInimigo;
            inimigo2X -= velocidadeInimigo;

            // INIMIGO 1
            if (inimigo1X < -100)
            {
                inimigo1X = screenWidth + GetRandomValue(200, 500);

                int faixaInimigo = GetRandomValue(0, 2);

                if (faixaInimigo == 0)
                {
                    inimigo1Y = cima;
                }
                else if (faixaInimigo == 1)
                {
                    inimigo1Y = meio;
                }
                else
                {
                    inimigo1Y = baixo;
                }
            }

            // INIMIGO 2
            if (inimigo2X < -100)
            {
                inimigo2X = screenWidth + GetRandomValue(200, 500);

                int faixaInimigo = GetRandomValue(0, 2);

                if (faixaInimigo == 0)
                {
                    inimigo2Y = cima;
                }
                else if (faixaInimigo == 1)
                {
                    inimigo2Y = meio;
                }
                else
                {
                    inimigo2Y = baixo;
                }
            }
        }

        // =========================
        // HITBOX PLAYER
        // =========================

        Rectangle playerRect = {
            playerX + 10,
            playerY - 22,
            40,
            40
        };

        // =========================
        // HITBOX INIMIGO 1
        // =========================

        Rectangle inimigo1Rect = {
            inimigo1X - 20,
            inimigo1Y - 20,
            40,
            40
        };

        // =========================
        // HITBOX INIMIGO 2
        // =========================

        Rectangle inimigo2Rect = {
            inimigo2X - 20,
            inimigo2Y - 20,
            40,
            40
        };

        // =========================
        // COLISÃO
        // =========================

        if (CheckCollisionRecs(playerRect, inimigo1Rect) ||
            CheckCollisionRecs(playerRect, inimigo2Rect))
        {
            jogoComecou = false;

            faixa = 1;

            inimigo1X = screenWidth + 100;
            inimigo1Y = meio;

            inimigo2X = screenWidth + 500;
            inimigo2Y = baixo;
        }

        // =========================
        // ANIMAÇÃO GNOMO
        // =========================

        timerAnimacao++;

        if (timerAnimacao > 10)
        {
            frameAtual++;
            timerAnimacao = 0;

            if (frameAtual > 2)
            {
                frameAtual = 0;
            }
        }

        // =========================
        // ANIMAÇÃO ET
        // =========================

        timerEtAnimacao++;

        if (timerEtAnimacao > 10)
        {
            frameEtAtual++;
            timerEtAnimacao = 0;

            if (frameEtAtual > 2)
            {
                frameEtAtual = 0;
            }
        }

        // =========================
        // DESENHAR
        // =========================

        BeginDrawing();

        // LIMPA A TELA
        ClearBackground(BLACK);

        // FUNDO
    DrawTexturePro(
        background,

        (Rectangle){
            0,
            0,
            (float)background.width,
            (float)background.height
        },

        (Rectangle){
            0,
            0,
            (float)screenWidth,
            (float)screenHeight
        },

        (Vector2){0, 0},

        0.0f,

        WHITE
    );

        // TEXTO
        if (!jogoComecou)
        {
            DrawText(
                "APERTE QUALQUER TECLA PARA COMEÇAR",
                320,
                80,
                30,
                WHITE
            );
        }

        // =========================
        // FRAME GNOMO
        // =========================

        Texture2D frameAtualTextura;

        if (frameAtual == 0)
        {
            frameAtualTextura = gnomo1;
        }
        else if (frameAtual == 1)
        {
            frameAtualTextura = gnomo2;
        }
        else
        {
            frameAtualTextura = gnomo3;
        }

        // DESENHAR GNOMO
        DrawTexturePro(
            frameAtualTextura,

            (Rectangle){
                0,
                0,
                frameAtualTextura.width,
                frameAtualTextura.height
            },

            (Rectangle){
                playerX,
                playerY - 32,
                64,
                64
            },

            (Vector2){0, 0},

            0.0f,
            WHITE
        );

        // =========================
        // FRAME ET
        // =========================

        Texture2D frameEtTextura;

        if (frameEtAtual == 0)
        {
            frameEtTextura = et1;
        }
        else if (frameEtAtual == 1)
        {
            frameEtTextura = et2;
        }
        else
        {
            frameEtTextura = et3;
        }

        // ET 1
        DrawTexturePro(
            frameEtTextura,

            (Rectangle){
                0,
                0,
                frameEtTextura.width,
                frameEtTextura.height
            },

            (Rectangle){
                inimigo1X - 32,
                inimigo1Y - 32,
                64,
                64
            },

            (Vector2){0, 0},

            0.0f,
            WHITE
        );

        // ET 2
        DrawTexturePro(
            frameEtTextura,

            (Rectangle){
                0,
                0,
                frameEtTextura.width,
                frameEtTextura.height
            },

            (Rectangle){
                inimigo2X - 32,
                inimigo2Y - 32,
                64,
                64
            },

            (Vector2){0, 0},

            0.0f,
            WHITE
        );

        EndDrawing();
    }

    // =========================
    // LIBERAR TEXTURAS
    // =========================

    UnloadTexture(background);

    UnloadTexture(gnomo1);
    UnloadTexture(gnomo2);
    UnloadTexture(gnomo3);

    UnloadTexture(et1);
    UnloadTexture(et2);
    UnloadTexture(et3);

    CloseWindow();

    return 0;
}