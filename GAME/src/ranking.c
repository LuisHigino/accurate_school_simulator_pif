#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ranking.h"

// ==========================================
// FUNÇÃO JUIZ: DECIDE QUEM FICA ACIMA NO RANKING
// Retorna 1 se o "novo" for melhor que o "atual", e 0 caso contrário.
// ==========================================
static int DeveFicarNaFrente(Ranking *novo, Ranking *atual) {
    int difNovo = novo->dados[0][1];
    int difAtual = atual->dados[0][1];
    
    int tempoNovo = novo->dados[0][0];
    int tempoAtual = atual->dados[0][0];
    
    int mortesNovo = novo->dados[1][0];
    int mortesAtual = atual->dados[1][0];

    // 1º Critério: Dificuldade Maior
    if (difNovo > difAtual) return 1;
    if (difNovo < difAtual) return 0;

    // 2º Critério (Se a dificuldade for igual): Menor Tempo
    if (tempoNovo < tempoAtual) return 1;
    if (tempoNovo > tempoAtual) return 0;

    // 3º Critério (Se empatar no tempo também): Menos Mortes
    if (mortesNovo < mortesAtual) return 1;
    
    // Se empatar em TUDO, quem já estava lá (o atual) fica na frente
    return 0; 
}


// ==========================================
// INSERIR NO RANKING (Agora com Inserção Ordenada!)
// ==========================================

void InserirRanking(
    Ranking **lista,
    char nome[],
    int tempoProfessora, // Recebe em SEGUNDOS TOTAIS
    int dificuldade,     // 1 = Fácil, 2 = Médio, 3 = Difícil
    int derrotasAbelha
)
{
    Ranking *novo = malloc(sizeof(Ranking));

    if (novo == NULL) return;

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
    // INSERÇÃO INTELIGENTE NA LISTA
    // =========================
    
    // CASO 1: A lista está vazia OU o novo jogador já é melhor que o 1º colocado
    if (*lista == NULL || DeveFicarNaFrente(novo, *lista))
    {
        novo->prox = *lista;
        *lista = novo;
    }
    // CASO 2: O novo jogador não é o 1º, então vamos procurar o lugar dele
    else
    {
        Ranking *aux = *lista;
        
        // Vai pulando de jogador em jogador enquanto o próximo existir 
        // E o novo jogador NÃO for melhor que o próximo.
        while (aux->prox != NULL && !DeveFicarNaFrente(novo, aux->prox))
        {
            aux = aux->prox;
        }
        
        // Achou a posição! Encaixa o novo jogador entre o 'aux' e o 'aux->prox'
        novo->prox = aux->prox;
        aux->prox = novo;
    }
}

// ==========================================
// MOSTRAR RANKING (No Terminal)
// ==========================================

void MostrarRanking(Ranking *lista)
{
    Ranking *aux = lista;

    printf("\n===== RANKING =====\n\n");

    while (aux != NULL)
    {
        printf("Nome: %s\n", aux->nome);

        int tempoTotal = aux->dados[0][0];
        int minutos = tempoTotal / 60;
        int segundos = tempoTotal % 60;
        printf("Tempo Sobrevivendo: %d:%02d\n", minutos, segundos); 

        int dif = aux->dados[0][1];
        if (dif == 1) printf("Dificuldade: Fácil\n");
        else if (dif == 2) printf("Dificuldade: Médio\n");
        else if (dif == 3) printf("Dificuldade: Difícil\n");
        else printf("Dificuldade: Desconhecida\n");

        printf("Derrotas na Abelha: %d\n", aux->dados[1][0]);
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

    if (arquivo == NULL) return;

    Ranking *aux = lista;

    while (aux != NULL)
    {
        int tempoTotal = aux->dados[0][0];
        int dif = aux->dados[0][1];
        
        int minutos = tempoTotal / 60;
        int segundos = tempoTotal % 60;

        char strDif[20] = "Facil";
        if (dif == 2) strcpy(strDif, "Medio");
        else if (dif == 3) strcpy(strDif, "Dificil");

        fprintf(
            arquivo,
            "%s %s %d:%02d %d\n",
            aux->nome,
            strDif,
            minutos,
            segundos,
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

    if (arquivo == NULL) return;

    char nome[50];
    char strDif[20];
    int minutos;
    int segundos;
    int derrotas;

    while (fscanf(arquivo, "%s %s %d:%d %d", nome, strDif, &minutos, &segundos, &derrotas) != EOF)
    {
        int difInt = 1; 
        if (strcmp(strDif, "Medio") == 0) difInt = 2;
        else if (strcmp(strDif, "Dificil") == 0) difInt = 3;

        int tempoEmSegundos = (minutos * 60) + segundos;

        // Como nossa InserirRanking agora é inteligente, ela mesma vai 
        // organizar a galera conforme o TXT for sendo lido!
        InserirRanking(
            lista,
            nome,
            tempoEmSegundos,
            difInt,
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