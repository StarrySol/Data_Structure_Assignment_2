#include "Ring.h"

#include <vector>
#include <cmath>
#include <limits>
#include <iterator>

Ring::Ring()
{
    
}

//Compute triangle area using cross product
double TriangleArea(const Vec2& a, const Vec2& b, const Vec2& c)
{
    return std::abs(
        (b.x - a.x) * (c.y - a.y) -
        (b.y - a.y) * (c.x - a.x)
    ) * 0.5;
}
