#include "raylib.h"
#include "Mouse.h"
#include "Gameplay.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_LEADERBOARD_ENTRIES 5

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, LEADERBOARD, ENDING } GameScreen;

typedef struct {
    char name[20];
    int score;
} LeaderboardEntry;

//------------------------------------------------------------------------------------
// Function Declarations
//------------------------------------------------------------------------------------
void DrawLeaderboard(LeaderboardEntry leaderboard[], int numEntries);
void LoadLeaderboard(LeaderboardEntry leaderboard[], int *numEntries);
void SaveLeaderboard(LeaderboardEntry leaderboard[], int numEntries);

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

    // Initialize buttons
    const char *playText = "PLAY";
    const char *leaderboardText = "LEADERBOARD";
    const char *quitText = "QUIT";

    int framesCounter = 0; // Useful to count frames

    // Inicializa o estado do jogo
    GameState gameState = InitGame();

    // Leaderboard
    LeaderboardEntry leaderboard[MAX_LEADERBOARD_ENTRIES];
    int numEntries = 0;

    LoadLeaderboard(leaderboard, &numEntries);

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
                // Atualiza o estado do jogo
                UpdateGame(&gameState);

                // Press enter to change to ENDING screen
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentScreen = ENDING;
                }
            }
            break;

            case LEADERBOARD:
            {
                // Draw the leaderboard
                if (IsKeyPressed(KEY_ENTER)) {
                    currentScreen = TITLE;
                }
            }
            break;

            case ENDING:
            {
                gameState = InitGame();
                // Press enter to return to TITLE screen
                if (IsKeyPressed(KEY_ENTER))
                {
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
                DrawGame(&gameState);
                DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
            }
            break;

            case LEADERBOARD:
            {
                ClearBackground(RAYWHITE);
                DrawLeaderboard(leaderboard, numEntries);
            }
            break;

            case ENDING:
            {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLUE);
                DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);
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

// Function to draw the leaderboard
void DrawLeaderboard(LeaderboardEntry leaderboard[], int numEntries) {
    DrawText("LEADERBOARD", 20, 20, 40, DARKGRAY);

    for (int i = 0; i < numEntries; i++) {
        char entry[100];
        sprintf(entry, "%d. %s - %d", i + 1, leaderboard[i].name, leaderboard[i].score);
        DrawText(entry, 100, 100 + 30 * i, 20, DARKGRAY);
    }

    DrawText("PRESS ENTER to RETURN", 100, 500, 20, DARKGRAY);
}

// Function to load the leaderboard from a binary file
void LoadLeaderboard(LeaderboardEntry leaderboard[], int *numEntries) {
    FILE *file = fopen("top5.bin", "rb");

    if (file == NULL) {
        *numEntries = 0;
        return;
    }

    *numEntries = fread(leaderboard, sizeof(LeaderboardEntry), MAX_LEADERBOARD_ENTRIES, file);

    fclose(file);
}

// Function to save the leaderboard to a binary file
void SaveLeaderboard(LeaderboardEntry leaderboard[], int numEntries) {
    FILE *file = fopen("top5.bin", "wb");

    if (file != NULL) {
        fwrite(leaderboard, sizeof(LeaderboardEntry), numEntries, file);
        fclose(file);
    }
}
