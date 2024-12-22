#include "Scene.hpp"

Scene::Scene() {

};
Scene::~Scene()
{
    for (Block *block : blocks)
    {
        free(block);
    }
};
void Scene::add_light(Light *light)
{
    lights.push_back(light);
}
void Scene::add_block(Block *block)
{
    blocks.push_back(block);
}
Block *Scene::get_block(Vector2 point)
{
    for (Block *block : blocks)
    {
        float d0 = Vector2DotProduct(Vector2{block->dioptres[0].x1 - block->dioptres[0].x0, block->dioptres[0].y1 - block->dioptres[0].y0}, Vector2{point.x - block->dioptres[0].x0, point.y - block->dioptres[0].y0});
        float d1 = Vector2DotProduct(Vector2{block->dioptres[1].x1 - block->dioptres[1].x0, block->dioptres[1].y1 - block->dioptres[1].y0}, Vector2{point.x - block->dioptres[1].x0, point.y - block->dioptres[1].y0});
        float d2 = Vector2DotProduct(Vector2{block->dioptres[2].x1 - block->dioptres[2].x0, block->dioptres[2].y1 - block->dioptres[2].y0}, Vector2{point.x - block->dioptres[2].x0, point.y - block->dioptres[2].y0});
        float d3 = Vector2DotProduct(Vector2{block->dioptres[3].x1 - block->dioptres[3].x0, block->dioptres[3].y1 - block->dioptres[3].y0}, Vector2{point.x - block->dioptres[3].x0, point.y - block->dioptres[3].y0});

        if (d0 > 0 && d1 > 0 && d2 > 0 && d3 > 0)
            return block;
    }

    return NULL;
}

void Scene::update_block()
{
    for (Block *block : blocks)
    {
        block->compute_dioptres();
    }
}


void Scene::update_lights()
{
    for (Block *block : blocks)
    {
        block->clearRays();
    }

    for (Light *l : lights)
    {
        l->update();
    }
}

void Scene::draw()
{
    for (Block *block : blocks)
    {
        block->draw();
    }
    for (Light *l : lights)
    {
        l->draw();
    }
}