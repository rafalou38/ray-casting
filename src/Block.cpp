#include "Block.hpp"

Block::Block(Vector2 position, Vector2 size, int index)
{
    this->position = position;
    this->size = size;
    this->index = index;

    Block::blocks.push_back(this);
    dioptres.resize(4, {0, 0, 0, 0});
}

Block::~Block()
{
}

void Block::draw()
{
    // DrawRectangleRec({position.x, position.y, size.x, size.y}, LIGHTGRAY);
    DrawLineV({position.x, position.y}, {position.x + size.x, position.y}, LIGHTGRAY);
    DrawLineV({position.x + size.x, position.y}, {position.x + size.x, position.y + size.y}, LIGHTGRAY);
    DrawLineV({position.x + size.x, position.y + size.y}, {position.x, position.y + size.y}, LIGHTGRAY);
    DrawLineV({position.x, position.y + size.y}, {position.x, position.y}, LIGHTGRAY);
}

void Block::compute_dioptres()
{
    dioptres[0] = {position.x, position.y, position.x + size.x, position.y};                   // top
    dioptres[1] = {position.x + size.x, position.y, position.x + size.x, position.y + size.y}; // right
    dioptres[2] = {position.x, position.y + size.y, position.x + size.x, position.y + size.y}; // bottom
    dioptres[3] = {position.x, position.y, position.x, position.y + size.y};                   // left
}

Vector2 Block::intersection(LightRay *ray)
{
    // printf("Intersecting block %d\n", index);
    Vector2 inter;
    float inter_dist = INFINITY;
    compute_dioptres();
    for (size_t i = 0; i < dioptres.size(); i++)
    {
        Dioptre dioptre = dioptres[i];
        Vector2 inter2 = dioptre.intersection(ray);
        float d2 = Vector2DistanceSqr(ray->start_pos, inter2);

        if (d2 < inter_dist)
        {
            inter = inter2;
            inter_dist = d2;
        }

        // printf("Dioptre %ld: %f, %f, alpha: %f, sin: %f\n", i, inter.x, inter.y, ray->start_angle, sin(ray->start_angle));
    }
    
    if (!Vector2Equals(inter, {0, 0}))
    {
        // printf("", );
        DrawCircleV(inter, 5, GREEN);
    }

    return inter;
}

Vector2 Dioptre::intersection(LightRay *ray)
{
    sync();
    // printf("Intersecting dioptre %f, %f, %f, %f, a=%f\n", x0, y0, x1, y1, a);

    float x;
    float y;

    if (a == INFINITY)
    {
        // Cas dioptre horizontal
        x = x0;
        y = (x - ray->start_pos.x) * (sin(ray->start_angle) / cos(ray->start_angle)) + ray->start_pos.y;
        if (y > y1 or y < y0)
        {
            return {0, 0};
        }
    }
    else if (abs(cos(ray->start_angle)) <= 0.0000001f)
    {
        // Cas rayon vertical
        x = ray->start_pos.x;
        y = y0;
        if (x > x1 or x < x0)
        {
            return {0, 0};
        }
    }
    else
    {
        // Cas Normal
        x = (cos(ray->start_angle) * (-ray->start_pos.y - (a)*x0 + y0) + ray->start_pos.x * sin(ray->start_angle)) / (sin(ray->start_angle) - a * cos(ray->start_angle));
        y = (x - ray->start_pos.x) * (sin(ray->start_angle) / cos(ray->start_angle)) + ray->start_pos.y;
        if (x > x1 or x < x0)
        {
            return {0, 0};
        }
    }

    // Check direction
    if ((sin(ray->start_angle) > 0 and y < ray->start_pos.y) or (sin(ray->start_angle) < 0 and y > ray->start_pos.y) or (cos(ray->start_angle) > 0 and x < ray->start_pos.x) or (cos(ray->start_angle) < 0 and x > ray->start_pos.x))
    {
        return {0, 0};
    }

    return {x, y};
}