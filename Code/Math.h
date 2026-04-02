#include <cmath>

struct Vec2
{
    double x = 0;
    double y = 0;
    Vec2 (double inX = 0, double inY = 0) : x(inX), y(inY) {

    }

    Vec2 operator+(const Vec2& rhs) const
    {
        return Vec2(x + rhs.x, y + rhs.y);
    }

    Vec2 operator-(const Vec2& rhs) const
    {
        return Vec2(x - rhs.x, y - rhs.y);
    }

    Vec2& operator+=(const Vec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    
    Vec2& operator-=(const Vec2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
};

double VecLength(const Vec2& lhs, const Vec2& rhs);