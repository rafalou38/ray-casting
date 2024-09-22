#include "Block.hpp"

Block::Block(Vector2 position, Vector2 size, float tilt, int index)
{
    this->position = position;
    this->size = size;
    this->index = index;

    this->tilt = tilt;

    Block::blocks.push_back(this);
    dioptres.resize(4, {0, 0, 0, 0});

    compute_dioptres();
}

Block::~Block()
{
}

void Block::draw()
{
    for (auto &dioptre : dioptres)
    {
        DrawLineV({dioptre.x0, dioptre.y0}, {dioptre.x1, dioptre.y1}, WHITE);
#if DEBUG
        DrawText(std::to_string(dioptre.id).c_str(), dioptre.x0, dioptre.y0, 20, WHITE);
        DrawCircleV({dioptre.x1, dioptre.y1}, 3, LIGHTGRAY);
#endif
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
    Vector2 p0 = Vector2Rotate({0, 0}, tilt);
    Vector2 p1 = Vector2Rotate({size.x, 0}, tilt);
    dioptres[0] = {
        position.x + p0.x,
        position.y + p0.y,
        position.x + p1.x,
        position.y + p1.y};

    p0 = Vector2Rotate({size.x, 0}, tilt);
    p1 = Vector2Rotate({size.x, size.y}, tilt);
    dioptres[1] = {
        position.x + p0.x,
        position.y + p0.y,
        position.x + p1.x,
        position.y + p1.y};

    p0 = Vector2Rotate({size.x, size.y}, tilt);
    p1 = Vector2Rotate({0, size.y}, tilt);
    dioptres[2] = {
        position.x + p0.x,
        position.y + p0.y,
        position.x + p1.x,
        position.y + p1.y};

    p0 = Vector2Rotate({0, size.y}, tilt);
    p1 = Vector2Rotate({0, 0}, tilt);
    dioptres[3] = {
        position.x + p0.x,
        position.y + p0.y,
        position.x + p1.x,
        position.y + p1.y};
}

Intersection Block::intersection(LightRay *ray)
{
    Intersection inter = {{0, 0}, NULL, INFINITY, 0};
    float inter_dist = INFINITY;

    for (size_t i = 0; i < dioptres.size(); i++)
    {
        Intersection inter2 = dioptres[i].intersection(ray);
        if (inter2.point.x == 0 && inter2.point.y == 0)
            continue;

        if (dioptres[i].id == ray->origin_dioptre_id)
            continue;

        float d2 = Vector2DistanceSqr(ray->start_pos, inter2.point);

        if (d2 < inter_dist)
        {
            inter = inter2;
            inter_dist = d2;
        }
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
        if (y > std::max(y0, y1) or y < std::min(y0, y1))
        {
            return no_inter;
        }
    }
    else if (abs(cos(ray->start_angle)) <= 0.0000001f)
    {
        // Cas rayon vertical
        x = ray->start_pos.x;
        y = a * (x - x0) + y0;
        if (x > std::max(x0, x1) or x < std::min(x0, x1))
        {
            return no_inter;
        }
    }
    else
    {
        x = (cos(ray->start_angle) * (-ray->start_pos.y - (a)*x0 + y0) + ray->start_pos.x * sin(ray->start_angle)) / (sin(ray->start_angle) - a * cos(ray->start_angle));
        y = (x - ray->start_pos.x) * (sin(ray->start_angle) / cos(ray->start_angle)) + ray->start_pos.y;

        if (x > std::max(x0, x1) or x < std::min(x0, x1))
        {
            return no_inter;
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
        return no_inter;
    }

    if (abs(x) == INFINITY or abs(y) == INFINITY or abs(x) == NAN or abs(y) == NAN)
    {
        return no_inter;
    }

    return Intersection{{x, y}, this, Vector2DistanceSqr({x, y}, ray->end_pos), 0};
}

void Block::clearRays()
{
    for (auto ray : outRays)
    {
        delete ray;
    }
    outRays.clear();
}

void Block::RegisterNewRay(LightRay *inRay, Intersection &inter)
{
    long oid = inter.dioptre->id;
    auto ray = new LightRay(inter.point, inRay->start_angle, inRay->iteration + 1, oid);
    ray->update();
    ray->draw();
    // printf("Registered ray: %f %f %f %f\n", ray->start_pos.x, ray->start_pos.y, ray->start_angle, ray->iteration);
    outRays.push_back(ray);
}