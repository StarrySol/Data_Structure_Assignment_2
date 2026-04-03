/******************************************************************************
 * File:        Collaspe.h
 * Project:     Data Structures Assigment 2
 *
 * Description:
 * This file contains the deceration of functions to apply Area-Preserving Segment Collapse (APSC)
 * to a polygon ring to try to reduce the number of polygons while preserving the area
 * 
 * For example, it tries to replace the path A->B->C->D with the shorter path
 * A->E->D, with 1 point (E) instead of the orginal 2 (B & C) in a way
 * that preserve the orignal area. Point E can be anywhere that matches the are
 * but it tries to limnit the area swept between the old and new path (areal displacement)
 * 

 ******************************************************************************/

#pragma once

 /* -------------------------------------------------------
 * The shoelace area formula tells us that the ring's area changes
 * only through the edges that change.  When we replace
 * A->B->C->D with A->E->D, the edges that change are:
 *
 *   REMOVED edges:  A->B,  B->C,  C->D
 *   ADDED   edges:  A->E,  E->D
 *
 * Area is preserved iff:
 *   cross(A,E) + cross(E,D) = cross(A,B) + cross(B,C) + cross(C,D)
 * where cross(p,q) = p.x*q.y - q.x*p.y  (shoelace term).
 *
 * This is a linear equation in E.  The solution set is a LINE.
 * We choose the point E on that line that lies on the
 * perpendicular bisector of segment A->D, which minimises the
 * maximum displacement of the new edge from the old path.
 *
 * ---PRIORITY QUEUE STRATEGY---
 * We maintain a min-heap of CollapseCandidate entries, one per
 * node B, sorted by collapse cost (= total area of the two
 * triangles A-B-C and A-C-D being replaced).
 *
 * After each collapse, the costs of the neighbouring nodes A
 * and D (and the repositioned C=E) change, so we push fresh
 * candidates for them.  Stale (outdated) candidates are
 * detected via the "version" counter on each Node and are
 * skipped when popped.
 ------------------------------------------------------- */

#include "Ring.h"

#include <queue>
#include <vector>
#include <limits>

/* ----------------------------------------------------------
 * CollapseCandidate
 *
 * One entry in the priority queue.  It records:
 *   node    -- The node B that would be collapsed
 *   cost    -- The areal cost at the time this entry was created
 *   id      -- A unique, ever-increasing insertion counter used for
 *              deterministic tie-breaking (lower id = inserted
 *              earlier = tried first when costs are equal)
 *   version -- Snapshot of node->version at insertion time
 *              if node->version has changed by the time we pop
 *              this entry, the entry is stale and is discarded.
 * ---------------------------------------------------------- */
struct CollapseCandidate
{
    Node* node;
    double cost;
    int id;
    int version;

    CollapseCandidate(Node* n, double c, int i, int v): node(n), cost(c), id(i), version(v) {//Empty by Design
      }

    //Min-heap comparison: lowest cost first
    //Ties broken by vertex_id (smaller id wins) then by insertion order
    bool operator>(const CollapseCandidate& other) const
    {
        if (std::abs(cost - other.cost) > 1e-9)
            return cost > other.cost;

        if (node->v.vertex_id != other.node->v.vertex_id)
            return node->v.vertex_id > other.node->v.vertex_id;

        return id > other.id;
    }
};

//Convenience alias
//a min-heap of CollapseCandidate entries
using MinHeap = std::priority_queue<
    CollapseCandidate,
    std::vector<CollapseCandidate>,
    std::greater<CollapseCandidate>
>;

/************************************************************************/
/*!
\brief
Computes the APSC collapse cost for the candidate rooted at node B.

The cost is the total area of the two triangles that would be
replaced by the collapse A->B->C->D => A->E->D

cost = |SignedArea(A,B,C)| + |SignedArea(A,C,D)|

This equals the areal displacement of the collapse when E is placed
optimally (on the perpendicular bisector of AD).  Collapses with
lower cost disturb the polygon shape less.

\param node
The candidate node B (B's prev is A; B->next is C; C->next is D).

\return
Collapse cost (non-negative real).
*/
/************************************************************************/
double ComputeCost(Node* node);

/************************************************************************/
/*!
\brief
Computes the new vertex position E for the collapse A->B->C->D => A->E->D.

AREA PRESERVATION DERIVATION
Replacing edges A->B, B->C, C->D with A->E, E->D changes the ring's
shoelace area by:

delta = [cross(A,E) + cross(E,D)] - [cross(A,B) + cross(B,C) + cross(C,D)]

where cross(p,q) = p.x*q.y - q.x*p.y.

Setting delta = 0 gives the area-preservation constraint:

cross(A,E) + cross(E,D) = cross(A,B) + cross(B,C) + cross(C,D) + ...

Expanding with E = (Ex, Ey):

(A.x - D.x)*Ey + (D.y - A.y)*Ex = RHS

This is a LINE in the (Ex,Ey) plane.  Any point on this line preserves
the ring's area exactly.

OPTIMAL PLACEMENT has minimum displacement
We minimise the areal displacement by choosing E on the perpendicular
bisector of segment A->D.  This keeps E "centred" between A and D,
which intuitively minimises the area swept between the old path ABCD
and the new path AED.

On the perpendicular bisector, E = mid(AD) + t * n_hat, where
mid(AD) = (A + D) / 2
n_hat   = unit normal to AD = (-AD.y, AD.x) / |AD|

Substituting into (*) and solving for t gives:

t * (-|AD|) = RHS - [mid.y*(A.x-D.x) + mid.x*(D.y-A.y)]

\param A 
Node before B in the ring.

\param B 
Node being collapsed (will be unlinked).

\param C 
Node after  B (will be repositioned to E).

\param D 
Node after  C (remains unchanged).

\return
The area-preserving, displacement-minimising point E.
*/
/************************************************************************/
Vec2 ComputeNewPoint(Node* A, Node* B, Node* C, Node* D);

/************************************************************************/
/*!
\brief
Checks whether the collapse A->B->C->D => A->E->D is topologically safe.

A collapse is SAFE if the two new edges A->E and E->D do not
intersect any existing edge of any ring (other than the edges
already adjacent to A, B, C, or D, which are naturally affected
by the collapse).

This check prevents the simplification from:
  - creating self-intersections inside a single ring, and
  - creating intersections between the exterior ring and a hole.

\param rings
All rings (read-only; used for intersection testing).

\param A, B, C, D
The four nodes of the candidate collapse.

\return
true  if the collapse is topologically safe.
false if the collapse would create an intersection.
*/
/************************************************************************/
bool IsCollapseValid(const std::vector<Ring>& rings,
                     Node* A, Node* B, Node* C, Node* D);

/************************************************************************/
/*!
\brief
Main simplification driver: collapses vertices globally until the
total vertex count across all rings reaches the target.

ALGORITHM STEPS
---------------
1. Build a global min-heap of CollapseCandidate entries, one per
   node B, sorted by cost (cheapest collapse first).

2. Pop the cheapest candidate.
   - Skip if the node has already been removed (node->valid == false).
   - Skip if the stored version differs from node->version (stale entry).

3. Find which ring owns the node.  If the ring has only 3 vertices,
   skip (we never reduce a ring below a triangle).

4. Call TryCollapse.  If it succeeds:
   - Decrement total vertex count.
   - Recompute and push fresh candidates for the two neighbours
     of E (which are now A and D, since B was removed and C moved).

5. Repeat until total vertex count <= target or the heap is empty.

\param rings   All rings (modified in place).
\param target  Desired maximum total vertex count across all rings.
*/
/************************************************************************/
void SimplifyAll(std::vector<Ring>& rings, int target);

/************************************************************************/
/*!
\brief
Attempts to perform one APSC collapse at node B.

Checks:
  1. The node is still valid (not already removed).
  2. The ring has more than 3 vertices (cannot shrink below a triangle).
  3. The collapse is topologically valid (IsCollapseValid).
  4. The displacement of E from C is below a stability threshold
     (prevents numerically unstable collapses in degenerate geometry).

If all checks pass, the collapse is applied:
  - C's position is updated to E.
  - B is unlinked from the list and marked invalid.
  - ring.size is decremented by 1.

\param rings
All rings (needed for topology check).

\param ring 
The ring that owns node B.

\param node 
The node B to collapse.

\return  
true  if the collapse was performed.
false if any check failed.
*/
/************************************************************************/
bool TryCollapse(std::vector<Ring>& rings, Ring& ring, Node* node);
