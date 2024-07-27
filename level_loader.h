#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include "game.h"

// Função para carregar a fase a partir de um arquivo
int LoadLevel(const char *filename, Block *blocks, int *numBlocks, Obstacle *obstacles, int *numObstacles);

#endif // LEVEL_LOADER_H
