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
    float timerAnimacao;
    int idleFrame;
    Vector2 posicao;
    Texture2D idleTexture1;
    Texture2D idleTexture2;
    Texture2D alertaTexture;
    Texture2D olhandoTexture;
} Professora;

void InitProfessora(Professora *prof, int dificuldadeEscolhida, Vector2 posInicial);
void UpdateProfessora(Professora *prof, float deltaTime);
void DrawProfessora(Professora *prof);
void UnloadProfessora(Professora *prof);

#endif