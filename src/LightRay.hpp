#pragma once

#include "raylib.h"
#include "raymath.h"


class LightRay
{
private:
public:
    Vector2 start_pos;
    float start_angle;
    Vector2 end_pos;

    LightRay(Vector2 start_pos, float angle);
    ~LightRay();

    void update();
    void draw();
};

