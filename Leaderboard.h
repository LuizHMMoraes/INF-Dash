// Leaderboard.h

#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#define MAX_LEADERBOARD_ENTRIES 5
#define MAX_NAME_LENGTH 20

// Estrutura para armazenar uma entrada do leaderboard
typedef struct {
    char name[20];
    int score;
} LeaderboardEntry;

// Declarações das funções
void DrawLeaderboard(LeaderboardEntry leaderboard[]);
void LoadLeaderboard(LeaderboardEntry leaderboard[]);
void SaveLeaderboard(LeaderboardEntry leaderboard[]);

LeaderboardEntry PlayerEntry(int score);
void Ranking(LeaderboardEntry leaderboard[], LeaderboardEntry newEntry);
#endif
