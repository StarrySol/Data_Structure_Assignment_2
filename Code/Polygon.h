
#pragma once
#include <vector>
#include "Math.h"

struct Ring
{
    unsigned int ring_id{};
    std::vector<Vec2> vertices;
};

struct Polygon
{
    std::vector<Ring> rings;
};

