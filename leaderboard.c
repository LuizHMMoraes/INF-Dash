#include "raylib.h"
#include "Leaderboard.h"
#include <stdio.h>
#include <string.h>

// Função para desenhar o leaderboard na tela
void DrawLeaderboard(LeaderboardEntry leaderboard[])
{
    DrawText("LEADERBOARD", 450, 20, 40, BLACK);

    for (int i = 0; i < MAX_LEADERBOARD_ENTRIES; i++)
    {
        char entry[100];
        sprintf(entry, "%d. %s - %d", i + 1, leaderboard[i].name, leaderboard[i].score);
        DrawText(entry, 470, 150 + 60 * i, 30, DARKGRAY);
    }

    DrawText("PRESS ENTER to RETURN", 465, 500, 20, BLACK);
}

// Função para carregar o leaderboard de um arquivo binário
void LoadLeaderboard(LeaderboardEntry leaderboard[])
{
    FILE *file = fopen("INF-Dash/top5.bin", "rb");

    if (file == NULL)
    {
        // Se o arquivo não existir, inicialize com zero entradas
        for (int i = 0; i < MAX_LEADERBOARD_ENTRIES; i++)
        {
            leaderboard[i].score = 0;
            strcpy(leaderboard[i].name, "");
        }
        fclose(file);
        return;
    }

    fread(leaderboard, sizeof(LeaderboardEntry), MAX_LEADERBOARD_ENTRIES, file);
    fclose(file);
}

// Função para salvar o leaderboard em um arquivo binário
void SaveLeaderboard(LeaderboardEntry leaderboard[])
{
    FILE *file = fopen("INF-Dash/top5.bin", "wb");

    if (file != NULL)
    {
        fwrite(leaderboard, sizeof(LeaderboardEntry), MAX_LEADERBOARD_ENTRIES, file);
        fclose(file);
    }
}

// Função para desenhar a caixa de entrada de nome do jogador e obter a entrada
LeaderboardEntry PlayerEntry(int score)
{
    LeaderboardEntry entry;
    entry.score = score;
    strcpy(entry.name, "");

    char nameBuffer[MAX_NAME_LENGTH + 1] = { 0 };
    int charIndex = 0;

    SetTargetFPS(60); // Set the desired framerate
    bool inputActive = true;

    while (inputActive)
    {
        // Update logic for text input
        if (IsKeyPressed(KEY_ENTER))
        {
            inputActive = false;
        }
        else if (IsKeyPressed(KEY_BACKSPACE))
        {
            if (charIndex > 0)
            {
                charIndex--;
                nameBuffer[charIndex] = '\0';
            }
        }
        else
        {
            for (int i = 0; i < 26; i++)
            {
                if (IsKeyPressed(KEY_A + i))
                {
                    if (charIndex < MAX_NAME_LENGTH - 1)
                    {
                        nameBuffer[charIndex++] = 'A' + i;
                        nameBuffer[charIndex] = '\0';
                    }
                }
                if (IsKeyPressed(KEY_A + i + 32)) // Handle lowercase letters
                {
                    if (charIndex < MAX_NAME_LENGTH - 1)
                    {
                        nameBuffer[charIndex++] = 'a' + i;
                        nameBuffer[charIndex] = '\0';
                    }
                }
            }
            if (IsKeyPressed(KEY_SPACE))
            {
                if (charIndex < MAX_NAME_LENGTH - 1)
                {
                    nameBuffer[charIndex++] = ' ';
                    nameBuffer[charIndex] = '\0';
                }
            }
        }

        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        int boxWidth = 400;
        int boxHeight = 60;
        int textSize = 20;

        Vector2 textSizeVec = MeasureTextEx(GetFontDefault(), nameBuffer, textSize, 1.0f);
        Vector2 textPosition = { (screenWidth - boxWidth) / 2 + 10, (screenHeight - boxHeight) / 2 + (boxHeight - textSize) / 2 };
        Vector2 boxPosition = { (screenWidth - boxWidth) / 2, (screenHeight - boxHeight) / 2 };

        DrawRectangleV(boxPosition, (Vector2){boxWidth, boxHeight}, LIGHTGRAY);
        DrawRectangleLinesEx((Rectangle){boxPosition.x, boxPosition.y, boxWidth, boxHeight}, 2, GRAY);

        DrawText("Enter your name (press ENTER to finish):", (screenWidth - MeasureText("Enter your name (press ENTER to finish):", textSize)) / 2, boxPosition.y - 40, textSize, DARKGRAY);
        DrawText(nameBuffer, textPosition.x, textPosition.y, textSize, BLACK);

        // Draw cursor
        float time = GetTime();
        if ((int)(time * 2) % 2 == 0)
        {
            int cursorX = textPosition.x + textSizeVec.x;
            DrawLine(cursorX, textPosition.y, cursorX, textPosition.y + textSize, BLACK);
        }

        EndDrawing();
    }

    strncpy(entry.name, nameBuffer, MAX_NAME_LENGTH);
    return entry;
}

// Função para determinar a posição e atualizar o leaderboard
void Ranking(LeaderboardEntry leaderboard[], LeaderboardEntry newEntry)
{
    int pos = -1;
    // Verifica a posição onde o novo entry deve ser inserido
    for (int i = 0; i < MAX_LEADERBOARD_ENTRIES; i++)
    {
        if (newEntry.score > leaderboard[i].score)
        {
            pos = i;
            break;
        }
    }

    // Se a nova pontuação não é suficiente para o top 5 e o leaderboard já está cheio, não a insere
    if (pos == -1 && leaderboard[MAX_LEADERBOARD_ENTRIES - 1].score >= newEntry.score)
    {
        return;
    }

    // Se a posição é inválida, significa que a nova entrada deve ser inserida no final
    if (pos == -1)
    {
        pos = MAX_LEADERBOARD_ENTRIES - 1;
    }
    else
    {
        // Desloca as entradas abaixo da nova posição
        for (int i = MAX_LEADERBOARD_ENTRIES - 1; i > pos; i--)
        {
            leaderboard[i] = leaderboard[i - 1];
        }
    }

    // Insere a nova entrada
    leaderboard[pos] = newEntry;
}
