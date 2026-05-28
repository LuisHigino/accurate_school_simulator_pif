#ifndef SUBMAIN_H
#define SUBMAIN_H

#include <stdbool.h>

void InitSubJogo(void);
void UpdateSubJogo(void);
void DrawSubJogo(void);
void UnloadSubJogo(void);
int GetDerrotasAbelha(void);
bool SubJogoFoiVencido(void);
bool SubJogoConsumirSolicitacaoRetornoMenu(void);

#endif