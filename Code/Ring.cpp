#include "Ring.h"

#include <vector>
#include <cmath>
#include <limits>
#include <iterator>

Ring::Ring()
{
    
}

void Ring::FreeRing()
{
    if (!head)
        return;

    Node* curr = head;
    Node* start = curr;

    do
    {
        Node* next = curr->next;
        delete curr;
        curr = next;
    } while (curr != start);

    head = nullptr;
}

//Compute triangle area using cross product
double SignedArea(const Vec2& a, const Vec2& b, const Vec2& c)
{
    return 0.5 * (
        (b.x - a.x)*(c.y - a.y) -
        (b.y - a.y)*(c.x - a.x)
    );
}

double ComputeRingArea(const Ring& ring)
{
    if (!ring.head)
        return 0.0;

    const Node* curr = ring.head;
    double area = 0.0;

    do
    {
        const Node* next = curr->next;
        area += (curr->v.pos.x * next->v.pos.y) -
                (next->v.pos.x * curr->v.pos.y);

        curr = next;
    } while (curr != ring.head);

    return 0.5 * area;
}
double ComputeDisplacement(const std::vector<Ring>& rings)
{
    double disp = 0.0;

    for (const Ring& ring : rings)
    {
        double newArea = ComputeRingArea(ring);
        disp += std::abs(newArea - ring.originalArea);
    }

    return disp;
}

double ComputeTotalArea(const std::vector<Ring>& rings)
{
    double total = 0.0;

    for (const Ring& ring : rings)
        total += ComputeRingArea(ring);

    return total;
}