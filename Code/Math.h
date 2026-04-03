/******************************************************************************
 * File:        Math.h
 * Project:     Data Structures Assigment 2
 *
 * Description:
 * This file contains the declaration/definition of struct vec2 to use for points x,y 
 * It also has operatorer needed to modify them
 ******************************************************************************/
#pragma once

#include <cmath>

struct Vec2
{
    double x = 0.0;
    double y = 0.0;

    Vec2(double inX = 0.0, double inY = 0.0) : x(inX), y(inY) {}

    Vec2  operator+ (const Vec2& rhs) const { return Vec2(x + rhs.x, y + rhs.y); }
    Vec2  operator- (const Vec2& rhs) const { return Vec2(x - rhs.x, y - rhs.y); }
    Vec2& operator+=(const Vec2& rhs)       { x += rhs.x; y += rhs.y; return *this; }
    Vec2& operator-=(const Vec2& rhs)       { x -= rhs.x; y -= rhs.y; return *this; }
};
