#include "raylib.h"
#include "game.h"
#include "level_loader.h"
#include "Obstacle.h"
// #include <stdlib.h>  // Inclui o cabeçalho stdlib.h para a função exit


GameState InitGame()
{
    GameState gameState;

    gameState.player = (Player){ { 100, screenHeight - TILE_SIZE - 50 }, { 50, 50 }, 5.0f, false, 0, 1.0f, -15.0f, 0.0f };
    gameState.numBlocks = 0;
    gameState.numObstacles = 0;

    // Carregar a fase a partir do arquivo
        LoadLevel("level.txt", gameState.blocks, &gameState.numBlocks, gameState.obstacles, &gameState.numObstacles)
        CloseWindow(); // Fecha a janela se houver erro ao carregar o arquivo
        return gameState; 
    


   
}

void UpdateGame(GameState *gameState)
{
    Player *player = &gameState->player;

    // Atualização
    if (IsKeyPressed(KEY_SPACE) && !player->isJumping) { // Verifica se a tecla de espaço foi pressionada e o jogador não está pulando
        player->isJumping = true; // Define o estado de pulo
        player->jumpSpeed = player->jumpHeight; // Define a velocidade do pulo
    }

    if (player->isJumping) { // Se o jogador estiver pulando
        player->position.y += player->jumpSpeed; // Atualiza a posição do jogador
        player->jumpSpeed += player->gravity; // Aplica a gravidade ao jogador
        player->rotation += 10.0f; // Atualiza a rotação durante o pulo

        // Verificar colisão com blocos do chão para parar o pulo
        for (int i = 0; i < gameState->numBlocks; i++) {
            if (CheckCollisionRecs(
                (Rectangle){ player->position.x, player->position.y, player->size.x, player->size.y },
                (Rectangle){ gameState->blocks[i].position.x, gameState->blocks[i].position.y, gameState->blocks[i].size.x, gameState->blocks[i].size.y }
            )) {
                // Se colidir com a parte superior do bloco, pare o pulo
                if (player->jumpSpeed > 0 && player->position.y + player->size.y > gameState->blocks[i].position.y && player->position.y < gameState->blocks[i].position.y + gameState->blocks[i].size.y) {
                    player->position.y = gameState->blocks[i].position.y - player->size.y; // Coloca o jogador em cima do bloco
                    player->isJumping = false; // Para o pulo
                    player->jumpSpeed = 0; // Reseta a velocidade do pulo
                    player->rotation = 0.0f; // Reseta a rotação ao voltar ao chão
                }
                // Se colidir com as laterais do bloco, trate a colisão
                if (player->position.x < gameState->blocks[i].position.x + gameState->blocks[i].size.x && player->position.x + player->size.x > gameState->blocks[i].position.x) {
                    if (player->position.y + player->size.y > gameState->blocks[i].position.y && player->position.y < gameState->blocks[i].position.y + gameState->blocks[i].size.y) {
                        // Colisões laterais, aqui podemos redefinir a posição do jogador ou outros efeitos
                        player->position.x = 100; // Reseta a posição do jogador como exemplo
                    }
                }
            }
        }
    }

    // Movimentação automática dos obstáculos
    for (int i = 0; i < gameState->numObstacles; i++) {
        gameState->obstacles[i].position.x -= gameState->obstacles[i].speed; // Move os obstáculos para a esquerda

        // Reposicionar o obstáculo se sair da tela
        if (gameState->obstacles[i].position.x < -gameState->obstacles[i].size.x) {
            gameState->obstacles[i].position.x = screenWidth + GetRandomValue(0, 200); // Reposiciona o obstáculo à direita da tela
        }

        // Detecta colisão com o jogador
        if (CheckCollisionRecs(
            (Rectangle){ player->position.x, player->position.y, player->size.x, player->size.y },
            (Rectangle){ gameState->obstacles[i].position.x, gameState->obstacles[i].position.y, gameState->obstacles[i].size.x, gameState->obstacles[i].size.y }
        )) {
            // Resetar o jogo ou implementar a lógica de colisão
            player->position = (Vector2){ 100, screenHeight - TILE_SIZE - 50 }; // Reseta a posição do jogador
            for (int j = 0; j < gameState->numObstacles; j++) {
                gameState->obstacles[j].position.x = screenWidth + j * 300; // Reposiciona os obstáculos
            }
        }
    }
}

void DrawGame(const GameState *gameState)
{
    DrawText("Press SPACE to jump", 10, 10, 20, DARKGRAY); // Desenha o texto de instrução

    // Desenha os blocos do chão
    for (int i = 0; i < gameState->numBlocks; i++) {
        DrawRectangleV(gameState->blocks[i].position, gameState->blocks[i].size, DARKGRAY); // Desenha cada bloco do chão
    }

    // Desenha o jogador com rotação durante o pulo
    DrawRectanglePro(
        (Rectangle){ gameState->player.position.x, gameState->player.position.y, gameState->player.size.x, gameState->player.size.y },
        (Vector2){ gameState->player.size.x / 2, gameState->player.size.y / 2 },
        gameState->player.rotation,
        BLUE
    );

    // Desenha os obstáculos
    for (int i = 0; i < gameState->numObstacles; i++) {
        DrawRectangleV(gameState->obstacles[i].position, gameState->obstacles[i].size, RED); // Desenha cada obstáculo
    }
}
