#include "raylib.h"
#include "level_loader.h"
#include <stdio.h>

int LoadLevel(const char *filename, Block *blocks, int *numBlocks, Obstacle *obstacles, int *numObstacles)
{
    FILE *file = fopen("level.txt", "r"); // Abre o arquivo para leitura
    
    if (!file) {
        printf("Erro ao abrir o arquivo de fase.\n"); // Imprime mensagem de erro se não conseguir abrir o arquivo
      
    }

    char line[256]; // Buffer para ler cada linha do arquivo
    int y = 0; // Coordenada y para posicionamento

    // Lê o arquivo linha por linha
    while (fgets(line, sizeof(line), file)) {
        int x = 0; // Coordenada x para posicionamento
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == '#') { // Se o caractere for '#', é um bloco do chão
                blocks[*numBlocks].position = (Vector2){ x * TILE_SIZE, y * TILE_SIZE };
                blocks[*numBlocks].size = (Vector2){ TILE_SIZE, TILE_SIZE };
                (*numBlocks)++;
            } else if (line[i] == 'O') { // Se o caractere for 'O', é um obstáculo
                obstacles[*numObstacles].position = (Vector2){ x * TILE_SIZE, y * TILE_SIZE };
                obstacles[*numObstacles].size = (Vector2){ TILE_SIZE, TILE_SIZE };
                obstacles[*numObstacles].speed = 5.0f; // Velocidade dos obstáculos
                (*numObstacles)++;
            }
            x++;
        }
        y++;
    }

    fclose(file); // Fecha o arquivo
    return 0; // Retorna sucesso
}
