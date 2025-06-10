#pragma once
#include <cmath>
#include <iostream>

class Vector2D {
public:
    double x, y;

    Vector2D() : x(0.0), y(0.0) {}
    Vector2D(double x, double y) : x(x), y(y) {}

    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator*(double scalar) const;
    Vector2D operator/(double scalar) const;
    
    double magnitude() const;
    Vector2D normalize() const;
    void print() const;
    
    // Dot product
    double dot(const Vector2D& other) const;
    
    // Distance between two points
    double distance(const Vector2D& other) const;
};