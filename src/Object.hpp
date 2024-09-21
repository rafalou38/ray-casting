#pragma once

#include "raylib.h"
#include <vector>
#include <memory>

class Object
{
private:
    // static std::vector<std::unique_ptr<Object>> objects;

public:
    Vector2 position;
    Object();
    ~Object();

    virtual void update() {};
    virtual void draw() {};
};
