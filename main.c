#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Teste Raylib - Janela funcionando!");

    SetTargetFPS(60);

    int posX = screenWidth / 2;
    int posY = screenHeight / 2;

    while (!WindowShouldClose()) // Fecha com ESC ou botão X
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Se você está vendo isso, funcionou!", 180, 200, 20, DARKGRAY);
        DrawCircle(posX, posY, 40, BLUE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}