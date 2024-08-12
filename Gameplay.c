#include "raylib.h"
#include "Gameplay.h"
#include <stdio.h>

#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 1200

GameState InitGame()
{
    GameState gameState;

    //Carrega a textura do jogador
    Image playerImage = LoadImage("tiles/player.png");
    Texture2D playerTexture = LoadTextureFromImage(playerImage);

    gameState.player = (Player)
    {
        { 100, SCREEN_HEIGHT - TILE_SIZE, TILE_SIZE, TILE_SIZE }, 5.0f, 0, false, 1.0f, -20.0f, 0.0f, 3
    };
    gameState.numBlocks = 0;
    gameState.numObstacles = 0;
    gameState.playerTexture = playerTexture;

    // Inicializa a câmera
    gameState.camera = (Camera2D)
    {
        { gameState.player.rect.x + gameState.player.rect.width, gameState.player.rect.y + gameState.player.rect.height }, // Alinha a câmera com o jogador
        { SCREEN_WIDTH, SCREEN_HEIGHT }, // Define a câmera para cobrir a área da tela
        0.0f, // Sem rotação
        1.0f // Sem zoom
    };

    //Carregar a fase a partir do arquivo
    LoadMap("level.txt", gameState.blocks, &gameState.numBlocks, gameState.obstacles, &gameState.numObstacles, &gameState.player);
    return gameState;
}

void ResetPlayer(GameState *gameState)
{
    // Reinicializa a posição do jogador e incrementa o contador de tentativas
    gameState->player.rect = (Rectangle)
    {
        100, SCREEN_HEIGHT - TILE_SIZE * 3, TILE_SIZE, TILE_SIZE
    };
    gameState->player.isJumping = false;
    gameState->player.jumpSpeed = 0;
    gameState->player.rotation = 0;
    gameState->player.attempts--;
}

bool CheckCollision(Player *player, Block *blocks, int numBlocks, Obstacle *obstacles, int numObstacles)
{
    for (int i = 0; i < numBlocks; i++)
    {
        if (CheckCollisionRecs(player->rect, blocks[i].rect))
        {
            return true;
        }
    }

    for (int i = 0; i < numObstacles; i++)
    {
        if (CheckCollisionRecs(player->rect, obstacles[i].rect))
        {
            return true;
        }
    }

    return false;
}

void UpdateGame(GameState *gameState)
{
    Player *player = &gameState->player;

    // Atualização
    if (IsKeyPressed(KEY_SPACE) && !player->isJumping)   // Verifica se a tecla de espaço foi pressionada e o jogador não está pulando
    {
        player->isJumping = true; // Define o estado de pulo
        player->jumpSpeed = player->jumpHeight; // Define a velocidade do pulo
    }

    // Atualizar posição do jogador
    player->rect.x += player->speed;

    if (player->isJumping)   // Se o jogador estiver pulando
    {
        player->rect.y += player->jumpSpeed; // Atualiza a posição do jogador
        player->jumpSpeed += player->gravity; // Aplica a gravidade ao jogador
        player->rotation += 7.5f; // Atualiza a rotação durante o pulo
    }

    // Verificar colisão com blocos do chão e obstáculos
    if (CheckCollision(player, gameState->blocks, gameState->numBlocks, gameState->obstacles, gameState->numObstacles))
    {
        for (int i = 0; i < gameState->numBlocks; i++)
        {
            bool isOnGround = false;
            // Se colidir com a parte superior do bloco, pare o pulo
            if ( player->isJumping && player->rect.y + player->rect.height > gameState->blocks[i].rect.y)
            {
                player->rect.y = gameState->blocks[i].rect.y - player->rect.height; // Coloca o jogador em cima do bloco
                player->isJumping = false; // Para o pulo
                player->jumpSpeed = 0; // Reseta a velocidade do pulo
                player->rotation = 0.0f; // Reseta a rotação ao voltar ao chão
                isOnGround = true;
            }

            if(player->rect.y + player->rect.height > gameState->blocks[i].rect.y)
            {
                isOnGround = true;
            }

            // Se colidir com as laterais de um bloco ou obstáculo, trata a colisão
            if (!isOnGround &&player->rect.x + player->rect.width > gameState->blocks[i].rect.x
                    &&player->rect.x < gameState->blocks[i].rect.x)
            {
                ResetPlayer(gameState); // Reseta o jogador para a posição inicial
            }
        }
    }

    else if(!player->isJumping)
    {
        // Se o jogador não está pulando e não está colidindo com o chão, aplique uma queda
        player->rect.y += player->gravity; // Modificador para simular a queda
    }

    // Atualizar a posição da câmera para seguir o jogador
    gameState->camera.target = (Vector2)
    {
        player->rect.x + player->rect.width, 600
    };

    // Limitar a posição vertical do jogador para que ele não saia da tela
    if (player->rect.y > SCREEN_HEIGHT - TILE_SIZE)
    {
        player->rect.y = SCREEN_HEIGHT - TILE_SIZE;
        player->isJumping = false;
        player->jumpSpeed = 0;
        player->rotation = 0.0f;
    }

}

int LoadMap(const char *filename, Block *blocks, int *numBlocks, Obstacle *obstacles, int *numObstacles, Player *player)
{
    FILE *file = fopen(filename, "r"); // Abre o arquivo para leitura

    if (!file)
    {
        printf("Erro ao abrir o arquivo de fase.\n"); // Imprime mensagem de erro se não conseguir abrir o arquivo
        return -1; // Retorna erro
    }

    char line[256]; // Buffer para ler cada linha do arquivo
    int y = 0; // Coordenada y para posicionamento

    // Inicializa contadores
    *numBlocks = 0;
    *numObstacles = 0;
    bool playerSet = false; // Verifica se o jogador já foi posicionado

    // Lê o arquivo linha por linha até o fim do arquivo ou até que 10 linhas tenham sido lidas
    while (fgets(line, sizeof(line), file) && y < 10)
    {
        int x = 0; // Coordenada x para posicionamento
        for (int i = 0; line[i] != '\0' && x < 300; i++) // Limita a leitura a 300 colunas
        {
            if (line[i] == 'B') // Se o caractere for 'B', é um bloco do chão
            {
                if (*numBlocks >= MAX_BLOCKS)
                {
                    printf("Número máximo de blocos excedido.\n");
                    fclose(file);
                    return -1;
                }
                blocks[*numBlocks].rect = (Rectangle)
                {
                    x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE
                };
                (*numBlocks)++;
            }
            else if (line[i] == 'O') // Se o caractere for 'O', é um obstáculo
            {
                if (*numObstacles >= MAX_OBSTACLES)
                {
                    printf("Número máximo de obstáculos excedido.\n");
                    fclose(file);
                    return -1;
                }
                obstacles[*numObstacles].rect = (Rectangle)
                {
                    x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE
                };
                (*numObstacles)++;
            }
            else if (line[i] == 'P' && !playerSet) // Se o caractere for 'P', é o local de nascimento do jogador
            {
                player->rect = (Rectangle)
                {
                    x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE
                };
                playerSet = true; // Marca que o jogador foi posicionado
            }
            x++;
        }
        y++;
    }

    fclose(file); // Fecha o arquivo
    return 0; // Retorna sucesso
}

void DrawGame(const GameState *gameState)
{
    BeginMode2D(gameState->camera);

    // Desenha blocos
    for (int i = 0; i < gameState->numBlocks; i++)
    {
        DrawRectangleRec(gameState->blocks[i].rect, BLUE);
    }

    // Desenha o jogador com rotação durante o pulo
    DrawTexturePro(
        gameState->playerTexture,
        (Rectangle)
    {
        0, 0, gameState->playerTexture.width, gameState->playerTexture.height
    },
    (Rectangle)
    {
        gameState->player.rect.x, gameState->player.rect.y, gameState->player.rect.width, gameState->player.rect.height
    },
    (Vector2)
    {
        gameState->player.rect.width, gameState->player.rect.height
    },
    gameState->player.rotation,
    WHITE
    );

    // Desenha o jogador
    DrawRectangleRec(gameState->player.rect, GREEN);

    // Desenha obstáculos
    for (int i = 0; i < gameState->numObstacles; i++)
    {
        DrawRectangleRec(gameState->obstacles[i].rect, RED);
    }

    EndMode2D();

    // Desenha o contador de tentativas
    DrawText(TextFormat("Tentativas: %d", gameState->player.attempts), 10, 10, 20, BLACK);
}
