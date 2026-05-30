#ifndef SALA_DE_AULA_H
#define SALA_DE_AULA_H

#include "raylib.h"

void InitSalaDeAula(int dificuldadeProfessora);
void UpdateSalaDeAula(float deltaTime);
void DrawSalaDeAula(void);
void UnloadSalaDeAula(void);
int GetTempoSobrevivencia(void);
int GetDificuldadeSala(void);

#endif