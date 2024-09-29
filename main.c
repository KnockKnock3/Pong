#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <math.h>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define BALL_SIZE 10
#define FPS 60

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

typedef struct Ball {
    Rectangle rect;
    Vector2 velocity;
    float speed;
    float lastScoreHit;
} Ball;

typedef enum GameState {
    PLAYING,
    GAME_OVER
} GameState;

void ResetBall(Ball *ball) {
    ball->rect = (Rectangle){WINDOW_WIDTH / 2 - BALL_SIZE, WINDOW_HEIGHT / 2 - BALL_SIZE, 2 * BALL_SIZE, 2 * BALL_SIZE};
    ball->velocity = Vector2Normalize((Vector2){1, 1});
    ball->speed = 500;
}

float playerSpeed = 300;

int main() {

    Rectangle playerPaddle = {10, 100, 30, 200};
    Ball ball = {0};
    ResetBall(&ball);

    GameState gameState = PLAYING;
    int score = 0;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pong");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        float deltaTime = GetFrameTime();

        if (IsKeyDown(KEY_DOWN)) {
            playerPaddle.y = MIN(WINDOW_HEIGHT - playerPaddle.height, playerPaddle.y + deltaTime * playerSpeed);
        }
        if (IsKeyDown(KEY_UP)) {
            playerPaddle.y = MAX(0, playerPaddle.y - deltaTime * playerSpeed);
        }
        if (IsKeyPressed(KEY_R)) {
            score = 0;
            ResetBall(&ball);
        }

        ball.rect.x = ball.rect.x + deltaTime * ball.velocity.x * ball.speed;
        ball.rect.y = ball.rect.y + deltaTime * ball.velocity.y * ball.speed;
        ball.lastScoreHit = MAX(0, ball.lastScoreHit - deltaTime);

        if (ball.rect.y <= 0) {
            ball.velocity.y = -ball.velocity.y;
        } else if (ball.rect.y >= WINDOW_HEIGHT - ball.rect.height) {
            ball.velocity.y = -ball.velocity.y;
        }
        if (ball.rect.x >= WINDOW_WIDTH - ball.rect.width) {
            ball.velocity.x = -ball.velocity.x;
        }

        Rectangle playerPaddleMain = (Rectangle){playerPaddle.x + playerPaddle.width - 10, playerPaddle.y, 10, playerPaddle.height};
        Rectangle playerPaddleTop = (Rectangle){playerPaddle.x, playerPaddle.y, playerPaddle.width, 10};
        Rectangle playerPaddleBottom = (Rectangle){playerPaddle.x, playerPaddle.y - playerPaddle.height - 10, playerPaddle.width, 10};

        if (CheckCollisionRecs(ball.rect, playerPaddleMain) && ball.rect.x > playerPaddleMain.x) {
            ball.velocity.x = fabs(ball.velocity.x);
            if (ball.lastScoreHit == 0) {
                ball.speed = ball.speed * 1.1f;
                ball.lastScoreHit = 0.2f;
                score++;
            }
        } else if (CheckCollisionRecs(ball.rect, playerPaddleTop)) {
            ball.velocity.y = -fabs(ball.velocity.y);
        } else if (CheckCollisionRecs(ball.rect, playerPaddleBottom)) {
            ball.velocity.y = fabs(ball.velocity.y);
        }

        if (ball.rect.x < 0) {
            score = 0;
            ResetBall(&ball);
        }

        BeginDrawing();

            ClearBackground(BLACK);

            DrawRectangleRec(playerPaddle, WHITE);
            DrawRectangleRec(ball.rect, WHITE);

            if (gameState == GAME_OVER) {
                DrawText("Game Over", 200, 200, 20, RED);
            }
            
            char *scoreText = TextFormat("%d", score);
            DrawText(scoreText, WINDOW_WIDTH/2 - MeasureText(scoreText, 40), 50, 40, WHITE);
            DrawFPS(0, 0);

        EndDrawing();

    }

    return 0;
}