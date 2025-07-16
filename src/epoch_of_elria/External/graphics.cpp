#include "graphics.h"
#include <cmath>
#include <algorithm>
#include <iostream>

namespace Graphics {

// Color utilities
sf::Color HSVtoRGB(float h, float s, float v) {
    float c = v * s;
    float x = c * (1 - std::abs(std::fmod(h / 60.0f, 2) - 1));
    float m = v - c;
    
    float r, g, b;
    if (h >= 0 && h < 60) {
        r = c; g = x; b = 0;
    } else if (h >= 60 && h < 120) {
        r = x; g = c; b = 0;
    } else if (h >= 120 && h < 180) {
        r = 0; g = c; b = x;
    } else if (h >= 180 && h < 240) {
        r = 0; g = x; b = c;
    } else if (h >= 240 && h < 300) {
        r = x; g = 0; b = c;
    } else {
        r = c; g = 0; b = x;
    }
    
    return sf::Color(
        static_cast<sf::Uint8>((r + m) * 255),
        static_cast<sf::Uint8>((g + m) * 255),
        static_cast<sf::Uint8>((b + m) * 255)
    );
}

sf::Color LerpColor(const sf::Color& a, const sf::Color& b, float t) {
    t = std::max(0.0f, std::min(1.0f, t));
    return sf::Color(
        static_cast<sf::Uint8>(a.r + (b.r - a.r) * t),
        static_cast<sf::Uint8>(a.g + (b.g - a.g) * t),
        static_cast<sf::Uint8>(a.b + (b.b - a.b) * t),
        static_cast<sf::Uint8>(a.a + (b.a - a.a) * t)
    );
}

// Shape utilities
void DrawCircleOutline(sf::RenderWindow& window, const sf::Vector2f& center, float radius, 
                      const sf::Color& color, float thickness) {
    sf::CircleShape circle(radius);
    circle.setPosition(center.x - radius, center.y - radius);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(color);
    circle.setOutlineThickness(thickness);
    window.draw(circle);
}

void DrawLine(sf::RenderWindow& window, const sf::Vector2f& start, const sf::Vector2f& end, 
              const sf::Color& color, float thickness) {
    sf::Vector2f direction = end - start;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    sf::RectangleShape line(sf::Vector2f(length, thickness));
    line.setPosition(start);
    line.setFillColor(color);
    
    float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159f;
    line.setRotation(angle);
    
    window.draw(line);
}

void DrawArrow(sf::RenderWindow& window, const sf::Vector2f& start, const sf::Vector2f& end, 
               const sf::Color& color, float thickness) {
    // Draw line
    DrawLine(window, start, end, color, thickness);
    
    // Draw arrowhead
    sf::Vector2f direction = end - start;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction /= length;
        
        float arrowSize = thickness * 3;
        sf::Vector2f perpendicular(-direction.y, direction.x);
        
        sf::Vector2f arrowPoint1 = end - direction * arrowSize + perpendicular * arrowSize * 0.5f;
        sf::Vector2f arrowPoint2 = end - direction * arrowSize - perpendicular * arrowSize * 0.5f;
        
        sf::Vertex triangle[] = {
            sf::Vertex(end, color),
            sf::Vertex(arrowPoint1, color),
            sf::Vertex(arrowPoint2, color)
        };
        
        window.draw(triangle, 3, sf::Triangles);
    }
}

// TextRenderer implementation
TextRenderer::TextRenderer() : fontLoaded(false) {}

bool TextRenderer::LoadFont(const std::string& fontPath) {
    fontLoaded = font.loadFromFile(fontPath);
    if (!fontLoaded) {
        std::cout << "Warning: Could not load font from " << fontPath << std::endl;
    }
    return fontLoaded;
}

void TextRenderer::DrawText(sf::RenderWindow& window, const std::string& text, 
                           const sf::Vector2f& position, unsigned int size, 
                           const sf::Color& color) {
    sf::Text textObj;
    if (fontLoaded) {
        textObj.setFont(font);
    }
    textObj.setString(text);
    textObj.setCharacterSize(size);
    textObj.setFillColor(color);
    textObj.setPosition(position);
    window.draw(textObj);
}

void TextRenderer::DrawCenteredText(sf::RenderWindow& window, const std::string& text, 
                                   const sf::Vector2f& center, unsigned int size, 
                                   const sf::Color& color) {
    sf::Text textObj;
    if (fontLoaded) {
        textObj.setFont(font);
    }
    textObj.setString(text);
    textObj.setCharacterSize(size);
    textObj.setFillColor(color);
    
    sf::FloatRect bounds = textObj.getLocalBounds();
    textObj.setPosition(center.x - bounds.width / 2, center.y - bounds.height / 2);
    window.draw(textObj);
}

// Particle implementation
Particle::Particle(const sf::Vector2f& pos, const sf::Vector2f& vel, 
                  const sf::Color& col, float lifetime, float particleSize)
    : position(pos), velocity(vel), color(col), life(lifetime), maxLife(lifetime), size(particleSize) {}

void Particle::Update(float deltaTime) {
    position += velocity * deltaTime;
    life -= deltaTime;
    
    // Fade out over time
    float alpha = life / maxLife;
    color.a = static_cast<sf::Uint8>(255 * alpha);
}

void Particle::Draw(sf::RenderWindow& window) const {
    sf::CircleShape shape(size);
    shape.setPosition(position.x - size, position.y - size);
    shape.setFillColor(color);
    window.draw(shape);
}

// ParticleSystem implementation
ParticleSystem::ParticleSystem(const sf::Vector2f& emissionPoint) : emissionPoint(emissionPoint) {}

void ParticleSystem::Emit(int count, const sf::Vector2f& velocity, const sf::Color& color, 
                         float lifetime, float spread) {
    for (int i = 0; i < count; ++i) {
        sf::Vector2f particleVel = velocity;
        
        // Add random spread
        float angle = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * spread * 2.0f * 3.14159f;
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);
        
        sf::Vector2f spreadVel(
            particleVel.x * cosA - particleVel.y * sinA,
            particleVel.x * sinA + particleVel.y * cosA
        );
        
        particles.emplace_back(emissionPoint, spreadVel, color, lifetime);
    }
}

void ParticleSystem::Update(float deltaTime) {
    // Update all particles
    for (auto& particle : particles) {
        particle.Update(deltaTime);
    }
    
    // Remove dead particles
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                      [](const Particle& p) { return !p.IsAlive(); }),
        particles.end()
    );
}

void ParticleSystem::Draw(sf::RenderWindow& window) {
    for (const auto& particle : particles) {
        particle.Draw(window);
    }
}

// Animator implementation
float Animator::EaseInOut(float t) {
    return t * t * (3.0f - 2.0f * t);
}

float Animator::EaseIn(float t) {
    return t * t;
}

float Animator::EaseOut(float t) {
    return 1.0f - (1.0f - t) * (1.0f - t);
}

float Animator::Bounce(float t) {
    if (t < 1.0f / 2.75f) {
        return 7.5625f * t * t;
    } else if (t < 2.0f / 2.75f) {
        t -= 1.5f / 2.75f;
        return 7.5625f * t * t + 0.75f;
    } else if (t < 2.5f / 2.75f) {
        t -= 2.25f / 2.75f;
        return 7.5625f * t * t + 0.9375f;
    } else {
        t -= 2.625f / 2.75f;
        return 7.5625f * t * t + 0.984375f;
    }
}

float Animator::Elastic(float t) {
    if (t == 0) return 0;
    if (t == 1) return 1;
    
    float p = 0.3f;
    float s = p / 4.0f;
    return std::pow(2, -10 * t) * std::sin((t - s) * (2 * 3.14159f) / p) + 1;
}

// Camera implementation
Camera::Camera(float width, float height) : zoom(1.0f), followSpeed(5.0f) {
    view.setSize(width, height);
    view.setCenter(width / 2, height / 2);
    position = sf::Vector2f(width / 2, height / 2);
    target = position;
}

void Camera::SetTarget(const sf::Vector2f& newTarget) {
    target = newTarget;
}

void Camera::SetPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
    view.setCenter(position);
}

void Camera::SetZoom(float newZoom) {
    zoom = std::max(0.1f, newZoom);
    sf::Vector2f size = view.getSize();
    view.setSize(size.x / zoom, size.y / zoom);
}

void Camera::Update(float deltaTime) {
    // Smoothly move towards target
    sf::Vector2f direction = target - position;
    position += direction * followSpeed * deltaTime;
    view.setCenter(position);
}

void Camera::Apply(sf::RenderWindow& window) {
    window.setView(view);
}

sf::Vector2f Camera::ScreenToWorld(const sf::Vector2i& screenPos, const sf::RenderWindow& window) const {
    return window.mapPixelToCoords(screenPos, view);
}

sf::Vector2i Camera::WorldToScreen(const sf::Vector2f& worldPos, const sf::RenderWindow& window) const {
    return window.mapCoordsToPixel(worldPos, view);
}

// SpriteManager implementation
bool SpriteManager::LoadTexture(const std::string& name, const std::string& filepath) {
    sf::Texture texture;
    if (texture.loadFromFile(filepath)) {
        textures.push_back(std::move(texture));
        textureNames.push_back(name);
        return true;
    }
    return false;
}

sf::Texture* SpriteManager::GetTexture(const std::string& name) {
    for (size_t i = 0; i < textureNames.size(); ++i) {
        if (textureNames[i] == name) {
            return &textures[i];
        }
    }
    return nullptr;
}

void SpriteManager::Clear() {
    textures.clear();
    textureNames.clear();
}

// GraphicsManager implementation
std::unique_ptr<GraphicsManager> GraphicsManager::instance = nullptr;

GraphicsManager& GraphicsManager::GetInstance() {
    if (!instance) {
        instance = std::make_unique<GraphicsManager>();
    }
    return *instance;
}

void GraphicsManager::Initialize() {
    textRenderer.LoadFont("arial.ttf");
    std::cout << "Graphics Manager initialized" << std::endl;
}

void GraphicsManager::Shutdown() {
    spriteManager.Clear();
    std::cout << "Graphics Manager shutdown" << std::endl;
}

} // namespace Graphics