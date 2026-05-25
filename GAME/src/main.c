#include "raylib.h"
#include "sala_de_aula.h"

int main(void) {

    // Inicializa a janela real do monitor
    InitWindow(1920, 1080, "Jogo Principal - Sala de Aula");
    SetTargetFPS(60);

    // Inicializa o estado completo da sala de aula.
    // Aqui passamos a dificuldade da professora (1 = Fácil, 2 = Médio, 3 = Difícil)
    InitSalaDeAula(2); 

    while (!WindowShouldClose()) {
        // --- ATUALIZAÇÃO (UPDATE) ---
        // Passa o tempo do frame (deltaTime) para a sala atualizar a professora e os timers
        UpdateSalaDeAula(GetFrameTime());

        // --- DESENHO (DRAW) ---
        BeginDrawing();
            ClearBackground(BLACK);

            // Delega todo o desenho (background, professora, tablet e game over) para o módulo da sala
            DrawSalaDeAula();

        EndDrawing();
    }

    // Descarrega todas as texturas e subjogos da memória de uma só vez
    UnloadSalaDeAula();
    CloseWindow();

    return 0;
}