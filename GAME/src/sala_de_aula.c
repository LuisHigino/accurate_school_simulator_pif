#include "sala_de_aula.h"
#include "prof.h"
#include "submain.h"
#include "raylib.h" // Garante o acesso às funções de áudio da Raylib

// Variáveis internas (visíveis apenas dentro deste arquivo)
static Texture2D backgroundAula;
static Texture2D tabletTexture;
static Texture2D jumpscareTexture1;
static Texture2D jumpscareTexture2;
static Texture2D alunoIdleTexture1;
static Texture2D alunoIdleTexture2;
static Texture2D alunoIdleTexture3;
static Professora professora;
static RenderTexture2D telaSubJogo;
static float gameOverJumpscareTimer = 0.0f;
static bool gameOverJumpscareFrame = false;
static float alunoIdleTimer = 0.0f;
static int alunoIdleFrame = 0;

// VARIÁVEIS DE ÁUDIO
static Sound somAbelha;
static Sound somEscola;
static Sound somGiz;
static Sound somJumpscare;
static Sound somLapis;
static Sound somMorte;
static Sound somVitoria;
static Sound somVoo;

// Controladores de estado para evitar que o som fique triggando em loop infinito
static bool somMorteTocado = false;
static bool somVitoriaTocado = false;
static bool somJumpscareTocado = false;

// AJEITAR POSIÇÃO DO TABLET
static Vector2 posicaoSubJogo = { 320, 320 };
static Vector2 posicaoBase = { 320, 320 };
static Vector2 posicaoAbaixado = { 320, 985 };
static Vector2 posicaoTablet = { 10, 30 }; 
static bool tabletLevantado = true;
static bool gameOverPrincipal = false;

void InitSalaDeAula(int dificuldadeProfessora) {
    // Inicializa o dispositivo de áudio (caso ainda não tenha sido inicializado na main principal)
    if (!IsAudioDeviceReady()) {
        InitAudioDevice();
    }

    // 1. Carrega o fundo da sala
    backgroundAula = LoadTexture("GAME/assets/images/backgroundAula.png");
    tabletTexture = LoadTexture("GAME/assets/images/Tablet.png");
    jumpscareTexture1 = LoadTexture("GAME/assets/images/professora_jumpscare1.png");
    jumpscareTexture2 = LoadTexture("GAME/assets/images/professora_jumpscare2.png");
    alunoIdleTexture1 = LoadTexture("GAME/assets/images/aluno_idle1.png");
    alunoIdleTexture2 = LoadTexture("GAME/assets/images/aluno_idle2.png");
    alunoIdleTexture3 = LoadTexture("GAME/assets/images/aluno_idle3.png");
    
    // 2. Carrega os Sons (Ajuste o caminho das pastas se necessário)
    somAbelha = LoadSound("GAME/assets/images/som_abelha.mp3");
    somEscola = LoadSound("GAME/assets/images/som_escola.mp3");
    somGiz = LoadSound("GAME/assets/images/som_giz.mp3");
    somJumpscare = LoadSound("GAME/assets/images/som_jumpscare.mp3");
    somLapis = LoadSound("GAME/assets/images/som_lapis.mp3");
    somMorte = LoadSound("GAME/assets/images/som_morte.mp3");
    somVitoria = LoadSound("GAME/assets/images/som_vitoria.mp3");
    somVoo = LoadSound("GAME/assets/images/som_voo.mp3");

    // Ajuste de volumes iniciais baseados na sua descrição
    SetSoundVolume(somEscola, 0.6f);
    SetSoundVolume(somAbelha, 0.4f);
    SetSoundVolume(somLapis, 0.25f); // Mais baixo que o da abelha
    SetSoundVolume(somGiz, 0.5f);
    SetSoundVolume(somJumpscare, 0.8f);

    // Começa a tocar as trilhas de fundo em loop
    PlaySound(somEscola);
    PlaySound(somAbelha);
    PlaySound(somLapis);
    
    // 3. Inicializa a professora
    InitProfessora(&professora, dificuldadeProfessora, (Vector2){ -40, 40 });
    gameOverJumpscareTimer = 0.0f;
    gameOverJumpscareFrame = false;
    alunoIdleTimer = 0.5f;
    alunoIdleFrame = 0;
                                                                    
    // 4. Inicializa o subjogo virtual do Gnomo/Abelha
                                                            //horizontal/vertical
                                                        // ++ = direita / ++ = cima                                
    // 3. Inicializa o subjogo virtual do Gnomo/Abelha
    telaSubJogo = LoadRenderTexture(1280, 720);
    InitSubJogo();
    
    gameOverPrincipal = false;
    tabletLevantado = true;

    // Reseta as flags de controle de som de fim de jogo
    somMorteTocado = false;
    somVitoriaTocado = false;
    somJumpscareTocado = false;
}

void UpdateSalaDeAula(float deltaTime) {
    // --- ESTADO 1: GAME OVER (LEVOU JUMPSCARE) ---
    if (gameOverPrincipal) {
        // Para os sons de fundo e jogo se ainda estiverem tocando
        if (IsSoundPlaying(somAbelha)) StopSound(somAbelha);
        if (IsSoundPlaying(somLapis)) StopSound(somLapis);
        if (IsSoundPlaying(somGiz)) StopSound(somGiz);

        // Toca o Jumpscare apenas uma vez
        if (!somJumpscareTocado) {
            PlaySound(somJumpscare);
            somJumpscareTocado = true;
        }

        gameOverJumpscareTimer += deltaTime;
        if (gameOverJumpscareTimer >= 0.35f) {
            gameOverJumpscareTimer = 0.0f;
            gameOverJumpscareFrame = !gameOverJumpscareFrame;
        }

        if (IsKeyPressed(KEY_SPACE)) { 
            // Para o som de jumpscare caso o jogador reinicie rápido
            StopSound(somJumpscare);
            InitSalaDeAula(professora.dificuldade); 
        }
        return; 
    }

    // Mantém as músicas de fundo tocando em loop se pararem (enquanto o jogo roda normal)
    if (!IsSoundPlaying(somEscola)) PlaySound(somEscola);
    if (!IsSoundPlaying(somAbelha)) PlaySound(somAbelha);
    if (!IsSoundPlaying(somLapis)) PlaySound(somLapis);

    alunoIdleTimer += deltaTime;
    if (alunoIdleTimer >= 0.25f) {
        alunoIdleTimer = 0.0f;
        alunoIdleFrame = (alunoIdleFrame + 1) % 3;
    }

    UpdateSubJogo(); 

    // --- ESTADO 2: SE O JOGO AINDA NÃO FOI VENCIDO ---
    if (!SubJogoFoiVencido())
    {
        UpdateProfessora(&professora, deltaTime);

        // CONTROLE DO GIZ: Toca se a professora NÃO estiver olhando (Ex: PROFE_DE_COSTAS)
        // Substitua pelo nome correto do estado de "não olhando" se for diferente de PROFE_OLHANDO
        if (professora.estadoAtual != PROFE_OLHANDO) {
            if (!IsSoundPlaying(somGiz)) PlaySound(somGiz);
        } else {
            StopSound(somGiz);
        }

        // CONTROLE DE VOO: Seta para cima ou para baixo
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) {
            PlaySound(somVoo);
        }

        // Se o gnomo morreu (Dependendo de como seu submain avisa a morte, você adapta a condição abaixo)
        // Como o código original não mostra a função de morte, simulei uma checagem comum:
        /* if (GnomoColidiuComInimigo()) { 
            if (!somMorteTocado) {
                PlaySound(somMorte);
                somMorteTocado = true;
            }
        }
        */

        if (IsKeyDown(KEY_SPACE)) {
            tabletLevantado = true;
            posicaoSubJogo = posicaoBase;     
        } else {
            tabletLevantado = false;
            posicaoSubJogo = posicaoAbaixado;
        }

        if (professora.estadoAtual == PROFE_OLHANDO && tabletLevantado) {
            gameOverPrincipal = true;
        }
    }
    // --- ESTADO 3: SE O JOGO FOI VENCIDO ---
    else
    {
        tabletLevantado = true;
        posicaoSubJogo = posicaoBase;

        // Para os sons do gameplay comum
        if (IsSoundPlaying(somAbelha)) StopSound(somAbelha);
        if (IsSoundPlaying(somLapis)) StopSound(somLapis);
        if (IsSoundPlaying(somGiz)) StopSound(somGiz);

        // Toca o som de vitória uma única vez
        if (!somVitoriaTocado) {
            PlaySound(somVitoria);
            somVitoriaTocado = true;
        }
    }

    BeginTextureMode(telaSubJogo);
        ClearBackground(BLACK);
        DrawSubJogo(); 
    EndTextureMode();
}

void DrawSalaDeAula(void) {
    // Desenha o background da sala de aula cobrindo toda a tela
    if (backgroundAula.width > 0) {
        DrawTexturePro(
            backgroundAula,
            (Rectangle){ 0, 0, (float)backgroundAula.width, (float)backgroundAula.height },
            (Rectangle){ 0, 0, 1920.0f, 1080.0f },
            (Vector2){ 0, 0 },
            0.0f, WHITE
        );
    } else {
        DrawRectangle(0, 0, 1920, 1080, DARKGRAY);
    }

    // Desenha o aluno animado em loop de idle
    Texture2D alunoTexture = alunoIdleTexture1;
    switch (alunoIdleFrame) {
        case 1: alunoTexture = alunoIdleTexture2; break;
        case 2: alunoTexture = alunoIdleTexture3; break;
    }
    if (alunoTexture.width > 0) {    
        DrawTextureEx(alunoTexture, (Vector2){ 0, 0 }, 0.0f, 12.0f, WHITE);
    }

    // Desenha a Professora
                                        //horizontal/vertical
                                    // ++ = direita / ++ = baixo
    }

    // Desenha a Professora (muda de cor dependendo do estado)
    DrawProfessora(&professora);

    // Desenha o tablet de fundo
    Rectangle orig = { 0, 0, 1280.0f, -720.0f };
    Rectangle dest = { posicaoSubJogo.x, posicaoSubJogo.y, 1280.0f, 720.0f };

    if (tabletLevantado && tabletTexture.width > 0) {
        Rectangle destTablet = { posicaoTablet.x, posicaoTablet.y, 1700.0f, 1050.0f };
        destTablet.width = 1900.0f;
        DrawTexturePro(
            tabletTexture,
            (Rectangle){ 0.0f, 0.0f, (float)tabletTexture.width, (float)tabletTexture.height },
            destTablet,
            (Vector2){ 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    }

    DrawTexturePro(telaSubJogo.texture, orig, dest, (Vector2){ 0, 0 }, 0.0f, WHITE);

    // Se deu Game Over na sala de aula
    if (gameOverPrincipal) {
        DrawRectangle(0, 0, 1920, 1080, Fade(BLACK, 1.0f));
        Texture2D jumpscare = gameOverJumpscareFrame ? jumpscareTexture2 : jumpscareTexture1;
        if (jumpscare.width > 0) {
            float scale = 12.0f;
            float destWidth = jumpscare.width * scale;
            float destHeight = jumpscare.height * scale;
            float posX = 960.0f - destWidth / 2.0f;
            float posY = 0.0f;
            DrawTexturePro(
                jumpscare,
                (Rectangle){ 0.0f, 0.0f, (float)jumpscare.width, (float)jumpscare.height },
                (Rectangle){ posX, posY, destWidth, destHeight },
                (Vector2){ 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        }
        DrawText("VOCE FOI PEGO PELA PROFESSORA!", 450, 25, 50, RED);
        DrawText("Pressione 'ESPAÇO' para tentar novamente", 600, 70, 30, RED);
    }
}

void UnloadSalaDeAula(void) {
    UnloadTexture(backgroundAula);
    UnloadTexture(tabletTexture);
    UnloadTexture(jumpscareTexture1);
    UnloadTexture(jumpscareTexture2);
    UnloadTexture(alunoIdleTexture1);
    UnloadTexture(alunoIdleTexture2);
    UnloadTexture(alunoIdleTexture3);
    
    // DESCARREGA OS SONS DA MEMÓRIA
    UnloadSound(somAbelha);
    UnloadSound(somEscola);
    UnloadSound(somGiz);
    UnloadSound(somJumpscare);
    UnloadSound(somLapis);
    UnloadSound(somMorte);
    UnloadSound(somVitoria);
    UnloadSound(somVoo);

    UnloadProfessora(&professora);
    UnloadSubJogo();
    UnloadRenderTexture(telaSubJogo);
    
    // Fecha o dispositivo de áudio se for fechar o jogo por completo
    // Se você tiver um menu principal depois desse arquivo, comente a linha abaixo 
    // e coloque o CloseAudioDevice() apenas na sua main.cpp / main.c real.
    // CloseAudioDevice(); 
}