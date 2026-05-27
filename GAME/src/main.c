#include "raylib.h"
#include "sala_de_aula.h"

// Funções matemáticas simples para descobrirmos o tamanho do monitor
#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

int main(void) {
    // 1. Avisa a Raylib que a janela pode ser redimensionada e deve abrir maximizada
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);

    // 2. Inicia a janela com um tamanho seguro. Como mandamos maximizar, ela vai se
    //    adaptar sozinha à resolução nativa do monitor de quem abrir o jogo.
    InitWindow(1280, 720, "Jogo Principal - Sala de Aula");
    SetTargetFPS(60);

    // 3. A MÁGICA: O "Canvas" Virtual (O jogo acha que é 1080p para sempre)
    int virtualWidth = 1920;
    int virtualHeight = 1080;
    RenderTexture2D canvas = LoadRenderTexture(virtualWidth, virtualHeight);

    // Inicializa a sala de aula normalmente
    InitSalaDeAula(2); 

    while (!WindowShouldClose()) {
        // --- ATUALIZAÇÃO (UPDATE) ---
        UpdateSalaDeAula(GetFrameTime());

        // --- DESENHO NO CANVAS VIRTUAL (1920x1080) ---
        // Aqui o jogo roda intocado, com as mesmas coordenadas de sempre
        BeginTextureMode(canvas);
            ClearBackground(BLACK);
            DrawSalaDeAula();
        EndTextureMode();

        // --- DESENHO FINAL NA TELA DO JOGADOR ---
        BeginDrawing();
            // Fundo preto para gerar "barras pretas" caso o monitor seja de proporção diferente
            ClearBackground(BLACK); 

            // Calcula o multiplicador exato para espremer ou esticar a imagem
            float scale = MIN((float)GetScreenWidth() / virtualWidth, (float)GetScreenHeight() / virtualHeight);

            // A Raylib desenha texturas virtuais de ponta-cabeça por padrão.
            // Passar a altura negativa (-canvas.texture.height) desvira ela.
            Rectangle areaOriginal = { 0.0f, 0.0f, (float)canvas.texture.width, -(float)canvas.texture.height };
            
            // Calcula o meio da tela para o jogo ficar centralizado
            Rectangle areaDestino = {
                (GetScreenWidth() - (virtualWidth * scale)) * 0.5f,
                (GetScreenHeight() - (virtualHeight * scale)) * 0.5f,
                virtualWidth * scale,
                virtualHeight * scale
            };

            // Desenha a "fita de vídeo" do jogo na tela do usuário
            DrawTexturePro(canvas.texture, areaOriginal, areaDestino, (Vector2){ 0, 0 }, 0.0f, WHITE);

        EndDrawing();
    }

    // Limpeza da memória
    UnloadRenderTexture(canvas);
    UnloadSalaDeAula();
    CloseWindow();

    return 0;
}