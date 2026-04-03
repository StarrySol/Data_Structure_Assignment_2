#include "Math.h"
#include <iomanip>
#include <iostream>
#include <cassert>
/************************************************* */
/********************ctor/dtor******************** */
/************************************************* */
Vec2::Vec2(double inX = 0, double inY = 0) : x(inX), y(inY) { /*empty by design*/}


/************************************************* */
/*************member functions******************** */
/************************************************* */


Vec2 &Vec2::operator+=(Vec2 const&v)
{
    x+=v.x;
    y+=v.y;
    return *this;
}
Vec2 &Vec2::operator-=(Vec2 const&v)
{
    x-=v.x;
    y-=v.y;
    return *this;
}

Vec2 &Vec2::operator*=(double s)
{
    x*=s;
    y*=s;
    return *this;
}
Vec2 &Vec2::operator/=(double s)
{
    assert(s != 0.0f); // divison by zero
    x /= s;
    y /= s;
    return *this;
}

// unary minus
Vec2 Vec2::operator-() const
{
    return Vec2(-x, -y);
}

// length / normalization
double Vec2::Len() const
{
    return std::sqrt(x * x + y * y);
}
double Vec2::LenSq() const
{
    return x * x + y * y;
}

Vec2 Vec2::Norm() const
{
    double L = Len();
    return (L > 0.0f) ? Vec2(x / L, y / L) : *this;
}

//comparison
bool Vec2::operator==(Vec2 const& other) const {
    return x == other.x && y == other.y;
}
bool Vec2::operator!=(Vec2 const& other) const {
    return !(*this == other);
}


void Vec2::Normalize()
{
    double L = Len();
    if (L > 0.0)
    {
        *this /= L;
    }
}

//non-member functions
double distance(Vec2 const&v0, Vec2 const&v1)
{
    double dx = v1.x - v0.x;
    double dy = v1.y - v0.y;
    return std::sqrt((dx * dx) + (dy * dy));
}

double distanceSq(Vec2 const&v0, Vec2 const&v1)
{
    double dx = v1.x - v0.x;
    double dy = v1.y - v0.y;
    return (dx * dx) + (dy * dy);
}

double dot(Vec2 const&v0, Vec2 const&v1)
{
    return (v0.x * v1.x) + (v0.y * v1.y);
}

double cross(Vec2 const&v0, Vec2 const&v1)
{
    return (v0.x * v1.y) - (v0.y * v1.x);
}

Vec2 operator+(Vec2 const&v0, Vec2 const&v1)
{
    Vec2 tmp{v0};
    return tmp += v1;
}
Vec2 operator-(Vec2 const&v0, Vec2 const&v1)
{
    Vec2 tmp{v0};
    return tmp -= v1;
}
Vec2 operator*(Vec2 const&v, double s)
{ // vector * scalar
    Vec2 tmp{v};
    return tmp *= s;
}
Vec2 operator*(double s, Vec2 const&v)
{ // scalar * vector
    Vec2 tmp{v};
    return tmp *= s;
}
Vec2 operator/(Vec2 const&v, double s)
{
    Vec2 tmp{v};
    return tmp /= s;
}

std::ostream &operator<<(std::ostream &os, Vec2 const &vec)
{
    //os << std::fixed << std::setprecision(3);
    return os << "Vec2(" << vec.x << ", " << vec.y << ")";
}











//wrapper
double VecLength(const Vec2& lhs, const Vec2& rhs)
{
    return distance(lhs,rhs);
}