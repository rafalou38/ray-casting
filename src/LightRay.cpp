#include "raylib.h"
#include "raymath.h"

#include "LightRay.hpp"
#include "Block.hpp"

LightRay::LightRay(Vector2 start_pos, float angle)
{
    this->start_pos = start_pos;
    this->start_angle = angle;
}

LightRay::~LightRay()
{
}

void LightRay::draw()
{
    DrawLineEx(start_pos, end_pos, 1, RED);
}

void LightRay::update()
{
    Vector2 inter = {0, 0};
    float d = INFINITY;
    for (size_t i = 0; i < Block::blocks.size(); i++)
    {
        Block *block = Block::blocks[i];
        Vector2 Inter2 = block->intersection(this);
        if (Inter2.x == 0 && Inter2.y == 0)
            continue;
        float d2 = Vector2DistanceSqr(Inter2, this->start_pos);
        if (d2 < d)
        {
            d = d2;
            inter = Inter2;
        }
    }
    if (inter.x != 0 && inter.y != 0)
    {
        this->end_pos.x = inter.x;
        this->end_pos.y = inter.y;
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