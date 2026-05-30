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

    float tempoEscrevendo = 5.0f;
    float tempoOlhando = 1.5f;

    if (prof->dificuldade == 1) {
        tempoEscrevendo = 6.0f;
        tempoOlhando = 1.0f;
    } 
    else if (prof->dificuldade == 2) {
        tempoEscrevendo = 5.0f;
        tempoOlhando = 1.5f;
    } 
    else if (prof->dificuldade == 3) {
        tempoEscrevendo = 4.5f;
        tempoOlhando = 2.0f;
    }

    switch (prof->estadoAtual) {
        
        case PROFE_ESCREVENDO:
            if (prof->timerAcao >= tempoEscrevendo) {
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
            if (prof->timerAcao >= tempoOlhando) {
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

    float scale = 12.0f;
    DrawTexturePro(
        textureToDraw,
        (Rectangle){ 0.0f, 0.0f, (float)textureToDraw.width, (float)textureToDraw.height },
        (Rectangle){ prof->posicao.x, prof->posicao.y, (float)textureToDraw.width * scale, (float)textureToDraw.height * scale },
        (Vector2){ 0.0f, 0.0f },
        0.0f,
        WHITE
    );
}

void UnloadProfessora(Professora *prof) {
    UnloadTexture(prof->idleTexture1);
    UnloadTexture(prof->idleTexture2);
    UnloadTexture(prof->alertaTexture);
    UnloadTexture(prof->olhandoTexture);
}