#include "raylib.h"
#include "raymath.h"
#include "Light.hpp"

#define PT_SIZE 10

Light::Light(Vector2 position, Color color, int ray_cnt)
{
    this->position = position;
    this->color = color;
    this->ray_cnt = ray_cnt;
}

Light::~Light()
{
}

void Light::draw()
{
    for (auto &ray : rays)
    {
        ray.draw();
    }

    DrawLineV(Vector2Add(position, Vector2{-PT_SIZE, 0}), Vector2Add(position, Vector2{PT_SIZE, 0}), WHITE);
    DrawLineV(Vector2Add(position, Vector2{0, -PT_SIZE}), Vector2Add(position, Vector2{0, PT_SIZE}), WHITE);
}

void Light::update()
{
    if (rays.size() != ray_cnt)
    {
        rays.clear();
        for (size_t i = 0; i < ray_cnt; i++)
        {
            rays.push_back(LightRay(position, 2 * M_PI * i / ray_cnt));
        }
    }


    for (auto &ray : rays)
    {
        ray.start_pos = position;
        ray.update();
    }
}