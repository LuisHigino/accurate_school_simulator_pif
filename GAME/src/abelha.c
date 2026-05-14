#include "abelha.h"

int cima;
int meio;
int baixo;


void InitAbelha(Abelha *abelha, Texture2D textura, int lane)
{
    abelha->textura = textura;

    abelha->x = 120;

    if (lane == 0)
        abelha->y = cima;
    else if (lane == 1)
        abelha->y = meio;
    else
        abelha->y = baixo;

    abelha->lane = lane;

    abelha->frameAtual = 0;
    abelha->timerAnimacao = 0;
}

void AtualizarAbelha(Abelha *abelha, float deltaTime)
{
    if (IsKeyPressed(KEY_UP))
    {
        if (abelha->lane > 0)
            abelha->lane--;
    }

    if (IsKeyPressed(KEY_DOWN))
    {
        if (abelha->lane < 2)
            abelha->lane++;
    }

    if (abelha->lane == 0)
        abelha->y = 250;
    else if (abelha->lane == 1)
        abelha->y = 400;
    else
        abelha->y = 550;

    abelha->timerAnimacao++;

    if (abelha->timerAnimacao > 10)
    {
        abelha->frameAtual++;
        abelha->timerAnimacao = 0;

        if (abelha->frameAtual > 2)
            abelha->frameAtual = 0;
    }
}

void DesenharAbelha(Abelha *abelha)
{
    DrawTexturePro(
        abelha->textura,

        (Rectangle){
            0,
            0,
            abelha->textura.width,
            abelha->textura.height
        },

        (Rectangle){
            abelha->x,
            abelha->y - 32,
            64,
            64
        },

        (Vector2){0, 0},

        0.0f,

        WHITE
    );
}

Rectangle GetHitboxAbelha(Abelha *abelha)
{
    Rectangle hitbox = {
        abelha->x + 10,
        abelha->y - 22,
        40,
        40
    };

    return hitbox;
}

void UnloadAbelha(Abelha *abelha)
{
    UnloadTexture(abelha->textura);
}