#include "raylib.h"
#include "raymath.h"


#include "LightRay.hpp"

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
    this->end_pos.x = this->start_pos.x + cos(this->start_angle) * 10000;
    this->end_pos.y = this->start_pos.y + sin(this->start_angle) * 10000;
}