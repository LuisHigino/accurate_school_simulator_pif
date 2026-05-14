#ifndef INIMIGO_H
#define INIMIGO_H

#include "raylib.h"

typedef struct {
    Texture2D textura;
    int x;
    int y;
    int lane;
    int frameAtual;
    int timerAnimacao;
    int meio;
    int cima;
    int baixo;
} Inimigo;

void InitInimigo(Inimigo *inimigo, Texture2D textura, int lane);

void AtualizarInimigo(Inimigo *inimigo, float deltaTime);

void DesenharInimigo(Inimigo *inimigo);

Rectangle GetHitboxInimigo(Inimigo *inimigo);

void UnloadInimigo(Inimigo *inimigo);

#endif