
#include "Polygon.h"
#include <cmath>

Ring::Ring(unsigned int id) : ring_id(id)
{
}

unsigned int Ring::GetRingID() const
{
    return ring_id;
}

void Ring::SetRingID(unsigned int id)
{
    ring_id = id;
}

void Ring::AddVertex(Vec2 const& v)
{
    vertices.push_back(v);
}

std::vector<Vec2> const& Ring::GetVertices() const
{
    return vertices;
}

// std::vector<Vec2>& Ring::GetVertices()
// {
//     return vertices;
// }

std::size_t Ring::VertexCount() const
{
    return vertices.size();
}

bool Ring::IsEmpty() const
{
    return vertices.empty();
}

//A polygon ring is closed when the first vertex and last vertex are the same point.
bool Ring::IsClosed() const
{
    if (vertices.size() < 2)
    {
        return false;
    }

    return vertices.front() == vertices.back();
}

double Ring::SignedArea() const
{
    if (vertices.size() < 3)
    {
        return 0.0;
    }

    double area = 0.0;

    for (std::size_t i = 0; i < vertices.size(); ++i)
    {
        Vec2 const& current = vertices[i];
        Vec2 const& next = vertices[(i + 1) % vertices.size()];

        area += (current.x * next.y) - (next.x * current.y);
    }

    return 0.5 * area;
}

double Ring::Area() const
{
    return std::abs(SignedArea());
}

bool Ring::IsClockwise() const
{
    return SignedArea() < 0.0;
}

void Polygon::AddRing(Ring const& ring)
{
    rings.push_back(ring);
}

std::vector<Ring> const& Polygon::GetRings() const
{
    return rings;
}

// std::vector<Ring>& Polygon::GetRings()
// {
//     return rings;
// }

std::size_t Polygon::RingCount() const
{
    return rings.size();
}

bool Polygon::IsEmpty() const
{
    return rings.empty();
}

Vec2 const& Ring::GetVertex(std::size_t index) const
{
    return vertices[index];
}

Ring const& Polygon::GetRing(std::size_t index) const
{
    return rings[index];
}

double Polygon::TotalSignedArea() const
{
    double total = 0.0;

    for (Ring const& ring : rings)
    {
        total += ring.SignedArea();
    }

    return total;
}

std::size_t Polygon::VertexCount() const
{
    std::size_t total = 0;

    for (Ring const& ring : rings)
    {
        total += ring.VertexCount();
    }

    return total;
}