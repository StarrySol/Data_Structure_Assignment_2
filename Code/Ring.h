#pragma once


#include "Math.h"
#include <vector>
#include <queue>

struct Vertice
{
    int ring_id = 0;
    int vertex_id = 0;
    Vec2 pos{};
};

struct Node
{
    Vertice v;

    Node* prev = nullptr;
    Node* next = nullptr;

    double cost = 0.0;//APSC cost

    bool valid = true;//deletion for PQ

    int version = 0; //increment whenever node changes
};

struct Ring
{
    int ringID = 0;
    Node* head = nullptr;//circular linked list
    int size = 0;

    double originalArea = 0.0; //Orginal area of ring

    //Only used when parsing input data
    std::vector<Vertice> vertices;

    Ring();

    //Frees all pointers
    void FreeRing();
};

/************************************************************************/
/*!
\brief
Computes signed area of triangle formed by 3 points

\param a
First point

\param b
Second point

\param c
Third point

\return
Signed area of triangle
*/
/************************************************************************/
double SignedArea(const Vec2& a, const Vec2& b, const Vec2& c);


/************************************************************************/
/*!
\brief
Computes full polygon area of a ring

\param ring
ring to compute

\return
signed polygon area
*/
/************************************************************************/
double ComputeRingArea(const Ring& ring);

/************************************************************************/
/*!
\brief
Computes approximate areal displacement

\param rings
All rings

\return
Displacement 
*/
/************************************************************************/
double ComputeDisplacement(const std::vector<Ring>& rings);

/************************************************************************/
/*!
\brief
Computes total signed area

\param rings
All rings

\return
Total area
*/
/************************************************************************/
double ComputeTotalArea(const std::vector<Ring>& rings);


//Helper funcs

/************************************************************************/
/*!
\brief
Computes orientation of ordered triplet (a,b,c)

\param a
First point

\param b
Second point

\param c
Third point

\return
0=collinear
1=clockwise
2=counterclockwise
*/
/************************************************************************/
int Orientation(const Vec2& a, const Vec2& b, const Vec2& c);

/************************************************************************/
/*!
\brief
Checks if point c lies on segment ab

\param a
Segment start

\param b
Segment end

\param c
Point

\return
True if on segment
*/
/************************************************************************/
bool OnSegment(const Vec2& a, const Vec2& b, const Vec2& c);

/************************************************************************/
/*!
\brief
Checks if two line segments intersect

\param p1,q1
First segment

\param p2,q2
Second segment

\return
True if intersect
*/
/************************************************************************/
bool SegmentsIntersect(const Vec2& p1, const Vec2& q1, const Vec2& p2, const Vec2& q2);