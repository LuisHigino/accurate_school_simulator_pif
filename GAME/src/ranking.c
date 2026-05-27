#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ranking.h"

// ==========================================
// INSERIR NO RANKING
// ==========================================

void InserirRanking(
    Ranking **lista,
    char nome[],
    int tempoProfessora,
    int dificuldade,
    int derrotasAbelha
)
{
    Ranking *novo = malloc(sizeof(Ranking));

    if (novo == NULL)
    {
        return;
    }

    strcpy(novo->nome, nome);

    // =========================
    // MATRIZ DE ESTATÍSTICAS
    // =========================

    novo->dados[0][0] = tempoProfessora;
    novo->dados[0][1] = dificuldade;

    novo->dados[1][0] = derrotasAbelha;
    novo->dados[1][1] = 0;

    novo->prox = NULL;

    // =========================
    // INSERÇÃO NA LISTA
    // =========================

    if (*lista == NULL)
    {
        *lista = novo;
    }
    else
    {
        Ranking *aux = *lista;

        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }

        aux->prox = novo;
    }
}

// ==========================================
// MOSTRAR RANKING
// ==========================================

void MostrarRanking(Ranking *lista)
{
    Ranking *aux = lista;

    printf("\n===== RANKING =====\n\n");

    while (aux != NULL)
    {
        printf("Nome: %s\n", aux->nome);

        printf(
            "Tempo Professora: %d\n",
            aux->dados[0][0]
        );

        printf(
            "Dificuldade: %d\n",
            aux->dados[0][1]
        );

        printf(
            "Derrotas Abelha: %d\n",
            aux->dados[1][0]
        );

        printf("-------------------------\n");

        aux = aux->prox;
    }
}

// ==========================================
// SALVAR TXT
// ==========================================

void SalvarRankingTXT(Ranking *lista)
{
    FILE *arquivo = fopen("ranking.txt", "w");

    if (arquivo == NULL)
    {
        return;
    }

    Ranking *aux = lista;

    while (aux != NULL)
    {
        fprintf(
            arquivo,
            "%s %d %d %d\n",

            aux->nome,

            aux->dados[0][0],
            aux->dados[0][1],

            aux->dados[1][0]
        );

        aux = aux->prox;
    }

    fclose(arquivo);
}

// ==========================================
// CARREGAR TXT
// ==========================================

void CarregarRankingTXT(Ranking **lista)
{
    FILE *arquivo = fopen("ranking.txt", "r");

    if (arquivo == NULL)
    {
        return;
    }

    char nome[50];

    int tempo;
    int dificuldade;
    int derrotas;

    while (
        fscanf(
            arquivo,
            "%s %d %d %d",

            nome,
            &tempo,
            &dificuldade,
            &derrotas
        ) != EOF
    )
    {
        InserirRanking(
            lista,
            nome,
            tempo,
            dificuldade,
            derrotas
        );
    }

    fclose(arquivo);
}

// ==========================================
// LIBERAR MEMÓRIA
// ==========================================

void LiberarRanking(Ranking *lista)
{
    Ranking *aux;

    while (lista != NULL)
    {
        aux = lista;

        lista = lista->prox;

        free(aux);
    }
}