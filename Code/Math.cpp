
#include "Math.h"
#include <cassert>
#include <iomanip>

/************************************************* */
/********************ctor/dtor******************** */
/************************************************* */
// default ctor
Vector2::Vector2() : m{0.0f, 0.0f},
                     x(m[0]), y(m[1])
{ /*empty by design*/
}

// 2 double param ctor
Vector2::Vector2(double x_, double y_) : m{x_, y_},
                                       x(m[0]), y(m[1])
{ /*empty by design*/
}

// copy ctor
Vector2::Vector2(Vector2 const &v) : m{v.m[0], v.m[1]},
                                     x(m[0]), y(m[1])
{ /*empty by design*/
}

// default dtor
Vector2::~Vector2() = default;

/************************************************* */
/*************member functions******************** */
/************************************************* */

// copy assignment
Vector2 &Vector2::operator=(Vector2 v)
{
    if (this != &v)
    {
        m[0] = v.m[0];
        m[1] = v.m[1];
    }
    return *this;
}

// element access
double &Vector2::operator[](size_t i)
{
    assert(i < 2);
    return m[i];
}
double Vector2::operator[](size_t i) const
{
    assert(i < 2);
    return m[i];
}

Vector2 &Vector2::operator+=(Vector2 const&v)
{
    m[0] += v.m[0];
    m[1] += v.m[1];
    return *this;
}
Vector2 &Vector2::operator-=(Vector2 const&v)
{
    m[0] -= v.m[0];
    m[1] -= v.m[1];
    return *this;
}

Vector2 &Vector2::operator*=(double s)
{
    m[0] *= s;
    m[1] *= s;
    return *this;
}
Vector2 &Vector2::operator/=(double s)
{
    assert(s != 0.0f); // divison by zero
    m[0] /= s;
    m[1] /= s;
    return *this;
}

// unary minus
Vector2 Vector2::operator-() const
{
    return Vector2(-m[0], -m[1]);
}

// length / normalization
double Vector2::Len() const
{
    return std::sqrt(m[0] * m[0] + m[1] * m[1]);
}
double Vector2::LenSq() const
{
    return m[0] * m[0] + m[1] * m[1];
}

Vector2 Vector2::Norm() const
{
    double L = Len();
    return (L > 0.0f) ? Vector2(m[0] / L, m[1] / L) : *this;
}

void Vector2::Normalize()
{
    double L = Len();
    if (L > 0.0f)
    {
        *this /= L;
    }
}

bool Vector2::operator==(Vector2 const& other) const {
    return x == other.x && y == other.y;
}
bool Vector2::operator!=(Vector2 const& other) const {
    return !(*this == other);
}

// double& Vector2::setX() { 
//     return m[0]; 
// }
// double& Vector2::setY() { 
//     return m[1]; 
// }
// double  Vector2::getX() const { 
//     return m[0]; 
// }
// double  Vector2::getY() const { 
//     return m[1]; 
// }
// Vector2& Vector2::Set(double x_, double y_)
// {
//     m[0] = x_;
//     m[1] = y_;
//     return *this;
// }
//non-member functions
double distance(Vector2 const&v0, Vector2 const&v1)
{
    double dx = v1.x - v0.x;
    double dy = v1.y - v0.y;
    return std::sqrt((dx * dx) + (dy * dy));
}

double distanceSq(Vector2 const&v0, Vector2 const&v1)
{
    double dx = v1.x - v0.x;
    double dy = v1.y - v0.y;
    return (dx * dx) + (dy * dy);
}

double dot(Vector2 const&v0, Vector2 const&v1)
{
    return (v0.x * v1.x) + (v0.y * v1.y);
}

double cross(Vector2 const&v0, Vector2 const&v1)
{
    return (v0.x * v1.y) - (v0.y * v1.x);
}

Vector2 operator+(Vector2 const&v0, Vector2 const&v1)
{
    Vector2 tmp{v0};
    return tmp += v1;
}
Vector2 operator-(Vector2 const&v0, Vector2 const&v1)
{
    Vector2 tmp{v0};
    return tmp -= v1;
}
Vector2 operator*(Vector2 const&v, double s)
{ // vector * scalar
    Vector2 tmp{v};
    return tmp *= s;
}
Vector2 operator*(double s, Vector2 const&v)
{ // scalar * vector
    Vector2 tmp{v};
    return tmp *= s;
}
Vector2 operator/(Vector2 const&v, double s)
{
    Vector2 tmp{v};
    return tmp /= s;
}

std::ostream &operator<<(std::ostream &os, Vector2 const &vec)
{
    os << std::fixed << std::setprecision(3);
    return os << "Vector2(" << vec[0] << ", " << vec[1] << ")";
}