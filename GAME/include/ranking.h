#ifndef RANKING_H
#define RANKING_H

typedef struct Ranking {
    char nome[50];
    int dados[2][2];
    struct Ranking *prox;
} Ranking;

void InserirRanking(Ranking **lista, char nome[], int tempoProfessora, int dificuldade, int derrotasAbelha);
void MostrarRanking(Ranking *lista);
void SalvarRankingTXT(Ranking *lista);
void CarregarRankingTXT(Ranking **lista);
void LiberarRanking(Ranking *lista);

#endif