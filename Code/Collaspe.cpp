#include "Collaspe.h"

#include <iostream>

#include <queue>

//Compute collapse cost (area error)
double ComputeCost(Node* node)
{
    Node* prev = node->prev;
    Node* next = node->next;

    //Compute triangle area magnitude
    return std::abs(SignedArea(prev->v.pos, node->v.pos, next->v.pos));
}

void Collapse(Ring& ring, Node* node)
{
    Node* prev = node->prev;
    Node* next = node->next;

    //Compute new position
    Vec2 newPos = ComputeNewPoint(prev, node, next);

    //Assign new position to next node
    next->v.pos = newPos;

    //If removing head, move it forward
    if (node == ring.head)
    {
        ring.head = next;
    }

    //Reconnect list
    prev->next = next;
    next->prev = prev;

    //Mark node invalid
    node->valid = false;
}

bool IsCollapseValid(const std::vector<Ring>& rings, Node* node)
{
    Node* prev = node->prev;
    Node* next = node->next;

    //New edges after collapse
    Vec2 a = prev->v.pos;
    Vec2 b = next->v.pos;

    //Check against ALL edges in ALL rings
    for (const Ring& ring : rings)
    {
        if (!ring.head)
            continue;

        const Node* curr = ring.head;

        do
        {
            const Node* e1 = curr;
            const Node* e2 = curr->next;

            //Skip edges connected to collapsing node
            if (e1 == node || e2 == node ||
                e1 == prev || e2 == prev ||
                e1 == next || e2 == next)
            {
                curr = curr->next;
                continue;
            }

            if (SegmentsIntersect(a, b, e1->v.pos, e2->v.pos))
                return false;

            curr = curr->next;

        } while (curr != ring.head);
    }

    return true;
}

void SimplifyAll(std::vector<Ring>& rings, int target)
{
    //Priority queue candidate
    struct Candidate
    {
        Node* node;
        double cost;

        bool operator>(const Candidate& other) const
        {
            return cost > other.cost;
        }
    };

    //Min heap
    std::priority_queue<Candidate, std::vector<Candidate>, std::greater<>> pq;

    int totalVertices = 0;

    //Initialize PQ and count vertices
    for (Ring& ring : rings)
    {
        if (!ring.head)
            continue;

        totalVertices += ring.size;

        Node* curr = ring.head;

        //Use safe circular traversal instead of size-based loop
        do
        {
            curr->cost = ComputeCost(curr);
            pq.push({curr, curr->cost});
            curr = curr->next;
        } while (curr != ring.head);
    }

    while (totalVertices > target && !pq.empty())
    {
        //Get best candidate
        Candidate top = pq.top();
        pq.pop();

        Node* node = top.node;

        //Skip invalid nodes
        if (!node || !node->valid)
            continue;

        //Find owning ring (safe traversal)
        Ring* ownerRing = nullptr;

        for (Ring& r : rings)
        {
            if (!r.head)
                continue;

            Node* curr = r.head;

            do
            {
                if (curr == node)
                {
                    ownerRing = &r;
                    break;
                }
                curr = curr->next;
            } while (curr != r.head);

            if (ownerRing)
                break;
        }

        //Skip if not found or too small
        if (!ownerRing || ownerRing->size <= 3)
            continue;

        Node* prev = node->prev;
        Node* next = node->next;

        //Attempt collapse (including topology + area preservation)
        if (!TryCollapse(rings, *ownerRing, node))
            continue;

        //Do not decrement ring->size here
        //Already handled inside TryCollapse oops 

        totalVertices--;

        //Recompute costs locally
        if (prev && prev->valid)
        {
            prev->cost = ComputeCost(prev);
            pq.push({prev, prev->cost});
        }

        if (next && next->valid)
        {
            next->cost = ComputeCost(next);
            pq.push({next, next->cost});
        }
    }
}


Vec2 ComputeNewPoint(Node* prev, Node* curr, Node* next)
{
    Vec2 A = prev->v.pos;
    Vec2 B = curr->v.pos;
    Vec2 C = next->v.pos;

    //Original signed area
    double targetArea = SignedArea(A,B,C);

    //Direction vector AC
    Vec2 dir = C - A;

    //Normal vector perpendicular to AC
    Vec2 normal(-dir.y, dir.x);

    //Normalize normal
    double len = std::sqrt(normal.x*normal.x + normal.y*normal.y);
    if (len < 1e-9)
        return B;//degenerate

    normal.x /= len;
    normal.y /= len;

    //We move midpoint along normal to match area
    Vec2 mid = Vec2(
        (A.x + C.x)*0.5,
        (A.y + C.y)*0.5
    );

    //Area scaling factor
    double base = std::sqrt((C.x - A.x)*(C.x - A.x) + (C.y - A.y)*(C.y - A.y));

    if (base < 1e-9)
        return mid;

    double height = (2.0 * targetArea) / base;

    //Final point
    Vec2 result(
        mid.x + normal.x * height,
        mid.y + normal.y * height
    );

    return result;
}

bool TryCollapse(std::vector<Ring>& rings, Ring& ring, Node* node)
{
    if (!node->valid)
        return false;

    if (ring.size <= 3)
        return false;

    //Check topology
    if (!IsCollapseValid(rings, node))
        return false;

    Node* prev = node->prev;
    Node* next = node->next;

    //Compute exact area-preserving point
    Vec2 newPos = ComputeNewPoint(prev,node,next);

    //Assign new position
    next->v.pos = newPos;

    //Update head if needed
    if (node == ring.head)
        ring.head = next;

    //Reconnect
    prev->next = next;
    next->prev = prev;

    node->valid = false;

    ring.size--;

    return true;
}