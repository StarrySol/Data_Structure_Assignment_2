/*
Data model for inputs
*/

#pragma once
#include "Math.h"
#include <vector>

struct Vertex
{
    int ring_id{};
    int vertex_id{};
    Vector2 p{};
};

struct Ring
{
    int ring_id{};
    std::vector<Vertex> vertices;
};

struct Polygon
{
    Ring exterior;
    std::vector<Ring> holes;
};