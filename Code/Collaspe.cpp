#include "Collaspe.h"

#include <queue>

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

bool IsCollapseValid(Node* node)
{
    //TODO: Do this laterrr
    (void)(node);
    return true;
}


void SimplifyAll(std::vector<Ring>& rings, int target)
{
    struct Candidate
    {
        Node* node;
        double cost;

        bool operator>(const Candidate& other) const
        {
            return cost > other.cost;
        }
    };

    std::priority_queue<Candidate, std::vector<Candidate>, std::greater<>> pq;

    int totalVertices = 0;

    //Initialize
    for (Ring& ring : rings)
    {
        if (!ring.head)
            continue;

        totalVertices += ring.size;

        Node* curr = ring.head;

        for (int i = 0; i < ring.size; ++i)
        {
            curr->cost = ComputeCost(curr);
            pq.push({curr, curr->cost});
            curr = curr->next;
        }
    }

    //Main loop
    while (totalVertices > target && !pq.empty())
    {
        auto top = pq.top();
        pq.pop();

        Node* node = top.node;

        if (!node->valid)
            continue;

        Ring* ring = nullptr;

        //Find owning ring
        for (Ring& r : rings)
        {
            Node* curr = r.head;
            for (int i = 0; i < r.size; ++i)
            {
                if (curr == node)
                {
                    ring = &r;
                    break;
                }
                curr = curr->next;
            }
            if (ring) break;
        }

        if (!ring || ring->size <= 3)
            continue;

        if (!IsCollapseValid(node))
            continue;

        Node* prev = node->prev;
        Node* next = node->next;

        Collapse(*ring, node);

        ring->size--;
        totalVertices--;

        prev->cost = ComputeCost(prev);
        next->cost = ComputeCost(next);

        pq.push({prev, prev->cost});
        pq.push({next, next->cost});
    }
}