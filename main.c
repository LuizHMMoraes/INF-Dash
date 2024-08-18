#include "raylib.h"
#include "Mouse.h"
#include "Leaderboard.h"
#include <stdio.h>
#include <stdlib.h>

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

    // Carrega as texturas do menu
    Image titleImage = LoadImage("tiles/inf-dash.png");
    Texture2D backgroundTitleImage = LoadTextureFromImage(titleImage);

    // Define a tela inicial do jogo
    GameScreen currentScreen = LOGO;

    // Initialize buttons positions
    int buttonX = 525;
    int buttonY = 300;
    int buttonSpacing = 60;

    // Inicia botões
    const char *playText = "PLAY";
    const char *leaderboardText = "LEADERBOARD";
    const char *quitText = "QUIT";

    //Inicia Leaderboard
    LeaderboardEntry leaderboard[MAX_LEADERBOARD_ENTRIES];
    LoadLeaderboard(leaderboard);

    int framesCounter = 0; // Useful to count frames
    float waitStartTime = 0.0f;
    bool isWaiting = false;
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

                if (IsKeyPressed(KEY_ENTER))
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
                ClearBackground(BLUE);
                DrawTexture(backgroundTitleImage, 200, 80, WHITE);
                DrawButton(playText, buttonX, buttonY);
                DrawButton(leaderboardText, buttonX, buttonY + buttonSpacing);
                DrawButton(quitText, buttonX, buttonY + 2 * buttonSpacing);
            }
            break;

            case GAMEPLAY:
            {
                DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 130, 220, 20, MAROON);
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

    UnloadTexture(backgroundTitleImage); // Unload background texture
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
