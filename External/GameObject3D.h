#pragma once
#include "Vector2D.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// 3D Vector class for 3D game objects
class Vector3D {
public:
    double x, y, z;

    Vector3D() : x(0.0), y(0.0), z(0.0) {}
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

    Vector3D operator+(const Vector3D& other) const;
    Vector3D operator-(const Vector3D& other) const;
    Vector3D operator*(double scalar) const;
    Vector3D operator/(double scalar) const;
    
    double magnitude() const;
    Vector3D normalize() const;
    double dot(const Vector3D& other) const;
    Vector3D cross(const Vector3D& other) const;
    
    // Convert to 2D for rendering (simple orthographic projection)
    Vector2D toVector2D() const { return Vector2D(x, y); }
};

// 3D GameObject base class
class GameObject3D {
protected:
    Vector3D position;
    Vector3D velocity;
    Vector3D rotation; // Euler angles (pitch, yaw, roll)
    Vector3D scale;
    std::string name;
    sf::Color color;
    bool active;

public:
    GameObject3D(const std::string& name, const Vector3D& pos, sf::Color color = sf::Color::White);
    virtual ~GameObject3D() = default;

    // Getters
    const Vector3D& getPosition() const { return position; }
    const Vector3D& getVelocity() const { return velocity; }
    const Vector3D& getRotation() const { return rotation; }
    const Vector3D& getScale() const { return scale; }
    const std::string& getName() const { return name; }
    sf::Color getColor() const { return color; }
    bool isActive() const { return active; }

    // Setters
    void setPosition(const Vector3D& pos) { position = pos; }
    void setVelocity(const Vector3D& vel) { velocity = vel; }
    void setRotation(const Vector3D& rot) { rotation = rot; }
    void setScale(const Vector3D& s) { scale = s; }
    void setColor(sf::Color c) { color = c; }
    void setActive(bool a) { active = a; }

    // Core methods
    virtual void Update(double deltaTime);
    virtual void Draw(sf::RenderWindow& window) const;
    
    // 3D specific methods
    virtual void Rotate(const Vector3D& angles);
    virtual void Scale(const Vector3D& factors);
    virtual void Translate(const Vector3D& translation);
    
    // Distance and collision (simplified for 3D)
    double DistanceTo(const GameObject3D& other) const;
    bool IsNear(const GameObject3D& other, double threshold) const;
};

// Specific 3D game objects
class Cube3D : public GameObject3D {
private:
    double size;

public:
    Cube3D(const std::string& name, const Vector3D& pos, double size, sf::Color color = sf::Color::White);
    void Draw(sf::RenderWindow& window) const override;
    double getSize() const { return size; }
};

class Sphere3D : public GameObject3D {
private:
    double radius;

public:
    Sphere3D(const std::string& name, const Vector3D& pos, double radius, sf::Color color = sf::Color::White);
    void Draw(sf::RenderWindow& window) const override;
    double getRadius() const { return radius; }
};