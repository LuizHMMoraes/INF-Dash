#ifndef GAME_H
#define GAME_H
#include "raylib.h"


#define MAX_BLOCKS 100
#define MAX_OBSTACLES 100
#define TILE_SIZE 50
#define screenWidth 1200
#define screenHeight 600

typedef struct Player {
    Vector2 position;
    Vector2 size;
    float speed;
    bool isJumping;
    float jumpSpeed;
    float gravity;
    float jumpHeight;
    float rotation;
} Player;

typedef struct Block {
    Vector2 position;
    Vector2 size;
} Block;

typedef struct Obstacle {
    Vector2 position;
    Vector2 size;
    float speed;
} Obstacle;

typedef struct GameState {
    Player player;
    Block blocks[MAX_BLOCKS];
    Obstacle obstacles[MAX_OBSTACLES];
    int numBlocks;
    int numObstacles;
} GameState;
typedef struct Square {
    int x;
    int y;
    int lado;
    int velocidadeHorizontal;
    int velocidadeVertical;
    bool pulando;
} Square;

GameState InitGame();
void UpdateGame(GameState *gameState);
void DrawGame(const GameState *gameState);

#endif // GAME_H
