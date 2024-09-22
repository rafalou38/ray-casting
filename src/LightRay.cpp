#include "raylib.h"
#include "raymath.h"

#include "LightRay.hpp"
#include "Block.hpp"

LightRay::LightRay(Vector2 start_pos, float angle, int iteration, size_t origin_dioptre_id)
{
    this->start_pos = start_pos;
    this->start_angle = angle;
    this->iteration = iteration;
    this->origin_dioptre_id = origin_dioptre_id;
    LightRay::ray_cnt++;
}

LightRay::~LightRay()
{
    LightRay::ray_cnt--;
}

void LightRay::draw()
{
    DrawLineEx(start_pos, end_pos, 1, ColorAlpha(RED, 1.0/std::min(this->iteration, 1.0f)));
}

void LightRay::update()
{
    Intersection inter = {{0,0}, NULL, INFINITY, 0 };
    float d = INFINITY;
    Block *inter_block = nullptr;
    for (size_t i = 0; i < Block::blocks.size(); i++)
    {
        Block *block = Block::blocks[i];
        Intersection inter2 = block->intersection(this);
        if (inter2.point.x == 0 && inter2.point.y == 0)
            continue;

        if (iteration > 0)
        {
            DrawCircleV(inter2.point, 10, BROWN);
        }
        

        float d2 = Vector2DistanceSqr(inter2.point, this->start_pos);

        if (d2 < d)
        {
            d = d2;
            inter = inter2;
            inter_block = block;
        }
    }
    if (inter.point.x != 0 && inter.point.y != 0)
    {
        this->end_pos.x = inter.point.x;
        this->end_pos.y = inter.point.y;

        if(this->iteration < 2) inter_block->RegisterNewRay(this, inter);
    }
    else
    {
#if DEBUG
        this->end_pos.x = this->start_pos.x + cos(this->start_angle) * 10000;
        this->end_pos.y = this->start_pos.y + sin(this->start_angle) * 10000;
#else
        this->end_pos.x = this->start_pos.x + cos(this->start_angle) * 250;
        this->end_pos.y = this->start_pos.y + sin(this->start_angle) * 250;
#endif
    }

    // DrawText(("Angle: " + std::to_string(this->start_angle * 180 / PI) + "° sin: " + std::to_string(sin(this->start_angle))).c_str() , this->start_pos.x + cos(this->start_angle) * 100, this->start_pos.y + sin(this->start_angle) * 100, 16, WHITE);
}