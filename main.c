#include "raylib.h"
#include <stdio.h>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

int main() {

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pong");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        float deltaTime = GetFrameTime();

        BeginDrawing();

            ClearBackground(BLACK);

            DrawFPS(0, 0);
            DrawText("Hello World", 100, 100, 20, RED);

        EndDrawing();

    }

    return 0;
}