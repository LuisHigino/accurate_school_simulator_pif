#include "inimigo.h"

int cima;
int meio;
int baixo;


void InitInimigo(Inimigo *inimigo, Texture2D textura, int lane)
{
    inimigo->textura = textura;

    inimigo->x = 120;

    if (lane == 0)
        inimigo->y = cima;
    else if (lane == 1)
        inimigo->y = meio;
    else
        inimigo->y = baixo;

    inimigo->lane = lane;

    inimigo->frameAtual = 0;
    inimigo->timerAnimacao = 0;
}

void AtualizarInimigo(Inimigo *inimigo, float deltaTime)
{
    if (IsKeyPressed(KEY_UP))
    {
        if (inimigo->lane > 0)
            inimigo->lane--;
    }

    if (IsKeyPressed(KEY_DOWN))
    {
        if (inimigo->lane < 2)
            inimigo->lane++;
    }

    if (inimigo->lane == 0)
        inimigo->y = 250;
    else if (inimigo->lane == 1)
        inimigo->y = 400;
    else
        inimigo->y = 550;

    inimigo->timerAnimacao++;

    if (inimigo->timerAnimacao > 10)
    {
        inimigo->frameAtual++;
        inimigo->timerAnimacao = 0;

        if (inimigo->frameAtual > 2)
            inimigo->frameAtual = 0;
    }
}

void DesenharInimigo(Inimigo *inimigo)
{
    DrawTexturePro(
        inimigo->textura,

        (Rectangle){
            0,
            0,
            inimigo->textura.width,
            inimigo->textura.height
        },

        (Rectangle){
            inimigo->x,
            inimigo->y - 32,
            64,
            64
        },

        (Vector2){0, 0},

        0.0f,

        WHITE
    );
}

Rectangle GetHitboxInimigo(Inimigo *inimigo)
{
    Rectangle hitbox = {
        inimigo->x + 10,
        inimigo->y - 22,
        40,
        40
    };

    return hitbox;
}

void UnloadInimigo(Inimigo *inimigo)
{
    UnloadTexture(inimigo->textura);
}