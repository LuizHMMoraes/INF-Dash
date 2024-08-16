// Leaderboard.h

#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#define MAX_LEADERBOARD_ENTRIES 5

// Estrutura para armazenar uma entrada do leaderboard
typedef struct {
    char name[20];
    int score;
} LeaderboardEntry;

// Declarações das funções
void DrawLeaderboard(LeaderboardEntry leaderboard[], int numEntries);
void LoadLeaderboard(LeaderboardEntry leaderboard[], int *numEntries);
void SaveLeaderboard(LeaderboardEntry leaderboard[], int numEntries);

#endif
