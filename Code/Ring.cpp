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
