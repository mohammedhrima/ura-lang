#include "raylib.h"

int main() {
    InitWindow(800, 600, "Ura Raylib");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){30, 30, 150, 255});
        DrawRectangle(350, 250, 100, 100, (Color){255, 220, 0, 255});
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
