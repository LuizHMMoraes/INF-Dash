#include "raylib.h"
#include "Mouse.h"
#include "Gameplay.h"
#include "Leaderboard.h"
#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------------------
// Definição de Tipos e Estruturas
//------------------------------------------------------------------------------------------
typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, LEADERBOARD, ENDING } GameScreen;

//------------------------------------------------------------------------------------
// Ponto de entrada principal do programa
//------------------------------------------------------------------------------------
int main(void)
{
    // Inicialização
    //--------------------------------------------------------------------------------------
    const int SCREEN_WIDTH = 1200;
    const int SCREEN_HEIGHT = 600;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib INF-Dash project");

    // Carrega as texturas do menu
    Image titleImage = LoadImage("tiles/inf-dash.png");
    Texture2D titleTexture = LoadTextureFromImage(titleImage);

    Image backgroundTitleImage = LoadImage("tiles/fundoTitle.png");
    Texture2D backgroundTitleTexture = LoadTextureFromImage(backgroundTitleImage);

    // Carrega as texturas do cenário
    Image gameplayImage = LoadImage("tiles/fundoCeu.png");
    Texture2D backgroundGameplayTexture = LoadTextureFromImage(gameplayImage);

    Image blockImage = LoadImage("tiles/block.png");
    Texture2D blockTexture = LoadTextureFromImage(blockImage);

    Image obstacleImage = LoadImage("tiles/obstacle.png");
    Texture2D obstacleTexture = LoadTextureFromImage(obstacleImage);

    // Carrega a textura do jogador
    Image playerImage = LoadImage("tiles/player.png");
    Texture2D playerTexture = LoadTextureFromImage(playerImage);

    // Define a tela inicial do jogo
    GameScreen currentScreen = LOGO;

    // Inicializa posições dos botões
    int buttonX = 510;
    int buttonY = 300;
    int buttonSpacing = 60;

    // Inicia botões
    const char *playText = "PLAY";
    const char *leaderboardText = "LEADERBOARD";
    const char *quitText = "QUIT";

    // Inicia Leaderboard
    LeaderboardEntry leaderboard[MAX_LEADERBOARD_ENTRIES];
    LoadLeaderboard(leaderboard);

    // Inicializa o estado do jogo
    GameState gameState = InitGame();

    int framesCounter = 0; // Útil para contar frames
    float waitStartTime = 0.0f;
    bool isWaiting = false;
    SetTargetFPS(60); // Define a taxa de frames desejada (frames por segundo)
    //--------------------------------------------------------------------------------------

    // Loop principal do jogo
    while (!WindowShouldClose()) // Detecta o botão de fechar janela ou a tecla ESC
    {
        // Atualização
        //----------------------------------------------------------------------------------
        switch (currentScreen)
        {
        case LOGO:
        {
            framesCounter++; // Conta frames

            // Espera 3 segundos (180 frames) antes de pular para a tela TITLE
            if (framesCounter > 180)
            {
                currentScreen = TITLE;
            }
        }
        break;

        case TITLE:
        {
            // Lida com os cliques nos botões
            if (IsButtonClicked(playText, buttonX, buttonY))
            {
                currentScreen = GAMEPLAY;
            }
            if (IsButtonClicked(leaderboardText, buttonX, buttonY + buttonSpacing))
            {
                currentScreen = LEADERBOARD;
            }
            if (IsButtonClicked(quitText, buttonX, buttonY + 2 * buttonSpacing))
            {
                CloseWindow();
                return 0;
            }
        }
        break;

        case GAMEPLAY:
        {

            // Atualiza o estado do jogo
            UpdateGame(&gameState);

            // Se as tentativas acabarem, muda para a tela de ENDING
            if (gameState.player.attempts == 0)
            {
                currentScreen = ENDING;
            }

        }
        break;

        case LEADERBOARD:
        {
            // Se a tecla ENTER for pressionada, volta para o menu TITLE
            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = TITLE;
            }
        }
        break;

        case ENDING:
        {
            if (!isWaiting)
            {
                waitStartTime = GetTime();
                isWaiting = true;
            }
            else
                // Espera 3 segundos
                if (GetTime() - waitStartTime >= 3.0f)
                {
                    int score = 2000; // Simula a pontuação do jogo

                    // Verifica se o jogador se qualifica para entrar no leaderboard
                    if (score >= leaderboard[MAX_LEADERBOARD_ENTRIES - 1].score)
                    {
                        LeaderboardEntry player = PlayerEntry(score);
                        Ranking(leaderboard, player);
                        SaveLeaderboard(leaderboard);
                    }
                    currentScreen = TITLE;
                }
        }
        break;

        default:
            break;
        }
        //----------------------------------------------------------------------------------

        // Desenho
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        switch (currentScreen)
        {
        case LOGO:
        {
            DrawText("UFRGS - Algoritmos e Programação", 20, 20, 40, LIGHTGRAY);
            DrawText("Turma D", 20, 70, 20, LIGHTGRAY);
            DrawText("Luiz H. M. Moraes", 450, 220, 30, GRAY);
            DrawText("Nathan S. Pacheco", 450, 260, 30, GRAY);
        }
        break;

        case TITLE:
        {
            ClearBackground(BLACK);
            // Gambiarra no background
            DrawTexture(backgroundTitleTexture, 0, 0, WHITE);
            DrawTexture(backgroundTitleTexture, 600, 0, WHITE);
            DrawTexture(backgroundTitleTexture, 0, 540, WHITE);
            DrawTexture(backgroundTitleTexture, 600, 540, WHITE);

            DrawTexture(titleTexture, 200, 80, WHITE);
            DrawButton(playText, buttonX, buttonY);
            DrawButton(leaderboardText, buttonX, buttonY + buttonSpacing);
            DrawButton(quitText, buttonX, buttonY + 2 * buttonSpacing);
        }
        break;

        case GAMEPLAY:
        {
            DrawGame(&gameState, backgroundGameplayTexture, playerTexture, blockTexture, obstacleTexture);
        }
        break;

        case LEADERBOARD:
        {
            ClearBackground(RAYWHITE);
            DrawLeaderboard(leaderboard);
        }
        break;

        case ENDING:
        {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
            DrawText("GAME OVER", 335, 250, 80, RED);
        }
        break;

        default:
            break;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // Desinicialização
    //--------------------------------------------------------------------------------------

    // Descarregar imagens (caso tenha usado diretamente as imagens em algum lugar)
    UnloadImage(titleImage);
    UnloadImage(backgroundTitleImage);
    UnloadImage(gameplayImage);
    UnloadImage(blockImage);
    UnloadImage(obstacleImage);
    UnloadImage(playerImage);
    // Descarregar texturas
    UnloadTexture(titleTexture);
    UnloadTexture(backgroundTitleTexture);
    UnloadTexture(backgroundGameplayTexture);
    UnloadTexture(blockTexture);
    UnloadTexture(obstacleTexture);
    UnloadTexture(playerTexture);
    CloseWindow(); // Fecha a janela e o contexto OpenGL
    //--------------------------------------------------------------------------------------

    return 0;
}
