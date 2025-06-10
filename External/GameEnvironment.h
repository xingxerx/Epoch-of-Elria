#pragma once
#include "GameObject.h"
#include "GameObject3D.h"
#include "graphics.h"
#include <vector>
#include <memory>
#include <thread>
#include <mutex>

// Game environment management
class GameEnvironment {
private:
    std::vector<std::unique_ptr<GameObject>> gameObjects2D;
    std::vector<std::unique_ptr<GameObject3D>> gameObjects3D;
    Graphics::Camera camera;
    Graphics::ParticleSystem particleSystem;
    
    // Threading
    std::mutex objectMutex;
    unsigned int numThreads;
    
    // Environment properties
    sf::Color backgroundColor;
    sf::Vector2f worldSize;
    sf::Vector2f gravity;
    
    // Performance tracking
    double lastUpdateTime;
    double lastRenderTime;
    int frameCount;
    double fps;

public:
    GameEnvironment(float worldWidth, float worldHeight);
    ~GameEnvironment();

    // Object management
    void AddGameObject(std::unique_ptr<GameObject> obj);
    void AddGameObject3D(std::unique_ptr<GameObject3D> obj);
    void RemoveInactiveObjects();
    
    // Environment properties
    void SetBackgroundColor(const sf::Color& color) { backgroundColor = color; }
    void SetGravity(const sf::Vector2f& g) { gravity = g; }
    void SetWorldSize(const sf::Vector2f& size) { worldSize = size; }
    
    sf::Color GetBackgroundColor() const { return backgroundColor; }
    sf::Vector2f GetGravity() const { return gravity; }
    sf::Vector2f GetWorldSize() const { return worldSize; }
    
    // Camera access
    Graphics::Camera& GetCamera() { return camera; }
    const Graphics::Camera& GetCamera() const { return camera; }
    
    // Particle system access
    Graphics::ParticleSystem& GetParticleSystem() { return particleSystem; }
    
    // Core loop methods
    void Update(double deltaTime);
    void Render(sf::RenderWindow& window);
    
    // Threading control
    void SetThreadCount(unsigned int count) { numThreads = count; }
    unsigned int GetThreadCount() const { return numThreads; }
    
    // Collision detection
    std::vector<std::pair<GameObject*, GameObject*>> DetectCollisions2D();
    std::vector<std::pair<GameObject3D*, GameObject3D*>> DetectCollisions3D(double threshold = 50.0);
    
    // Spatial queries
    std::vector<GameObject*> GetObjectsInRadius2D(const Vector2D& center, double radius);
    std::vector<GameObject3D*> GetObjectsInRadius3D(const Vector3D& center, double radius);
    
    // Statistics
    size_t GetObjectCount2D() const { return gameObjects2D.size(); }
    size_t GetObjectCount3D() const { return gameObjects3D.size(); }
    double GetFPS() const { return fps; }
    double GetLastUpdateTime() const { return lastUpdateTime; }
    double GetLastRenderTime() const { return lastRenderTime; }
    
    // Utility methods
    void Clear();
    void ApplyGravityToAll();
    void BoundsCheckAll();

private:
    void UpdateObjects2DParallel(double deltaTime);
    void UpdateObjects3DParallel(double deltaTime);
    void UpdateRange2D(size_t start, size_t end, double deltaTime);
    void UpdateRange3D(size_t start, size_t end, double deltaTime);
    void CalculateFPS(double deltaTime);
};

// Specialized game environments
class PlatformerEnvironment : public GameEnvironment {
private:
    std::vector<sf::FloatRect> platforms;
    
public:
    PlatformerEnvironment(float worldWidth, float worldHeight);
    
    void AddPlatform(const sf::FloatRect& platform);
    void RemovePlatform(size_t index);
    const std::vector<sf::FloatRect>& GetPlatforms() const { return platforms; }
    
    void Update(double deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    
    bool IsOnPlatform(const GameObject& obj) const;
    sf::FloatRect* GetPlatformBelow(const GameObject& obj);
};

class SpaceEnvironment : public GameEnvironment {
private:
    std::vector<Vector3D> stars;
    double starSpeed;
    
public:
    SpaceEnvironment(float worldWidth, float worldHeight);
    
    void SetStarSpeed(double speed) { starSpeed = speed; }
    void GenerateStars(int count);
    
    void Update(double deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    
private:
    void UpdateStars(double deltaTime);
    void RenderStars(sf::RenderWindow& window);
};