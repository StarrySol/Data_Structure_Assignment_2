#pragma once

#include "Ring.h"

#include <queue>
#include <vector>

//Candidate for priority queue
struct CollapseCandidate
{
    Node* node;
    double cost;

    bool operator>(const CollapseCandidate& other) const
    {
        return cost > other.cost;
    }
};

//Min-heap priority queue alias
using MinHeap = std::priority_queue<
    CollapseCandidate,
    std::vector<CollapseCandidate>,
    std::greater<CollapseCandidate>
>;

/************************************************************************/
/*!
\brief
Computes APSC cost for a node based on triangle area

\param node
Current node

\return
Cost value
*/
/************************************************************************/
double ComputeCost(Node* node);

/************************************************************************/
/*!
\brief
Computes new vertex position after collapse

\param prev
Previous node

\param curr
Current node

\param next
Next node

\return
New vertex position
*/
/************************************************************************/
Vec2 ComputeNewPoint(Node* prev, Node* curr, Node* next);

/************************************************************************/
/*!
\brief
Collapses a node by removing it and reconnecting neighbors

\param node
Node to remove
*/
/************************************************************************/
void Collapse(Node* node);

/************************************************************************/
/*!
\brief
Simplifies a ring using APSC algorithm

\param ring
Ring to simplify

\param totalVertices
Total vertex count across all rings

\param target
Target vertex count
*/
/************************************************************************/
void SimplifyRing(Ring& ring, int& totalVertices, int target);