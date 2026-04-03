/******************************************************************************
 * File:        Ring.h
 * Project:     Data Structures Assigment 2
 *
 * Description:
 * This file contains the declaration of classes involving a ring and required structs
 * A ring is a closed polyline of a polygon (or one of its holes)
 * It also can helper functions to check area, segement, .etc.
 ******************************************************************************/

#pragma once

#include "Math.h"
#include <vector>
#include <queue>

//One vertex as read from the CSV input.  
struct Vertice
{
    int ring_id = 0;
    int vertex_id = 0;
    Vec2 pos{};
};

//One node in the circular doubly-linked list that represents
//a ring.  Think of it as a "live" vertex that the algorithm
//can remove or reposition.
struct Node
{
    Vertice v; //the vertex data

    Node* prev = nullptr;    //previous node in the ring (circular)
    Node* next = nullptr;    //next     node in the ring (circular)

    double cost = 0.0;    //cached APSC collapse cost for this node as "B" in A->B->C->D
    bool valid = true;    //false once this node has been collapsed and removed
    int version = 0;      //incremented every time cost changes; used to skip stale PQ entries
};

//One closed boundary (exterior or interior) of the input polygon.  
//After parsing, the vertex array is converted into a circular doubly linked list and then cleared.
//Doubley linked list, head -> N0 <-> N1 <-> N2 <-> ... <-> Nk <-> (back to N0)
struct Ring
{
    int ringID = 0;         //Ring it belongs to
    Node* head = nullptr;   //First node in circular linked list
    int size = 0;           //Number of nodes in list

    double originalArea = 0.0;       //Signed area before any simplification

    //Temporary storage used only during CSV parsing
    //Cleared once ConvertToLinkedList() runs.
    std::vector<Vertice> vertices;

    Ring();

    //Deletes all nodes in linked list
    void FreeRing();
};

//Geometry helpers functions
// SignedArea        -- signed area of a triangle
// ComputeRingArea   -- signed area of a full ring (shoelace)
// ComputeDisplacement / ComputeTotalArea -- output metrics
// Orientation / OnSegment / SegmentsIntersect -- line-segment tests

/************************************************************************/
/*!
\brief
Computes the SIGNED area of the triangle formed by three points.

The sign is orientation:
+  counterclockwise (CCW)
-  clockwise (CW)
0  collinear (degenerate)

Formula used:
SignedArea = 0.5 * ((B-A) x (C-A))
where "x" is the 2-D cross product  (B-A).x*(C-A).y - (B-A).y*(C-A).x

\param a
First  vertex of the triangle

\param b
Second vertex of the triangle

\param c
Third  vertex of the triangle

\return
Double variable of signed area
(positive = CCW, negative = CW)
*/
/************************************************************************/
double SignedArea(const Vec2& a, const Vec2& b, const Vec2& c);

/************************************************************************/
/*!
\brief
Computes the signed area of a full ring using the shoelace formula.

The shoelace formula sums cross products of consecutive vertex pairs:
2 * Area = sum_i (x_i * y_{i+1} - x_{i+1} * y_i)

For counterclockwise ring the result is positive
For a clockwise ring (hole) it is negative.
Sign convention is what makes the total signed area meaningful when summed across exterior + holes.

\param ring  The ring to measure.
\return Signed polygon area.
*/
/************************************************************************/
double ComputeRingArea(const Ring& ring);

/************************************************************************/
/*!
\brief
Computes the total areal displacement across all rings
Displacement, sum over rings of current_area - original_area

Because the APSC formula preserves area exactly per collapse, this should remain at ~1e-10

\param rings  All rings after simplification.

\return Total unsigned areal displacement.
*/
/************************************************************************/
double ComputeDisplacement(const std::vector<Ring>& rings);

/************************************************************************/
/*!
\brief
Computes the algebraic sum of signed areas across all rings.

For a polygon with holes the exterior ring contributes positive area
and each hole contributes negative area, so the total equals the
"filled area" of the polygon.

\param rings
All rings in vector to check

\return Total signed area.
*/
/************************************************************************/
double ComputeTotalArea(const std::vector<Ring>& rings);

/************************************************************************/
/*!
\brief
Orientation test for an ordered triplet of points (a, b, c).

Uses the sign of the cross product (b-a) x (c-b).

\param a
First  point

\param b
Second point

\param c
Third  point

\return  
0, collinear
1, clockwise
2, counter-clockwise
*/
/************************************************************************/
int Orientation(const Vec2& a, const Vec2& b, const Vec2& c);

/************************************************************************/
/*!
\brief
Checks whether point c lies on the closed line segment [a, b].

Used as a special case inside SegmentsIntersect when three points
are collinear.

\param a  
Segment start

\param b  
Segment end

\param c  
Point to test

\return 
true if c is on [a, b]
*/
/************************************************************************/
bool OnSegment(const Vec2& a, const Vec2& b, const Vec2& c);

/************************************************************************/
/*!
\brief
Tests whether two line segments (p1,q1) and (p2,q2) intersect.

Uses the standard orientation-based algorithm:
1. General case: orientations of the four triplets differ.
2. Special cases: collinear overlap (one endpoint lies on the other segment).

This test is the core of the topology-validity check: after a
proposed collapse we check whether any new edge A->E or E->D
intersects any existing edge of any ring.

\param p1, q1  
First  segment (endpoints)

\param p2, q2  
Second segment (endpoints)

\return 
true if the segments intersect
*/
/************************************************************************/
bool SegmentsIntersect(const Vec2& p1, const Vec2& q1, const Vec2& p2, const Vec2& q2);
