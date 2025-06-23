#include "GameObject.h"
#include <algorithm>

GameObject::GameObject(const std::string& name, double x, double y, double w, double h, sf::Color color)
    : name(name), position(x, y), width(w), height(h), color(color), velocity(0, 0), active(true) {}

void GameObject::Update(double deltaTime) {
    if (active) {
        position = position + (velocity * deltaTime);
    }
}

void GameObject::Draw(sf::RenderWindow& window) const {
    if (!active) return;
    
    sf::RectangleShape shape(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
    shape.setPosition(static_cast<float>(position.x), static_cast<float>(position.y));
    shape.setFillColor(color);
    shape.setOutlineThickness(1.0f);
    shape.setOutlineColor(sf::Color::Black);
    window.draw(shape);
}

void GameObject::DrawSVG(std::ofstream& svg) const {
    if (!active) return;
    
    svg << "<rect x='" << position.x << "' y='" << position.y
        << "' width='" << width << "' height='" << height
        << "' fill='rgb(" << static_cast<int>(color.r) << "," 
        << static_cast<int>(color.g) << "," << static_cast<int>(color.b) 
        << ")' stroke='black' stroke-width='1'/>\n";
}

bool GameObject::CheckCollision(const GameObject& other) const {
    if (!active || !other.active) return false;
    
    return (position.x < other.position.x + other.width &&
            position.x + width > other.position.x &&
            position.y < other.position.y + other.height &&
            position.y + height > other.position.y);
}

Vector2D GameObject::GetCenter() const {
    return Vector2D(position.x + width / 2.0, position.y + height / 2.0);
}

bool GameObject::IsInBounds(double minX, double minY, double maxX, double maxY) const {
    return (position.x >= minX && position.y >= minY &&
            position.x + width <= maxX && position.y + height <= maxY);
}

void GameObject::ClampToBounds(double minX, double minY, double maxX, double maxY) {
    position.x = std::max(minX, std::min(position.x, maxX - width));
    position.y = std::max(minY, std::min(position.y, maxY - height));
}