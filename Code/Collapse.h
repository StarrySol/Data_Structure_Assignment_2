#pragma once

#include "Ring.h"

#include <queue>
#include <vector>

//Candidate for priority queue
struct CollapseCandidate
{
    Node* node;
    double cost;
    int id;
    int version;

    //Constructor (REQUIRED for pq.push)
    CollapseCandidate(Node* n, double c, int i, int v)
        : node(n), cost(c), id(i), version(v)
    {}

    bool operator>(const CollapseCandidate& other) const
    {
        if (std::abs(cost - other.cost) > 1e-9)
            return cost > other.cost;

        if (node->v.vertex_id != other.node->v.vertex_id)
            return node->v.vertex_id > other.node->v.vertex_id;

        return id > other.id;
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

\param ring
Ring containing node

\param node
Node to remove
*/
/************************************************************************/
void Collapse(Ring& ring, Node* node);

/************************************************************************/
/*!
\brief
Checks if collapsing node introduces intersection

\param rings
All rings

\param node
Node to collapse

\return
True if safe
False if not safe
*/
/************************************************************************/
bool IsCollapseValid(const std::vector<Ring>& rings, Node* node);


/************************************************************************/
/*!
\brief
Global APSC simplification across all rings

\param rings
All rings

\param target
Target vertex count
*/
/************************************************************************/
void SimplifyAll(std::vector<Ring>& rings, int target);

/************************************************************************/
/*!
\brief
Computes new vertex position using true APSC (area-preserving polyline collapse)

\param prev
Previous node

\param curr
Current node

\param next
Next node

\return
New point preserving triangle area
*/
/************************************************************************/
Vec2 ComputeNewPoint(Node* prev, Node* curr, Node* next);

/************************************************************************/
/*!
\brief
Performs safe APSC collapse with topology and area preservation

\param rings
All rings

\param ring
Ring of node

\param node
Node to collapse

\return
True if collapse performed
False if collapse not performed
*/
/************************************************************************/
bool TryCollapse(std::vector<Ring>& rings, Ring& ring, Node* node);