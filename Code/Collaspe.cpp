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

    Vec2 newPos = ComputeNewPoint(prev, node, next);

    //New edges
    Vec2 e1a = prev->v.pos;
    Vec2 e1b = newPos;

    Vec2 e2a = newPos;
    Vec2 e2b = next->v.pos;

    for (const Ring& ring : rings)
    {
        if (!ring.head)
            continue;

        const Node* curr = ring.head;

        do
        {
            const Node* a = curr;
            const Node* b = curr->next;

            //Skip adjacent edges
            if (a == node || b == node ||
                a == prev || b == prev ||
                a == next || b == next)
            {
                curr = curr->next;
                continue;
            }

            //Check both new edges
            if (SegmentsIntersect(e1a, e1b, a->v.pos, b->v.pos) ||
                SegmentsIntersect(e2a, e2b, a->v.pos, b->v.pos))
            {
                return false;
            }

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

        if (!TryCollapse(rings, *ownerRing, node))
        {
            node->valid = false;//prevent reprocessing bad nodes
            continue;
        }

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
    const Vec2& A = prev->v.pos;
    const Vec2& B = curr->v.pos;
    const Vec2& C = next->v.pos;

    //Compute twice signed area of triangle ABC
    double area2 = (B.x - A.x)*(C.y - A.y) - (B.y - A.y)*(C.x - A.x);

    //Midpoint of AC
    Vec2 mid((A.x + C.x)*0.5, (A.y + C.y)*0.5);

    //Direction AC
    Vec2 d = C - A;
    double len = std::sqrt(d.x*d.x + d.y*d.y);

    if (len < 1e-9)
        return B;//degenerate case

    //Perpendicular unit normal
    Vec2 n(-d.y / len, d.x / len);

    //Height needed to preserve area
    double h = area2 / len;

    //Final point
    return Vec2(mid.x + n.x * h, mid.y + n.y * h);
}

bool TryCollapse(std::vector<Ring>& rings, Ring& ring, Node* node)
{
    if (!node || !node->valid)
        return false;

    if (ring.size <= 3)
        return false;

    Node* prev = node->prev;
    Node* next = node->next;

    //Check topology BEFORE modifying anything
    if (!IsCollapseValid(rings, node))
        return false;

    //Compute new vertex
    Vec2 newPos = ComputeNewPoint(prev, node, next);

    //Apply position to NEXT node (APSC standard)
    next->v.pos = newPos;

    //Update head if needed
    if (node == ring.head)
        ring.head = next;

    //Reconnect list
    prev->next = next;
    next->prev = prev;

    node->valid = false;

    ring.size--;

    return true;
}