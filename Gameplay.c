#include "raylib.h"
#include "Gameplay.h"
#include <stdio.h>

#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 1200

GameState InitGame()
{
    GameState gameState;

    gameState.player = (Player)
    {
        { 100, SCREEN_HEIGHT - TILE_SIZE, TILE_SIZE, TILE_SIZE }, 5.0f, 0, false, 1.0f, -20.0f, 0.0f, 3, 0
    };
    gameState.numBlocks = 0;
    gameState.numObstacles = 0;

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

// Função para calcular a posição futura do jogador
Rectangle CalculateFuturePosition(const Rectangle *currentRect, float speed, float jumpSpeed, float gravity, float deltaTime, bool isJumping)
{
    Rectangle futureRect = *currentRect;
    futureRect.x += speed * deltaTime; // Calcula o deslocamento horizontal
    futureRect.y += (isJumping ? jumpSpeed : 0) * deltaTime; // Calcula o deslocamento vertical

    if (isJumping)
    {
        // Aplica a gravidade se o jogador está pulando
        futureRect.y += gravity * deltaTime;
    }

    return futureRect;
}

// Função para verificar colisão com a posição futura de blocos
bool CheckFutureBlockCollision(const Rectangle *futureRect, Block *blocks, int numBlocks)
{
    for (int i = 0; i < numBlocks; i++)
    {
        if (CheckCollisionRecs(*futureRect, blocks[i].rect))
        {
            return true; // Colidiu com um bloco
        }
    }

    return false; // Sem colisão
}

// Função para verificar colisão com a posição futura de obstáculos
bool CheckFutureObstacleCollision(const Rectangle *futureRect, Obstacle *obstacles, int numObstacles)
{
    for (int i = 0; i < numObstacles; i++)
    {
        if (CheckCollisionRecs(*futureRect, obstacles[i].rect))
        {
            return true; // Colidiu com um obstáculo
        }
    }

    return false; // Sem colisão
}

void UpdateGame(GameState *gameState)
{
    Player *player = &gameState->player;
    float deltaTime = 50 * GetFrameTime(); // Obtém o tempo do último quadro

    // Calcula a posição futura do jogador
    Rectangle futureRect = CalculateFuturePosition(&player->rect, player->speed, player->jumpSpeed, player->gravity, deltaTime, player->isJumping);

    // Verifica colisão com a posição futura
    bool collisionFutureBlock = CheckFutureBlockCollision(&futureRect, gameState->blocks, gameState->numBlocks);

    bool collisionFutureObstacle = CheckFutureObstacleCollision(&futureRect, gameState->obstacles, gameState->numObstacles);

    // Atualização
    if (IsKeyPressed(KEY_SPACE) && !player->isJumping)
    {
        player->isJumping = true;
        player->jumpSpeed = player->jumpHeight;
    }

    //Se Colide com obstculo reseta o jogador
    if(collisionFutureObstacle)
    {
        ResetPlayer(gameState);
    }

    // Atualiza a posição do jogador
    if (!collisionFutureBlock)
    {
        player->score ++;
        //OBS: se der tempo refatorar (criar uma função "MovePlayer") para não haver duplicação de código
        player->rect.x += player->speed * deltaTime;
        if (player->isJumping)
        {
            player->rect.y += player->jumpSpeed * deltaTime;
            player->jumpSpeed += player->gravity * deltaTime;
            player->rotation += 7.5f * deltaTime;
        }
        else
        {
            // Caso não esteja pulando, aplica a gravidade
            bool onGround = false;
            for (int i = 0; i < gameState->numBlocks; i++)
            {
                Rectangle futureRectGround = { player->rect.x, player->rect.y + player->rect.height + player->gravity * deltaTime, player->rect.width, player->rect.height };

                if (CheckCollisionRecs(futureRectGround, gameState->blocks[i].rect))
                {
                    //OBS: se der tempo refatorar esse bloco de código para uma função "PutPlayerOnBlock" para evitar duplicação de código
                    player->rect.y = gameState->blocks[i].rect.y - player->rect.height; // Coloca o jogador em cima do bloco
                    player->isJumping = false; // Para o pulo
                    player->jumpSpeed = 0; // Reseta a velocidade do pulo
                    player->rotation = 0.0f; // Reseta a rotação ao voltar ao chão
                    onGround = true;
                    break;
                }
            }

            // Se não estiver no chão, aplica a gravidade
            if (!onGround)
            {
                player->rect.y += player->gravity * deltaTime * 10;
            }
        }
    }
    else if(collisionFutureBlock && player->isJumping)
    {
        // Coloca o jogador em cima do bloco se a colisão futura for durante um pulo
        for (int i = 0; i < gameState->numBlocks; i++)
        {
            if (CheckCollisionRecs((Rectangle)
        {
            futureRect.x, futureRect.y + player->rect.height, player->rect.width, player->rect.height
        }, gameState->blocks[i].rect))
            {
                //OBS: se der tempo refatorar esse bloco de código para uma função "PutPlayerOnBlock" para evitar duplicação de código
                player->rect.y = gameState->blocks[i].rect.y - player->rect.height; // Coloca o jogador em cima do bloco
                player->isJumping = false; // Para o pulo
                player->jumpSpeed = 0; // Reseta a velocidade do pulo
                player->rotation = 0.0f; // Reseta a rotação ao voltar ao chão
                break;
            }
        }
    }
    else
    {
        // Caso Colisão futura quando não está pulando
        for (int i = 0; i < gameState->numBlocks; i++)
        {
            if (CheckCollisionRecs((Rectangle)
        {
            futureRect.x, futureRect.y, player->rect.width, player->rect.height
        }, gameState->blocks[i].rect))
            {
                ResetPlayer(gameState);
                break;
            }
        }
    }

    // Atualiza a posição da câmera
    gameState->camera.target = (Vector2)
    {
        player->rect.x + player->rect.width / 2, 600
    };

    // Limita a posição vertical do jogador
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

void DrawGame(const GameState *gameState, Texture2D background, Texture2D playerTexture, Texture2D blockTexture, Texture2D obstacleTexture)
{
    BeginMode2D(gameState->camera);

    float deltaTime = GetFrameTime(); // Obtém o tempo do último quadro

    // Calcule a posição da textura do background com base na posição da câmera e o deltaTime
    float bgX = fmodf(gameState->camera.target.x * deltaTime, background.width);
    float bgY = fmodf(gameState->camera.target.y * deltaTime, background.height);

    // Desenha o background em um loop para cobrir toda a fase, considerando a matriz
    int numHorizontalTiles = ceilf((300 * TILE_SIZE) / background.width); // Número de vezes que o background deve se repetir horizontalmente
    int numVerticalTiles = ceilf((10 * TILE_SIZE) / background.height);   // Número de vezes que o background deve se repetir verticalmente

    for (int y = -1; y <= numVerticalTiles; y++)
    {
        for (int x = -1; x <= numHorizontalTiles; x++)
        {
            DrawTexture(background, bgX + x * background.width, bgY + y * background.height, WHITE);
        }
    }

    for (int i = 0; i < gameState->numBlocks; i++)
    {
        // Obtém o bloco atual
        Block *block = &gameState->blocks[i];

        // Cria um retângulo para a textura correspondente ao tamanho do bloco
        Rectangle sourceRec = { 0.0f, 0.0f, (float)blockTexture.width, (float)blockTexture.height };

        // Cria um retângulo para desenhar a textura no bloco
        Rectangle destRec = { block->rect.x, block->rect.y, block->rect.width, block->rect.height };

        // Desenha a textura do bloco usando DrawTexturePro para ajustar o tamanho
        DrawTexturePro(blockTexture, sourceRec, destRec, (Vector2)
        {
            0.0f, 0.0f
        }, 0.0f, WHITE);
    }

// Desenha obstáculos com a textura extendida
    for (int i = 0; i < gameState->numObstacles; i++)
    {
        // Obtém o obstáculo atual
        Obstacle *obstacle = &gameState->obstacles[i];

        // Cria um retângulo para a textura correspondente ao tamanho do obstáculo
        Rectangle sourceRec = { 0.0f, 0.0f, (float)obstacleTexture.width, (float)obstacleTexture.height };

        // Cria um retângulo para desenhar a textura no obstáculo
        Rectangle destRec = { obstacle->rect.x, obstacle->rect.y, obstacle->rect.width, obstacle->rect.height };

        // Desenha a textura do obstáculo usando DrawTexturePro para ajustar o tamanho
        DrawTexturePro(obstacleTexture, sourceRec, destRec, (Vector2)
        {
            0.0f, 0.0f
        }, 0.0f, WHITE);
    }

    // Define o ponto de origem da rotação (centro da imagem)
    Vector2 origin = (Vector2)
    {
        gameState->player.rect.width / 2, gameState->player.rect.height / 2
    };

    // Desenha o jogador com a textura, aplicando a rotação durante o pulo
    DrawTexturePro(
        playerTexture, // Textura do jogador
        (Rectangle)
    {
        0, 0,
        (float)playerTexture.width,  // Largura da textura
        (float)playerTexture.height  // Altura da textura
    },
    (Rectangle)
    {
        gameState->player.rect.x + gameState->player.rect.width / 2,  // Centro X do jogador
                  gameState->player.rect.y + gameState->player.rect.height / 2, // Centro Y do jogador
                  gameState->player.rect.width,  // Largura do retângulo de destino
                  gameState->player.rect.height  // Altura do retângulo de destino
    },
    origin,  // Ponto de origem da rotação
    gameState->player.rotation,  // Ângulo de rotação
    WHITE  // Cor para aplicar à textura (branca para usar a original)
    );

    EndMode2D();

    //Desenha o contador de tentativas
    DrawText(TextFormat("Tentativas: %d", gameState->player.attempts), 10, 10, 20, BLACK);

    //Desenha o contador de Score
    DrawText(TextFormat("Score: %d", gameState->player.score), 200, 10, 20, BLACK);
}
