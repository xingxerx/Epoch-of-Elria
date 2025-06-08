#ifndef GAMEOBJECT3D_H
#define GAMEOBJECT3D_H

#include "Vector3D.h"
#include <string>
#include <vector>
#include <memory>

// Forward declarations
class World3D;

// --- Base 3D GameObject ---
class GameObject3D {
protected:
    Transform3D transform;
    Vector3D velocity;
    Vector3D size; // Bounding box size
    std::string name;
    bool active;
    bool visible;

public:
    GameObject3D(const std::string& objName, const Vector3D& pos = Vector3D::Zero(), 
                 const Vector3D& sz = Vector3D::One())
        : name(objName), transform(pos), velocity(Vector3D::Zero()), 
          size(sz), active(true), visible(true) {}

    virtual ~GameObject3D() = default;

    // Getters
    const Vector3D& getPosition() const { return transform.position; }
    const Vector3D& getRotation() const { return transform.rotation; }
    const Vector3D& getScale() const { return transform.scale; }
    const Vector3D& getVelocity() const { return velocity; }
    const Vector3D& getSize() const { return size; }
    const std::string& getName() const { return name; }
    bool isActive() const { return active; }
    bool isVisible() const { return visible; }
    const Transform3D& getTransform() const { return transform; }

    // Setters
    void setPosition(const Vector3D& pos) { transform.position = pos; }
    void setRotation(const Vector3D& rot) { transform.rotation = rot; }
    void setScale(const Vector3D& scl) { transform.scale = scl; }
    void setVelocity(const Vector3D& vel) { velocity = vel; }
    void setActive(bool isActive) { active = isActive; }
    void setVisible(bool isVisible) { visible = isVisible; }

    // Movement
    void move(const Vector3D& offset) { transform.position += offset; }
    void rotate(const Vector3D& rotOffset) { transform.rotation += rotOffset; }

    // Virtual methods for derived classes
    virtual void Update(double deltaTime, World3D* world = nullptr) {
        if (!active) return;
        transform.position += velocity * deltaTime;
    }

    virtual void Draw(const Camera3D& camera) const {
        if (!active || !visible) return;
        // Base implementation - derived classes will override
    }

    // 3D Collision detection (AABB)
    bool CheckCollision(const GameObject3D& other) const {
        if (!active || !other.active) return false;

        Vector3D thisMin = transform.position - size * 0.5;
        Vector3D thisMax = transform.position + size * 0.5;
        Vector3D otherMin = other.transform.position - other.size * 0.5;
        Vector3D otherMax = other.transform.position + other.size * 0.5;

        return (thisMin.x <= otherMax.x && thisMax.x >= otherMin.x) &&
               (thisMin.y <= otherMax.y && thisMax.y >= otherMin.y) &&
               (thisMin.z <= otherMax.z && thisMax.z >= otherMin.z);
    }

    // Distance calculations
    double distanceTo(const GameObject3D& other) const {
        return transform.position.distanceTo(other.transform.position);
    }

    // Check if point is within object bounds
    bool containsPoint(const Vector3D& point) const {
        Vector3D min = transform.position - size * 0.5;
        Vector3D max = transform.position + size * 0.5;
        
        return (point.x >= min.x && point.x <= max.x) &&
               (point.y >= min.y && point.y <= max.y) &&
               (point.z >= min.z && point.z <= max.z);
    }
};

// --- 3D Player Class with WASD Movement ---
class Player3D : public GameObject3D {
private:
    double moveSpeed;
    double rotationSpeed;
    double jumpPower;
    bool onGround;
    Vector3D inputDirection;

public:
    Player3D(const Vector3D& startPos = Vector3D(0, 1, 0))
        : GameObject3D("Player", startPos, Vector3D(1.0, 2.0, 1.0)),
          moveSpeed(5.0), rotationSpeed(90.0), jumpPower(8.0), onGround(true) {
        std::cout << "3D Player created at position ";
        transform.position.print();
        std::cout << std::endl;
    }

    // Input handling
    void setInputDirection(const Vector3D& direction) {
        inputDirection = direction.normalize();
    }

    void jump() {
        if (onGround) {
            velocity.y = jumpPower;
            onGround = false;
        }
    }

    void Update(double deltaTime, World3D* world = nullptr) override;

    void Draw(const Camera3D& camera) const override {
        if (!active || !visible) return;
        
        std::cout << "PLAYER 3D at ";
        transform.position.print();
        std::cout << " facing ";
        transform.getForward().print();
        std::cout << std::endl;
    }

    // Getters
    double getMoveSpeed() const { return moveSpeed; }
    bool isOnGround() const { return onGround; }
    void setOnGround(bool ground) { onGround = ground; }
};

// --- 3D Collectible Class ---
class Collectible3D : public GameObject3D {
private:
    int value;
    bool collected;
    double animationTime;
    Vector3D originalPosition;
    double bobHeight;
    double rotationSpeed;

public:
    Collectible3D(const Vector3D& pos, int val = 10)
        : GameObject3D("Collectible3D", pos, Vector3D(0.5, 0.5, 0.5)),
          value(val), collected(false), animationTime(0.0),
          originalPosition(pos), bobHeight(0.3), rotationSpeed(45.0) {}

    int getValue() const { return value; }
    bool isCollected() const { return collected; }
    void setCollected(bool status) { 
        collected = status; 
        active = !collected;
        visible = !collected;
    }

    void Update(double deltaTime, World3D* world = nullptr) override {
        if (collected) return;

        animationTime += deltaTime;
        
        // Bobbing animation
        transform.position.y = originalPosition.y + std::sin(animationTime * 2.0) * bobHeight;
        
        // Rotation animation
        transform.rotation.y += rotationSpeed * deltaTime;
        if (transform.rotation.y > 360.0) transform.rotation.y -= 360.0;

        GameObject3D::Update(deltaTime, world);
    }

    void Draw(const Camera3D& camera) const override {
        if (!active || !visible || collected) return;
        
        std::cout << "COLLECTIBLE 3D (value: " << value << ") at ";
        transform.position.print();
        std::cout << std::endl;
    }
};

// --- 3D Terrain/Platform Class ---
class Platform3D : public GameObject3D {
private:
    Vector3D color;
    bool solid;

public:
    Platform3D(const Vector3D& pos, const Vector3D& sz, bool isSolid = true)
        : GameObject3D("Platform3D", pos, sz), solid(isSolid), color(0.5, 0.8, 0.3) {}

    bool isSolid() const { return solid; }
    const Vector3D& getColor() const { return color; }
    void setColor(const Vector3D& col) { color = col; }

    void Update(double deltaTime, World3D* world = nullptr) override {
        // Platforms are usually static, but could have moving platforms
        GameObject3D::Update(deltaTime, world);
    }

    void Draw(const Camera3D& camera) const override {
        if (!active || !visible) return;
        
        std::cout << "PLATFORM 3D at ";
        transform.position.print();
        std::cout << " size ";
        size.print();
        std::cout << std::endl;
    }
};

// --- 3D World Bounds ---
struct WorldBounds3D {
    Vector3D min;
    Vector3D max;
    
    WorldBounds3D(const Vector3D& minBounds = Vector3D(-50, -10, -50),
                  const Vector3D& maxBounds = Vector3D(50, 50, 50))
        : min(minBounds), max(maxBounds) {}

    bool contains(const Vector3D& point) const {
        return (point.x >= min.x && point.x <= max.x) &&
               (point.y >= min.y && point.y <= max.y) &&
               (point.z >= min.z && point.z <= max.z);
    }

    Vector3D clamp(const Vector3D& point) const {
        return Vector3D(
            std::max(min.x, std::min(max.x, point.x)),
            std::max(min.y, std::min(max.y, point.y)),
            std::max(min.z, std::min(max.z, point.z))
        );
    }

    Vector3D getSize() const {
        return max - min;
    }

    Vector3D getCenter() const {
        return (min + max) * 0.5;
    }
};

#endif // GAMEOBJECT3D_H
