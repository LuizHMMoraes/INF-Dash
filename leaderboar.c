#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYERS 5
#define MAX_NAME_LENGTH 20

typedef struct {
    char name[MAX_NAME_LENGTH];
    int score;
} PLAYER;

PLAYER leaderboard[MAX_PLAYERS];
int numPlayers = 0;
//void para carregar 
void LoadLeaderboard(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open leaderboard file.\n");
        return;
    }
    numPlayers = 0;
    while (fscanf(file, "%s %d", leaderboard[numPlayers].name, &leaderboard[numPlayers].score) != EOF && numPlayers < MAX_PLAYERS) {
        numPlayers++;
    }
    fclose(file);
}
// ppara salvar o arquivo txt alterado
void SaveLeaderboard(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to open leaderboard file.\n");
        return;
    }
    for (int i = 0; i < numPlayers; i++) {
        fprintf(file, "%s %d\n", leaderboard[i].name, leaderboard[i].score);
    }
    fclose(file);
}
//void para adicionar nova pontuação,se o player novo tiver a pontuação maior substitui pelo valor e coloca na colocação exata, se for maior que todos, vai pra liderança.
void AddNewScore(const char *name, int score) {
    if (numPlayers < MAX_PLAYERS) {
        strcpy(leaderboard[numPlayers].name, name);
        leaderboard[numPlayers].score = score;
        numPlayers++;
    } else {
        // Substituir a menor pontuação se a nova pontuação for maior
        int minIndex = 0;
        for (int i = 1; i < numPlayers; i++) {
            if (leaderboard[i].score < leaderboard[minIndex].score) {
                minIndex = i;
            }
        }
        if (score > leaderboard[minIndex].score) {
            strcpy(leaderboard[minIndex].name, name);
            leaderboard[minIndex].score = score;
        }
    }
    // Ordenar o leaderboard por pontuação decrescente
    for (int i = 0; i < numPlayers - 1; i++) {
        for (int j = i + 1; j < numPlayers; j++) {
            if (leaderboard[j].score > leaderboard[i].score) {
                PLAYER temp = leaderboard[i];
                leaderboard[i] = leaderboard[j];
                leaderboard[j] = temp;
            }
        }
    }
}

void DrawLeaderboard(int screenWidth, int screenHeight) {
    int startX = screenWidth / 2 - 100;
    int startY = screenHeight / 2 - (numPlayers * 20) / 2;

    for (int i = 0; i < numPlayers; i++) {
        DrawText(leaderboard[i].name, startX, startY + i * 20, 20, WHITE);
        char scoreText[10];
        sprintf(scoreText, "%d", leaderboard[i].score);
        DrawText(scoreText, startX + 200, startY + i * 20, 20, WHITE);
    }
}

int main(void) {
    // Inicializar a janela
    int screenWidth = 800;
    int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Leaderboard Example");

    SetTargetFPS(60);

    LoadLeaderboard("leaderboard.txt");

    // Variáveis para adicionar nova pontuação
    char newPlayerName[MAX_NAME_LENGTH] = {0};
    int newPlayerScore = 0;
    bool addScore = false;
    bool nameEntered = false;
    bool scoreEntered = false;
    int charCount = 0;

    // Loop principal do jogo
    while (!WindowShouldClose()) {
        // Entrada de texto para o nome do jogador
        if (!nameEntered) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (charCount < MAX_NAME_LENGTH - 1)) {
                    newPlayerName[charCount] = (char)key;
                    newPlayerName[charCount + 1] = '\0';
                    charCount++;
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                charCount--;
                if (charCount < 0) charCount = 0;
                newPlayerName[charCount] = '\0';
            }
            if (IsKeyPressed(KEY_ENTER)) {
                nameEntered = true;
            }
        } else if (!scoreEntered) {
            if (IsKeyPressed(KEY_ZERO)) newPlayerScore = newPlayerScore * 10 + 0;
            if (IsKeyPressed(KEY_ONE)) newPlayerScore = newPlayerScore * 10 + 1;
            if (IsKeyPressed(KEY_TWO)) newPlayerScore = newPlayerScore * 10 + 2;
            if (IsKeyPressed(KEY_THREE)) newPlayerScore = newPlayerScore * 10 + 3;
            if (IsKeyPressed(KEY_FOUR)) newPlayerScore = newPlayerScore * 10 + 4;
            if (IsKeyPressed(KEY_FIVE)) newPlayerScore = newPlayerScore * 10 + 5;
            if (IsKeyPressed(KEY_SIX)) newPlayerScore = newPlayerScore * 10 + 6;
            if (IsKeyPressed(KEY_SEVEN)) newPlayerScore = newPlayerScore * 10 + 7;
            if (IsKeyPressed(KEY_EIGHT)) newPlayerScore = newPlayerScore * 10 + 8;
            if (IsKeyPressed(KEY_NINE)) newPlayerScore = newPlayerScore * 10 + 9;
            if (IsKeyPressed(KEY_BACKSPACE)) newPlayerScore /= 10;
            if (IsKeyPressed(KEY_ENTER)) {
                scoreEntered = true;
                addScore = true;
                AddNewScore(newPlayerName, newPlayerScore);
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("Leaderboard", screenWidth / 2 - MeasureText("Leaderboard", 40) / 2, 50, 40, WHITE);
        DrawLeaderboard(screenWidth, screenHeight);

        if (!nameEntered) {
            DrawText("Enter Player Name:", 10, screenHeight - 90, 20, LIGHTGRAY);
            DrawText(newPlayerName, 10, screenHeight - 60, 20, LIGHTGRAY);
        } else if (!scoreEntered) {
            DrawText("Enter Player Score:", 10, screenHeight - 90, 20, LIGHTGRAY);
            char scoreText[10];
            sprintf(scoreText, "%d", newPlayerScore);
            DrawText(scoreText, 10, screenHeight - 60, 20, LIGHTGRAY);
        }

        EndDrawing();
    }

    SaveLeaderboard("leaderboard.txt");

    CloseWindow();
    return 0;
}
