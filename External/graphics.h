#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>

// Graphics utility functions and classes
namespace Graphics {
    
    // Color utilities
    sf::Color HSVtoRGB(float h, float s, float v);
    sf::Color LerpColor(const sf::Color& a, const sf::Color& b, float t);
    
    // Shape utilities
    void DrawCircleOutline(sf::RenderWindow& window, const sf::Vector2f& center, float radius, 
                          const sf::Color& color, float thickness = 1.0f);
    void DrawLine(sf::RenderWindow& window, const sf::Vector2f& start, const sf::Vector2f& end, 
                  const sf::Color& color, float thickness = 1.0f);
    void DrawArrow(sf::RenderWindow& window, const sf::Vector2f& start, const sf::Vector2f& end, 
                   const sf::Color& color, float thickness = 1.0f);
    
    // Text utilities
    class TextRenderer {
    private:
        sf::Font font;
        bool fontLoaded;
        
    public:
        TextRenderer();
        bool LoadFont(const std::string& fontPath);
        void DrawText(sf::RenderWindow& window, const std::string& text, 
                     const sf::Vector2f& position, unsigned int size = 24, 
                     const sf::Color& color = sf::Color::White);
        void DrawCenteredText(sf::RenderWindow& window, const std::string& text, 
                             const sf::Vector2f& center, unsigned int size = 24, 
                             const sf::Color& color = sf::Color::White);
    };
    
    // Particle system
    struct Particle {
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Color color;
        float life;
        float maxLife;
        float size;
        
        Particle(const sf::Vector2f& pos, const sf::Vector2f& vel, 
                const sf::Color& col, float lifetime, float particleSize = 2.0f);
        void Update(float deltaTime);
        void Draw(sf::RenderWindow& window) const;
        bool IsAlive() const { return life > 0; }
    };
    
    class ParticleSystem {
    private:
        std::vector<Particle> particles;
        sf::Vector2f emissionPoint;
        
    public:
        ParticleSystem(const sf::Vector2f& emissionPoint);
        void SetEmissionPoint(const sf::Vector2f& point) { emissionPoint = point; }
        void Emit(int count, const sf::Vector2f& velocity, const sf::Color& color, 
                 float lifetime, float spread = 0.5f);
        void Update(float deltaTime);
        void Draw(sf::RenderWindow& window);
        void Clear() { particles.clear(); }
        size_t GetParticleCount() const { return particles.size(); }
    };
    
    // Animation utilities
    class Animator {
    public:
        static float EaseInOut(float t);
        static float EaseIn(float t);
        static float EaseOut(float t);
        static float Bounce(float t);
        static float Elastic(float t);
    };
    
    // Camera system
    class Camera {
    private:
        sf::View view;
        sf::Vector2f target;
        sf::Vector2f position;
        float zoom;
        float followSpeed;
        
    public:
        Camera(float width, float height);
        void SetTarget(const sf::Vector2f& target);
        void SetPosition(const sf::Vector2f& position);
        void SetZoom(float zoom);
        void SetFollowSpeed(float speed) { followSpeed = speed; }
        void Update(float deltaTime);
        void Apply(sf::RenderWindow& window);
        
        sf::Vector2f GetPosition() const { return position; }
        float GetZoom() const { return zoom; }
        sf::Vector2f ScreenToWorld(const sf::Vector2i& screenPos, const sf::RenderWindow& window) const;
        sf::Vector2i WorldToScreen(const sf::Vector2f& worldPos, const sf::RenderWindow& window) const;
    };
    
    // Sprite management
    class SpriteManager {
    private:
        std::vector<sf::Texture> textures;
        std::vector<std::string> textureNames;
        
    public:
        bool LoadTexture(const std::string& name, const std::string& filepath);
        sf::Texture* GetTexture(const std::string& name);
        void Clear();
        size_t GetTextureCount() const { return textures.size(); }
    };
    
    // Global graphics manager
    class GraphicsManager {
    private:
        static std::unique_ptr<GraphicsManager> instance;
        TextRenderer textRenderer;
        SpriteManager spriteManager;
        
    public:
        static GraphicsManager& GetInstance();
        TextRenderer& GetTextRenderer() { return textRenderer; }
        SpriteManager& GetSpriteManager() { return spriteManager; }
        
        void Initialize();
        void Shutdown();
    };
}