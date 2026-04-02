


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
#include <iostream>
#include <cstddef>   // for size_t
#include <cmath>     // for sqrt

struct Vector2
{
public:
	// ctr and dtr
	Vector2();
	Vector2(double x_compo, double y_compo);
	Vector2(Vector2 const &v);
	~Vector2();

	// copy assignment operator= - assign vector to another
	Vector2 &operator=(Vector2 vec);

	// subscript operator[] - access element of vector
	double &operator[](size_t i);
	double operator[](size_t i) const;

	// arithmetic operators: vector operations +-
	Vector2 &operator+=(Vector2 const&vec);
	Vector2 &operator-=(Vector2 const&vec);

	// arithmetic operators: scalar operations */
	Vector2 &operator*=(double scalar);
	Vector2 &operator/=(double scalar);

	// arithmetic operators: negation
	Vector2 operator-(void) const;

	// length of vector
	double Len() const;

	// squared length of vector
	double LenSq() const;

	// normalization
	Vector2 Norm() const; // non-mutating, returns a normalized copy
	void Normalize();	  // mutating, normalizes in place
	//

	//comparison operator
	bool operator==(Vector2 const& other) const;
	bool operator!=(Vector2 const& other) const;

	//getter/setter
	// double& setX();
	// double& setY();
	// double  getX() const;
	// double  getY() const;
	// Vector2& Set(double x, double y);

private:
	double m[2];

public:
	double  x{};  //Access to X component.
	double  y{}; // Access to Y component.
}; // end of vector class

double distance(Vector2 const&v0, Vector2 const&v1);
double distanceSq(Vector2 const&v0, Vector2 const&v1);
double dot(Vector2 const&v0, Vector2 const&v1);
double cross(Vector2 const&v0, Vector2 const&v1);

Vector2 operator+(Vector2 const&v0, Vector2 const&v1);
Vector2 operator-(Vector2 const&v0, Vector2 const&v1);
Vector2 operator*(Vector2 const&v, double s); // vector * scalar
Vector2 operator*(double s, Vector2 const&v); // scalar * vector
Vector2 operator/(Vector2 const&v, double s); // vector / scalar
std::ostream &operator<<(std::ostream &os, Vector2 const &vec);