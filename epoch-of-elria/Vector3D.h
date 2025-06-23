#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>
#include <iostream>

// --- Vector3D Class for 3D Open World ---
class Vector3D {
public:
    double x, y, z; // 3D coordinates

    // Constructors
    Vector3D() : x(0.0), y(0.0), z(0.0) {}
    Vector3D(double in_x, double in_y, double in_z) : x(in_x), y(in_y), z(in_z) {}

    // Vector operations
    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    Vector3D operator-(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    Vector3D operator*(double scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    Vector3D operator/(double scalar) const {
        if (scalar != 0) {
            return Vector3D(x / scalar, y / scalar, z / scalar);
        }
        return Vector3D(0.0, 0.0, 0.0);
    }

    Vector3D& operator+=(const Vector3D& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    Vector3D& operator-=(const Vector3D& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }

    Vector3D& operator*=(double scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }

    // Unary minus
    Vector3D operator-() const {
        return Vector3D(-x, -y, -z);
    }

    // Magnitude and normalization
    double magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    double magnitudeSquared() const {
        return x * x + y * y + z * z;
    }

    Vector3D normalize() const {
        double mag = magnitude();
        if (mag != 0) {
            return *this / mag;
        }
        return Vector3D(0.0, 0.0, 0.0);
    }

    // Dot and cross products
    double dot(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3D cross(const Vector3D& other) const {
        return Vector3D(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // Distance calculations
    double distanceTo(const Vector3D& other) const {
        return (*this - other).magnitude();
    }

    double distanceSquaredTo(const Vector3D& other) const {
        return (*this - other).magnitudeSquared();
    }

    // Interpolation
    Vector3D lerp(const Vector3D& other, double t) const {
        return *this + (other - *this) * t;
    }

    // Utility functions
    void print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }

    // Static utility vectors
    static Vector3D Zero() { return Vector3D(0.0, 0.0, 0.0); }
    static Vector3D One() { return Vector3D(1.0, 1.0, 1.0); }
    static Vector3D Up() { return Vector3D(0.0, 1.0, 0.0); }
    static Vector3D Down() { return Vector3D(0.0, -1.0, 0.0); }
    static Vector3D Forward() { return Vector3D(0.0, 0.0, 1.0); }
    static Vector3D Back() { return Vector3D(0.0, 0.0, -1.0); }
    static Vector3D Left() { return Vector3D(-1.0, 0.0, 0.0); }
    static Vector3D Right() { return Vector3D(1.0, 0.0, 0.0); }
};

// --- 3D Camera Class ---
class Camera3D {
private:
    Vector3D position;
    Vector3D forward;
    Vector3D up;
    Vector3D right;
    
    double yaw;   // Rotation around Y-axis (left/right)
    double pitch; // Rotation around X-axis (up/down)
    
    double fov;        // Field of view
    double nearPlane;  // Near clipping plane
    double farPlane;   // Far clipping plane

public:
    Camera3D(const Vector3D& pos = Vector3D(0, 0, 0), double fieldOfView = 75.0)
        : position(pos), yaw(0.0), pitch(0.0), fov(fieldOfView), nearPlane(0.1), farPlane(1000.0) {
        updateVectors();
    }

    // Getters
    const Vector3D& getPosition() const { return position; }
    const Vector3D& getForward() const { return forward; }
    const Vector3D& getUp() const { return up; }
    const Vector3D& getRight() const { return right; }
    double getYaw() const { return yaw; }
    double getPitch() const { return pitch; }
    double getFOV() const { return fov; }

    // Setters
    void setPosition(const Vector3D& pos) { position = pos; }
    void setYaw(double y) { yaw = y; updateVectors(); }
    void setPitch(double p) { 
        // Constrain pitch to prevent camera flipping
        pitch = std::max(-89.0, std::min(89.0, p)); 
        updateVectors(); 
    }
    void setFOV(double f) { fov = std::max(1.0, std::min(179.0, f)); }

    // Movement
    void move(const Vector3D& offset) { position += offset; }
    void moveForward(double distance) { position += forward * distance; }
    void moveBackward(double distance) { position -= forward * distance; }
    void moveLeft(double distance) { position -= right * distance; }
    void moveRight(double distance) { position += right * distance; }
    void moveUp(double distance) { position += up * distance; }
    void moveDown(double distance) { position -= up * distance; }

    // Rotation
    void rotate(double deltaYaw, double deltaPitch) {
        yaw += deltaYaw;
        pitch += deltaPitch;
        
        // Wrap yaw around 360 degrees
        while (yaw > 360.0) yaw -= 360.0;
        while (yaw < 0.0) yaw += 360.0;
        
        // Constrain pitch
        pitch = std::max(-89.0, std::min(89.0, pitch));
        
        updateVectors();
    }

    // Look at a target
    void lookAt(const Vector3D& target) {
        Vector3D direction = (target - position).normalize();
        
        // Calculate yaw and pitch from direction vector
        yaw = std::atan2(direction.x, direction.z) * 180.0 / M_PI;
        pitch = std::asin(-direction.y) * 180.0 / M_PI;
        
        updateVectors();
    }

private:
    void updateVectors() {
        // Convert angles to radians
        double yawRad = yaw * M_PI / 180.0;
        double pitchRad = pitch * M_PI / 180.0;
        
        // Calculate forward vector
        forward.x = std::sin(yawRad) * std::cos(pitchRad);
        forward.y = -std::sin(pitchRad);
        forward.z = std::cos(yawRad) * std::cos(pitchRad);
        forward = forward.normalize();
        
        // Calculate right vector (cross product of world up and forward)
        Vector3D worldUp(0.0, 1.0, 0.0);
        right = forward.cross(worldUp).normalize();
        
        // Calculate up vector (cross product of forward and right)
        up = right.cross(forward).normalize();
    }
};

// --- 3D Transform Class ---
class Transform3D {
public:
    Vector3D position;
    Vector3D rotation; // Euler angles (pitch, yaw, roll)
    Vector3D scale;

    Transform3D(const Vector3D& pos = Vector3D::Zero(), 
                const Vector3D& rot = Vector3D::Zero(), 
                const Vector3D& scl = Vector3D::One())
        : position(pos), rotation(rot), scale(scl) {}

    // Get forward vector based on rotation
    Vector3D getForward() const {
        double yawRad = rotation.y * M_PI / 180.0;
        double pitchRad = rotation.x * M_PI / 180.0;
        
        return Vector3D(
            std::sin(yawRad) * std::cos(pitchRad),
            -std::sin(pitchRad),
            std::cos(yawRad) * std::cos(pitchRad)
        ).normalize();
    }

    Vector3D getRight() const {
        Vector3D forward = getForward();
        Vector3D worldUp(0.0, 1.0, 0.0);
        return forward.cross(worldUp).normalize();
    }

    Vector3D getUp() const {
        Vector3D forward = getForward();
        Vector3D right = getRight();
        return right.cross(forward).normalize();
    }
};

#endif // VECTOR3D_H
