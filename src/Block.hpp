#pragma once
#include "Object.hpp"
#include "LightRay.hpp"
#include "Config.hpp"

struct Intersection;
class LightRay;

class Dioptre
{

public:
    long id;
    float x0;
    float y0;
    float x1;
    float y1;
    float a;
    float b;
    float index;
    Dioptre(float x0, float y0, float x1, float y1, float index)
    {
        printf("Dioptre created\n");
        this->id = random()%255;
        this->x0 = x0;
        this->y0 = y0;
        this->x1 = x1;
        this->y1 = y1;
        this->index = index;
        sync();
    }
    void sync()
    {
        this->a = (y1 - y0) / (x1 - x0);
        this->b = y0 - a * x0;
    }
    Intersection intersection(LightRay *ray);
};

class Block : public Object
{
private:
    std::vector<Dioptre> dioptres;

public:
    inline static std::vector<Block *> blocks;
    std::vector<LightRay *> outRays;
    Vector2 size;
    float tilt;
    float index;

    Block(Vector2 position, Vector2 size, float tilt = 0, float index = 1);
    ~Block();

    void compute_dioptres();
    Intersection intersection(LightRay *ray);

    void clearRays();

    void RegisterNewRay(LightRay *inRay, Intersection &inter);

    void draw();
};