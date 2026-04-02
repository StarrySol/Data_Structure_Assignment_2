#include <cmath>

struct Vec2
{
    double x = 0;
    double y = 0;

    Vec2 operator+(Vec2& rhs)
    {
        Vec2 retVec = this;
        retVec.x += rhs.x;
        retVec.y += rhs.y;

        return retVec;
    }

    Vec2 operator-(Vec2& rhs)
    {
        Vec2 retVec = this;
        retVec.x -= rhs.x;
        retVec.y -= rhs.y;

        return retVec;
    }

}

double Length(const Vec2& lhs, const Vec2& rhs)
{
    Vec2 total = lhs - rhs;
    return std::sqrt((total.x * total.x) + (total.y * total.y));
}