#pragma once
#include "Vector2D.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>

class GameObject {
protected:
    Vector2D position;
    Vector2D velocity;
    double width, height;
    std::string name;
    sf::Color color;
    bool active;

public:
    GameObject(const std::string& name, double x, double y, double w, double h, sf::Color color = sf::Color::White);
    virtual ~GameObject() = default;

    // Getters
    const Vector2D& getPosition() const { return position; }
    const Vector2D& getVelocity() const { return velocity; }
    double getWidth() const { return width; }
    double getHeight() const { return height; }
    const std::string& getName() const { return name; }
    sf::Color getColor() const { return color; }
    bool isActive() const { return active; }

    // Setters
    void setPosition(const Vector2D& pos) { position = pos; }
    void setPosition(double x, double y) { position = Vector2D(x, y); }
    void setVelocity(const Vector2D& vel) { velocity = vel; }
    void setVelocity(double vx, double vy) { velocity = Vector2D(vx, vy); }
    void setColor(sf::Color c) { color = c; }
    void setActive(bool a) { active = a; }

    // Core methods
    virtual void Update(double deltaTime);
    virtual void Draw(sf::RenderWindow& window) const;
    virtual void DrawSVG(std::ofstream& svg) const;
    
    // Collision detection
    bool CheckCollision(const GameObject& other) const;
    Vector2D GetCenter() const;
    
    // Bounds checking
    bool IsInBounds(double minX, double minY, double maxX, double maxY) const;
    void ClampToBounds(double minX, double minY, double maxX, double maxY);
};