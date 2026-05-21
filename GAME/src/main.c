#include "raylib.h"
#include "submain.h"

int main(void) {

    InitWindow(1920, 1080, "Jogo Principal - Sala de Aula");
    
    RenderTexture2D telaSubJogo = LoadRenderTexture(1280, 720);
    

    InitSubJogo();

    Texture2D backgroundAula = LoadTexture("GAME/assets/images/background_sala_de_aula.png");
    
    Vector2 posicaoSubJogo = { 320, 320 };
    Vector2 posicaoBase = { 320, 320};
    Vector2 posicaoAbaixado = { 320, 985};
    bool TabletLevantado = true;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // --- ATUALIZAÇÃO (UPDATE) ---
        
        // Exemplo de interação: Se apertar espaço, move o subjogo para baixo
        if (IsKeyDown(KEY_SPACE)) {
            posicaoSubJogo.y = posicaoAbaixado.y;
            TabletLevantado = false;

        } else {
            posicaoSubJogo.y = posicaoBase.y;
            TabletLevantado = true;
        }
        

        

        UpdateSubJogo();

        // --- DESENHO (DRAW) ---
        
        // PASSO A: Desenhar o subjogo DENTRO da tela virtual
        BeginTextureMode(telaSubJogo);
            ClearBackground(BLACK);
            DrawSubJogo(); // Aqui dentro as coordenadas de 1280x720 funcionam perfeitamente!
        EndTextureMode();

        // PASSO B: Desenhar tudo na tela REAL do monitor
        BeginDrawing();
            ClearBackground(BLACK);

            DrawTexturePro(
                backgroundAula,
                (Rectangle){ 0, 0, (float)backgroundAula.width, (float)backgroundAula.height },
                (Rectangle){ 0, 0, 1920.0f, 1080.0f },
                (Vector2){ 0, 0 },
                0.0f,
                WHITE
            );

            
            Rectangle orig = { 0, 0, 1280.0f, -720.0f };
            Rectangle dest = { posicaoSubJogo.x, posicaoSubJogo.y, 1280.0f, 720.0f };
            Vector2 origin = { 0, 0 };
    
            DrawTexturePro(telaSubJogo.texture, orig, dest, origin, 0.0f, WHITE);

            
        EndDrawing();
    }

    // Descarrega as coisas da memória
    UnloadTexture(backgroundAula);
    UnloadSubJogo();
    UnloadRenderTexture(telaSubJogo);
    CloseWindow();

    return 0;
}