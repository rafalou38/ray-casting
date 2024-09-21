#include "raylib.h"
#include <iostream>
#include <vector>
#include <string>

#include "Config.hpp"
#include "Light.hpp"
#include "Block.hpp"


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

    Light light = Light(Vector2{100, height / 2.0f}, RED, 2);
    Block block = Block(Vector2{800, 400}, Vector2{750, 250}, PI/4);
    Block block2 = Block(Vector2{1000, 600}, Vector2{750, 250});

    while (!WindowShouldClose())
    {
        if (IsWindowResized())
        {
            width = GetRenderWidth();
            height = GetRenderHeight();
        }

        if (GetMouseWheelMove() != 0){
            light.ray_cnt *= 1 + GetMouseWheelMove()/2;
            light.ray_cnt = std::max((int)light.ray_cnt, 1);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawText((std::to_string(GetFPS()) + " FPS" + "\n" + std::to_string(light.ray_cnt) + " rays").c_str(), 0, 0, 20, WHITE);

        light.position = GetMousePosition();

        light.update();
        light.draw();

        block.tilt += 0.01f;
        block.compute_dioptres();
        block.draw();
        block2.draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}