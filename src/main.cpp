#include "raylib.h"
#include <iostream>
#include <vector>
#include <string>

#include "Config.hpp"
#include "Light.hpp"
#include "Block.hpp"

int main(int argc, char *argv[])
{

    int width = 1500;
    int height = 450;

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(width, height, "GravitX");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowState(FLAG_WINDOW_MAXIMIZED);
    SetTargetFPS(0);

    width = GetRenderWidth();
    height = GetRenderHeight();

    LightRay::ray_cnt = 0;

    Scene main_scene = Scene();

    Light *light = new Light(&main_scene, Vector2{100, height / 2.0f}, RED, 5);
    main_scene.add_light(light);

    Block *block = new Block(&main_scene, Vector2{200, 200}, Vector2{700, 250}, PI / 4, 1.3);
    main_scene.add_block(block);


    while (!WindowShouldClose())
    {
#if DEBUG
        printf("\033[2J\033[1;1HFrame\n");
#endif
        if (IsWindowResized())
        {
            width = GetRenderWidth();
            height = GetRenderHeight();

            // float cell_height = 40;
            // for (int i = 0; i < (height - 100) / cell_height; i++)
            // {
            //     main_scene.add_block(new Block(&main_scene, Vector2{0, 50 + (float)i * (cell_height - 1)}, Vector2{(float)width, cell_height}, 0, 1.0 + 0.1 * i));
            // }
        }

        // -------------------------

        if (GetMouseWheelMove() != 0)
        {
            if (GetMouseWheelMove() > 0 && light->ray_cnt == 1)
            {
                light->ray_cnt++;
            }
            else
            {
                light->ray_cnt *= 1 + GetMouseWheelMove() / 2;
                light->ray_cnt = std::max((int)light->ray_cnt, 1);
            }
        }

        // if (IsKeyDown(KEY_LEFT))
        //     block->tilt += 0.02f;
        // if (IsKeyDown(KEY_RIGHT))
        //     block->tilt -= 0.02f;
        if (IsKeyPressed(KEY_I))
        {
            light->stale = true;
            light->infinity = !light->infinity;
        }
        light->position = GetMousePosition();

        // --------------------
        BeginDrawing();

        main_scene.update_block();
        main_scene.update_lights();

        ClearBackground(BLACK);

        main_scene.draw();

        DrawText((std::to_string(GetFPS()) + " FPS" + "\n" + std::to_string(LightRay::ray_cnt) + " rays").c_str(), 0, 0, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}