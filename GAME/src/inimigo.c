#include "inimigo.h"
#include "constantes.h"



int SortearFaixa(void) {
    int sorteio = GetRandomValue(0, 2);
    if (sorteio == 0) return CIMA;
    if (sorteio == 1) return MEIO;
    return BAIXO;
}

void InitInimigos(Inimigo inimigos[], int quantidadeAtiva, int screenWidth) {
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        if (i < quantidadeAtiva) {
            inimigos[i].ativo = true;
            // Posição inicial randômica para cada um não nascer grudado
            inimigos[i].x = screenWidth + (i * 300);
            inimigos[i].y = SortearFaixa();
            inimigos[i].velocidade = 10;
        } else {
            inimigos[i].ativo = false; // Deixa desativado os que sobrarem
        }
    }
}

void AtualizarInimigos(Inimigo inimigos[], int screenWidth) {
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        if (inimigos[i].ativo) {
            inimigos[i].x -= inimigos[i].velocidade;

            // Se o inimigo sair da tela pela esquerda, ele "renasce" na direita
            if (inimigos[i].x < -100) {
                inimigos[i].x = screenWidth + GetRandomValue(100, 500);
                inimigos[i].y = SortearFaixa();
            }
        }
    }
}

void DesenharInimigos(Inimigo inimigos[], Texture2D texturaFrame) {
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        if (inimigos[i].ativo) {
            
            // Trava de segurança do Inimigo/ET
            if (texturaFrame.width > 0) {
                DrawTexturePro(
                    texturaFrame,
                    (Rectangle){0, 0, texturaFrame.width, texturaFrame.height},
                    (Rectangle){inimigos[i].x - 32, inimigos[i].y - 32, 64, 64},
                    (Vector2){0, 0}, 0, WHITE
                );
            } else {
                // Se falhar, desenha o ET como um quadrado verde
                DrawRectangle(inimigos[i].x - 32, inimigos[i].y - 32, 64, 64, GREEN);
                DrawText("ET", inimigos[i].x, inimigos[i].y - 45, 10, WHITE);
            }
        }
    }
}

bool ChecarColisaoInimigos(Inimigo inimigos[], Rectangle hitboxAbelha) {
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        if (inimigos[i].ativo) {
            Rectangle inimigoHitbox = { inimigos[i].x - 20, inimigos[i].y - 20, 40, 40 };
            
            if (CheckCollisionRecs(hitboxAbelha, inimigoHitbox)) {
                return true; // Bateu em alguém!
            }
        }
    }
    return false; // Sobreviveu
}