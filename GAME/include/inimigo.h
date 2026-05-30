#ifndef INIMIGO_H
#define INIMIGO_H

#include "raylib.h"
#include "constantes.h"

#define MAX_INIMIGOS 10

typedef struct {
    Texture2D textura;
    int x;
    int y;
    int velocidade;
    int cima;
    int meio;
    int baixo;
    bool ativo;
} Inimigo;

void InitInimigos(Inimigo inimigos[], int quantidadeAtiva, int screenWidth, int dificuldade);

void AtualizarInimigos(Inimigo inimigos[], int screenWidth);

void DesenharInimigos(Inimigo inimigos[], Texture2D texturaFrame);

bool ChecarColisaoInimigos(Inimigo inimigos[], Rectangle hitboxAbelha);

#endif