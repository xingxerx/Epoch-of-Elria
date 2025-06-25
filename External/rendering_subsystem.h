#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

// Forward declarations
class GameObject;
class GameObject3D;

namespace Rendering {

    // Render layer system
    enum class RenderLayer {
        Background = 0,
        Environment = 1,
        GameObjects = 2,
        Effects = 3,
        UI = 4,
        Debug = 5
    };

    // Renderable interface
    class IRenderable {
    public:
        virtual ~IRenderable() = default;
        virtual void Render(sf::RenderWindow& window) const = 0;
        virtual RenderLayer GetRenderLayer() const = 0;
        virtual int GetRenderOrder() const { return 0; } // Within layer ordering
    };

    // Render command for batching
    struct RenderCommand {
        RenderLayer layer;
        int order;
        std::function<void(sf::RenderWindow&)> renderFunc;
        
        RenderCommand(RenderLayer l, int o, std::function<void(sf::RenderWindow&)> func)
            : layer(l), order(o), renderFunc(func) {}
    };

    // Shader management
    class ShaderManager {
    private:
        std::unordered_map<std::string, sf::Shader> shaders;
        
    public:
        bool LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
        bool LoadShaderFromString(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
        sf::Shader* GetShader(const std::string& name);
        void Clear();
        
        // Built-in shaders
        void LoadBuiltInShaders();
    };

    // Texture atlas for efficient rendering
    class TextureAtlas {
    private:
        sf::Texture atlasTexture;
        std::unordered_map<std::string, sf::IntRect> textureRegions;
        sf::Vector2u atlasSize;
        
    public:
        TextureAtlas(unsigned int width, unsigned int height);
        bool AddTexture(const std::string& name, const sf::Image& image);
        sf::IntRect GetTextureRegion(const std::string& name) const;
        const sf::Texture& GetAtlasTexture() const { return atlasTexture; }
        void Clear();
        
    private:
        sf::Vector2u FindFreeSpace(const sf::Vector2u& size);
        std::vector<sf::IntRect> usedRegions;
    };

    // Sprite batch for efficient rendering
    class SpriteBatch {
    private:
        std::vector<sf::Vertex> vertices;
        const sf::Texture* currentTexture;
        sf::RenderStates renderStates;
        bool isDrawing;
        
    public:
        SpriteBatch();
        
        void Begin(const sf::Texture* texture = nullptr, const sf::RenderStates& states = sf::RenderStates::Default);
        void Draw(const sf::Vector2f& position, const sf::IntRect& textureRect, const sf::Color& color = sf::Color::White);
        void Draw(const sf::Vector2f& position, const sf::Vector2f& size, const sf::IntRect& textureRect, const sf::Color& color = sf::Color::White);
        void End(sf::RenderWindow& window);
        
        void Clear();
        size_t GetVertexCount() const { return vertices.size(); }
    };

    // Post-processing effects
    class PostProcessor {
    private:
        sf::RenderTexture renderTexture;
        sf::Sprite screenSprite;
        std::vector<sf::Shader*> effects;
        bool enabled;
        
    public:
        PostProcessor(unsigned int width, unsigned int height);
        
        void BeginCapture();
        void EndCapture();
        void AddEffect(sf::Shader* shader);
        void RemoveEffect(sf::Shader* shader);
        void ClearEffects();
        void Render(sf::RenderWindow& window);
        
        void SetEnabled(bool enable) { enabled = enable; }
        bool IsEnabled() const { return enabled; }
        
        sf::RenderTexture& GetRenderTexture() { return renderTexture; }
    };

    // Main rendering subsystem
    class RenderingSubsystem {
    private:
        sf::RenderWindow* window;
        ShaderManager shaderManager;
        std::unique_ptr<TextureAtlas> textureAtlas;
        std::unique_ptr<SpriteBatch> spriteBatch;
        std::unique_ptr<PostProcessor> postProcessor;
        
        // Render commands
        std::vector<RenderCommand> renderCommands;
        
        // Statistics
        struct RenderStats {
            int drawCalls;
            int verticesRendered;
            int texturesUsed;
            double frameTime;
            
            void Reset() {
                drawCalls = 0;
                verticesRendered = 0;
                texturesUsed = 0;
                frameTime = 0;
            }
        } stats;
        
        // Settings
        bool vsyncEnabled;
        bool debugMode;
        sf::Color clearColor;
        
    public:
        RenderingSubsystem();
        ~RenderingSubsystem();
        
        // Initialization
        bool Initialize(sf::RenderWindow* renderWindow);
        void Shutdown();
        
        // Frame management
        void BeginFrame();
        void EndFrame();
        void Present();
        
        // Command submission
        void SubmitRenderCommand(RenderLayer layer, int order, std::function<void(sf::RenderWindow&)> renderFunc);
        void SubmitRenderable(const IRenderable& renderable);
        
        // Direct rendering (for immediate mode)
        void RenderGameObject(const GameObject& obj);
        void RenderGameObject3D(const GameObject3D& obj);
        void RenderSprite(const sf::Sprite& sprite, RenderLayer layer = RenderLayer::GameObjects);
        void RenderText(const sf::Text& text, RenderLayer layer = RenderLayer::UI);
        void RenderShape(const sf::Shape& shape, RenderLayer layer = RenderLayer::GameObjects);
        
        // Batch rendering
        SpriteBatch& GetSpriteBatch() { return *spriteBatch; }
        
        // Resource management
        ShaderManager& GetShaderManager() { return shaderManager; }
        TextureAtlas& GetTextureAtlas() { return *textureAtlas; }
        
        // Post-processing
        PostProcessor& GetPostProcessor() { return *postProcessor; }
        
        // Settings
        void SetVSync(bool enabled);
        void SetDebugMode(bool enabled) { debugMode = enabled; }
        void SetClearColor(const sf::Color& color) { clearColor = color; }
        
        bool IsVSyncEnabled() const { return vsyncEnabled; }
        bool IsDebugMode() const { return debugMode; }
        sf::Color GetClearColor() const { return clearColor; }
        
        // Statistics
        const RenderStats& GetStats() const { return stats; }
        void ResetStats() { stats.Reset(); }
        
        // Utility
        sf::Vector2f WorldToScreen(const sf::Vector2f& worldPos) const;
        sf::Vector2f ScreenToWorld(const sf::Vector2f& screenPos) const;
        
    private:
        void ExecuteRenderCommands();
        void SortRenderCommands();
        void RenderDebugInfo();
        void UpdateStats();
    };

    // Global access
    extern std::unique_ptr<RenderingSubsystem> g_RenderingSubsystem;
    
    // Convenience functions
    void Initialize(sf::RenderWindow* window);
    void Shutdown();
    RenderingSubsystem& GetRenderer();
    
    // Built-in shader sources
    namespace Shaders {
        extern const char* DefaultVertexShader;
        extern const char* DefaultFragmentShader;
        extern const char* BlurFragmentShader;
        extern const char* BloomFragmentShader;
        extern const char* ColorGradingFragmentShader;
    }
}