#include "Block.hpp"
#include <iostream>
#include <string>

Block::Block(Vector2 position, Vector2 size, float tilt, float index)
{
    this->position = position;
    this->size = size;
    this->index = index;

    this->tilt = tilt;

    Block::blocks.push_back(this);
    // dioptres.resize(4, {0, 0, 0, 0, index});

    for (size_t i = 0; i < 4; i++)
    {
        dioptres.push_back(Dioptre(0, 0, 0, 0, 0));
    }

    compute_dioptres();
}

Block::~Block()
{
}

Block *Block::get_block(Vector2 point)
{
    for (auto block : blocks)
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

void Block::draw()
{
    for (auto &dioptre : dioptres)
    {
        DrawLineV({dioptre.x0, dioptre.y0}, {dioptre.x1, dioptre.y1}, WHITE);
#if DEBUG
        DrawText(std::to_string(dioptre.id).c_str(), (dioptre.x0 + dioptre.x1) / 2, (dioptre.y0 + dioptre.y1) / 2, 20, WHITE);
        DrawCircleV({dioptre.x1, dioptre.y1}, 3, LIGHTGRAY);
#endif
    }
    // #if DEBUG
    //     DrawLineV({position.x, position.y}, {position.x + size.x, position.y}, BLUE);
    //     DrawLineV({position.x + size.x, position.y}, {position.x + size.x, position.y + size.y}, BLUE);
    //     DrawLineV({position.x + size.x, position.y + size.y}, {position.x, position.y + size.y}, BLUE);
    //     DrawLineV({position.x, position.y + size.y}, {position.x, position.y}, BLUE);
    // #endif
}

void Block::compute_dioptres()
{
    Vector2 p0 = Vector2Rotate({0, 0}, tilt);
    Vector2 p1 = Vector2Rotate({size.x, 0}, tilt); // TOP

    dioptres[0].x0 = position.x + p0.x;
    dioptres[0].y0 = position.y + p0.y;
    dioptres[0].x1 = position.x + p1.x;
    dioptres[0].y1 = position.y + p1.y;
    dioptres[0].index = this->index;

    p0 = Vector2Rotate({size.x, 0}, tilt);
    p1 = Vector2Rotate({size.x, size.y}, tilt); // Right
    dioptres[1].x0 = position.x + p0.x;
    dioptres[1].y0 = position.y + p0.y;
    dioptres[1].x1 = position.x + p1.x;
    dioptres[1].y1 = position.y + p1.y;
    dioptres[1].index = this->index;

    p0 = Vector2Rotate({size.x, size.y}, tilt);
    p1 = Vector2Rotate({0, size.y}, tilt); // bottom
    dioptres[2].x0 = position.x + p0.x;
    dioptres[2].y0 = position.y + p0.y;
    dioptres[2].x1 = position.x + p1.x;
    dioptres[2].y1 = position.y + p1.y;
    dioptres[2].index = this->index;

    p0 = Vector2Rotate({0, size.y}, tilt);
    p1 = Vector2Rotate({0, 0}, tilt); // Left
    dioptres[3].x0 = position.x + p0.x;
    dioptres[3].y0 = position.y + p0.y;
    dioptres[3].x1 = position.x + p1.x;
    dioptres[3].y1 = position.y + p1.y;
    dioptres[3].index = this->index;
}

Intersection Block::intersection(LightRay *ray)
{
    Intersection inter = {{0, 0}, NULL, INFINITY, 0};
    float d = INFINITY;

#if DEBUG
    printf("\tintersection %f with block %f %f \n", ray->iteration, this->position.x, this->position.y);
#endif

    for (size_t i = 0; i < dioptres.size(); i++)
    {
        if (dioptres[i].id == ray->origin_dioptre_id)
            continue;

        Intersection inter2 = dioptres[i].intersection(ray);
        if (ray->iteration == 42)
            DrawCircleV(inter2.point, 6, BLUE);
        if (inter2.dioptre == NULL)
            continue;

        float d2 = Vector2DistanceSqr(inter2.point, ray->start_pos);
        if (ray->iteration == 42)
            DrawCircleV(inter2.point, 6, YELLOW);
#if DEBUG
        printf("\t\t new candidate: %ld\n", dioptres[i].id);
#endif
        if (d2 < d)
        {
            if (ray->iteration == 42)
                DrawCircleV(inter2.point, 6, RED);
            inter = inter2;
            d = d2;
        }
    }

    if (d != INFINITY)
    {
        if (ray->iteration == 42)
            DrawCircleV(inter.point, 10, GREEN);
#if DEBUG
        printf("\t\t-> saved: %ld\n", inter.dioptre->id);
#endif
    }

    return inter;
}

Intersection Dioptre::intersection(LightRay *ray)
{
    sync();
    Intersection no_inter = Intersection{{0, 0}, NULL, 0, 0};

    float x;
    float y;

    if (abs(a) == INFINITY)
    {
        // Cas dioptre horizontal
        x = x0;
        y = (x - ray->start_pos.x) * (sin(ray->start_angle) / cos(ray->start_angle)) + ray->start_pos.y;
#if DEBUG
        if (ray->iteration == 3)
            DrawCircle(x, y, 5, BLUE);
#endif
        if (y > std::max(y0, y1) or y < std::min(y0, y1))
            return no_inter;
    }
    else if (abs(cos(ray->start_angle)) <= 0.0000001f)
    {
        // Cas rayon vertical
        x = ray->start_pos.x;
        y = a * (x - x0) + y0;
#if DEBUG
        if (ray->iteration == 3)
            DrawCircle(x, y, 5, GREEN);
#endif
        if (x > std::max(x0, x1) or x < std::min(x0, x1))
            return no_inter;
    }
    else
    {
        x = (cos(ray->start_angle) * (-ray->start_pos.y - (a)*x0 + y0) + ray->start_pos.x * sin(ray->start_angle)) / (sin(ray->start_angle) - a * cos(ray->start_angle));
        y = (x - ray->start_pos.x) * (sin(ray->start_angle) / cos(ray->start_angle)) + ray->start_pos.y;

#if DEBUG
        if (ray->iteration == 3)
            DrawCircle(x, y, 5, PINK);
#endif
        if (x > std::max(x0, x1) or x < std::min(x0, x1))
            return no_inter;
    }

    // Check direction
    if (
        (sin(ray->start_angle) > 0 and y < ray->start_pos.y)    //
        or (sin(ray->start_angle) < 0 and y > ray->start_pos.y) //
        or (cos(ray->start_angle) > 0 and x < ray->start_pos.x) //
        or (cos(ray->start_angle) < 0 and x > ray->start_pos.x) //
    )
        return no_inter;

    if (abs(x) == INFINITY or abs(y) == INFINITY or abs(x) == NAN or abs(y) == NAN)
        return no_inter;

    // TODO: Check angle
    return Intersection{{x, y}, this, Vector2DistanceSqr(ray->start_pos, {x, y}), 0};
}

void Block::clearRays()
{
    for (auto ray : outRays)
        delete ray;

    outRays.clear();
}

void Block::RegisterNewRay(LightRay *inRay, Intersection &inter)
{

    Vector2 OJ = Vector2({
        inter.dioptre->x0 - inter.point.x,
        inter.dioptre->y0 - inter.point.y,
    });

    Vector2 OL = Vector2(
        {
            inRay->start_pos.x - inter.point.x,
            inRay->start_pos.y - inter.point.y,
        });

    // Vecteur normal
    Vector2 n = Vector2Rotate(OJ, PI / 2);

#if DEBUG
    DrawLineV(inter.point, Vector2Add(inter.point, Vector2Scale(Vector2Normalize(n), 150)), YELLOW);
    DrawLineV(inter.point, Vector2Add(inter.point, Vector2Scale(Vector2Normalize(n), 100)), YELLOW);
#endif

    // entrant ou sortant
    bool leaving = Vector2Angle(n, OL) < -PI / 2 or Vector2Angle(n, OL) > PI / 2;
    if (leaving)
        n = Vector2Scale(n, -1);

#if DEBUG
    DrawLineV(inter.point, Vector2Add(inter.point, Vector2Scale(Vector2Normalize(n), 100)), PURPLE);
#endif
    float i1 = Vector2Angle(n, OL);
    float n1 = inRay->origin_index;

    float n2 = this->index;
    if (leaving)
        n2 = 1;
    float i2 = asin(sin(i1) * (n1 / n2));

    // TODO déterminer correctement n1 et n2

#if DEBUG
    DrawText(std::to_string((int)round((i1 * 180 / PI))).c_str(), inter.point.x, inter.point.y - 20, 16, WHITE);
    DrawText(std::to_string((int)round((i2 * 180 / PI))).c_str(), inter.point.x, inter.point.y + 20, 16, WHITE);
#endif
    Vector2 dir = Vector2Rotate(Vector2Scale(n, -1), i2);
    // DrawLineV(inter.point, Vector2Add(inter.point, Vector2Scale(Vector2Normalize(), 100)), YELLOW);

    long oid = inter.dioptre->id;

    auto ray = new LightRay(inter.point, Vector2Angle({1, 0}, dir), inRay->iteration + 1, oid, n2);

    ray->update();
    ray->draw();

    outRays.push_back(ray);
}