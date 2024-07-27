#ifndef OBSTACLE_H
#define OBSTACLE_H
#include "raylib.h"

typedef struct Obstacle {
    Vector2 position;
    Vector2 size;
    float speed;
} Obstacle;

#endif // OBSTACLE_H
