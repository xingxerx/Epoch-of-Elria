#ifndef GAME_ENVIRONMENT_H
#define GAME_ENVIRONMENT_H

#include "Vector2D.h"
#include "graphics.h"
#include <vector>
#include <memory>

// Forward declarations
class GameObject;

// --- Camera Class ---
class Camera {
private:
    Vector2D position;
    Vector2D size;
    Vector2D worldBounds;
    
public:
    Camera(const Vector2D& viewSize, const Vector2D& worldSize);
    
    void SetPosition(const Vector2D& pos);
    void Move(const Vector2D& offset);
    void FollowTarget(const GameObject& target, double smoothing = 0.1);
    
    Vector2D WorldToScreen(const Vector2D& worldPos) const;
    Vector2D ScreenToWorld(const Vector2D& screenPos) const;
    
    bool IsInView(const Vector2D& worldPos, const Vector2D& objectSize) const;
    
    const Vector2D& GetPosition() const { return position; }
    const Vector2D& GetSize() const { return size; }
};

// --- Background Layer ---
class BackgroundLayer {
private:
    std::shared_ptr<SVGTexture> texture;
    Vector2D scrollSpeed;
    Vector2D position;
    Vector2D tileSize;
    bool repeating;
    
public:
    BackgroundLayer(std::shared_ptr<SVGTexture> tex, const Vector2D& speed, bool repeat = true);
    
    void Update(double deltaTime, const Camera& camera);
    void Draw(Renderer& renderer, const Camera& camera);
    
    void SetScrollSpeed(const Vector2D& speed) { scrollSpeed = speed; }
    void SetTileSize(const Vector2D& size) { tileSize = size; }
};

// --- Terrain/Platform System ---
class Platform {
private:
    Vector2D position;
    Vector2D size;
    std::shared_ptr<SVGTexture> texture;
    bool solid;
    Color color;
    
public:
    Platform(const Vector2D& pos, const Vector2D& sz, const Color& col = Color::GREEN, bool isSolid = true);
    Platform(const Vector2D& pos, const Vector2D& sz, std::shared_ptr<SVGTexture> tex, bool isSolid = true);
    
    void Draw(Renderer& renderer, const Camera& camera);
    bool CheckCollision(const Vector2D& objectPos, const Vector2D& objectSize) const;
    
    const Vector2D& GetPosition() const { return position; }
    const Vector2D& GetSize() const { return size; }
    bool IsSolid() const { return solid; }
};

// --- Particle System ---
struct Particle {
    Vector2D position;
    Vector2D velocity;
    Color color;
    double life;
    double maxLife;
    double size;
    
    Particle(const Vector2D& pos, const Vector2D& vel, const Color& col, double lifetime, double particleSize = 2.0);
    bool Update(double deltaTime);
    void Draw(Renderer& renderer, const Camera& camera);
};

class ParticleSystem {
private:
    std::vector<Particle> particles;
    Vector2D emitterPosition;
    int maxParticles;
    
public:
    ParticleSystem(const Vector2D& pos, int maxCount = 100);
    
    void EmitParticle(const Vector2D& velocity, const Color& color, double lifetime, double size = 2.0);
    void EmitBurst(int count, const Vector2D& baseVelocity, const Color& color, double lifetime, double spread = 50.0);
    
    void Update(double deltaTime);
    void Draw(Renderer& renderer, const Camera& camera);
    
    void SetPosition(const Vector2D& pos) { emitterPosition = pos; }
    int GetParticleCount() const { return particles.size(); }
};

// --- Game Environment ---
class GameEnvironment {
private:
    std::vector<BackgroundLayer> backgroundLayers;
    std::vector<Platform> platforms;
    std::vector<std::unique_ptr<ParticleSystem>> particleSystems;
    
    Camera camera;
    Vector2D worldSize;
    Color ambientColor;
    
    // Environment effects
    bool enableParallax;
    double windStrength;
    Vector2D windDirection;
    
public:
    GameEnvironment(const Vector2D& viewSize, const Vector2D& worldBounds);
    
    // Background management
    void AddBackgroundLayer(std::shared_ptr<SVGTexture> texture, const Vector2D& scrollSpeed, bool repeating = true);
    void CreateProceduralBackground(const std::string& name, const Color& skyColor, const Color& groundColor);
    
    // Platform management
    void AddPlatform(const Vector2D& position, const Vector2D& size, const Color& color = Color::GREEN);
    void AddPlatform(const Vector2D& position, const Vector2D& size, std::shared_ptr<SVGTexture> texture);
    void CreateLevel1Platforms(); // Predefined level
    
    // Particle effects
    ParticleSystem* CreateParticleSystem(const Vector2D& position, int maxParticles = 100);
    void CreateCollectionEffect(const Vector2D& position);
    void CreateExplosionEffect(const Vector2D& position);
    
    // Environment effects
    void SetWind(const Vector2D& direction, double strength);
    void SetAmbientColor(const Color& color) { ambientColor = color; }
    
    // Update and rendering
    void Update(double deltaTime, const GameObject& player);
    void Draw(Renderer& renderer);
    
    // Collision detection
    bool CheckPlatformCollision(const Vector2D& objectPos, const Vector2D& objectSize, Vector2D& correctionOffset);
    std::vector<Platform*> GetNearbyPlatforms(const Vector2D& position, double radius);
    
    // Camera access
    Camera& GetCamera() { return camera; }
    const Camera& GetCamera() const { return camera; }
    
    // World properties
    const Vector2D& GetWorldSize() const { return worldSize; }
    bool IsInWorldBounds(const Vector2D& position) const;
};

// --- Asset Creation Utilities ---
namespace EnvironmentAssets {
    // Create common SVG textures
    std::shared_ptr<SVGTexture> CreateGrassTexture(double width = 64, double height = 32);
    std::shared_ptr<SVGTexture> CreateStoneTexture(double width = 64, double height = 32);
    std::shared_ptr<SVGTexture> CreateSkyTexture(double width = 800, double height = 200);
    std::shared_ptr<SVGTexture> CreateCloudTexture(double width = 100, double height = 50);
    std::shared_ptr<SVGTexture> CreateTreeTexture(double width = 80, double height = 120);
    
    // Create player and collectible textures
    std::shared_ptr<SVGTexture> CreatePlayerTexture(double size = 50);
    std::shared_ptr<SVGTexture> CreateCoinTexture(double size = 20);
    std::shared_ptr<SVGTexture> CreateGemTexture(double size = 20, const Color& gemColor = Color::BLUE);
}

#endif // GAME_ENVIRONMENT_H
