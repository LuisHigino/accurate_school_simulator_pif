#include "raylib.h"
#include "../GAME/include/abelha.h"

int main(void)
{
    const int screenWidth = 1390;
    const int screenHeight = 797;

    InitWindow(screenWidth, screenHeight, "Gnomo VS Aliens");
    SetTargetFPS(60);

    bool jogoComecou = false;

    int meio = 400;
    int baixo = 550;
    int cima = 250;

    // =========================
    // BACKGROUND
    // =========================

    Texture2D background = LoadTexture("background.png");
    SetTextureFilter(background, TEXTURE_FILTER_POINT);

    // =========================
    // ABELHA
    // =========================

    Texture2D texturaAbelha = LoadTexture("gnomo_jetpack1.png");
    SetTextureFilter(texturaAbelha, TEXTURE_FILTER_POINT);

    Abelha abelha;
    InitAbelha(&abelha, texturaAbelha, 1);

    // =========================
    // INIMIGOS
    // =========================

    Texture2D et1 = LoadTexture("et1.png");
    Texture2D et2 = LoadTexture("et2.png");
    Texture2D et3 = LoadTexture("et3.png");

    SetTextureFilter(et1, TEXTURE_FILTER_POINT);
    SetTextureFilter(et2, TEXTURE_FILTER_POINT);
    SetTextureFilter(et3, TEXTURE_FILTER_POINT);

    int frameEtAtual = 0;
    int timerEtAnimacao = 0;

    int inimigo1X = screenWidth + 100;
    int inimigo1Y = meio;

    int inimigo2X = screenWidth + 500;
    int inimigo2Y = baixo;

    int velocidadeInimigo = 15;

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
        // ABELHA
        // =========================

        if (jogoComecou)
        {
            AtualizarAbelha(&abelha, GetFrameTime());
        }

        // =========================
        // INIMIGOS
        // =========================

        if (jogoComecou)
        {
            inimigo1X -= velocidadeInimigo;
            inimigo2X -= velocidadeInimigo;

            if (inimigo1X < -100)
            {
                inimigo1X = screenWidth + GetRandomValue(200, 500);

                int faixa = GetRandomValue(0, 2);

                if (faixa == 0)
                    inimigo1Y = cima;
                else if (faixa == 1)
                    inimigo1Y = meio;
                else
                    inimigo1Y = baixo;
            }

            if (inimigo2X < -100)
            {
                inimigo2X = screenWidth + GetRandomValue(200, 500);

                int faixa = GetRandomValue(0, 2);

                if (faixa == 0)
                    inimigo2Y = cima;
                else if (faixa == 1)
                    inimigo2Y = meio;
                else
                    inimigo2Y = baixo;
            }
        }

        // =========================
        // HITBOX
        // =========================

        Rectangle inimigo1Rect = {
            inimigo1X - 20,
            inimigo1Y - 20,
            40,
            40
        };

        Rectangle inimigo2Rect = {
            inimigo2X - 20,
            inimigo2Y - 20,
            40,
            40
        };

        // =========================
        // COLISÃO
        // =========================

        if (CheckCollisionRecs(GetHitboxAbelha(&abelha), inimigo1Rect) ||
            CheckCollisionRecs(GetHitboxAbelha(&abelha), inimigo2Rect))
        {
            jogoComecou = false;

            inimigo1X = screenWidth + 100;
            inimigo1Y = meio;

            inimigo2X = screenWidth + 500;
            inimigo2Y = baixo;
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
                frameEtAtual = 0;
        }

        Texture2D frameEt;

        if (frameEtAtual == 0)
            frameEt = et1;
        else if (frameEtAtual == 1)
            frameEt = et2;
        else
            frameEt = et3;

        // =========================
        // DESENHAR
        // =========================

        BeginDrawing();

        ClearBackground(BLACK);

        DrawTexturePro(
            background,
            (Rectangle){0, 0, background.width, background.height},
            (Rectangle){0, 0, screenWidth, screenHeight},
            (Vector2){0, 0},
            0,
            WHITE
        );

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

        DesenharAbelha(&abelha);

        DrawTexturePro(
            frameEt,
            (Rectangle){0, 0, frameEt.width, frameEt.height},
            (Rectangle){inimigo1X - 32, inimigo1Y - 32, 64, 64},
            (Vector2){0, 0},
            0,
            WHITE
        );

        DrawTexturePro(
            frameEt,
            (Rectangle){0, 0, frameEt.width, frameEt.height},
            (Rectangle){inimigo2X - 32, inimigo2Y - 32, 64, 64},
            (Vector2){0, 0},
            0,
            WHITE
        );

        EndDrawing();
    }

    // =========================
    // LIBERAR
    // =========================

    UnloadAbelha(&abelha);

    UnloadTexture(background);

    UnloadTexture(et1);
    UnloadTexture(et2);
    UnloadTexture(et3);

    CloseWindow();

    return 0;
}