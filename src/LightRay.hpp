#pragma once

#include "raylib.h"
#include "raymath.h"
#include "Block.hpp"

class Dioptre;

struct Intersection
{
    Vector2 point;

    Dioptre *dioptre;
    float distance;
    float angle;
};


class LightRay
{
private:
public:
    inline static int ray_cnt = -1;
    Vector2 start_pos;
    float start_angle;
    Vector2 end_pos;

    float iteration;
    long origin_dioptre_id;


    LightRay(Vector2 start_pos, float angle, int iteration = 0, size_t origin_dioptre_index = -1);
    ~LightRay();

    void update();
    void draw();
};

