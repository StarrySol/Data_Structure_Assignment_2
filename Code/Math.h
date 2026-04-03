
/*================================================ */
/********************Math.h******************** */
/*================================================*/
/*
Contains pure math/Geometry helpers 
*/
#pragma once
#include <iosfwd>


struct Vec2
{
    //data member
    double x{};
    double y{};
    //functions
    //Constructor that takes in 2 arguments X and Y
    Vec2 (double inX = 0.0, double inY = 0.0);

    // arithmetic operators: vector operations +-
	Vec2 &operator+=(Vec2 const&vec);
	Vec2 &operator-=(Vec2 const&vec);

    // arithmetic operators: scalar operations */
	Vec2 &operator*=(double scalar);
	Vec2 &operator/=(double scalar);

    // arithmetic operators: negation
	Vec2 operator-(void) const;

    // length of vector
	double Len() const;

	// squared length of vector
	double LenSq() const;

	// normalization
	Vec2 Norm() const; // non-mutating, returns a normalized copy
	void Normalize();	  // mutating, normalizes in place

	//comparison operator
	bool operator==(Vec2 const& other) const;
	bool operator!=(Vec2 const& other) const;
};

double VecLength(const Vec2& lhs, const Vec2& rhs);

double distance(Vec2 const&v0, Vec2 const&v1);
double distanceSq(Vec2 const&v0, Vec2 const&v1);
double dot(Vec2 const&v0, Vec2 const&v1);
double cross(Vec2 const&v0, Vec2 const&v1);

Vec2 operator+(Vec2 const&v0, Vec2 const&v1);
Vec2 operator-(Vec2 const&v0, Vec2 const&v1);
Vec2 operator*(Vec2 const&v, double s); // vector * scalar
Vec2 operator*(double s, Vec2 const&v); // scalar * vector
Vec2 operator/(Vec2 const&v, double s); // vector / scalar

/*
extra math for polygon
*/
//cross overloaded 
double cross(Vec2 const& a, Vec2 const& b, Vec2 const& c);
double SignedTriangleArea(Vec2 const& a, Vec2 const& b, Vec2 const& c);
int Orientation(Vec2 const& a, Vec2 const& b, Vec2 const& c);
bool OnSegment(Vec2 const& a, Vec2 const& b, Vec2 const& p);
bool SegmentsIntersect(Vec2 const& p1, Vec2 const& q1,
                       Vec2 const& p2, Vec2 const& q2);

bool NearlyEqual(double a, double b, double eps = 1e-9);
bool NearlyEqual(Vec2 const& a, Vec2 const& b, double eps = 1e-9);
double PointLineSide(Vec2 const& a, Vec2 const& b, Vec2 const& p);
double DistancePointToLine(Vec2 const& a, Vec2 const& b, Vec2 const& p);                       

//
Vec2 LineIntersection(Vec2 const& p1, Vec2 const& p2,
                      Vec2 const& q1, Vec2 const& q2);

double TriangleArea(Vec2 const& a, Vec2 const& b, Vec2 const& c);

int SideOfDirectedLine(Vec2 const& a, Vec2 const& b, Vec2 const& p);

double SignedDistancePointToLine(Vec2 const& a, Vec2 const& b, Vec2 const& p);

std::ostream &operator<<(std::ostream &os, Vec2 const &vec);

