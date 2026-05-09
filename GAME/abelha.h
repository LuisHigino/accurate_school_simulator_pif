#ifndef ABELHA_H
#define ABELHA_H

#include "raylib.h"

typedef struct Abelha{
    Texture2D textura;
    int lane;
    float x;
    float y;
    int ativa;
}Abelha;

void InicializarAbelha(Abelha *abelha, Texture2D textura, int lane);

void atualizarAbelha(Abelha *abelha, float deltaTime);

void desenharAbelha(Abelha *abelha);

void destruirAbelha(Abelha *abelha);

#endif