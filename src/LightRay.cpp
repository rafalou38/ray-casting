#include "raylib.h"
#include "raymath.h"

#include "LightRay.hpp"
#include "Block.hpp"

LightRay::LightRay(Vector2 start_pos, float angle, int iteration, long origin_dioptre_id, float origin_index)
{
    // printf("%f %f \n", start_pos.x, start_pos.y);
    this->start_pos = start_pos;
    this->start_angle = angle;
    this->iteration = iteration;
    this->origin_dioptre_id = origin_dioptre_id;
    this->origin_index = origin_index;
    LightRay::ray_cnt++;
}

LightRay::~LightRay()
{
    LightRay::ray_cnt--;
}

void LightRay::draw()
{
    DrawLineEx(start_pos, end_pos, 1, ColorAlpha(RED, 1.0 / std::max(this->iteration / 1.5f, 1.0f)));
}

void LightRay::update()
{
    Intersection inter = {{0, 0}, NULL, INFINITY, 0};
    float d = INFINITY;
    Block *inter_block = nullptr;
    for (size_t i = 0; i < Block::blocks.size(); i++)
    {
        Block *block = Block::blocks[i];
        Intersection inter2 = block->intersection(this);
        if (inter2.point.x == 0 && inter2.point.y == 0)
            continue;

        float d2 = Vector2DistanceSqr(inter2.point, this->start_pos);

        if (d2 < d)
        {
            d = d2;
            inter = inter2;
            inter_block = block;
        }
    }
    // printf("%f %f\n", inter.point.x, inter.point.y);
    #if DEBUG
    DrawCircle(inter.point.x, inter.point.y, 10, PURPLE);
    #endif
    if (inter.point.x != 0 && inter.point.y != 0)
    {
        this->end_pos.x = inter.point.x;
        this->end_pos.y = inter.point.y;

#if DEBUG
        printf("Ray intersects from %ld to %ld dir: %f\n", this->origin_dioptre_id, inter.dioptre->id, this->start_angle);
        DrawText(std::to_string((int)iteration).c_str(), this->end_pos.x, this->end_pos.y, 20, WHITE);
#endif

        if (this->iteration < 4)
            inter_block->RegisterNewRay(this, inter);
    }
    else
    {
#if DEBUG
        this->end_pos.x = this->start_pos.x + cos(this->start_angle) * 10000;
        this->end_pos.y = this->start_pos.y + sin(this->start_angle) * 10000;
#else
        this->end_pos.x = this->start_pos.x + cos(this->start_angle) * 10000;
        this->end_pos.y = this->start_pos.y + sin(this->start_angle) * 10000;
#endif
    }
}