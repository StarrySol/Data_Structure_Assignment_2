
/*
Contains pure math/Geometry helpers 
    Vec2
    cross product
    orientation
    signed triangle area
    signed ring area
    segment intersection
    point-line side test
    distance from point to line
    line-line intersection
    shoelace area
*/
#pragma once
#include <cmath>
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

std::ostream &operator<<(std::ostream &os, Vec2 const &vec);

