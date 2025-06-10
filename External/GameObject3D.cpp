#include "GameObject3D.h"
#include <cmath>
#include <algorithm>

// Vector3D implementations
Vector3D Vector3D::operator+(const Vector3D& other) const {
    return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D Vector3D::operator-(const Vector3D& other) const {
    return Vector3D(x - other.x, y - other.y, z - other.z);
}

Vector3D Vector3D::operator*(double scalar) const {
    return Vector3D(x * scalar, y * scalar, z * scalar);
}

Vector3D Vector3D::operator/(double scalar) const {
    if (scalar != 0) {
        return Vector3D(x / scalar, y / scalar, z / scalar);
    }
    return Vector3D(0, 0, 0);
}

double Vector3D::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::normalize() const {
    double mag = magnitude();
    if (mag != 0) {
        return *this / mag;
    }
    return Vector3D(0, 0, 0);
}

double Vector3D::dot(const Vector3D& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector3D Vector3D::cross(const Vector3D& other) const {
    return Vector3D(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

// GameObject3D implementations
GameObject3D::GameObject3D(const std::string& name, const Vector3D& pos, sf::Color color)
    : name(name), position(pos), color(color), velocity(0, 0, 0), 
      rotation(0, 0, 0), scale(1, 1, 1), active(true) {}

void GameObject3D::Update(double deltaTime) {
    if (active) {
        position = position + (velocity * deltaTime);
    }
}

void GameObject3D::Draw(sf::RenderWindow& window) const {
    if (!active) return;
    
    // Simple 2D projection for base class
    Vector2D projected = position.toVector2D();
    sf::CircleShape shape(10.0f);
    shape.setPosition(static_cast<float>(projected.x), static_cast<float>(projected.y));
    shape.setFillColor(color);
    window.draw(shape);
}

void GameObject3D::Rotate(const Vector3D& angles) {
    rotation = rotation + angles;
}

void GameObject3D::Scale(const Vector3D& factors) {
    scale = Vector3D(scale.x * factors.x, scale.y * factors.y, scale.z * factors.z);
}

void GameObject3D::Translate(const Vector3D& translation) {
    position = position + translation;
}

double GameObject3D::DistanceTo(const GameObject3D& other) const {
    Vector3D diff = position - other.position;
    return diff.magnitude();
}

bool GameObject3D::IsNear(const GameObject3D& other, double threshold) const {
    return DistanceTo(other) <= threshold;
}

// Cube3D implementations
Cube3D::Cube3D(const std::string& name, const Vector3D& pos, double size, sf::Color color)
    : GameObject3D(name, pos, color), size(size) {}

void Cube3D::Draw(sf::RenderWindow& window) const {
    if (!active) return;
    
    // Draw cube as a square with depth indication
    Vector2D projected = position.toVector2D();
    float scaledSize = static_cast<float>(size * scale.x);
    
    // Draw main face
    sf::RectangleShape mainFace(sf::Vector2f(scaledSize, scaledSize));
    mainFace.setPosition(static_cast<float>(projected.x), static_cast<float>(projected.y));
    mainFace.setFillColor(color);
    mainFace.setOutlineThickness(2.0f);
    mainFace.setOutlineColor(sf::Color::Black);
    window.draw(mainFace);
    
    // Draw depth indication (simple offset)
    float depthOffset = scaledSize * 0.2f;
    sf::RectangleShape depthFace(sf::Vector2f(scaledSize, scaledSize));
    depthFace.setPosition(static_cast<float>(projected.x + depthOffset), 
                         static_cast<float>(projected.y - depthOffset));
    sf::Color depthColor = color;
    depthColor.r = static_cast<sf::Uint8>(depthColor.r * 0.7);
    depthColor.g = static_cast<sf::Uint8>(depthColor.g * 0.7);
    depthColor.b = static_cast<sf::Uint8>(depthColor.b * 0.7);
    depthFace.setFillColor(depthColor);
    depthFace.setOutlineThickness(1.0f);
    depthFace.setOutlineColor(sf::Color::Black);
    window.draw(depthFace);
    
    // Draw connecting lines
    sf::Vertex lines[] = {
        sf::Vertex(sf::Vector2f(projected.x, projected.y), sf::Color::Black),
        sf::Vertex(sf::Vector2f(projected.x + depthOffset, projected.y - depthOffset), sf::Color::Black),
        
        sf::Vertex(sf::Vector2f(projected.x + scaledSize, projected.y), sf::Color::Black),
        sf::Vertex(sf::Vector2f(projected.x + scaledSize + depthOffset, projected.y - depthOffset), sf::Color::Black),
        
        sf::Vertex(sf::Vector2f(projected.x, projected.y + scaledSize), sf::Color::Black),
        sf::Vertex(sf::Vector2f(projected.x + depthOffset, projected.y + scaledSize - depthOffset), sf::Color::Black),
        
        sf::Vertex(sf::Vector2f(projected.x + scaledSize, projected.y + scaledSize), sf::Color::Black),
        sf::Vertex(sf::Vector2f(projected.x + scaledSize + depthOffset, projected.y + scaledSize - depthOffset), sf::Color::Black)
    };
    
    window.draw(lines, 8, sf::Lines);
}

// Sphere3D implementations
Sphere3D::Sphere3D(const std::string& name, const Vector3D& pos, double radius, sf::Color color)
    : GameObject3D(name, pos, color), radius(radius) {}

void Sphere3D::Draw(sf::RenderWindow& window) const {
    if (!active) return;
    
    Vector2D projected = position.toVector2D();
    float scaledRadius = static_cast<float>(radius * scale.x);
    
    sf::CircleShape shape(scaledRadius);
    shape.setPosition(static_cast<float>(projected.x - scaledRadius), 
                     static_cast<float>(projected.y - scaledRadius));
    shape.setFillColor(color);
    shape.setOutlineThickness(2.0f);
    shape.setOutlineColor(sf::Color::Black);
    
    // Add some shading to indicate 3D
    sf::CircleShape highlight(scaledRadius * 0.3f);
    sf::Color highlightColor = sf::Color::White;
    highlightColor.a = 100;
    highlight.setFillColor(highlightColor);
    highlight.setPosition(static_cast<float>(projected.x - scaledRadius * 0.3f), 
                         static_cast<float>(projected.y - scaledRadius * 0.7f));
    
    window.draw(shape);
    window.draw(highlight);
}