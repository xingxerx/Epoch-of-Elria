#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>
#include <iostream>

// --- Vector2D Class ---
// Represents a 2D point or vector in space.
// Essential for positions, velocities, and sizes in games.
class Vector2D {
public:
    double x; // X-coordinate
    double y; // Y-coordinate

    // Default constructor
    Vector2D() : x(0.0), y(0.0) {}

    // Parameterized constructor
    Vector2D(double in_x, double in_y) : x(in_x), y(in_y) {}

    // Overload the + operator for vector addition
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    // Overload the - operator for vector subtraction
    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    // Overload the * operator for scalar multiplication
    Vector2D operator*(double scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    // Overload the * operator for component-wise multiplication
    Vector2D operator*(const Vector2D& other) const {
        return Vector2D(x * other.x, y * other.y);
    }

    // Unary minus operator
    Vector2D operator-() const {
        return Vector2D(-x, -y);
    }

    // Overload the / operator for scalar division
    Vector2D operator/(double scalar) const {
        if (scalar != 0) {
            return Vector2D(x / scalar, y / scalar);
        }
        return Vector2D(0.0, 0.0); // Avoid division by zero
    }

    // Overload the += operator
    Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Overload the -= operator
    Vector2D& operator-=(const Vector2D& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Overload the *= operator for scalar multiplication
    Vector2D& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // Overload the /= operator for scalar division
    Vector2D& operator/=(double scalar) {
        if (scalar != 0) {
            x /= scalar;
            y /= scalar;
        }
        return *this;
    }

    // Equality operator
    bool operator==(const Vector2D& other) const {
        return (std::abs(x - other.x) < 1e-9) && (std::abs(y - other.y) < 1e-9);
    }

    // Inequality operator
    bool operator!=(const Vector2D& other) const {
        return !(*this == other);
    }

    // Calculate the magnitude (length) of the vector
    double magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    // Calculate the squared magnitude (more efficient when you don't need the actual magnitude)
    double magnitudeSquared() const {
        return x * x + y * y;
    }

    // Normalize the vector (make its magnitude 1)
    Vector2D normalize() const {
        double mag = magnitude();
        if (mag != 0) {
            return *this / mag;
        }
        return Vector2D(0.0, 0.0); // Return zero vector if magnitude is zero
    }

    // Calculate dot product with another vector
    double dot(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }

    // Calculate cross product (returns scalar for 2D)
    double cross(const Vector2D& other) const {
        return x * other.y - y * other.x;
    }

    // Calculate distance to another vector
    double distanceTo(const Vector2D& other) const {
        return (*this - other).magnitude();
    }

    // Calculate squared distance to another vector (more efficient)
    double distanceSquaredTo(const Vector2D& other) const {
        return (*this - other).magnitudeSquared();
    }

    // Rotate the vector by an angle (in radians)
    Vector2D rotate(double angleRadians) const {
        double cos_a = std::cos(angleRadians);
        double sin_a = std::sin(angleRadians);
        return Vector2D(x * cos_a - y * sin_a, x * sin_a + y * cos_a);
    }

    // Get the angle of this vector (in radians)
    double angle() const {
        return std::atan2(y, x);
    }

    // Linear interpolation between this vector and another
    Vector2D lerp(const Vector2D& other, double t) const {
        return *this + (other - *this) * t;
    }

    // Clamp the vector's magnitude to a maximum value
    Vector2D clampMagnitude(double maxMagnitude) const {
        double mag = magnitude();
        if (mag > maxMagnitude) {
            return normalize() * maxMagnitude;
        }
        return *this;
    }

    // Print the vector (for debugging)
    void print() const {
        std::cout << "(" << x << ", " << y << ")";
    }

    // Static utility functions
    static Vector2D Zero() { return Vector2D(0.0, 0.0); }
    static Vector2D One() { return Vector2D(1.0, 1.0); }
    static Vector2D Up() { return Vector2D(0.0, -1.0); }    // Negative Y is up in screen coordinates
    static Vector2D Down() { return Vector2D(0.0, 1.0); }
    static Vector2D Left() { return Vector2D(-1.0, 0.0); }
    static Vector2D Right() { return Vector2D(1.0, 0.0); }
};

#endif // VECTOR2D_H
