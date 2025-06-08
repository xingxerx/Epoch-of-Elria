#include "GameEnvironment.h"
#include "GameObject.h"
#include <algorithm>
#include <random>
#include <cmath>

// --- Camera Implementation ---
Camera::Camera(const Vector2D& viewSize, const Vector2D& worldSize) 
    : position(0, 0), size(viewSize), worldBounds(worldSize) {}

void Camera::SetPosition(const Vector2D& pos) {
    position = pos;
    
    // Clamp camera to world bounds
    position.x = std::max(0.0, std::min(position.x, worldBounds.x - size.x));
    position.y = std::max(0.0, std::min(position.y, worldBounds.y - size.y));
}

void Camera::Move(const Vector2D& offset) {
    SetPosition(position + offset);
}

void Camera::FollowTarget(const GameObject& target, double smoothing) {
    Vector2D targetPos = target.getPosition();
    Vector2D desiredPos = targetPos - size * 0.5; // Center target on screen
    
    Vector2D newPos = position + (desiredPos - position) * smoothing;
    SetPosition(newPos);
}

Vector2D Camera::WorldToScreen(const Vector2D& worldPos) const {
    return worldPos - position;
}

Vector2D Camera::ScreenToWorld(const Vector2D& screenPos) const {
    return screenPos + position;
}

bool Camera::IsInView(const Vector2D& worldPos, const Vector2D& objectSize) const {
    Vector2D screenPos = WorldToScreen(worldPos);
    return !(screenPos.x + objectSize.x < 0 || screenPos.x > size.x ||
             screenPos.y + objectSize.y < 0 || screenPos.y > size.y);
}

// --- BackgroundLayer Implementation ---
BackgroundLayer::BackgroundLayer(std::shared_ptr<SVGTexture> tex, const Vector2D& speed, bool repeat)
    : texture(tex), scrollSpeed(speed), position(0, 0), repeating(repeat) {
    if (texture && texture->IsLoaded()) {
        tileSize = texture->GetSize();
    } else {
        tileSize = Vector2D(800, 600); // Default size
    }
}

void BackgroundLayer::Update(double deltaTime, const Camera& camera) {
    // Update position based on camera movement and scroll speed
    Vector2D cameraOffset = camera.GetPosition() * scrollSpeed;
    position = -cameraOffset;
    
    if (repeating) {
        // Wrap position for seamless scrolling
        while (position.x > 0) position.x -= tileSize.x;
        while (position.x < -tileSize.x) position.x += tileSize.x;
        while (position.y > 0) position.y -= tileSize.y;
        while (position.y < -tileSize.y) position.y += tileSize.y;
    }
}

void BackgroundLayer::Draw(Renderer& renderer, const Camera& camera) {
    if (!texture || !texture->IsLoaded()) return;
    
    if (repeating) {
        // Draw tiled background
        Vector2D cameraSize = camera.GetSize();
        int tilesX = static_cast<int>(std::ceil(cameraSize.x / tileSize.x)) + 2;
        int tilesY = static_cast<int>(std::ceil(cameraSize.y / tileSize.y)) + 2;
        
        for (int y = -1; y < tilesY; ++y) {
            for (int x = -1; x < tilesX; ++x) {
                Vector2D tilePos = position + Vector2D(x * tileSize.x, y * tileSize.y);
                renderer.DrawTexture(*texture, tilePos);
            }
        }
    } else {
        // Draw single background
        renderer.DrawTexture(*texture, position);
    }
}

// --- Platform Implementation ---
Platform::Platform(const Vector2D& pos, const Vector2D& sz, const Color& col, bool isSolid)
    : position(pos), size(sz), color(col), solid(isSolid), texture(nullptr) {}

Platform::Platform(const Vector2D& pos, const Vector2D& sz, std::shared_ptr<SVGTexture> tex, bool isSolid)
    : position(pos), size(sz), texture(tex), solid(isSolid), color(Color::GREEN) {}

void Platform::Draw(Renderer& renderer, const Camera& camera) {
    Vector2D screenPos = camera.WorldToScreen(position);
    
    if (camera.IsInView(position, size)) {
        if (texture && texture->IsLoaded()) {
            renderer.DrawTexture(*texture, screenPos);
        } else {
            renderer.DrawRectangle(screenPos, size, color);
        }
    }
}

bool Platform::CheckCollision(const Vector2D& objectPos, const Vector2D& objectSize) const {
    return !(objectPos.x >= position.x + size.x ||
             objectPos.x + objectSize.x <= position.x ||
             objectPos.y >= position.y + size.y ||
             objectPos.y + objectSize.y <= position.y);
}

// --- Particle Implementation ---
Particle::Particle(const Vector2D& pos, const Vector2D& vel, const Color& col, double lifetime, double particleSize)
    : position(pos), velocity(vel), color(col), life(lifetime), maxLife(lifetime), size(particleSize) {}

bool Particle::Update(double deltaTime) {
    position += velocity * deltaTime;
    life -= deltaTime;
    
    // Fade out over time
    double alpha = life / maxLife;
    color.a = static_cast<float>(alpha);
    
    // Apply gravity
    velocity.y += 98.0 * deltaTime; // Simple gravity
    
    return life > 0;
}

void Particle::Draw(Renderer& renderer, const Camera& camera) {
    Vector2D screenPos = camera.WorldToScreen(position);
    if (camera.IsInView(position, Vector2D(size, size))) {
        renderer.DrawCircle(screenPos, size, color);
    }
}

// --- ParticleSystem Implementation ---
ParticleSystem::ParticleSystem(const Vector2D& pos, int maxCount)
    : emitterPosition(pos), maxParticles(maxCount) {
    particles.reserve(maxCount);
}

void ParticleSystem::EmitParticle(const Vector2D& velocity, const Color& color, double lifetime, double size) {
    if (particles.size() < maxParticles) {
        particles.emplace_back(emitterPosition, velocity, color, lifetime, size);
    }
}

void ParticleSystem::EmitBurst(int count, const Vector2D& baseVelocity, const Color& color, double lifetime, double spread) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> angleDist(0, 2 * M_PI);
    std::uniform_real_distribution<> speedDist(0.5, 1.5);
    
    for (int i = 0; i < count && particles.size() < maxParticles; ++i) {
        double angle = angleDist(gen);
        double speed = speedDist(gen);
        Vector2D velocity = baseVelocity + Vector2D(std::cos(angle), std::sin(angle)) * spread * speed;
        particles.emplace_back(emitterPosition, velocity, color, lifetime);
    }
}

void ParticleSystem::Update(double deltaTime) {
    // Update all particles and remove dead ones
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [deltaTime](Particle& p) { return !p.Update(deltaTime); }),
        particles.end()
    );
}

void ParticleSystem::Draw(Renderer& renderer, const Camera& camera) {
    for (auto& particle : particles) {
        particle.Draw(renderer, camera);
    }
}

// --- GameEnvironment Implementation ---
GameEnvironment::GameEnvironment(const Vector2D& viewSize, const Vector2D& worldBounds)
    : camera(viewSize, worldBounds), worldSize(worldBounds), ambientColor(Color::WHITE),
      enableParallax(true), windStrength(0.0), windDirection(Vector2D::Right()) {}

void GameEnvironment::AddBackgroundLayer(std::shared_ptr<SVGTexture> texture, const Vector2D& scrollSpeed, bool repeating) {
    backgroundLayers.emplace_back(texture, scrollSpeed, repeating);
}

void GameEnvironment::CreateProceduralBackground(const std::string& name, const Color& skyColor, const Color& groundColor) {
    // Create a simple gradient background
    std::string skyGradient = 
        "<svg width=\"800\" height=\"600\" xmlns=\"http://www.w3.org/2000/svg\">\n"
        "  <defs>\n"
        "    <linearGradient id=\"skyGradient\" x1=\"0%\" y1=\"0%\" x2=\"0%\" y2=\"100%\">\n"
        "      <stop offset=\"0%\" style=\"stop-color:rgb(" + 
        std::to_string((int)(skyColor.r * 255)) + "," + 
        std::to_string((int)(skyColor.g * 255)) + "," + 
        std::to_string((int)(skyColor.b * 255)) + ");stop-opacity:1\" />\n"
        "      <stop offset=\"100%\" style=\"stop-color:rgb(" + 
        std::to_string((int)(groundColor.r * 255)) + "," + 
        std::to_string((int)(groundColor.g * 255)) + "," + 
        std::to_string((int)(groundColor.b * 255)) + ");stop-opacity:1\" />\n"
        "    </linearGradient>\n"
        "  </defs>\n"
        "  <rect width=\"100%\" height=\"100%\" fill=\"url(#skyGradient)\" />\n"
        "</svg>";
    
    auto bgTexture = TextureManager::GetInstance().CreateProceduralTexture(name, skyGradient);
    if (bgTexture) {
        AddBackgroundLayer(bgTexture, Vector2D(0.1, 0.1), true);
    }
}

void GameEnvironment::AddPlatform(const Vector2D& position, const Vector2D& size, const Color& color) {
    platforms.emplace_back(position, size, color);
}

void GameEnvironment::AddPlatform(const Vector2D& position, const Vector2D& size, std::shared_ptr<SVGTexture> texture) {
    platforms.emplace_back(position, size, texture);
}

void GameEnvironment::CreateLevel1Platforms() {
    // Ground platforms
    AddPlatform(Vector2D(0, worldSize.y - 50), Vector2D(worldSize.x, 50), Color::GREEN);

    // Floating platforms
    AddPlatform(Vector2D(200, worldSize.y - 150), Vector2D(150, 20), Color::GREEN);
    AddPlatform(Vector2D(400, worldSize.y - 250), Vector2D(150, 20), Color::GREEN);
    AddPlatform(Vector2D(600, worldSize.y - 180), Vector2D(150, 20), Color::GREEN);
    AddPlatform(Vector2D(800, worldSize.y - 300), Vector2D(150, 20), Color::GREEN);

    // Walls (optional boundaries)
    AddPlatform(Vector2D(-10, 0), Vector2D(10, worldSize.y), Color::RED);
    AddPlatform(Vector2D(worldSize.x, 0), Vector2D(10, worldSize.y), Color::RED);
}

ParticleSystem* GameEnvironment::CreateParticleSystem(const Vector2D& position, int maxParticles) {
    auto system = std::make_unique<ParticleSystem>(position, maxParticles);
    ParticleSystem* ptr = system.get();
    particleSystems.push_back(std::move(system));
    return ptr;
}

void GameEnvironment::CreateCollectionEffect(const Vector2D& position) {
    auto system = CreateParticleSystem(position, 20);
    if (system) {
        system->EmitBurst(15, Vector2D(0, -50), Color::YELLOW, 1.0, 30.0);
    }
}

void GameEnvironment::CreateExplosionEffect(const Vector2D& position) {
    auto system = CreateParticleSystem(position, 50);
    if (system) {
        system->EmitBurst(30, Vector2D(0, -100), Color::ORANGE, 1.5, 80.0);
        system->EmitBurst(20, Vector2D(0, -80), Color::RED, 1.2, 60.0);
    }
}

void GameEnvironment::SetWind(const Vector2D& direction, double strength) {
    windDirection = direction.normalize();
    windStrength = strength;
}

void GameEnvironment::Update(double deltaTime, const GameObject& player) {
    // Update camera to follow player
    camera.FollowTarget(player, 0.1);

    // Update background layers
    for (auto& layer : backgroundLayers) {
        layer.Update(deltaTime, camera);
    }

    // Update particle systems
    for (auto& system : particleSystems) {
        system->Update(deltaTime);
    }

    // Remove empty particle systems
    particleSystems.erase(
        std::remove_if(particleSystems.begin(), particleSystems.end(),
            [](const std::unique_ptr<ParticleSystem>& system) {
                return system->GetParticleCount() == 0;
            }),
        particleSystems.end()
    );
}

void GameEnvironment::Draw(Renderer& renderer) {
    // Draw background layers (back to front)
    for (auto& layer : backgroundLayers) {
        layer.Draw(renderer, camera);
    }

    // Draw platforms
    for (auto& platform : platforms) {
        platform.Draw(renderer, camera);
    }

    // Draw particle systems
    for (auto& system : particleSystems) {
        system->Draw(renderer, camera);
    }
}

bool GameEnvironment::CheckPlatformCollision(const Vector2D& objectPos, const Vector2D& objectSize, Vector2D& correctionOffset) {
    correctionOffset = Vector2D::Zero();
    bool collisionFound = false;

    for (const auto& platform : platforms) {
        if (platform.IsSolid() && platform.CheckCollision(objectPos, objectSize)) {
            // Simple collision resolution - push object out of platform
            Vector2D platformCenter = platform.GetPosition() + platform.GetSize() * 0.5;
            Vector2D objectCenter = objectPos + objectSize * 0.5;
            Vector2D direction = (objectCenter - platformCenter).normalize();

            // Calculate overlap
            Vector2D overlap;
            overlap.x = (objectSize.x + platform.GetSize().x) * 0.5 - std::abs(objectCenter.x - platformCenter.x);
            overlap.y = (objectSize.y + platform.GetSize().y) * 0.5 - std::abs(objectCenter.y - platformCenter.y);

            // Resolve collision along the axis with smallest overlap
            if (overlap.x < overlap.y) {
                correctionOffset.x = (direction.x > 0) ? overlap.x : -overlap.x;
            } else {
                correctionOffset.y = (direction.y > 0) ? overlap.y : -overlap.y;
            }

            collisionFound = true;
            break; // Handle one collision at a time for simplicity
        }
    }

    return collisionFound;
}

std::vector<Platform*> GameEnvironment::GetNearbyPlatforms(const Vector2D& position, double radius) {
    std::vector<Platform*> nearby;
    for (auto& platform : platforms) {
        Vector2D platformCenter = platform.GetPosition() + platform.GetSize() * 0.5;
        if (position.distanceTo(platformCenter) <= radius) {
            nearby.push_back(&platform);
        }
    }
    return nearby;
}

bool GameEnvironment::IsInWorldBounds(const Vector2D& position) const {
    return position.x >= 0 && position.x <= worldSize.x &&
           position.y >= 0 && position.y <= worldSize.y;
}

// --- EnvironmentAssets Implementation ---
namespace EnvironmentAssets {

std::shared_ptr<SVGTexture> CreateGrassTexture(double width, double height) {
    std::string grassSVG =
        "<svg width=\"" + std::to_string(width) + "\" height=\"" + std::to_string(height) + "\" xmlns=\"http://www.w3.org/2000/svg\">\n"
        "  <rect width=\"100%\" height=\"100%\" fill=\"#4a7c59\"/>\n"
        "  <rect x=\"0\" y=\"0\" width=\"100%\" height=\"8\" fill=\"#5d8a6b\"/>\n"
        "  <!-- Grass blades -->\n"
        "  <path d=\"M5,8 Q7,4 9,8\" stroke=\"#6b9b73\" stroke-width=\"1\" fill=\"none\"/>\n"
        "  <path d=\"M15,8 Q17,3 19,8\" stroke=\"#6b9b73\" stroke-width=\"1\" fill=\"none\"/>\n"
        "  <path d=\"M25,8 Q27,5 29,8\" stroke=\"#6b9b73\" stroke-width=\"1\" fill=\"none\"/>\n"
        "  <path d=\"M35,8 Q37,2 39,8\" stroke=\"#6b9b73\" stroke-width=\"1\" fill=\"none\"/>\n"
        "  <path d=\"M45,8 Q47,4 49,8\" stroke=\"#6b9b73\" stroke-width=\"1\" fill=\"none\"/>\n"
        "  <path d=\"M55,8 Q57,3 59,8\" stroke=\"#6b9b73\" stroke-width=\"1\" fill=\"none\"/>\n"
        "</svg>";

    return TextureManager::GetInstance().CreateProceduralTexture("grass_" + std::to_string(width) + "x" + std::to_string(height), grassSVG);
}

std::shared_ptr<SVGTexture> CreateStoneTexture(double width, double height) {
    std::string stoneSVG =
        "<svg width=\"" + std::to_string(width) + "\" height=\"" + std::to_string(height) + "\" xmlns=\"http://www.w3.org/2000/svg\">\n"
        "  <rect width=\"100%\" height=\"100%\" fill=\"#8b8680\"/>\n"
        "  <rect x=\"2\" y=\"2\" width=\"" + std::to_string(width-4) + "\" height=\"" + std::to_string(height-4) + "\" fill=\"#9d9892\" stroke=\"#6b6660\" stroke-width=\"1\"/>\n"
        "  <!-- Stone texture details -->\n"
        "  <circle cx=\"" + std::to_string(width*0.3) + "\" cy=\"" + std::to_string(height*0.4) + "\" r=\"2\" fill=\"#7a756f\"/>\n"
        "  <circle cx=\"" + std::to_string(width*0.7) + "\" cy=\"" + std::to_string(height*0.6) + "\" r=\"1.5\" fill=\"#7a756f\"/>\n"
        "  <circle cx=\"" + std::to_string(width*0.5) + "\" cy=\"" + std::to_string(height*0.2) + "\" r=\"1\" fill=\"#7a756f\"/>\n"
        "</svg>";

    return TextureManager::GetInstance().CreateProceduralTexture("stone_" + std::to_string(width) + "x" + std::to_string(height), stoneSVG);
}

std::shared_ptr<SVGTexture> CreateSkyTexture(double width, double height) {
    std::string skySVG =
        "<svg width=\"" + std::to_string(width) + "\" height=\"" + std::to_string(height) + "\" xmlns=\"http://www.w3.org/2000/svg\">\n"
        "  <defs>\n"
        "    <linearGradient id=\"skyGradient\" x1=\"0%\" y1=\"0%\" x2=\"0%\" y2=\"100%\">\n"
        "      <stop offset=\"0%\" style=\"stop-color:#87CEEB;stop-opacity:1\" />\n"
        "      <stop offset=\"100%\" style=\"stop-color:#E0F6FF;stop-opacity:1\" />\n"
        "    </linearGradient>\n"
        "  </defs>\n"
        "  <rect width=\"100%\" height=\"100%\" fill=\"url(#skyGradient)\" />\n"
        "</svg>";

    return TextureManager::GetInstance().CreateProceduralTexture("sky_" + std::to_string(width) + "x" + std::to_string(height), skySVG);
}

std::shared_ptr<SVGTexture> CreateCloudTexture(double width, double height) {
    std::string cloudSVG =
        "<svg width=\"" + std::to_string(width) + "\" height=\"" + std::to_string(height) + "\" xmlns=\"http://www.w3.org/2000/svg\">\n"
        "  <ellipse cx=\"" + std::to_string(width*0.3) + "\" cy=\"" + std::to_string(height*0.6) + "\" rx=\"" + std::to_string(width*0.2) + "\" ry=\"" + std::to_string(height*0.3) + "\" fill=\"white\" opacity=\"0.8\"/>\n"
        "  <ellipse cx=\"" + std::to_string(width*0.5) + "\" cy=\"" + std::to_string(height*0.5) + "\" rx=\"" + std::to_string(width*0.25) + "\" ry=\"" + std::to_string(height*0.35) + "\" fill=\"white\" opacity=\"0.8\"/>\n"
        "  <ellipse cx=\"" + std::to_string(width*0.7) + "\" cy=\"" + std::to_string(height*0.6) + "\" rx=\"" + std::to_string(width*0.2) + "\" ry=\"" + std::to_string(height*0.3) + "\" fill=\"white\" opacity=\"0.8\"/>\n"
        "</svg>";

    return TextureManager::GetInstance().CreateProceduralTexture("cloud_" + std::to_string(width) + "x" + std::to_string(height), cloudSVG);
}

std::shared_ptr<SVGTexture> CreateTreeTexture(double width, double height) {
    std::string treeSVG =
        "<svg width=\"" + std::to_string(width) + "\" height=\"" + std::to_string(height) + "\" xmlns=\"http://www.w3.org/2000/svg\">\n"
        "  <!-- Tree trunk -->\n"
        "  <rect x=\"" + std::to_string(width*0.4) + "\" y=\"" + std::to_string(height*0.6) + "\" width=\"" + std::to_string(width*0.2) + "\" height=\"" + std::to_string(height*0.4) + "\" fill=\"#8B4513\"/>\n"
        "  <!-- Tree foliage -->\n"
        "  <circle cx=\"" + std::to_string(width*0.5) + "\" cy=\"" + std::to_string(height*0.4) + "\" r=\"" + std::to_string(width*0.3) + "\" fill=\"#228B22\"/>\n"
        "  <circle cx=\"" + std::to_string(width*0.4) + "\" cy=\"" + std::to_string(height*0.3) + "\" r=\"" + std::to_string(width*0.25) + "\" fill=\"#32CD32\"/>\n"
        "  <circle cx=\"" + std::to_string(width*0.6) + "\" cy=\"" + std::to_string(height*0.35) + "\" r=\"" + std::to_string(width*0.2) + "\" fill=\"#32CD32\"/>\n"
        "</svg>";

    return TextureManager::GetInstance().CreateProceduralTexture("tree_" + std::to_string(width) + "x" + std::to_string(height), treeSVG);
}

std::shared_ptr<SVGTexture> CreatePlayerTexture(double size) {
    std::string playerSVG =
        "<svg width=\"" + std::to_string(size) + "\" height=\"" + std::to_string(size) + "\" xmlns=\"http://www.w3.org/2000/svg\">\n"
        "  <!-- Player body -->\n"
        "  <rect x=\"" + std::to_string(size*0.2) + "\" y=\"" + std::to_string(size*0.3) + "\" width=\"" + std::to_string(size*0.6) + "\" height=\"" + std::to_string(size*0.5) + "\" fill=\"#4169E1\" rx=\"5\"/>\n"
        "  <!-- Player head -->\n"
        "  <circle cx=\"" + std::to_string(size*0.5) + "\" cy=\"" + std::to_string(size*0.2) + "\" r=\"" + std::to_string(size*0.15) + "\" fill=\"#FFE4B5\"/>\n"
        "  <!-- Eyes -->\n"
        "  <circle cx=\"" + std::to_string(size*0.45) + "\" cy=\"" + std::to_string(size*0.18) + "\" r=\"2\" fill=\"black\"/>\n"
        "  <circle cx=\"" + std::to_string(size*0.55) + "\" cy=\"" + std::to_string(size*0.18) + "\" r=\"2\" fill=\"black\"/>\n"
        "  <!-- Arms -->\n"
        "  <rect x=\"" + std::to_string(size*0.1) + "\" y=\"" + std::to_string(size*0.4) + "\" width=\"" + std::to_string(size*0.15) + "\" height=\"" + std::to_string(size*0.3) + "\" fill=\"#FFE4B5\" rx=\"3\"/>\n"
        "  <rect x=\"" + std::to_string(size*0.75) + "\" y=\"" + std::to_string(size*0.4) + "\" width=\"" + std::to_string(size*0.15) + "\" height=\"" + std::to_string(size*0.3) + "\" fill=\"#FFE4B5\" rx=\"3\"/>\n"
        "  <!-- Legs -->\n"
        "  <rect x=\"" + std::to_string(size*0.3) + "\" y=\"" + std::to_string(size*0.75) + "\" width=\"" + std::to_string(size*0.15) + "\" height=\"" + std::to_string(size*0.2) + "\" fill=\"#4169E1\" rx=\"3\"/>\n"
        "  <rect x=\"" + std::to_string(size*0.55) + "\" y=\"" + std::to_string(size*0.75) + "\" width=\"" + std::to_string(size*0.15) + "\" height=\"" + std::to_string(size*0.2) + "\" fill=\"#4169E1\" rx=\"3\"/>\n"
        "</svg>";

    return TextureManager::GetInstance().CreateProceduralTexture("player_" + std::to_string(size), playerSVG);
}

std::shared_ptr<SVGTexture> CreateCoinTexture(double size) {
    std::string coinSVG =
        "<svg width=\"" + std::to_string(size) + "\" height=\"" + std::to_string(size) + "\" xmlns=\"http://www.w3.org/2000/svg\">\n"
        "  <circle cx=\"" + std::to_string(size*0.5) + "\" cy=\"" + std::to_string(size*0.5) + "\" r=\"" + std::to_string(size*0.4) + "\" fill=\"#FFD700\" stroke=\"#FFA500\" stroke-width=\"2\"/>\n"
        "  <circle cx=\"" + std::to_string(size*0.5) + "\" cy=\"" + std::to_string(size*0.5) + "\" r=\"" + std::to_string(size*0.25) + "\" fill=\"none\" stroke=\"#FFA500\" stroke-width=\"1\"/>\n"
        "  <text x=\"" + std::to_string(size*0.5) + "\" y=\"" + std::to_string(size*0.6) + "\" text-anchor=\"middle\" font-family=\"serif\" font-size=\"" + std::to_string(size*0.4) + "\" fill=\"#B8860B\">$</text>\n"
        "</svg>";

    return TextureManager::GetInstance().CreateProceduralTexture("coin_" + std::to_string(size), coinSVG);
}

std::shared_ptr<SVGTexture> CreateGemTexture(double size, const Color& gemColor) {
    std::string gemSVG =
        "<svg width=\"" + std::to_string(size) + "\" height=\"" + std::to_string(size) + "\" xmlns=\"http://www.w3.org/2000/svg\">\n"
        "  <polygon points=\"" + std::to_string(size*0.5) + "," + std::to_string(size*0.1) + " " +
                               std::to_string(size*0.8) + "," + std::to_string(size*0.4) + " " +
                               std::to_string(size*0.7) + "," + std::to_string(size*0.9) + " " +
                               std::to_string(size*0.3) + "," + std::to_string(size*0.9) + " " +
                               std::to_string(size*0.2) + "," + std::to_string(size*0.4) + "\" " +
        "fill=\"rgb(" + std::to_string((int)(gemColor.r * 255)) + "," +
                        std::to_string((int)(gemColor.g * 255)) + "," +
                        std::to_string((int)(gemColor.b * 255)) + ")\" " +
        "stroke=\"rgb(" + std::to_string((int)(gemColor.r * 200)) + "," +
                          std::to_string((int)(gemColor.g * 200)) + "," +
                          std::to_string((int)(gemColor.b * 200)) + ")\" stroke-width=\"1\"/>\n"
        "  <!-- Gem highlight -->\n"
        "  <polygon points=\"" + std::to_string(size*0.5) + "," + std::to_string(size*0.1) + " " +
                               std::to_string(size*0.6) + "," + std::to_string(size*0.3) + " " +
                               std::to_string(size*0.4) + "," + std::to_string(size*0.3) + "\" " +
        "fill=\"white\" opacity=\"0.6\"/>\n"
        "</svg>";

    std::string colorName = "blue";
    if (gemColor.r > 0.8) colorName = "red";
    else if (gemColor.g > 0.8) colorName = "green";
    else if (gemColor.r > 0.5 && gemColor.g > 0.5) colorName = "yellow";

    return TextureManager::GetInstance().CreateProceduralTexture("gem_" + colorName + "_" + std::to_string(size), gemSVG);
}

} // namespace EnvironmentAssets
