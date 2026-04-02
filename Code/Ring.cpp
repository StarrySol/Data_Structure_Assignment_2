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

int Orientation(const Vec2& a, const Vec2& b, const Vec2& c)
{
    double val = (b.y - a.y)*(c.x - b.x) - (b.x - a.x)*(c.y - b.y);

    if (std::abs(val) < 1e-9)
        return 0;//collinear

    return (val > 0) ? 1 : 2;
}

bool OnSegment(const Vec2& a, const Vec2& b, const Vec2& c)
{
    return (c.x <= std::max(a.x,b.x) && c.x >= std::min(a.x,b.x) &&
            c.y <= std::max(a.y,b.y) && c.y >= std::min(a.y,b.y));
}

bool SegmentsIntersect(const Vec2& p1, const Vec2& q1, const Vec2& p2, const Vec2& q2)
{
    int o1 = Orientation(p1,q1,p2);
    int o2 = Orientation(p1,q1,q2);
    int o3 = Orientation(p2,q2,p1);
    int o4 = Orientation(p2,q2,q1);

    //General case
    if (o1 != o2 && o3 != o4)
        return true;

    //Special cases(collinear)
    if (o1 == 0 && OnSegment(p1,q1,p2)) return true;
    if (o2 == 0 && OnSegment(p1,q1,q2)) return true;
    if (o3 == 0 && OnSegment(p2,q2,p1)) return true;
    if (o4 == 0 && OnSegment(p2,q2,q1)) return true;

    return false;
}