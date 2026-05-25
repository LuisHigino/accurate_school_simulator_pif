#include "prof.h"

void InitProfessora(Professora *prof, int dificuldadeEscolhida, Vector2 posInicial) {
    prof->estadoAtual = PROFE_ESCREVENDO;
    prof->dificuldade = dificuldadeEscolhida;
    prof->timerAcao = 0.0f;
    prof->posicao = posInicial;
}

void UpdateProfessora(Professora *prof, float deltaTime) {

    prof->timerAcao += deltaTime;

    switch (prof->estadoAtual) {
        
        case PROFE_ESCREVENDO:

            if (prof->timerAcao >= 5.0f) {
                prof->timerAcao = 0.0f;

                int dado = GetRandomValue(0, 40);
                

                int chanceVira = prof->dificuldade * 10; 


                if (dado <= chanceVira) {
                    prof->estadoAtual = PROFE_ALERTA;
                }

            }
            break;

        case PROFE_ALERTA:

            if (prof->timerAcao >= 1.0f) {
                prof->timerAcao = 0.0f;
                prof->estadoAtual = PROFE_OLHANDO;
            }
            break;

        case PROFE_OLHANDO:

            if (prof->timerAcao >= 2.0f) {
                prof->timerAcao = 0.0f;
                prof->estadoAtual = PROFE_ESCREVENDO;
            }
            break;
    }
}

void DrawProfessora(Professora *prof) {

    Rectangle corpo = { prof->posicao.x, prof->posicao.y, 100, 250 };

    switch (prof->estadoAtual) {
        
        case PROFE_ESCREVENDO:
            // Azul Escuro = De costas, segura
            DrawRectangleRec(corpo, DARKBLUE);
            DrawText("ESCREVENDO...", prof->posicao.x - 20, prof->posicao.y - 30, 20, BLUE);
            break;

        case PROFE_ALERTA:
            // Amarelo = O "Tell" (Aviso), perigo iminente!
            DrawRectangleRec(corpo, YELLOW);
            DrawText("!?!?", prof->posicao.x + 25, prof->posicao.y - 50, 40, RED);
            break;

        case PROFE_OLHANDO:
            // Vermelho = Olhando para a turma, abaixe o tablet!
            DrawRectangleRec(corpo, RED);
            DrawText("VIGIANDO!", prof->posicao.x - 10, prof->posicao.y - 30, 20, MAROON);
            
            // Desenha dois olhinhos brancos só pra dar um ar de que ela está virada pra frente
            DrawCircle(prof->posicao.x + 30, prof->posicao.y + 40, 15, WHITE);
            DrawCircle(prof->posicao.x + 70, prof->posicao.y + 40, 15, WHITE);
            DrawCircle(prof->posicao.x + 30, prof->posicao.y + 40, 5, BLACK); // Pupila
            DrawCircle(prof->posicao.x + 70, prof->posicao.y + 40, 5, BLACK); // Pupila
            break;
    }
}