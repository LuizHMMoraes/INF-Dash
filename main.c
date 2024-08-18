#include "raylib.h"
#include "Mouse.h"
<<<<<<< HEAD
#include "Gameplay.h"

=======
#include "Leaderboard.h"
#include <stdio.h>
#include <stdlib.h>
>>>>>>> leaderboards

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, LEADERBOARD, ENDING } GameScreen;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int SCREEN_WIDTH = 1200;
    const int SCREEN_HEIGHT = 600;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib INF-Dash project");

<<<<<<< HEAD
    //carrega as texturas menu
    Image titleImage = LoadImage("tiles/inf-dash.png");
    Texture2D titleTexture = LoadTextureFromImage(titleImage);

    Image backgroundTitleImage = LoadImage("tiles/fundoTitle.png");
    Texture2D backgroundTitleTexture = LoadTextureFromImage(backgroundTitleImage);

    //carrega as texturas do cenário
    Image gameplayImage = LoadImage("tiles/fundoCeu.png");
    Texture2D backgroundGameplayTexture = LoadTextureFromImage(gameplayImage);

    Image blockImage = LoadImage("tiles/block.png");
    Texture2D blockTexture = LoadTextureFromImage(blockImage);

    Image obstacleImage = LoadImage("tiles/obstacle.png");
    Texture2D obstacleTexture = LoadTextureFromImage(obstacleImage);

    //Carrega a textura do jogador
    Image playerImage = LoadImage("tiles/player.png");
    Texture2D playerTexture = LoadTextureFromImage(playerImage);

    //define a tela inicial do jogo
=======
    // Carrega as texturas do menu
    Image titleImage = LoadImage("tiles/inf-dash.png");
    Texture2D backgroundTitleImage = LoadTextureFromImage(titleImage);

    // Define a tela inicial do jogo
>>>>>>> leaderboards
    GameScreen currentScreen = LOGO;

    // Initialize buttons positions
    int buttonX = 510;
    int buttonY = 300;
    int buttonSpacing = 60;

    // Inicia botões
    const char *playText = "PLAY";
    const char *leaderboardText = "LEADERBOARD";
    const char *quitText = "QUIT";

    //Inicia Leaderboard
    LeaderboardEntry leaderboard[MAX_LEADERBOARD_ENTRIES];
    LoadLeaderboard(leaderboard);

<<<<<<< HEAD
    // Inicializa o estado do jogo
    GameState gameState = InitGame();

=======
    int framesCounter = 0; // Useful to count frames
    float waitStartTime = 0.0f;
    bool isWaiting = false;
>>>>>>> leaderboards
    SetTargetFPS(60); // Set desired framerate (frames-per-second)
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch (currentScreen)
        {
            case LOGO:
            {
                framesCounter++; // Count frames

                // Wait for 3 seconds (180 frames) before jumping to TITLE screen
                if (framesCounter > 180)
                {
                    currentScreen = TITLE;
                }
            }
            break;

            case TITLE:
            {
                // Handle button clicks
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
<<<<<<< HEAD
                // Atualiza o estado do jogo
                UpdateGame(&gameState);

                // Se as tentativas acabaram, muda para a tela de ENDING
                if (gameState.player.attempts == 0)
=======

                if (IsKeyPressed(KEY_ENTER))
>>>>>>> leaderboards
                {
                    currentScreen = ENDING;
                }

            }
            break;

        case LEADERBOARD:
            {
<<<<<<< HEAD
                gameState = InitGame();
                // Press enter to return to TITLE screen
=======
                // Se a tecla ENTER for pressionada, volta para o menu TITLE
>>>>>>> leaderboards
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
                    int score = 2000; // Simulate game score

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

        // Draw
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
                //Gambiarra no background
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
<<<<<<< HEAD
                DrawGame(&gameState, backgroundGameplayTexture, playerTexture, blockTexture, obstacleTexture);
=======
                DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 130, 220, 20, MAROON);
            }
            break;

            case LEADERBOARD:
            {
                ClearBackground(RAYWHITE);
                DrawLeaderboard(leaderboard);
>>>>>>> leaderboards
            }
            break;

            case ENDING:
            {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
                DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, LIGHTGRAY);
            }
            break;

            default:
                break;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    UnloadTexture(titleTexture);
    UnloadTexture(backgroundTitleTexture); // Unload background texture
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
