#pragma once


#include "Vertice.h"
#include <vector>

struct Shape
{
    int shapeID = 0;
    std::vector<Vertice> vertices{};

    Shape();
};

double TriangleArea(const Vec2& a, const Vec2& b, const Vec2& c);

