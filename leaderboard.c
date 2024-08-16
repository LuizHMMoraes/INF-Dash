// Leaderboard.c

#include "raylib.h"
#include "Leaderboard.h"
#include <stdio.h>

// Função para desenhar o leaderboard na tela
void DrawLeaderboard(LeaderboardEntry leaderboard[], int numEntries) {
    DrawText("LEADERBOARD", 20, 20, 40, DARKGRAY);

    for (int i = 0; i < numEntries; i++) {
        char entry[100];
        sprintf(entry, "%d. %s - %d", i + 1, leaderboard[i].name, leaderboard[i].score);
        DrawText(entry, 100, 100 + 30 * i, 20, DARKGRAY);
    }

    DrawText("PRESS ENTER to RETURN", 100, 500, 20, DARKGRAY);
}

// Função para carregar o leaderboard de um arquivo binário
void LoadLeaderboard(LeaderboardEntry leaderboard[], int *numEntries) {
    FILE *file = fopen("top5.bin", "rb");

    if (file == NULL) {
        *numEntries = 0;
        return;
    }

    *numEntries = fread(leaderboard, sizeof(LeaderboardEntry), MAX_LEADERBOARD_ENTRIES, file);

    fclose(file);
}

// Função para salvar o leaderboard em um arquivo binário
void SaveLeaderboard(LeaderboardEntry leaderboard[], int numEntries) {
    FILE *file = fopen("top5.bin", "wb");

    if (file != NULL) {
        fwrite(leaderboard, sizeof(LeaderboardEntry), numEntries, file);
        fclose(file);
    }
}
