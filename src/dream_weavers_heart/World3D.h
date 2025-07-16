#ifndef WORLD3D_H
#define WORLD3D_H

#include "GameObject3D.h"
#include <vector>
#include <memory>
#include <algorithm>

// --- 3D World Manager ---
class World3D {
private:
    std::vector<std::unique_ptr<GameObject3D>> gameObjects;
    std::vector<std::unique_ptr<Platform3D>> platforms;
    std::vector<std::unique_ptr<Collectible3D>> collectibles;
    
    WorldBounds3D bounds;
    Vector3D gravity;
    Camera3D camera;
    
    // Environment settings
    Vector3D ambientLight;
    Vector3D skyColor;
    double timeOfDay; // 0.0 = midnight, 0.5 = noon, 1.0 = midnight

public:
    World3D(const WorldBounds3D& worldBounds = WorldBounds3D())
        : bounds(worldBounds), gravity(0, -9.81, 0), 
          ambientLight(0.3, 0.3, 0.4), skyColor(0.5, 0.8, 1.0), timeOfDay(0.5) {
        
        // Position camera above the world center
        Vector3D worldCenter = bounds.getCenter();
        camera.setPosition(Vector3D(worldCenter.x, worldCenter.y + 5, worldCenter.z - 10));
        camera.lookAt(worldCenter);
    }

    // Getters
    const WorldBounds3D& getBounds() const { return bounds; }
    const Vector3D& getGravity() const { return gravity; }
    Camera3D& getCamera() { return camera; }
    const Camera3D& getCamera() const { return camera; }
    double getTimeOfDay() const { return timeOfDay; }
    const Vector3D& getSkyColor() const { return skyColor; }

    // Setters
    void setGravity(const Vector3D& grav) { gravity = grav; }
    void setTimeOfDay(double time) { timeOfDay = std::max(0.0, std::min(1.0, time)); }
    void setSkyColor(const Vector3D& color) { skyColor = color; }

    // Object management
    void addGameObject(std::unique_ptr<GameObject3D> obj) {
        gameObjects.push_back(std::move(obj));
    }

    void addPlatform(std::unique_ptr<Platform3D> platform) {
        platforms.push_back(std::move(platform));
    }

    void addCollectible(std::unique_ptr<Collectible3D> collectible) {
        collectibles.push_back(std::move(collectible));
    }

    // Create common objects
    void createGround(double y = 0.0) {
        Vector3D groundSize = bounds.getSize();
        groundSize.y = 1.0; // Ground thickness
        Vector3D groundPos = bounds.getCenter();
        groundPos.y = y - groundSize.y * 0.5;
        
        auto ground = std::unique_ptr<Platform3D>(new Platform3D(groundPos, groundSize));
        ground->setColor(Vector3D(0.3, 0.7, 0.2)); // Green ground
        addPlatform(std::move(ground));
    }

    void createRandomCollectibles(int count) {
        for (int i = 0; i < count; ++i) {
            Vector3D pos(
                bounds.min.x + (rand() % (int)(bounds.getSize().x * 0.8)) + bounds.getSize().x * 0.1,
                bounds.min.y + 2.0 + (rand() % 10),
                bounds.min.z + (rand() % (int)(bounds.getSize().z * 0.8)) + bounds.getSize().z * 0.1
            );
            
            int value = (i % 3 == 0) ? 25 : 10; // Some collectibles worth more
            auto collectible = std::unique_ptr<Collectible3D>(new Collectible3D(pos, value));
            addCollectible(std::move(collectible));
        }
    }

    void createPlatforms() {
        // Create some floating platforms
        for (int i = 0; i < 5; ++i) {
            Vector3D pos(
                bounds.min.x + (rand() % (int)(bounds.getSize().x * 0.6)) + bounds.getSize().x * 0.2,
                bounds.min.y + 3.0 + i * 2.0,
                bounds.min.z + (rand() % (int)(bounds.getSize().z * 0.6)) + bounds.getSize().z * 0.2
            );
            Vector3D size(3.0 + rand() % 5, 0.5, 3.0 + rand() % 5);
            
            auto platform = std::unique_ptr<Platform3D>(new Platform3D(pos, size));
            platform->setColor(Vector3D(0.6, 0.4, 0.2)); // Brown platforms
            addPlatform(std::move(platform));
        }
    }

    // Physics and collision
    bool checkPlatformCollision(const Vector3D& position, const Vector3D& size, Vector3D& correctionOffset) {
        correctionOffset = Vector3D::Zero();
        
        for (const auto& platform : platforms) {
            if (!platform->isSolid()) continue;
            
            // Create temporary object for collision check
            GameObject3D tempObj("temp", position, size);
            if (tempObj.CheckCollision(*platform)) {
                // Simple collision resolution - push object out of platform
                Vector3D platformCenter = platform->getPosition();
                Vector3D objectCenter = position;
                Vector3D direction = (objectCenter - platformCenter).normalize();
                
                // Calculate overlap
                Vector3D platformSize = platform->getSize();
                Vector3D overlap;
                overlap.x = (size.x + platformSize.x) * 0.5 - std::abs(objectCenter.x - platformCenter.x);
                overlap.y = (size.y + platformSize.y) * 0.5 - std::abs(objectCenter.y - platformCenter.y);
                overlap.z = (size.z + platformSize.z) * 0.5 - std::abs(objectCenter.z - platformCenter.z);
                
                // Resolve collision along the axis with smallest overlap
                if (overlap.y <= overlap.x && overlap.y <= overlap.z) {
                    correctionOffset.y = (direction.y > 0) ? overlap.y : -overlap.y;
                } else if (overlap.x <= overlap.z) {
                    correctionOffset.x = (direction.x > 0) ? overlap.x : -overlap.x;
                } else {
                    correctionOffset.z = (direction.z > 0) ? overlap.z : -overlap.z;
                }
                
                return true;
            }
        }
        return false;
    }

    bool isOnGround(const Vector3D& position, const Vector3D& size) {
        Vector3D groundCheckPos = position;
        groundCheckPos.y -= size.y * 0.5 + 0.1; // Check slightly below object
        
        Vector3D groundCheckSize = Vector3D(size.x * 0.8, 0.2, size.z * 0.8);
        Vector3D dummy;
        return checkPlatformCollision(groundCheckPos, groundCheckSize, dummy);
    }

    // Update world
    void update(double deltaTime) {
        // Update time of day (optional day/night cycle)
        // timeOfDay += deltaTime * 0.01; // Very slow day/night cycle
        // if (timeOfDay > 1.0) timeOfDay -= 1.0;

        // Update all game objects
        for (auto& obj : gameObjects) {
            if (obj && obj->isActive()) {
                obj->Update(deltaTime, this);
            }
        }

        // Update platforms
        for (auto& platform : platforms) {
            if (platform && platform->isActive()) {
                platform->Update(deltaTime, this);
            }
        }

        // Update collectibles
        for (auto& collectible : collectibles) {
            if (collectible && collectible->isActive()) {
                collectible->Update(deltaTime, this);
            }
        }

        // Remove collected items
        collectibles.erase(
            std::remove_if(collectibles.begin(), collectibles.end(),
                [](const std::unique_ptr<Collectible3D>& c) {
                    return c->isCollected();
                }),
            collectibles.end()
        );
    }

    // Rendering
    void draw() const {
        std::cout << "\n=== 3D WORLD VIEW ===\n";
        std::cout << "Camera at ";
        camera.getPosition().print();
        std::cout << " looking ";
        camera.getForward().print();
        std::cout << "\n";
        
        std::cout << "Sky Color: ";
        skyColor.print();
        std::cout << " (Time: " << timeOfDay << ")\n";

        // Draw all objects
        for (const auto& obj : gameObjects) {
            if (obj && obj->isVisible()) {
                obj->Draw(camera);
            }
        }

        for (const auto& platform : platforms) {
            if (platform && platform->isVisible()) {
                platform->Draw(camera);
            }
        }

        for (const auto& collectible : collectibles) {
            if (collectible && collectible->isVisible()) {
                collectible->Draw(camera);
            }
        }

        std::cout << "Collectibles remaining: " << collectibles.size() << "\n";
        std::cout << "===================\n";
    }

    // Utility functions
    std::vector<Collectible3D*> getNearbyCollectibles(const Vector3D& position, double radius) {
        std::vector<Collectible3D*> nearby;
        for (auto& collectible : collectibles) {
            if (collectible && !collectible->isCollected()) {
                double distance = position.distanceTo(collectible->getPosition());
                if (distance <= radius) {
                    nearby.push_back(collectible.get());
                }
            }
        }
        return nearby;
    }

    int getCollectibleCount() const {
        return static_cast<int>(collectibles.size());
    }

    bool isInBounds(const Vector3D& position) const {
        return bounds.contains(position);
    }

    Vector3D clampToBounds(const Vector3D& position) const {
        return bounds.clamp(position);
    }
};

#endif // WORLD3D_H
