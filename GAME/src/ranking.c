#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ranking.h"

static int DeveFicarNaFrente(Ranking *novo, Ranking *atual) {
    int difNovo = novo->dados[0][1], difAtual = atual->dados[0][1];
    int tempoNovo = novo->dados[0][0], tempoAtual = atual->dados[0][0];
    int mortesNovo = novo->dados[1][0], mortesAtual = atual->dados[1][0];

    if (difNovo != difAtual) return difNovo > difAtual;
    if (tempoNovo != tempoAtual) return tempoNovo < tempoAtual;
    return mortesNovo < mortesAtual;
}

void InserirRanking(
    Ranking **lista,
    char nome[],
    int tempoProfessora,
    int dificuldade,
    int derrotasAbelha
)
{
    Ranking *novo = malloc(sizeof *novo);
    if (!novo) return;

    strcpy(novo->nome, nome);

    novo->dados[0][0] = tempoProfessora;
    novo->dados[0][1] = dificuldade;
    novo->dados[1][0] = derrotasAbelha;
    novo->dados[1][1] = 0;

    novo->prox = NULL;

    if (*lista == NULL || DeveFicarNaFrente(novo, *lista)) {
        novo->prox = *lista;
        *lista = novo;
    } else {
        Ranking *aux = *lista;
        while (aux->prox != NULL && !DeveFicarNaFrente(novo, aux->prox)) {
            aux = aux->prox;
        }
        novo->prox = aux->prox;
        aux->prox = novo;
    }
}

static const char *TextoDificuldade(int dif) {
    if (dif == 1) return "Fácil";
    if (dif == 2) return "Médio";
    if (dif == 3) return "Difícil";
    return "Desconhecida";
}

void MostrarRanking(Ranking *lista)
{
    printf("\n===== RANKING =====\n\n");
    while (lista != NULL) {
        int tempoTotal = lista->dados[0][0];
        printf("Nome: %s\nTempo Sobrevivendo: %d:%02d\nDificuldade: %s\nDerrotas na Abelha: %d\n-------------------------\n",
               lista->nome, tempoTotal / 60, tempoTotal % 60, TextoDificuldade(lista->dados[0][1]), lista->dados[1][0]);
        lista = lista->prox;
    }
}

void SalvarRankingTXT(Ranking *lista)
{
    FILE *arquivo = fopen("ranking.txt", "w"); 
    if (!arquivo) return;

    while (lista != NULL) {
        int tempoTotal = lista->dados[0][0];
        const char *strDif = "Facil";

        if (lista->dados[0][1] == 2) strDif = "Medio";
        else if (lista->dados[0][1] == 3) strDif = "Dificil";

        fprintf(arquivo, "%s %s %d:%02d %d\n", lista->nome, strDif, tempoTotal / 60, tempoTotal % 60, lista->dados[1][0]);
        lista = lista->prox;
    }

    fclose(arquivo);
}

void CarregarRankingTXT(Ranking **lista)
{
    FILE *arquivo = fopen("ranking.txt", "r");
    if (!arquivo) return;

    char nome[50];
    char strDif[20];
    int minutos;
    int segundos;
    int derrotas;

    while (fscanf(arquivo, "%s %s %d:%d %d", nome, strDif, &minutos, &segundos, &derrotas) != EOF) {
        int difInt = 1;
        if (strcmp(strDif, "Medio") == 0) difInt = 2;
        else if (strcmp(strDif, "Dificil") == 0) difInt = 3;

        InserirRanking(lista, nome, (minutos * 60) + segundos, difInt, derrotas);
    }

    fclose(arquivo);
}

void LiberarRanking(Ranking *lista)
{
    while (lista != NULL) {
        Ranking *aux = lista;
        lista = lista->prox;
        free(aux);
    }
}