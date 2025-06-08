#include "GameObject.h"
#include "GameEnvironment.h"
#include <cmath>

// --- GameObject Implementation ---
GameObject::GameObject(std::string obj_name, double start_x, double start_y, double obj_width, double obj_height)
    : name(obj_name), position(start_x, start_y), width(obj_width), height(obj_height), 
      velocity(0.0, 0.0), texture(nullptr), tintColor(Color::WHITE), active(true) {}

GameObject::GameObject(std::string obj_name, double start_x, double start_y, std::shared_ptr<SVGTexture> tex)
    : name(obj_name), position(start_x, start_y), texture(tex), 
      velocity(0.0, 0.0), tintColor(Color::WHITE), active(true) {
    if (texture && texture->IsLoaded()) {
        Vector2D texSize = texture->GetSize();
        width = texSize.x;
        height = texSize.y;
    } else {
        width = 32.0;
        height = 32.0;
    }
}

void GameObject::setTexture(std::shared_ptr<SVGTexture> tex) {
    texture = tex;
    if (texture && texture->IsLoaded()) {
        Vector2D texSize = texture->GetSize();
        width = texSize.x;
        height = texSize.y;
    }
}

void GameObject::Update(double deltaTime, GameEnvironment* environment) {
    if (!active) return;
    
    // Basic physics update
    position = position + (velocity * deltaTime);
    
    // Check world bounds if environment is provided
    if (environment) {
        Vector2D worldSize = environment->GetWorldSize();
        
        // Keep object within world bounds
        if (position.x < 0) {
            position.x = 0;
            velocity.x = 0;
        } else if (position.x + width > worldSize.x) {
            position.x = worldSize.x - width;
            velocity.x = 0;
        }
        
        if (position.y < 0) {
            position.y = 0;
            velocity.y = 0;
        } else if (position.y + height > worldSize.y) {
            position.y = worldSize.y - height;
            velocity.y = 0;
        }
        
        // Check platform collisions
        Vector2D correctionOffset;
        if (environment->CheckPlatformCollision(position, Vector2D(width, height), correctionOffset)) {
            position += correctionOffset;
            
            // Stop velocity in the direction of collision
            if (std::abs(correctionOffset.x) > std::abs(correctionOffset.y)) {
                velocity.x = 0;
            } else {
                velocity.y = 0;
            }
        }
    }
}

void GameObject::Draw(Renderer& renderer, const Camera& camera) const {
    if (!active) return;
    
    Vector2D screenPos = camera.WorldToScreen(position);
    
    // Only draw if object is visible on screen
    if (camera.IsInView(position, Vector2D(width, height))) {
        if (texture && texture->IsLoaded()) {
            renderer.DrawTexture(*texture, screenPos);
        } else {
            // Draw a simple colored rectangle as fallback
            renderer.DrawRectangle(screenPos, Vector2D(width, height), tintColor);
        }
    }
}

bool GameObject::CheckCollision(const GameObject& other) const {
    if (!active || !other.active) return false;
    
    return !(position.x >= other.position.x + other.width ||
             position.x + width <= other.position.x ||
             position.y >= other.position.y + other.height ||
             position.y + height <= other.position.y);
}

bool GameObject::CheckCollision(const Vector2D& point) const {
    if (!active) return false;
    
    return (point.x >= position.x && point.x <= position.x + width &&
            point.y >= position.y && point.y <= position.y + height);
}

double GameObject::DistanceTo(const GameObject& other) const {
    Vector2D thisCenter = GetCenter();
    Vector2D otherCenter = other.GetCenter();
    return thisCenter.distanceTo(otherCenter);
}

Vector2D GameObject::DirectionTo(const GameObject& other) const {
    Vector2D thisCenter = GetCenter();
    Vector2D otherCenter = other.GetCenter();
    return (otherCenter - thisCenter).normalize();
}

bool GameObject::IsInBounds(const Vector2D& minBounds, const Vector2D& maxBounds) const {
    return (position.x >= minBounds.x && position.x + width <= maxBounds.x &&
            position.y >= minBounds.y && position.y + height <= maxBounds.y);
}
