#ifndef ABELHA_H
#define ABELHA_H

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
} Abelha;

void InitAbelha(Abelha *abelha, Texture2D textura, int lane);

void AtualizarAbelha(Abelha *abelha, float deltaTime);

void DesenharAbelha(Abelha *abelha);

Rectangle GetHitboxAbelha(Abelha *abelha);

void UnloadAbelha(Abelha *abelha);

#endif