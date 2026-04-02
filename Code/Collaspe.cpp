#include "Collaspe.h"

//Compute collapse cost (area error)
double ComputeCost(Node* node)
{
    Node* prev = node->prev;
    Node* next = node->next;

    //Compute triangle area magnitude
    return std::abs(SignedArea(prev->v.pos, node->v.pos, next->v.pos));
}

Vec2 ComputeNewPoint(Node* prev, Node* curr, Node* next)
{
    (void)curr;
    //Compute midpoint as approximation
    Vec2 mid(
        (prev->v.pos.x + next->v.pos.x) * 0.5,
        (prev->v.pos.y + next->v.pos.y) * 0.5
    );

    return mid;
}

void Collapse(Node* node)
{
    Node* prev = node->prev;
    Node* next = node->next;

    //Compute new position
    Vec2 newPos = ComputeNewPoint(prev, node, next);

    //Assign new position to next node
    next->v.pos = newPos;

    //Reconnect list
    prev->next = next;
    next->prev = prev;

    //Mark node invalid
    node->valid = false;
}

void SimplifyRing(Ring& ring, int& totalVertices, int target)
{
    MinHeap pq;//priority queue

    //Initialize PQ
    Node* curr = ring.head;

    for (int i = 0; i < ring.size; ++i)
    {
        curr->cost = ComputeCost(curr);
        pq.push({curr, curr->cost});
        curr = curr->next;
    }

    //Iterative simplification
    while (totalVertices > target && !pq.empty())
    {
        CollapseCandidate top = pq.top();
        pq.pop();

        Node* node = top.node;

        //Skip invalid nodes
        if (!node->valid)
            continue;

        Node* prev = node->prev;
        Node* next = node->next;

        //Prevent collapsing below triangle
        if (ring.size <= 3)
            break;

        //TODO:topology check(required for full correctness)

        //Collapse node
        Collapse(node);

        ring.size--;
        totalVertices--;

        //Update neighbors
        prev->cost = ComputeCost(prev);
        next->cost = ComputeCost(next);

        pq.push({prev, prev->cost});
        pq.push({next, next->cost});
    }
}