/******************************************************************************
 * File:        Ring.cpp
 * Project:     Data Structures Assigment 2
 *
 * Description:
 * This file contains the definition of classes involving a ring and required structs
 * A ring is a closed polyline of a polygon (or one of its holes)
 * It also can helper functions to check area, segement, .etc.
 ******************************************************************************/

#include "Ring.h"

#include <vector>
#include <cmath>
#include <limits>
#include <iterator>


Ring::Ring() {
    //Empty by design
 }

void Ring::FreeRing()
{
    if (!head) return;

    Node* curr  = head;
    Node* start = curr;

    //Delete all nodes via loo[]
    do
    {
        Node* next = curr->next;
        delete curr;
        curr = next;
    } while (curr != start);

    head = nullptr;
}


double SignedArea(const Vec2& a, const Vec2& b, const Vec2& c)
{
    return 0.5 * ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)
    );
}


 double ComputeRingArea(const Ring& ring)
 {
     if (!ring.head) return 0.0;
     
     const Node* curr = ring.head;
     double area = 0.0;
     
    //Shoelace formula:  2*Area = sum_i (x_i * y_{i+1} - x_{i+1} * y_i)
    //Loop thorugh circular linked list once, sum the cross products
    do
    {
        const Node* next = curr->next;

        // Shoelace term for edge curr -> next
        area += (curr->v.pos.x * next->v.pos.y) - (next->v.pos.x * curr->v.pos.y);

        curr = next;
    } while (curr != ring.head);

    return 0.5 * area;
}


double ComputeDisplacement(const std::vector<Ring>& rings)
{
    //The areal displacement measures how much the shape changed.
    //Compare it to orginal area and sum acrros all rings

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
    double val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);

    if (std::abs(val) < 1e-9) return 0;  //collinear
    return (val > 0) ? 1 : 2;            //1=CW, 2=CCW
}

bool OnSegment(const Vec2& a, const Vec2& b, const Vec2& c)
{
    return (c.x <= std::max(a.x, b.x) && c.x >= std::min(a.x, b.x) &&
            c.y <= std::max(a.y, b.y) && c.y >= std::min(a.y, b.y));
}

bool SegmentsIntersect(const Vec2& p1, const Vec2& q1, const Vec2& p2, const Vec2& q2)
{
    //Tests whether segment (p1,q1) intersects segment (p2,q2).
    int o1 = Orientation(p1, q1, p2);
    int o2 = Orientation(p1, q1, q2);
    int o3 = Orientation(p2, q2, p1);
    int o4 = Orientation(p2, q2, q1);

    //Each segment straddles the other's line
    if (o1 != o2 && o3 != o4) return true;

    //Special cases: collinear overlap
    if (o1 == 0 && OnSegment(p1, q1, p2)) return true;
    if (o2 == 0 && OnSegment(p1, q1, q2)) return true;
    if (o3 == 0 && OnSegment(p2, q2, p1)) return true;
    if (o4 == 0 && OnSegment(p2, q2, q1)) return true;

    return false;
}
