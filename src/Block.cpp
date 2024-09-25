#include "Block.hpp"

Block::Block(Vector2 position, Vector2 size, float tilt, float index)
{
    this->position = position;
    this->size = size;
    this->index = index;

    this->tilt = tilt;

    Block::blocks.push_back(this);
    dioptres.resize(4, {0, 0, 0, 0, index});

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
    Vector2 p1 = Vector2Rotate({size.x, 0}, tilt); // TOP
    dioptres[0] = {
        position.x + p0.x,
        position.y + p0.y,
        position.x + p1.x,
        position.y + p1.y,
        this->index};

    p0 = Vector2Rotate({size.x, 0}, tilt);
    p1 = Vector2Rotate({size.x, size.y}, tilt); // Right
    dioptres[1] = {
        position.x + p0.x,
        position.y + p0.y,
        position.x + p1.x,
        position.y + p1.y,
        this->index};

    p0 = Vector2Rotate({size.x, size.y}, tilt);
    p1 = Vector2Rotate({0, size.y}, tilt); // bottom
    dioptres[2] = {
        position.x + p0.x,
        position.y + p0.y,
        position.x + p1.x,
        position.y + p1.y,
        this->index};

    p0 = Vector2Rotate({0, size.y}, tilt);
    p1 = Vector2Rotate({0, 0}, tilt); // Left
    dioptres[3] = {
        position.x + p0.x,
        position.y + p0.y,
        position.x + p1.x,
        position.y + p1.y,
        this->index};
}

Intersection Block::intersection(LightRay *ray)
{
    Intersection inter = {{0, 0}, NULL, INFINITY, 0};

    for (size_t i = 0; i < dioptres.size(); i++)
    {
        Intersection inter2 = dioptres[i].intersection(ray);
        if (inter2.point.x == 0 && inter2.point.y == 0)
            continue;

        if (dioptres[i].id == ray->origin_dioptre_id)
            continue;

        if (inter2.distance < inter.distance)
            inter = inter2;
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
            return no_inter;
    }
    else if (abs(cos(ray->start_angle)) <= 0.0000001f)
    {
        // Cas rayon vertical
        x = ray->start_pos.x;
        y = a * (x - x0) + y0;
        if (x > std::max(x0, x1) or x < std::min(x0, x1))
            return no_inter;
    }
    else
    {
        x = (cos(ray->start_angle) * (-ray->start_pos.y - (a)*x0 + y0) + ray->start_pos.x * sin(ray->start_angle)) / (sin(ray->start_angle) - a * cos(ray->start_angle));
        y = (x - ray->start_pos.x) * (sin(ray->start_angle) / cos(ray->start_angle)) + ray->start_pos.y;

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
    // if (inter.dioptre->x1 > inter.dioptre->x0)
    // {
    //     u.x = inter.dioptre->x1 - inter.point.x;
    //     u.y = inter.dioptre->y1 - inter.point.y;
    // }

    Vector2 OL = Vector2(
        {
            inRay->start_pos.x - inter.point.x,
            inRay->start_pos.y - inter.point.y,
        });

    Vector2 n = Vector2Rotate(OJ, PI / 2);

    // DrawLineV(inter.point, Vector2Add(inter.point, Vector2Scale(Vector2Normalize(n), 150)), YELLOW);
    // DrawLineV(inter.point, Vector2Add(inter.point, Vector2Scale(Vector2Normalize(n), 100)), YELLOW);
    bool leaving = Vector2Angle(n, OL) < -PI / 2 or Vector2Angle(n, OL) > PI / 2;
    if (leaving)
        n = Vector2Scale(n, -1);

    // DrawLineV(inter.point, Vector2Add(inter.point, Vector2Scale(Vector2Normalize(n), 100)), PURPLE);

    float i1 = Vector2Angle(n, OL);
    float i2;
    if (leaving)
    {
        i2 = asin(sin(i1) * (1 / inRay->origin_index));
    }
    else
    {
        i2 = asin(sin(i1) * (index / inRay->origin_index));
    }

    // printf("%f %f %f %f \n", i1, inRay->origin_index, i2, index);

    // DrawText(std::to_string((int)round((i1 * 180 / PI))).c_str(), inter.point.x, inter.point.y, 20, WHITE);
    // DrawText(std::to_string((int)round((i2 * 180 / PI))).c_str(), inter.point.x + 30, inter.point.y + 30, 20, WHITE);

    Vector2 dir = Vector2Rotate(Vector2Scale(n, -1), i2);
    // DrawLineV(inter.point, Vector2Add(inter.point, Vector2Scale(Vector2Normalize(), 100)), YELLOW);

    long oid = inter.dioptre->id;
    auto ray = new LightRay(inter.point, Vector2Angle({1, 0}, dir), inRay->iteration + 1, oid, this->index);

    ray->update();
    ray->draw();

    outRays.push_back(ray);
}