#include "raylib.h"
#include <iostream>
#include <vector>
#include <string>

#include "Light.hpp"


int main(int argc, char *argv[])
{

    int width = 800;
    int height = 450;

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(width, height, "GravitX");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowState(FLAG_WINDOW_MAXIMIZED);
    SetTargetFPS(60);

    width = GetRenderWidth();
    height = GetRenderHeight();

    Light light = Light(Vector2{width / 2.0f, height / 2.0f}, RED, 100);

    while (!WindowShouldClose())
    {
        if (IsWindowResized())
        {
            width = GetRenderWidth();
            height = GetRenderHeight();
        }

        if (GetMouseWheelMove() != 0){
            light.ray_cnt += GetMouseWheelMove();
            light.ray_cnt = std::max((int)light.ray_cnt, 1);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawText(std::to_string(GetFPS()).c_str(), 0, height - 10, 6, WHITE);

        light.position = GetMousePosition();

        light.update();
        light.draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}