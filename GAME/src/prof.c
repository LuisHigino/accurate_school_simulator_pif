#include "prof.h"

void InitProfessora(Professora *prof, int dificuldadeEscolhida, Vector2 posInicial) {
    prof->estadoAtual = PROFE_ESCREVENDO;
    prof->dificuldade = dificuldadeEscolhida;
    prof->timerAcao = 0.0f;
    prof->timerAnimacao = 0.0f;
    prof->idleFrame = 0;
    prof->posicao = posInicial;
    prof->idleTexture1 = LoadTexture("GAME/assets/images/professora_idle1.png");
    prof->idleTexture2 = LoadTexture("GAME/assets/images/professora_idle2.png");
    prof->alertaTexture = LoadTexture("GAME/assets/images/professora_olhando1.png");
    prof->olhandoTexture = LoadTexture("GAME/assets/images/professora_olhando2.png");
}

void UpdateProfessora(Professora *prof, float deltaTime) {

    prof->timerAcao += deltaTime;
    prof->timerAnimacao += deltaTime;

    if (prof->timerAnimacao >= 0.4f) {
        prof->timerAnimacao = 0.0f;
        prof->idleFrame = 1 - prof->idleFrame;
    }

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
    Texture2D textureToDraw = prof->alertaTexture;

    switch (prof->estadoAtual) {
        case PROFE_ESCREVENDO:
            textureToDraw = (prof->idleFrame == 0) ? prof->idleTexture1 : prof->idleTexture2;
            break;
        case PROFE_ALERTA:
            textureToDraw = prof->alertaTexture;
            break;
        case PROFE_OLHANDO:
            textureToDraw = prof->olhandoTexture;
            break;
    }

    if (textureToDraw.width > 0) {
        float scale = 12.0f;
        DrawTexturePro(
            textureToDraw,
            (Rectangle){ 0.0f, 0.0f, (float)textureToDraw.width, (float)textureToDraw.height },
            (Rectangle){ prof->posicao.x, prof->posicao.y, (float)textureToDraw.width * scale, (float)textureToDraw.height * scale },
            (Vector2){ 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    } else {
        Rectangle corpo = { prof->posicao.x, prof->posicao.y, 100, 250 };

        switch (prof->estadoAtual) {
            case PROFE_ESCREVENDO:
                DrawRectangleRec(corpo, DARKBLUE);
                DrawText("ESCREVENDO...", prof->posicao.x - 20, prof->posicao.y - 30, 20, BLUE);
                break;
            case PROFE_ALERTA:
                DrawRectangleRec(corpo, YELLOW);
                DrawText("!?!?", prof->posicao.x + 25, prof->posicao.y - 50, 40, RED);
                break;
            case PROFE_OLHANDO:
                DrawRectangleRec(corpo, RED);
                DrawText("VIGIANDO!", prof->posicao.x - 10, prof->posicao.y - 30, 20, MAROON);
                DrawCircle(prof->posicao.x + 30, prof->posicao.y + 40, 15, WHITE);
                DrawCircle(prof->posicao.x + 70, prof->posicao.y + 40, 15, WHITE);
                DrawCircle(prof->posicao.x + 30, prof->posicao.y + 40, 5, BLACK);
                DrawCircle(prof->posicao.x + 70, prof->posicao.y + 40, 5, BLACK);
                break;
        }
    }
}

void UnloadProfessora(Professora *prof) {
    UnloadTexture(prof->idleTexture1);
    UnloadTexture(prof->idleTexture2);
    UnloadTexture(prof->alertaTexture);
    UnloadTexture(prof->olhandoTexture);
}
