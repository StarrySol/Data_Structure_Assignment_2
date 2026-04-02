#pragma once


#include "Math.h"
#include <vector>

struct Vertice
{
    int ring_id = 0;
    int vertex_id = 0;
    Vec2 pos{};
};

struct Node
{
    Vertice v;

    Node* prev = nullptr;
    Node* next = nullptr;

    double cost = 0.0;//APSC error metric

    bool valid = true;//lazy deletion for PQ
};

struct Ring
{
    int ringID = 0;
    std::vector<Vertice> vertices{};

    Ring();
};

double TriangleArea(const Vec2& a, const Vec2& b, const Vec2& c);

