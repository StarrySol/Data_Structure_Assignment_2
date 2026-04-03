/******************************************************************************
 * File:        Collaspe.cpp
 * Project:     Data Structures Assigment 2
 *
 * Description:
 * This file contains the definition of functions to apply Area-Preserving Segment Collapse (APSC)
 * to a polygon ring to try to reduce the number of polygons while preserving the area
 * 
 * For example, it tries to replace the path A->B->C->D with the shorter path
 * A->E->D, with 1 point (E) instead of the orginal 2 (B & C) in a way
 * that preserve the orignal area. Point E can be anywhere that matches the are
 * but it tries to limnit the area swept between the old and new path (areal displacement)
 * 

 ******************************************************************************/

#include "Collaspe.h"

#include <iostream>
#include <queue>
#include <cmath>
#include <limits>

double ComputeCost(Node* node)
{
    //node is B in the 4-node window A -> B -> C -> D
    Node* A = node->prev;
    Node* C = node->next;
    Node* D = C->next;

    //Sanity check: the ring must have at least 4 distinct nodes
    if (A == C || A == D || node == D)
        return std::numeric_limits<double>::max();

    const Vec2& a = A->v.pos;
    const Vec2& b = node->v.pos;
    const Vec2& c = C->v.pos;
    const Vec2& d = D->v.pos;

    //Cost = sum of absolute areas of the two triangles being replaced.
    //A collapse that flattens a tall spike costs more than one that
    //removes a barely-visible bump.
    double cost = std::abs(SignedArea(a, b, c))+ std::abs(SignedArea(a, c, d));

    return cost;
}


bool IsCollapseValid(const std::vector<Ring>& rings, Node* A, Node* B, Node* C, Node* D)
{
    //Compute where E will be placed
    Vec2 E = ComputeNewPoint(A, B, C, D);

    //The two new edges introduced by the collapse
    Vec2 e1a = A->v.pos;  //edge 1: A -> E
    Vec2 e1b = E;
    Vec2 e2a = E; //edge 2: E -> D
    Vec2 e2b = D->v.pos;

    //Test against every edge of every ring
    for (const Ring& ring : rings)
    {
        if (!ring.head) continue;

        const Node* curr = ring.head;

        do
        {
            const Node* edgeA = curr;
            const Node* edgeB = curr->next;

            //Skip edges that touch A, B, C, or D -- they are
            //part of the collapsing neighbourhood and will be
            //relinked; intersections here are not real violations.
            bool adjacent = (edgeA == A || edgeB == A ||
                             edgeA == B || edgeB == B ||
                             edgeA == C || edgeB == C ||
                             edgeA == D || edgeB == D);

            if (!adjacent)
            {
                const Vec2& ea = edgeA->v.pos;
                const Vec2& eb = edgeB->v.pos;

                if (SegmentsIntersect(e1a, e1b, ea, eb) || SegmentsIntersect(e2a, e2b, ea, eb))
                    return false;   //topology would be violated
            }

            curr = curr->next;

        } while (curr != ring.head);
    }

    return true;  //no intersections found -- safe to collapse
}


void SimplifyAll(std::vector<Ring>& rings, int target)
{
    MinHeap pq; //global min-heap of collapse candidates
    int totalVertices = 0;
    int globalID = 0; //monotone counter for stable tie-breaking

    
    //--Seed the heap with one candidate per node--
    
    //Each node is treated as "B" in the 4-node window A->B->C->D.
    //We compute its initial cost and push a CollapseCandidate.
    
    for (Ring& ring : rings)
    {
        if (!ring.head) continue;

        totalVertices += ring.size;

        Node* curr = ring.head;

        do
        {
            curr->cost = ComputeCost(curr);

            pq.push(CollapseCandidate(
                curr,
                curr->cost,
                globalID++,
                curr->version  //version = 0 at this point
            ));

            curr = curr->next;

        } while (curr != ring.head);
    }

    //Greedy collapse loop
    //Pop the cheapest candidate, validate it, apply it, and push updated candidates for the affected neighbours
    
    while (totalVertices > target && !pq.empty())
    {
        CollapseCandidate top = pq.top();
        pq.pop();

        Node* node = top.node;

        //Skip if the node was already removed by a previous collapse.
        if (!node || !node->valid) continue;

        //Skip stale entries: the node's cost was updated after
        //this entry was pushed, so a fresher entry is already in
        //the heap (or was already processed).
        if (top.version != node->version) continue;

        
        //Find the ring that owns this node.
        //
        //We walk every ring's circular list looking for a pointer
        //match.  This is O(total_vertices) in the worst case but
        //avoids storing a back-pointer from Node to Ring.
        
        Ring* ownerRing = nullptr;

        for (Ring& r : rings)
        {
            if (!r.head) continue;

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

            if (ownerRing) break;
        }

        //Can't collapse if the ring is too small or not found.
        if (!ownerRing || ownerRing->size <= 3) continue;

        //Capture the neighbours BEFORE the collapse changes pointers.
        //A = prev of B (stays)
        //C = next of B (will become E)
        //D = next of C (stays)
        Node* A = node->prev;
        Node* C = node->next;
        Node* D = (C != nullptr) ? C->next : nullptr;

        //Attempt the collapse.  If it fails (topology check, stability
        //check, or ring too small) we simply skip this candidate.
        if (!TryCollapse(rings, *ownerRing, node)) continue;

        totalVertices--;

        
        //Push updated candidates for the three nodes whose 4-node
        //windows changed:
        //
        //  A  : now its window is (A->prev) -> A -> E -> D
        //  C/E: now its window is A -> E -> D -> (D->next)
        //  D  : now its window is E -> D -> (D->next) -> ...
        //
        //We increment each node's version to invalidate the old heap entries for them
        
        if (A && A->valid)
        {
            A->version++;
            A->cost = ComputeCost(A);
            pq.push(CollapseCandidate(A, A->cost, globalID++, A->version));
        }

        if (C && C->valid)   //C is now E
        {
            C->version++;
            C->cost = ComputeCost(C);
            pq.push(CollapseCandidate(C, C->cost, globalID++, C->version));
        }

        if (D && D->valid && D != A && D != C)
        {
            D->version++;
            D->cost = ComputeCost(D);
            pq.push(CollapseCandidate(D, D->cost, globalID++, D->version));
        }
    }
}


Vec2 ComputeNewPoint(Node* A, Node* B, Node* C, Node* D)
{
    const Vec2& a = A->v.pos;
    const Vec2& b = B->v.pos;
    const Vec2& c = C->v.pos;
    const Vec2& d = D->v.pos;

    //cross(p,q) = p.x*q.y - q.x*p.y   (one shoelace term)
    auto cross2 = [](const Vec2& p, const Vec2& q) -> double {
        return p.x * q.y - q.x * p.y;
    };

    //RHS: total shoelace contribution of the three OLD edges
    double rhs = cross2(a, b) + cross2(b, c) + cross2(c, d);

    //Midpoint and direction of segment A -> D
    Vec2 mid((a.x + d.x) * 0.5, (a.y + d.y) * 0.5);
    Vec2 ad = d - a;
    double len = std::sqrt(ad.x * ad.x + ad.y * ad.y);

    if (len < 1e-9)
        return c;   //degenerate: A and D are the same point; keep C's position

    //Unit normal to A->D (rotated 90 CCW)
    Vec2 n_hat(-ad.y / len, ad.x / len);

    //Evaluate the constraint equation at E = mid (t = 0):
    //  (A.x - D.x)*mid.y + (D.y - A.y)*mid.x  = lhs_const
    double lhs_const = mid.y * (a.x - d.x) + mid.x * (d.y - a.y);

    //Derivative of the constraint equation along n_hat:
    //  (A.x - D.x)*n_hat.y + (D.y - A.y)*n_hat.x
    //It can be shown algebraically that this equals -|AD| = -len.
    double t = (rhs - lhs_const) / (-len);

    //Final point: the area-preserving, displacement-minimising E
    return Vec2(mid.x + n_hat.x * t, mid.y + n_hat.y * t);
}


bool TryCollapse(std::vector<Ring>& rings, Ring& ring, Node* node)
{
    //node is B in A -> B -> C -> D

    //Guard: node must be live and the ring must have room to shrink.
    if (!node || !node->valid) return false;
    if (ring.size <= 3)        return false;

    Node* A = node->prev; //will remain, A->next will point to E after collapse
    Node* B = node; //will be unlinked
    Node* C = node->next; //will be repositioned to E
    Node* D = C->next; //will remain, D->prev will point to E after collapse

    //Degenerate ring guard: all four pointers must be distinct.
    if (A == C || B == D || A == D) return false;

    //Topology check: ensure the new edges A->E and E->D don't
    //intersect any existing edge of any ring.
    if (!IsCollapseValid(rings, A, B, C, D)) return false;

    //Compute the new vertex position E.
    Vec2 E = ComputeNewPoint(A, B, C, D);

    //Stability guard: if E is very far from C, the geometry is
    //degenerate (nearly-parallel edges with a tiny shared area).
    //Reject such collapses to prevent flying-away vertices.
    double moveDist = std::hypot(E.x - C->v.pos.x,E.y - C->v.pos.y);
    if (moveDist > 500.0) 
        return false;   //threshold is tunable

    //----Apply the collapse----

    //Move C to E (C's node stays in the list; only its position changes).
    C->v.pos = E;

    //If B was the ring's entry point, advance head to C (= E).
    if (B == ring.head)
        ring.head = C;

    //Unlink B from the doubly-linked list:
    //  A <-> B <-> C   becomes   A <-> C
    A->next = C;
    C->prev = A;

    //Mark B as removed so the heap skips any stale entries for it.
    B->valid = false;

    ring.size--;

    return true;
}
