#pragma once

#include <iostream>
#include "Block.hpp"
#include "Light.hpp"

class Block;
class Light;

class Scene {
private:
    std::vector<Light *> lights;
public:
    std::vector<Block *> blocks;
    Scene();
    ~Scene();
    Block *get_block(Vector2 point);
    void add_block(Block *block);
    void add_light(Light *light);

    void update_lights();
    void update_block();

    void draw();
};