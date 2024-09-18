#pragma once

#include <vector>

#include "raylib.h"
#include "Object.hpp"
#include "LightRay.hpp"

class Light : public Object
{
private:
    
public:
    Color color = RED;
    size_t ray_cnt;

    std::vector<LightRay> rays;

    Light(Vector2 position, Color color, int ray_cnt);
    ~Light();

    void draw();
    void update();
};

