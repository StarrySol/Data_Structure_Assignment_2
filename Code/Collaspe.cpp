#include "Collaspe.h"

#include <iostream>

#include <queue>

//Compute collapse cost (area error)
double ComputeCost(Node* node)
{
    Node* prev = node->prev;
    Node* next = node->next;

    const Vec2& A = prev->v.pos;
    const Vec2& B = node->v.pos;
    const Vec2& C = next->v.pos;

    //Triangle area (importance of removing this vertex)
    double area = std::abs(SignedArea(A, B, C));

    //Edge lengths
    double len1 = std::hypot(B.x - A.x, B.y - A.y);
    double len2 = std::hypot(C.x - B.x, C.y - B.y);

    //Prevent division by zero
    if (len1 < 1e-9 || len2 < 1e-9)
        return std::numeric_limits<double>::max();

    //Angle penalty (preserve sharp corners)
    double dot = (B.x - A.x)*(C.x - B.x) + (B.y - A.y)*(C.y - B.y);
    double anglePenalty = std::abs(dot) / (len1 * len2);

    //Final weighted cost
    return area * (1.0 + anglePenalty);
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

            if (SegmentsIntersect(e1a, e1b, a->v.pos, b->v.pos) ||
                SegmentsIntersect(e2a, e2b, a->v.pos, b->v.pos))
                return false;

            curr = curr->next;

        } while (curr != ring.head);
    }

    return true;
}
void SimplifyAll(std::vector<Ring>& rings, int target)
{
    using MinHeap = std::priority_queue<
        CollapseCandidate,
        std::vector<CollapseCandidate>,
        std::greater<CollapseCandidate>
    >;

    MinHeap pq;

    int totalVertices = 0;
    int globalID = 0;

    //Initialize PQ
    for (Ring& ring : rings)
    {
        if (!ring.head)
            continue;

        totalVertices += ring.size;

        Node* curr = ring.head;

        do
        {
            curr->cost = ComputeCost(curr);

            pq.push({
                curr,
                curr->cost,
                globalID++,
                curr->version
            });

            curr = curr->next;

        } while (curr != ring.head);
    }

    //Main loop
    while (totalVertices > target && !pq.empty())
    {
        CollapseCandidate top = pq.top();
        pq.pop();

        Node* node = top.node;

        //Skip invalid nodes
        if (!node || !node->valid)
            continue;

        //Skip outdated PQ entries
        if (top.version != node->version)
            continue;

        //Find owning ring
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

        if (!ownerRing || ownerRing->size <= 3)
            continue;

        Node* prev = node->prev;
        Node* next = node->next;

        if (!TryCollapse(rings, *ownerRing, node))
            continue;

        totalVertices--;

        //Update neighbors
        if (prev && prev->valid)
        {
            prev->version++; //invalidate old PQ entries
            prev->cost = ComputeCost(prev);
            pq.push(CollapseCandidate(prev, prev->cost, globalID++, prev->version));
        }
        
        if (next && next->valid)
        {
            next->version++; //invalidate old PQ entries
            next->cost = ComputeCost(next);
            pq.push(CollapseCandidate(next, next->cost, globalID++, next->version));
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

    //Check topology
    if (!IsCollapseValid(rings, node))
        return false;

    //Compute new position
    Vec2 newPos = ComputeNewPoint(prev, node, next);

    //Reject unstable movement
    double moveDist = std::hypot(
        newPos.x - node->v.pos.x,
        newPos.y - node->v.pos.y
    );

    if (moveDist > 500.0) //tunable threshold
        return false;

    //Apply new position
    next->v.pos = newPos;

    //Update head
    if (node == ring.head)
        ring.head = next;

    //Reconnect list
    prev->next = next;
    next->prev = prev;

    node->valid = false;

    ring.size--;

    return true;
}