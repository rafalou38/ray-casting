#include "raylib.h"
#include "raymath.h"
#include "Light.hpp"

#define PT_SIZE 10

Light::Light(Scene *scene, Vector2 position, Color color, int ray_cnt, bool infinity, double angle)
{
    this->scene = scene;
    this->start_angle = angle;
    this->infinity = infinity;
    this->position = position;
    this->color = color;
    this->ray_cnt = ray_cnt;
}

Light::~Light()
{
}

void Light::draw()
{
    for (auto ray : rays)
    {
        ray->draw();
    }

    DrawLineV(Vector2Add(position, Vector2{-PT_SIZE, 0}), Vector2Add(position, Vector2{PT_SIZE, 0}), WHITE);
    DrawLineV(Vector2Add(position, Vector2{0, -PT_SIZE}), Vector2Add(position, Vector2{0, PT_SIZE}), WHITE);
}

void Light::update()
{
    if (rays.size() != ray_cnt || stale)
    {
        stale = false;
        for (auto ray : rays)
        {
            delete ray;
        }
        rays.clear();

        for (size_t i = 0; i < ray_cnt; i++)
        {
            if (infinity)
            {
                rays.push_back(new LightRay(this, position, start_angle, 0, 0));
            }
            else
            {
                rays.push_back(new LightRay(this, position, start_angle + sqrt(3) + 2 * M_PI * i / ray_cnt, 0, 0));
            }
        }
    }

    for (int i = 0; i < (int)ray_cnt; i++)
    {
        auto ray = rays[i];
        if (infinity)
        {
            ray->start_pos = {position.x + (i*2 - (int)ray_cnt/2) * 2 * (float)sin(PI -start_angle  ), position.y + (i*2 - (int)ray_cnt/2) * 2 * (float)cos(PI -start_angle  )};
        }
        else
        {
            ray->start_pos = {position.x, position.y};
        }

        ray->update();
    }
}