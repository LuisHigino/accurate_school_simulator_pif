#include "raylib.h"
#include "submain.h"
#include "abelha.h"
#include "inimigo.h"
#include "constantes.h"
#include "ranking.h"
#include "sala_de_aula.h"

#include <stdio.h>
#include <string.h>

static const int screenWidth = 1280;
static const int screenHeight = 720;
static const float tempoParaLinhaChegada = 60.0f;
static const float velocidadeLinhaChegada = 8.0f;

static bool jogoComecou = false;
static bool jogoVencido = false;
static bool linhaChegadaLiberada = false;
static bool aguardandoNome = false;
static bool rankingSalvo = false;
static bool solicitarRetornoMenu = false;
static bool retornoLiberadoAposSalvar = false;

static float tempoDecorrido = 0.0f;
static float linhaChegadaX = 1280 + 200;
static char nomeJogador[50] = { 0 };
static int nomeJogadorTamanho = 0;
static Ranking *rankingLista = NULL;

static int totalDerrotasAbelha = 0;

static Texture2D background;
static Texture2D texturaAbelha1;
static Texture2D texturaAbelha2;
static Texture2D texturaAbelha3;
static Texture2D et1;
static Texture2D et2;
static Texture2D et3;
static Texture2D frameEt;
static Sound somMorte;

static int frameAbelhaAtual = 0;
static int timerAbelhaAnimacao = 0;
static int frameEtAtual = 0;
static int timerEtAnimacao = 0;

static Abelha abelha;
static Inimigo inimigos[MAX_INIMIGOS];

static Rectangle botaoVoltarMenu = { 470.0f, 605.0f, 340.0f, 40.0f };

static Texture2D CarregarTexturaComFiltro(const char *caminho)
{
    Texture2D textura = LoadTexture(caminho);
    SetTextureFilter(textura, TEXTURE_FILTER_POINT);
    return textura;
}

static void ReiniciarEstadoSubJogo(void)
{
    jogoComecou = false;
    jogoVencido = false;
    linhaChegadaLiberada = false;
    aguardandoNome = false;
    rankingSalvo = false;
    solicitarRetornoMenu = false;
    retornoLiberadoAposSalvar = false;
    tempoDecorrido = 0.0f;
    linhaChegadaX = screenWidth + 200;
    nomeJogador[0] = '\0';
    nomeJogadorTamanho = 0;
    totalDerrotasAbelha = 0;
}

void InitSubJogo(void)
{
    if (rankingLista != NULL) {
        LiberarRanking(rankingLista);
        rankingLista = NULL;
    }

    CarregarRankingTXT(&rankingLista);

    background = CarregarTexturaComFiltro("GAME/assets/images/background.png");
    texturaAbelha1 = CarregarTexturaComFiltro("GAME/assets/images/gnomo_jetpack1.png");
    texturaAbelha2 = CarregarTexturaComFiltro("GAME/assets/images/gnomo_jetpack2.png");
    texturaAbelha3 = CarregarTexturaComFiltro("GAME/assets/images/gnomo_jetpack3.png");
    InitAbelha(&abelha, texturaAbelha1, 1);

    et1 = CarregarTexturaComFiltro("GAME/assets/images/et1.png");
    et2 = CarregarTexturaComFiltro("GAME/assets/images/et2.png");
    et3 = CarregarTexturaComFiltro("GAME/assets/images/et3.png");

    somMorte = LoadSound("GAME/assets/sounds/som_morte.mp3");

    SetSoundVolume(somMorte, 0.2f);

    InitInimigos(inimigos, 0, screenWidth, GetDificuldadeSala());

    ReiniciarEstadoSubJogo();
}

void UpdateSubJogo(void)
{
    int teclaPressionada = GetKeyPressed();

    if (jogoVencido) {
        aguardandoNome = true;

        int caractere = GetCharPressed();
        while (caractere > 0) {
            if (caractere >= 32 && caractere <= 125 && nomeJogadorTamanho < 49) {
                nomeJogador[nomeJogadorTamanho++] = (char)caractere;
                nomeJogador[nomeJogadorTamanho] = '\0';
            }
            caractere = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && nomeJogadorTamanho > 0) nomeJogador[--nomeJogadorTamanho] = '\0';

        if (!rankingSalvo && IsKeyPressed(KEY_ENTER)) {
            char nomeFinal[50];
            strcpy(nomeFinal, (nomeJogadorTamanho == 0) ? "ANONIMO" : nomeJogador);
            InserirRanking(&rankingLista, nomeFinal, GetTempoSobrevivencia(), GetDificuldadeSala(), GetDerrotasAbelha());
            SalvarRankingTXT(rankingLista);
            rankingSalvo = true;
            aguardandoNome = false;
            retornoLiberadoAposSalvar = false;
        }

        if (rankingSalvo) {
            if (!retornoLiberadoAposSalvar) {
                retornoLiberadoAposSalvar = true;
            }
            else {
                if (IsKeyPressed(KEY_ENTER)) {
                    solicitarRetornoMenu = true;
                }
            }
        }

        return;
    }

    if (!jogoComecou && teclaPressionada != 0) {
        jogoComecou = true;
        jogoVencido = false;
        linhaChegadaLiberada = false;
        tempoDecorrido = 0.0f;
        linhaChegadaX = screenWidth + 200;

        abelha.x = 120;
        abelha.lane = 1;

        InitInimigos(inimigos, 3, screenWidth, GetDificuldadeSala());
    }


    timerAbelhaAnimacao++;
    if (timerAbelhaAnimacao >= 10) {
        timerAbelhaAnimacao = 0;
        frameAbelhaAtual = (frameAbelhaAtual + 1) % 3;
        abelha.textura = (frameAbelhaAtual == 0) ? texturaAbelha1 : (frameAbelhaAtual == 1) ? texturaAbelha2 : texturaAbelha3;
    }

    if (jogoComecou && !jogoVencido) {
        tempoDecorrido += GetFrameTime();

        if (tempoDecorrido >= tempoParaLinhaChegada)
            linhaChegadaLiberada = true;

        if (linhaChegadaLiberada) {
            linhaChegadaX -= velocidadeLinhaChegada * 60 * GetFrameTime();
        }

        AtualizarAbelha(&abelha, GetFrameTime(), screenWidth);
    }

    if (jogoComecou && !jogoVencido) {
        AtualizarInimigos(inimigos, screenWidth);
    }

    if (jogoComecou && !jogoVencido) {
        if (ChecarColisaoInimigos(inimigos, GetHitboxAbelha(&abelha))) {
            totalDerrotasAbelha++;

            jogoComecou = false;
            jogoVencido = false;
            linhaChegadaLiberada = false;
            tempoDecorrido = 0.0f;
            linhaChegadaX = screenWidth + 200;
            if (!IsSoundPlaying(somMorte)) PlaySound(somMorte);
        }
    }

    if (jogoComecou && !jogoVencido && linhaChegadaLiberada && linhaChegadaX <= abelha.x + 20) {
        jogoVencido = true;
        jogoComecou = false;
        aguardandoNome = true;
        nomeJogador[0] = '\0';
        nomeJogadorTamanho = 0;
    }

    if (jogoComecou && !jogoVencido)
        timerEtAnimacao++;

    if (timerEtAnimacao > 10) {
        frameEtAtual++;
        timerEtAnimacao = 0;

        if (frameEtAtual > 2) frameEtAtual = 0;
    }

    if (frameEtAtual == 0) frameEt = et1;
    else if (frameEtAtual == 1) frameEt = et2;
    else frameEt = et3;
}

static void DesenharFundoSubJogo(void)
{
    if (background.width > 0) {
        DrawTexturePro(background, (Rectangle){0, 0, (float)background.width, (float)background.height}, (Rectangle){0, 0, (float)screenWidth, (float)screenHeight}, (Vector2){0, 0}, 0, WHITE);
        return;
    }

    DrawRectangle(0, 0, screenWidth, screenHeight, DARKGRAY);
    DrawText("FALTA BACKGROUND", 10, 10, 20, RED);
}

static void DesenharFimDeJogo(void)
{
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.92f));

    char nomeTexto[80];
    snprintf(nomeTexto, sizeof(nomeTexto), "NOME: %s", nomeJogadorTamanho > 0 ? nomeJogador : "");
    int tituloLargura = MeasureText("PARABENS VOCE VENCEU O JOGO", 30);
    int instrucaoLargura = MeasureText("DIGITE SEU NOME E APERTE ENTER PARA SALVAR NO RANKING", 24);
    int nomeLargura = MeasureText(nomeTexto, 28);

    DrawText("PARABENS VOCE VENCEU O JOGO", (screenWidth - tituloLargura) / 2, screenHeight / 2 - 70, 30, YELLOW);
    DrawText("DIGITE SEU NOME E APERTE ENTER PARA SALVAR NO RANKING", (screenWidth - instrucaoLargura) / 2, screenHeight / 2 - 20, 24, WHITE);
    DrawText(nomeTexto, (screenWidth - nomeLargura) / 2, screenHeight / 2 + 25, 28, GREEN);

    if (!rankingSalvo) return;

    int rankingLargura = MeasureText("RANKING ATUALIZADO", 22);
    DrawText("RANKING ATUALIZADO", (screenWidth - rankingLargura) / 2, screenHeight / 2 + 72, 22, LIME);
    DrawRectangleRec(botaoVoltarMenu, Fade(WHITE, 0.12f));
    DrawRectangleLinesEx(botaoVoltarMenu, 1.0f, LIGHTGRAY);

    const char *textoBotao = "APERTE ENTER PARA VOLTAR AO MENU";
    int textoLargura = MeasureText(textoBotao, 20);
    DrawText(textoBotao, (int)(botaoVoltarMenu.x + (botaoVoltarMenu.width - textoLargura) * 0.5f), (int)botaoVoltarMenu.y + 10, 20, WHITE);
}

void DrawSubJogo(void)
{
    ClearBackground(BLACK);
    DesenharFundoSubJogo();

    if (!jogoComecou) {
        DrawText("APERTE QUALQUER TECLA PARA COMEÇAR", 320, 80, 30, WHITE);
        DrawText("SOBREVIVA 1 MINUTO PARA LIBERAR A LINHA DE CHEGADA", 250, 120, 24, LIGHTGRAY);
    }

    if (linhaChegadaLiberada) {
        DrawLine((int)linhaChegadaX, 0, (int)linhaChegadaX, screenHeight, YELLOW);
        DrawText("CHEGADA", (int)linhaChegadaX - 30, 20, 22, YELLOW);
    }

    DesenharAbelha(&abelha);

    if (jogoComecou || jogoVencido) {
        float tempoRestante = tempoParaLinhaChegada - tempoDecorrido;
        if (tempoRestante < 0.0f) tempoRestante = 0.0f;
        DrawText(TextFormat("TEMPO: %02d", (int)tempoRestante), 30, 30, 28, WHITE);
    }

    DesenharInimigos(inimigos, frameEt);

    if (jogoVencido) DesenharFimDeJogo();
}

void UnloadSubJogo(void)
{
    UnloadAbelha(&abelha);
    UnloadTexture(background);
    UnloadTexture(texturaAbelha1);
    UnloadTexture(texturaAbelha2);
    UnloadTexture(texturaAbelha3);
    UnloadTexture(et1);
    UnloadTexture(et2);
    UnloadTexture(et3);

    LiberarRanking(rankingLista);
    rankingLista = NULL;
}

bool SubJogoFoiVencido(void)
{
    return jogoVencido;
}

bool SubJogoConsumirSolicitacaoRetornoMenu(void)
{
    if (solicitarRetornoMenu)
    {
        solicitarRetornoMenu = false;
        return true;
    }

    return false;
}

int GetDerrotasAbelha(void) {
    return totalDerrotasAbelha;
}