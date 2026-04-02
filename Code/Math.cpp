#include "Math.h"

Vec2 Vec2::operator+(const Vec2& rhs) const
{
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2& rhs) const
{
    return Vec2(x - rhs.x, y - rhs.y);
}
Vec2& Vec2::operator+=(const Vec2& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Vec2& Vec2::operator-=(const Vec2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

double Vec2::VecLength(const Vec2& lhs, const Vec2& rhs)
{
    Vec2 total = lhs;
    total = total - rhs;
    return std::sqrt((total.x * total.x) + (total.y * total.y));
}