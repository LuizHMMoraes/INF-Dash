#include "raylib.h"

typedef struct Obstacle {
    int x;
    int y;
    int width;
    int height;
    int speed;
} Obstacle;

typedef struct Quadrado {
    int x;
    int y;
    int lado;
    int velocidade;
    int velocidadeVertical;
    bool pulando;
} Quadrado;

#define NUM_OBSTACLES 10

int main(void) {
    const int Larguratela = 800; // screenwidth
    const int Alturatela = 450;  // screenheight
    
    InitWindow(Larguratela, Alturatela, "Rapper Slide");
    
    SetTargetFPS(60);
    
    // Inicializa o quadrado
    Quadrado quadrado = { 0, Alturatela - 50, 50, 2, 0, false };
    
    // Carrega textura de fundo
    Texture2D backgroundTexture = LoadTexture("fundo.png");
    
    // Variáveis para o pulo
    int velocidadedopulo = -15;
    int gravidade = 1;

    // Obstáculos
    Obstacle obstacles[NUM_OBSTACLES];
    for (int i = 0; i < NUM_OBSTACLES; i++) {
        obstacles[i].width = 20;
        obstacles[i].height = 40;
        obstacles[i].x = Larguratela + i * 100; // espaçamento inicial entre os obstáculos
        obstacles[i].y = Alturatela - obstacles[i].height;
        obstacles[i].speed = 4;
    }

    // Loop principal do jogo
    while (!WindowShouldClose()) {
        // Atualiza a posição do quadrado no eixo X
        quadrado.x += quadrado.velocidade;
       
        // Se o quadrado sair da tela, retorna ao início
        if (quadrado.x > Larguratela) {
            quadrado.x = -quadrado.lado;
        }

        // Mecânica de pulo
        if (quadrado.pulando) {
            quadrado.velocidadeVertical += gravidade; // Aplica a gravidade
            quadrado.y += quadrado.velocidadeVertical; // Atualiza a posição Y do quadrado

            // Verifica se o quadrado aterrissou no chão
            if (quadrado.y >= Alturatela - quadrado.lado) {
                quadrado.y = Alturatela - quadrado.lado; // Mantém o quadrado no chão
                quadrado.pulando = false; // Termina o pulo
                quadrado.velocidadeVertical = 0; // Reseta a velocidade vertical
            }
        } else {
            // Inicia o pulo se a tecla espaço for pressionada
            if (IsKeyPressed(KEY_SPACE)) {
                quadrado.pulando = true;
                quadrado.velocidadeVertical = velocidadedopulo;
            }
        }

        // Atualiza a posição dos obstáculos
        for (int i = 0; i < NUM_OBSTACLES; i++) {
            obstacles[i].x -= obstacles[i].speed;

            // Se o obstáculo sair da tela, reposiciona-o à direita
            if (obstacles[i].x < -obstacles[i].width) {
                obstacles[i].x = Larguratela;
            }

            // Detecta colisão com o quadrado
            if (CheckCollisionRecs((Rectangle){quadrado.x, quadrado.y, quadrado.lado, quadrado.lado}, 
                                   (Rectangle){obstacles[i].x, obstacles[i].y, obstacles[i].width, obstacles[i].height})) {
                // Se colidir, resetar o jogo (ou qualquer outra lógica de colisão)
                quadrado.x = 0;
                quadrado.y = Alturatela - quadrado.lado;
                quadrado.pulando = false;
                quadrado.velocidadeVertical = 0;
                for (int j = 0; j < NUM_OBSTACLES; j++) {
                    obstacles[j].x = Larguratela + j * 200;
                }
            }
        }

        // Inicia o desenho
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(backgroundTexture, 0, 0, WHITE);

        DrawText("ABRINDO A PRIMEIRA JANELA NA RAÇA", 190, 200, 20, LIGHTGRAY);

        // Desenha o quadrado
        DrawRectangle(quadrado.x, quadrado.y, quadrado.lado, quadrado.lado, BLUE);

        // Desenha os obstáculos
        for (int i = 0; i < NUM_OBSTACLES; i++) {
            DrawRectangle(obstacles[i].x, obstacles[i].y, obstacles[i].width, obstacles[i].height, RED);
        }

        // Termina o desenho
        EndDrawing();
    }
    
    // Fecha a janela e limpa os recursos
    UnloadTexture(backgroundTexture);
    CloseWindow();
    
    return 0;
}
