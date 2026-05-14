#include "raylib.h"
#include "abelha.h"
#include "constantes.h"


void InitAbelha(Abelha *abelha, Texture2D textura, int lane)
{
    abelha->textura = textura;

    abelha->x = 120;

    if (lane == 0)
        abelha->y = CIMA;
    else if (lane == 1)
        abelha->y = MEIO;
    else
        abelha->y = BAIXO;

    abelha->lane = lane;

    abelha->frameAtual = 0;
    abelha->timerAnimacao = 0;
}


void AtualizarAbelha(Abelha *abelha, float deltaTime, int screenWidth) {

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        if (abelha->lane > 0) {
            abelha->lane--;
        }
    }

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        if (abelha->lane < 2) {
            abelha->lane++;
        }
    }

    if (abelha->lane == 0) {
        abelha->y = CIMA;
    } else if (abelha->lane == 1) {
        abelha->y = MEIO;
    } else if (abelha->lane == 2) {
        abelha->y = BAIXO;
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