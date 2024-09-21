#include "Block.hpp"

Block::Block(Vector2 position, Vector2 size, float tilt, int index)
{
    this->position = position;
    this->size = size;
    this->index = index;

    this->tilt = tilt;

    Block::blocks.push_back(this);
    dioptres.resize(4, {0, 0, 0, 0});
}

Block::~Block()
{
}

void Block::draw()
{
    // DrawRectangleRec({position.x, position.y, size.x, size.y}, LIGHTGRAY);

    for (auto &dioptre : dioptres)
    {
        DrawLineV({dioptre.x0, dioptre.y0}, {dioptre.x1, dioptre.y1}, LIGHTGRAY);
        // Draw arrow tip
        DrawCircleV({dioptre.x1, dioptre.y1}, 3, LIGHTGRAY);
    }
#if DEBUG
    DrawLineV({position.x, position.y}, {position.x + size.x, position.y}, BLUE);
    DrawLineV({position.x + size.x, position.y}, {position.x + size.x, position.y + size.y}, BLUE);
    DrawLineV({position.x + size.x, position.y + size.y}, {position.x, position.y + size.y}, BLUE);
    DrawLineV({position.x, position.y + size.y}, {position.x, position.y}, BLUE);
#endif
}

void Block::compute_dioptres()
{
    Vector2 p0 = Vector2Rotate({position.x, position.y}, tilt);
    Vector2 p1 = Vector2Rotate({position.x + size.x, position.y}, tilt);
    dioptres[0] = {p0.x, p0.y, p1.x, p1.y};

    p0 = Vector2Rotate({position.x + size.x, position.y}, tilt);
    p1 = Vector2Rotate({position.x + size.x, position.y + size.y}, tilt);
    dioptres[1] = {p0.x, p0.y, p1.x, p1.y};

    p0 = Vector2Rotate({position.x + size.x, position.y + size.y}, tilt);
    p1 = Vector2Rotate({position.x, position.y + size.y}, tilt);
    dioptres[2] = {p1.x, p1.y, p0.x, p0.y};

    p0 = Vector2Rotate({position.x, position.y + size.y}, tilt);
    p1 = Vector2Rotate({position.x, position.y}, tilt);
    dioptres[3] = {p1.x, p1.y, p0.x, p0.y};
}

Vector2 Block::intersection(LightRay *ray)
{
    // printf("Intersecting block %d\n", index);
    Vector2 inter = {0, 0};
    float inter_dist = INFINITY;
    compute_dioptres();
    for (size_t i = 0; i < dioptres.size(); i++)
    {
        Dioptre dioptre = dioptres[i];
        Vector2 inter2 = dioptre.intersection(ray);
        if (inter2.x == 0 && inter2.y == 0)
            continue;

        float d2 = Vector2DistanceSqr(ray->start_pos, inter2);

        if (d2 < inter_dist)
        {
            inter = inter2;
            inter_dist = d2;
        }
    }

#if DEBUG
    DrawCircleV(inter, 5, GREEN);
#endif

    return inter;
}

Vector2 Dioptre::intersection(LightRay *ray)
{
    sync();
    // printf("Intersecting dioptre %f, %f, %f, %f, a=%f\n", x0, y0, x1, y1, a);

    float x;
    float y;

    if (a < 0)
    {
        float px0 = x0;
        float py0 = y0;
        float px1 = x1;
        float py1 = y1;
        x0 = px1;
        y0 = py1;
        x1 = px0;
        y1 = py0;
    }

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
        y = a * (x - x0) + y0;
        if (x > x1 or x < x0)
        {
            return {0, 0};
        }
    }
    else
    {
        x = (cos(ray->start_angle) * (-ray->start_pos.y - (a)*x0 + y0) + ray->start_pos.x * sin(ray->start_angle)) / (sin(ray->start_angle) - a * cos(ray->start_angle));
        y = (x - ray->start_pos.x) * (sin(ray->start_angle) / cos(ray->start_angle)) + ray->start_pos.y;

#if DEBUG
        DrawCircleV({x, y}, 3, BLUE);
#endif
        if (x > x1 or x < x0)
        {
            return {0, 0};
        }
    }

    // Check direction
    if (
        (sin(ray->start_angle) > 0 and y < ray->start_pos.y)    //
        or (sin(ray->start_angle) < 0 and y > ray->start_pos.y) //
        or (cos(ray->start_angle) > 0 and x < ray->start_pos.x) //
        or (cos(ray->start_angle) < 0 and x > ray->start_pos.x) //
    )
    {
        return {0, 0};
    }
#if DEBUG
    DrawCircleV({x, y}, 3, YELLOW);
#endif

    return {x, y};
}