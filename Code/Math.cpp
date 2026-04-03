
/*================================================ */
/********************Math.cpp******************** */
/*================================================*/
/*
Contains pure math/Geometry helpers 
*/
#include "Math.h"
#include <iomanip>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <cmath>
/************************************************* */
/********************ctor/dtor******************** */
/************************************************* */
Vec2::Vec2(double inX, double inY) : x(inX), y(inY) { /*empty by design*/}


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

//
bool NearlyEqual(double a, double b, double eps)
{
    return std::abs(a - b) <= eps;
}

bool NearlyEqual(Vec2 const& a, Vec2 const& b, double eps)
{
    return NearlyEqual(a.x, b.x, eps) && NearlyEqual(a.y, b.y, eps);
}

// 2D cross product of AB x AC
double cross(Vec2 const& a, Vec2 const& b, Vec2 const& c)
{
    return (b.x - a.x) * (c.y - a.y) -
           (b.y - a.y) * (c.x - a.x);
}

double SignedTriangleArea(Vec2 const& a, Vec2 const& b, Vec2 const& c)
{
    return 0.5 * cross(a, b, c);
}

int Orientation(Vec2 const& a, Vec2 const& b, Vec2 const& c)
{
    double val = cross(a, b, c);

    if (NearlyEqual(val, 0.0))
        return 0; // collinear

    return (val > 0.0) ? 2 : 1; // 2 = counterclockwise, 1 = clockwise
}

bool OnSegment(Vec2 const& a, Vec2 const& b, Vec2 const& p)
{
    if (Orientation(a, b, p) != 0)
        return false;

    return p.x <= std::max(a.x, b.x) + 1e-9 &&
           p.x >= std::min(a.x, b.x) - 1e-9 &&
           p.y <= std::max(a.y, b.y) + 1e-9 &&
           p.y >= std::min(a.y, b.y) - 1e-9;
}

bool SegmentsIntersect(Vec2 const& p1, Vec2 const& q1,
                       Vec2 const& p2, Vec2 const& q2)
{
    int o1 = Orientation(p1, q1, p2);
    int o2 = Orientation(p1, q1, q2);
    int o3 = Orientation(p2, q2, p1);
    int o4 = Orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special collinear cases
    if (o1 == 0 && OnSegment(p1, q1, p2)) return true;
    if (o2 == 0 && OnSegment(p1, q1, q2)) return true;
    if (o3 == 0 && OnSegment(p2, q2, p1)) return true;
    if (o4 == 0 && OnSegment(p2, q2, q1)) return true;

    return false;
}

// positive if p is to the left of ab, negative if right, 0 if on line
double PointLineSide(Vec2 const& a, Vec2 const& b, Vec2 const& p)
{
    return cross(a, b, p);
}

double DistancePointToLine(Vec2 const& a, Vec2 const& b, Vec2 const& p)
{
    double len = distance(a, b);

    if (NearlyEqual(len, 0.0))
        return distance(a, p);

    return std::abs(cross(a, b, p)) / len;
}

std::ostream &operator<<(std::ostream &os, Vec2 const &vec)
{
    //os << std::fixed << std::setprecision(3);
    return os << "Vec2(" << vec.x << ", " << vec.y << ")";
}

//
Vec2 LineIntersection(Vec2 const& p1, Vec2 const& p2,
                      Vec2 const& q1, Vec2 const& q2)
{
    Vec2 r = p2 - p1;
    Vec2 s = q2 - q1;

    double denom = cross(r, s);

    if (NearlyEqual(denom, 0.0))
    {
        return p1; // fallback for parallel/nearly parallel lines
    }

    double t = cross(q1 - p1, s) / denom;
    return p1 + t * r;
}

double TriangleArea(Vec2 const& a, Vec2 const& b, Vec2 const& c)
{
    return std::abs(SignedTriangleArea(a, b, c));
}

int SideOfDirectedLine(Vec2 const& a, Vec2 const& b, Vec2 const& p)
{
    double side = PointLineSide(a, b, p);

    if (NearlyEqual(side, 0.0))
    {
        return 0;
    }

    return (side > 0.0) ? 1 : -1;
}

double SignedDistancePointToLine(Vec2 const& a, Vec2 const& b, Vec2 const& p)
{
    double len = distance(a, b);

    if (NearlyEqual(len, 0.0))
    {
        return distance(a, p);
    }

    return cross(a, b, p) / len;
}

//wrapper
double VecLength(const Vec2& lhs, const Vec2& rhs)
{
    return distance(lhs,rhs);
}