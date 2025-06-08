#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Vector2D.h"
#include "graphics.h"
#include <string>
#include <memory>

// Forward declarations
class GameEnvironment;

// --- GameObject Class ---
// A base class for any entity in the game world (player, enemy, projectile, etc.).
class GameObject {
protected:
    Vector2D position;  // Current position in the game world
    Vector2D velocity;  // Current velocity (how fast and in which direction it's moving)
    double width;       // Width of the object
    double height;      // Height of the object
    std::string name;   // Name of the object (e.g., "Player", "Enemy1")
    std::shared_ptr<SVGTexture> texture; // Visual representation
    Color tintColor;    // Color tint for the texture
    bool active;        // Is the object active in the game world?

public:
    // Constructor
    GameObject(std::string obj_name, double start_x, double start_y, double obj_width, double obj_height);
    
    // Constructor with texture
    GameObject(std::string obj_name, double start_x, double start_y, std::shared_ptr<SVGTexture> tex);

    // Virtual destructor to ensure proper cleanup for derived classes
    virtual ~GameObject() = default;

    // --- Getters ---
    const Vector2D& getPosition() const { return position; }
    const Vector2D& getVelocity() const { return velocity; }
    double getWidth() const { return width; }
    double getHeight() const { return height; }
    const std::string& getName() const { return name; }
    std::shared_ptr<SVGTexture> getTexture() const { return texture; }
    const Color& getTintColor() const { return tintColor; }
    bool isActive() const { return active; }

    // --- Setters ---
    void setPosition(double x, double y) { position.x = x; position.y = y; }
    void setPosition(const Vector2D& pos) { position = pos; }
    void setVelocity(double vx, double vy) { velocity.x = vx; velocity.y = vy; }
    void setVelocity(const Vector2D& vel) { velocity = vel; }
    void setTexture(std::shared_ptr<SVGTexture> tex);
    void setTintColor(const Color& color) { tintColor = color; }
    void setActive(bool isActive) { active = isActive; }

    // --- Core Game Loop Methods (virtual for overriding by derived classes) ---

    // Update method: Called every frame to update the object's state (e.g., position based on velocity).
    virtual void Update(double deltaTime, GameEnvironment* environment = nullptr);

    // Draw method: Called every frame to render the object.
    virtual void Draw(Renderer& renderer, const Camera& camera) const;

    // Basic collision detection (Axis-Aligned Bounding Box - AABB)
    bool CheckCollision(const GameObject& other) const;
    bool CheckCollision(const Vector2D& point) const;
    
    // Get bounding box
    Vector2D GetCenter() const { return position + Vector2D(width, height) * 0.5; }
    Vector2D GetSize() const { return Vector2D(width, height); }
    
    // Movement helpers
    void Move(const Vector2D& offset) { position += offset; }
    void AddVelocity(const Vector2D& vel) { velocity += vel; }
    void ApplyForce(const Vector2D& force, double mass = 1.0) { velocity += force / mass; }
    
    // Utility methods
    double DistanceTo(const GameObject& other) const;
    Vector2D DirectionTo(const GameObject& other) const;
    bool IsInBounds(const Vector2D& minBounds, const Vector2D& maxBounds) const;
};

#endif // GAMEOBJECT_H
