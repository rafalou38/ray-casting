#include "raylib.h"
#include <iostream>
#include <vector>
#include <string>

#include "Config.hpp"
#include "Light.hpp"
#include "Block.hpp"

void config_lens(Scene *main_scene)
{
    float refractive_index = 1.2; // Fixed refractive index for the lens
    float lens_opacity = 0.3;     // Opacity for lens blocks

    float lens_height = 100; // Total height of the lens
    float lens_width = 100;  // Total width of the lens

    // Center of the lens
    // float lens_center_y = 300;

    float steps = 45;
    for (float y = 0; y <= 1; y += 1 / steps)
    {
        float cell_height = lens_height / steps; // Height of each segment
        float w = sqrt(1 - y * y);

        Vector2 pos = Vector2{600, 800};

        pos.y -= y * (lens_height - 10);
        pos.x -= w * lens_width;
        // Add the block to the scene
        main_scene->add_block(new Block(
            main_scene,
            pos,
            Vector2{w * 2 * lens_width, cell_height},
            0,
            refractive_index,
            ColorAlpha(WHITE, lens_opacity)));

        float yy = y + 1 / steps;
        float ww = sqrt(1 - yy * yy);


        float t_to_next = sqrt(pow((w - ww) * lens_width, 2) + pow(cell_height, 2));
        main_scene->add_block(new Block(
            main_scene,
            pos,
            Vector2{t_to_next, cell_height},
            -atan2(cell_height, ((w - ww) * lens_width)),
            refractive_index,
            ColorAlpha(WHITE, lens_opacity)));
        pos.y -= cell_height;
        pos.x = 600 + ww * lens_width;
        main_scene->add_block(new Block(
            main_scene,
            pos,
            Vector2{t_to_next, cell_height},
            PI/2 - atan2(((w - ww) * lens_width), cell_height),
            refractive_index,
            ColorAlpha(WHITE, lens_opacity)));
    }
}

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

    // Block *block = new Block(&main_scene, Vector2{200, 200}, Vector2{700, 250}, PI / 4, 4);
    // main_scene.add_block(block);
    config_lens(&main_scene);

    while (!WindowShouldClose())
    {
#if DEBUG
        printf("\033[2J\033[1;1HFrame\n");
#endif
        if (IsWindowResized())
        {
            width = GetRenderWidth();
            height = GetRenderHeight();

            // float cell_height = 10;
            // for (int i = 0; i < (height - 100) / cell_height; i++)
            // {
            //     main_scene.add_block(new Block(&main_scene, Vector2{50, 50 + (float)i * (cell_height - 1)}, Vector2{(float)width - 50, cell_height}, 0, 1.0 + 0.1 * i, ColorAlpha(WHITE, 0.2)));
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
                if (IsKeyDown(KEY_LEFT_CONTROL))
                {
                    light->start_angle += GetMouseWheelMove() / 4;
                    light->stale = true;
                }
                else
                {
                    light->ray_cnt *= 1 + GetMouseWheelMove() / 2;
                    light->ray_cnt = std::max((int)light->ray_cnt, 1);
                }
            }
        }

        // if (IsKeyDown(KEY_LEFT))
        //     block->tilt += 0.002f;
        // if (IsKeyDown(KEY_RIGHT))
        //     block->tilt -= 0.002f;
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