
#pragma once
#include <vector>
#include <cstddef> //size_t
#include "Math.h"

class Ring
{
public:
    Ring() = default;
    explicit Ring(unsigned int id);

    unsigned int GetRingID() const;
    void SetRingID(unsigned int id);

    void AddVertex(Vec2 const& v);

    std::vector<Vec2> const& GetVertices() const;
    //std::vector<Vec2>& GetVertices();

    std::size_t VertexCount() const;
    bool IsEmpty() const;

    bool IsClosed() const;
    double SignedArea() const;
    double Area() const;
    bool IsClockwise() const;

private:
    unsigned int ring_id{};
    std::vector<Vec2> vertices;
};

class Polygon
{
public:
    void AddRing(const Ring& ring);

    std::vector<Ring> const& GetRings() const;
    //std::vector<Ring>& GetRings();

    std::size_t RingCount() const;
    bool IsEmpty() const;

private:
    std::vector<Ring> rings;
};
