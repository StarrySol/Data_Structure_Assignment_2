#pragma once


#include "Math.h"
#include <vector>
#include <queue>

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

    double cost = 0.0;//APSC cost

    bool valid = true;//deletion for PQ
};

struct Ring
{
    int ringID = 0;
    Node* head = nullptr;//circular linked list
    int size = 0;

    //Only used when parsing input data
    std::vector<Vertice> vertices;

    Ring();

    //Frees all pointers
    void FreeRing();
};

/************************************************************************/
/*!
\brief
Computes signed area of triangle formed by 3 points

\param a
First point

\param b
Second point

\param c
Third point

\return
Signed area of triangle
*/
/************************************************************************/
double SignedArea(const Vec2& a, const Vec2& b, const Vec2& c);

