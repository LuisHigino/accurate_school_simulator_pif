#include "raylib.h"
#include "abelha.h"

int main(void)
{
    const int screenWidth = 1390;
    const int screenHeight = 797;
    const float tempoParaLinhaChegada = 60.0f;
    const float velocidadeLinhaChegada = 8.0f;
    const int limiteEsquerdoAbelha = 60;
    const int limiteDireitoAbelha = screenWidth - 80;

    InitWindow(screenWidth, screenHeight, "Gnomo VS Aliens");
    SetTargetFPS(60);

    bool jogoComecou = false;
    bool jogoVencido = false;
    bool linhaChegadaLiberada = false;

    float tempoDecorrido = 0.0f;
    float linhaChegadaX = screenWidth + 200;

    int meio = 400;
    int baixo = 550;
    int cima = 250;

    

    // =========================
    // BACKGROUND
    // =========================

    Texture2D background = LoadTexture("GAME/assets/images/background.png");
    SetTextureFilter(background, TEXTURE_FILTER_POINT);

    // =========================
    // ABELHA
    // =========================

    Texture2D texturaAbelha = LoadTexture("GAME/assets/images/gnomo_jetpack1.png");
    SetTextureFilter(texturaAbelha, TEXTURE_FILTER_POINT);

    Abelha abelha;
    InitAbelha(&abelha, texturaAbelha, 1);

    abelha.meio = meio;
    abelha.cima = cima;
    abelha.baixo = baixo;

    // =========================
    // INIMIGOS
    // =========================

    Texture2D et1 = LoadTexture("GAME/assets/images/et1.png");
    Texture2D et2 = LoadTexture("GAME/assets/images/et2.png");
    Texture2D et3 = LoadTexture("GAME/assets/images/et3.png");

    SetTextureFilter(et1, TEXTURE_FILTER_POINT);
    SetTextureFilter(et2, TEXTURE_FILTER_POINT);
    SetTextureFilter(et3, TEXTURE_FILTER_POINT);

    int frameEtAtual = 0;
    int timerEtAnimacao = 0;

    int inimigo1X = screenWidth + 100;
    int inimigo1Y = meio;

    int inimigo2X = screenWidth + 500;
    int inimigo2Y = baixo;

    int velocidadeInimigo = 5;

    while (!WindowShouldClose())
    {
        int teclaPressionada = GetKeyPressed();

        // =========================
        // COMEÇAR JOGO
        // =========================

        if (!jogoComecou && teclaPressionada != 0)
        {
            jogoComecou = true;
            jogoVencido = false;
            linhaChegadaLiberada = false;
            tempoDecorrido = 0.0f;
            linhaChegadaX = screenWidth + 200;

            abelha.x = 120;
            abelha.lane = 1;
            abelha.y = 400;

            inimigo1X = screenWidth + 100;
            inimigo1Y = meio;

            inimigo2X = screenWidth + 500;
            inimigo2Y = baixo;
        }

        // =========================
        // ABELHA
        // =========================

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

            AtualizarAbelha(&abelha, GetFrameTime());

            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
                abelha.x -= 7;

            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
                abelha.x += 7;

            if (abelha.x < limiteEsquerdoAbelha)
                abelha.x = limiteEsquerdoAbelha;

            if (abelha.x > limiteDireitoAbelha)
                abelha.x = limiteDireitoAbelha;
        }

        // =========================
        // INIMIGOS
        // =========================

        if (jogoComecou && !jogoVencido)
        {
            inimigo1X -= velocidadeInimigo;

            if (inimigo1X < -100)
            {
                inimigo1X = screenWidth + (GetRandomValue(20, 50) * 10);

                int faixa = GetRandomValue(0, 2);

                if (faixa == 0)
                    inimigo1Y = cima;
                else if (faixa == 1)
                    inimigo1Y = meio;
                else
                    inimigo1Y = baixo;
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

        if (jogoComecou && !jogoVencido &&
            (CheckCollisionRecs(GetHitboxAbelha(&abelha), inimigo1Rect) ||
             CheckCollisionRecs(GetHitboxAbelha(&abelha), inimigo2Rect)))
        {
            jogoComecou = false;
            jogoVencido = false;
            linhaChegadaLiberada = false;
            tempoDecorrido = 0.0f;
            linhaChegadaX = screenWidth + 200;

            inimigo1X = screenWidth + 100;
            inimigo1Y = meio;

            inimigo2X = screenWidth + 500;
            inimigo2Y = baixo;
        }

        // =========================
        // VITÓRIA
        // =========================

        if (jogoComecou && !jogoVencido && linhaChegadaLiberada && abelha.x >= linhaChegadaX - 20)
        {
            jogoVencido = true;
            jogoComecou = false;
        }

        // =========================
        // ANIMAÇÃO ET
        // =========================

        if (jogoComecou && !jogoVencido)
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

            DrawText(
                "SOBREVIVA 1 MINUTO PARA LIBERAR A LINHA DE CHEGADA",
                250,
                120,
                24,
                LIGHTGRAY
            );
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

            if (tempoRestante < 0.0f)
                tempoRestante = 0.0f;

            DrawText(TextFormat("TEMPO: %02d", (int)tempoRestante), 30, 30, 28, WHITE);
        }

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

        if (jogoVencido)
        {
            DrawRectangle(0, screenHeight / 2 - 60, screenWidth, 120, Fade(BLACK, 0.75f));
            DrawText("PARABENS VOCE VENCEU O JOGO", 360, screenHeight / 2 - 15, 30, YELLOW);
            DrawText("PRESSIONE QUALQUER TECLA PARA JOGAR NOVAMENTE", 280, screenHeight / 2 + 25, 22, WHITE);
        }

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