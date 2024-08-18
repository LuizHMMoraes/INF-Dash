#ifndef GAMEPLAY_H_INCLUDED
#define GAMEPLAY_H_INCLUDED

#define TILE_SIZE 60
#define MAX_BLOCKS 3000
#define MAX_OBSTACLES 300

typedef struct Player {
    Rectangle rect;
    float speed;
    float jumpSpeed;
    bool isJumping;
    float gravity;
    float jumpHeight;
    float rotation;
    int attempts;
    int score;
} Player;

typedef struct Block {
    Rectangle rect;
} Block;

typedef struct Obstacle {
    Rectangle rect;
} Obstacle;

typedef struct GameState {
    Player player;
    Block blocks[MAX_BLOCKS];
    Obstacle obstacles[MAX_OBSTACLES];
    int numBlocks;
    int numObstacles;
    Camera2D camera;
} GameState;

GameState InitGame();
void UpdateGame(GameState *gameState);
void DrawGame(const GameState *gameState, Texture2D background, Texture2D playerTexture, Texture2D blockTexture, Texture2D obstacleTexture);
bool CheckCollision(Player *player, Block *blocks, int numBlocks, Obstacle *obstacles, int numObstacles);

// Função para carregar a fase a partir de um arquivo
int LoadMap(const char *filename, Block *blocks, int *numBlocks, Obstacle *obstacles, int *numObstacles, Player *player);

#endif // GAMEPLAY_H_INCLUDED
