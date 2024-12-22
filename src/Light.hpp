#pragma once

#include <vector>

#include "raylib.h"
#include "Object.hpp"
#include "LightRay.hpp"
#include "Scene.hpp"

class LightRay;
class Scene;

class Light : public Object
{
private:
    
public:
    Scene *scene;

    Color color = RED;
    size_t ray_cnt;

    bool stale = true;
    bool infinity = false;
    double start_angle = 0;
    std::vector<LightRay*> rays;

    Light(Scene *scene, Vector2 position, Color color, int ray_cnt, bool infinity = false, double angle = 0.00001f);
    ~Light();

    void draw();
    void update();
};

