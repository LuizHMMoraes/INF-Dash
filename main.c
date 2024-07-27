#include "raylib.h"
#include "game.h"
#include "menu.h"
//#include "level_loader.h"
//#include <stdlib.h>
#include "Mouse.h"
//#include <stdio.h>


typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

int main(void)
{
    // Inicializacao
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 450;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib INF-Dash project");

    Image titleImage = LoadImage("INFDash/tiles/inf-dash.png");
    Texture2D backgroundTitleImage = LoadTextureFromImage(titleImage);
   

    GameScreen currentScreen = LOGO;
    int framesCounter = 0; // Útil para contar frames

    // Inicializa o estado do jogo
    GameState gameState = InitGame();

    SetTargetFPS(60); // Define a taxa de quadros por segundo

    // Loop principal do jogo
    while (!WindowShouldClose()) // Detecta se a janela deve fechar
    {
        // Atualiza
        switch (currentScreen)
        {
            case LOGO:
            {
                framesCounter++; // Conta frames

                // Espera por 3 segundos (180 frames) antes de pular para a tela TITLE
                if (framesCounter > 180)
                {
                    currentScreen = TITLE;
                }
            }
            break;

            case TITLE:
            {
                // Lida com cliques nos botões
                if (IsButtonClicked("PLAY", 525, 300))
                {
                    currentScreen = GAMEPLAY;
                }
                if (IsButtonClicked("LEADERBOARD", 525, 360))
                {
                    // Lógica do leaderboard
                }
                if (IsButtonClicked("QUIT", 525, 420))
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

                // Pressiona enter para mudar para a tela ENDING
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentScreen = ENDING;
                }
            }
            break;

            case ENDING:
            {
                // Pressiona enter para retornar para a tela TITLE
                if (IsKeyPressed(KEY_ENTER))
                {
                    currentScreen = TITLE;
                }
            }
            break;

            default:
                break;
        }

        // Desenha
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
                DrawButton("PLAY", 525, 300);
                DrawButton("LEADERBOARD", 525, 360);
                DrawButton("QUIT", 525, 420);
            }
            break;

            case GAMEPLAY:
            {
                DrawGame(&gameState);
                DrawText("PRESS ENTER to END GAME", 20, 20, 20, MAROON);
            }
            break;

            case ENDING:
            {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLUE);
                DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                DrawText("PRESS ENTER to RETURN to TITLE", 120, 220, 20, DARKBLUE);
            }
            break;

            default:
                break;
        }

        EndDrawing();
    }

    // Desinicialização
    UnloadImage(titleImage); // Descarrega a imagem da memória
    UnloadTexture(backgroundTitleImage); // Descarrega a textura do título
    CloseWindow(); // Fecha a janela e o contexto OpenGL

    return 0;
}
