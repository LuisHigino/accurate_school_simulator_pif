#ifndef PROF_H
#define PROF_H

#include "raylib.h"


typedef enum {
    PROFE_ESCREVENDO,
    PROFE_ALERTA,
    PROFE_OLHANDO
} EstadoProfessora;


typedef struct {
    EstadoProfessora estadoAtual;
    int dificuldade;
    float timerAcao;
    Vector2 posicao;
} Professora;


void InitProfessora(Professora *prof, int dificuldadeEscolhida, Vector2 posInicial);
void UpdateProfessora(Professora *prof, float deltaTime);
void DrawProfessora(Professora *prof);

#endif