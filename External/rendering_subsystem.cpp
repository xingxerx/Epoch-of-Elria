#include "rendering_subsystem.h"
#include "GameObject.h"
#include "GameObject3D.h"
#include <algorithm>
#include <chrono>
#include <iostream>

namespace Rendering {

// Global rendering subsystem instance
std::unique_ptr<RenderingSubsystem> g_RenderingSubsystem = nullptr;

// ShaderManager implementation
bool ShaderManager::LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) {
    sf::Shader shader;
    if (shader.loadFromFile(vertexPath, fragmentPath)) {
        shaders[name] = std::move(shader);
        return true;
    }
    return false;
}

bool ShaderManager::LoadShaderFromString(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) {
    sf::Shader shader;
    if (shader.loadFromMemory(vertexSource, fragmentSource)) {
        shaders[name] = std::move(shader);
        return true;
    }
    return false;
}

sf::Shader* ShaderManager::GetShader(const std::string& name) {
    auto it = shaders.find(name);
    return (it != shaders.end()) ? &it->second : nullptr;
}

void ShaderManager::Clear() {
    shaders.clear();
}

void ShaderManager::LoadBuiltInShaders() {
    LoadShaderFromString("default", Shaders::DefaultVertexShader, Shaders::DefaultFragmentShader);
    LoadShaderFromString("blur", Shaders::DefaultVertexShader, Shaders::BlurFragmentShader);
    LoadShaderFromString("bloom", Shaders::DefaultVertexShader, Shaders::BloomFragmentShader);
    LoadShaderFromString("color_grading", Shaders::DefaultVertexShader, Shaders::ColorGradingFragmentShader);
}

// TextureAtlas implementation
TextureAtlas::TextureAtlas(unsigned int width, unsigned int height) : atlasSize(width, height) {
    if (!atlasTexture.create(width, height)) {
        std::cerr << "Failed to create texture atlas" << std::endl;
    }
}

bool TextureAtlas::AddTexture(const std::string& name, const sf::Image& image) {
    sf::Vector2u imageSize = image.getSize();
    sf::Vector2u position = FindFreeSpace(imageSize);
    
    if (position.x + imageSize.x > atlasSize.x || position.y + imageSize.y > atlasSize.y) {
        return false; // Not enough space
    }
    
    atlasTexture.update(image, position.x, position.y);
    textureRegions[name] = sf::IntRect(position.x, position.y, imageSize.x, imageSize.y);
    usedRegions.emplace_back(position.x, position.y, imageSize.x, imageSize.y);
    
    return true;
}

sf::IntRect TextureAtlas::GetTextureRegion(const std::string& name) const {
    auto it = textureRegions.find(name);
    return (it != textureRegions.end()) ? it->second : sf::IntRect();
}

void TextureAtlas::Clear() {
    textureRegions.clear();
    usedRegions.clear();
    atlasTexture.create(atlasSize.x, atlasSize.y);
}

sf::Vector2u TextureAtlas::FindFreeSpace(const sf::Vector2u& size) {
    // Simple algorithm - can be improved with better packing
    for (unsigned int y = 0; y <= atlasSize.y - size.y; ++y) {
        for (unsigned int x = 0; x <= atlasSize.x - size.x; ++x) {
            sf::IntRect candidate(x, y, size.x, size.y);
            bool overlaps = false;
            
            for (const auto& used : usedRegions) {
                if (candidate.intersects(used)) {
                    overlaps = true;
                    break;
                }
            }
            
            if (!overlaps) {
                return sf::Vector2u(x, y);
            }
        }
    }
    
    return sf::Vector2u(atlasSize.x, atlasSize.y); // No space found
}

// SpriteBatch implementation
SpriteBatch::SpriteBatch() : currentTexture(nullptr), isDrawing(false) {
    vertices.reserve(1000); // Pre-allocate for performance
}

void SpriteBatch::Begin(const sf::Texture* texture, const sf::RenderStates& states) {
    if (isDrawing) {
        std::cerr << "SpriteBatch::Begin called while already drawing" << std::endl;
        return;
    }
    
    currentTexture = texture;
    renderStates = states;
    renderStates.texture = texture;
    vertices.clear();
    isDrawing = true;
}

void SpriteBatch::Draw(const sf::Vector2f& position, const sf::IntRect& textureRect, const sf::Color& color) {
    Draw(position, sf::Vector2f(textureRect.width, textureRect.height), textureRect, color);
}

void SpriteBatch::Draw(const sf::Vector2f& position, const sf::Vector2f& size, const sf::IntRect& textureRect, const sf::Color& color) {
    if (!isDrawing) {
        std::cerr << "SpriteBatch::Draw called without Begin" << std::endl;
        return;
    }
    
    // Create quad vertices
    sf::Vector2f texCoords[4] = {
        sf::Vector2f(textureRect.left, textureRect.top),
        sf::Vector2f(textureRect.left + textureRect.width, textureRect.top),
        sf::Vector2f(textureRect.left + textureRect.width, textureRect.top + textureRect.height),
        sf::Vector2f(textureRect.left, textureRect.top + textureRect.height)
    };
    
    sf::Vector2f positions[4] = {
        position,
        sf::Vector2f(position.x + size.x, position.y),
        sf::Vector2f(position.x + size.x, position.y + size.y),
        sf::Vector2f(position.x, position.y + size.y)
    };
    
    // Add two triangles (quad)
    vertices.emplace_back(positions[0], color, texCoords[0]);
    vertices.emplace_back(positions[1], color, texCoords[1]);
    vertices.emplace_back(positions[2], color, texCoords[2]);
    
    vertices.emplace_back(positions[0], color, texCoords[0]);
    vertices.emplace_back(positions[2], color, texCoords[2]);
    vertices.emplace_back(positions[3], color, texCoords[3]);
}

void SpriteBatch::End(sf::RenderWindow& window) {
    if (!isDrawing) {
        std::cerr << "SpriteBatch::End called without Begin" << std::endl;
        return;
    }
    
    if (!vertices.empty()) {
        window.draw(&vertices[0], vertices.size(), sf::Triangles, renderStates);
    }
    
    isDrawing = false;
}

void SpriteBatch::Clear() {
    vertices.clear();
}

// PostProcessor implementation
PostProcessor::PostProcessor(unsigned int width, unsigned int height) : enabled(true) {
    if (!renderTexture.create(width, height)) {
        std::cerr << "Failed to create post-processing render texture" << std::endl;
    }
    
    screenSprite.setTexture(renderTexture.getTexture());
}

void PostProcessor::BeginCapture() {
    if (enabled) {
        renderTexture.clear();
    }
}

void PostProcessor::EndCapture() {
    if (enabled) {
        renderTexture.display();
    }
}

void PostProcessor::AddEffect(sf::Shader* shader) {
    if (shader && std::find(effects.begin(), effects.end(), shader) == effects.end()) {
        effects.push_back(shader);
    }
}

void PostProcessor::RemoveEffect(sf::Shader* shader) {
    effects.erase(std::remove(effects.begin(), effects.end(), shader), effects.end());
}

void PostProcessor::ClearEffects() {
    effects.clear();
}

void PostProcessor::Render(sf::RenderWindow& window) {
    if (!enabled) return;
    
    if (effects.empty()) {
        // No effects, just draw the texture directly
        window.draw(screenSprite);
    } else {
        // Apply effects in sequence
        sf::RenderStates states;
        for (sf::Shader* effect : effects) {
            states.shader = effect;
            window.draw(screenSprite, states);
        }
    }
}

// RenderingSubsystem implementation
RenderingSubsystem::RenderingSubsystem() 
    : window(nullptr), vsyncEnabled(true), debugMode(false), clearColor(sf::Color::Black) {
    stats.Reset();
}

RenderingSubsystem::~RenderingSubsystem() {
    Shutdown();
}

bool RenderingSubsystem::Initialize(sf::RenderWindow* renderWindow) {
    if (!renderWindow) {
        std::cerr << "Invalid render window provided to RenderingSubsystem" << std::endl;
        return false;
    }
    
    window = renderWindow;
    
    // Initialize subsystems
    textureAtlas = std::make_unique<TextureAtlas>(2048, 2048);
    spriteBatch = std::make_unique<SpriteBatch>();
    
    sf::Vector2u windowSize = window->getSize();
    postProcessor = std::make_unique<PostProcessor>(windowSize.x, windowSize.y);
    
    // Load built-in shaders
    shaderManager.LoadBuiltInShaders();
    
    // Set initial settings
    window->setVerticalSyncEnabled(vsyncEnabled);
    
    std::cout << "RenderingSubsystem initialized successfully" << std::endl;
    return true;
}

void RenderingSubsystem::Shutdown() {
    renderCommands.clear();
    postProcessor.reset();
    spriteBatch.reset();
    textureAtlas.reset();
    shaderManager.Clear();
    window = nullptr;
    
    std::cout << "RenderingSubsystem shutdown" << std::endl;
}

void RenderingSubsystem::BeginFrame() {
    if (!window) return;
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    stats.Reset();
    renderCommands.clear();
    
    if (postProcessor->IsEnabled()) {
        postProcessor->BeginCapture();
        window = &postProcessor->GetRenderTexture();
    }
    
    window->clear(clearColor);
    
    auto endTime = std::chrono::high_resolution_clock::now();
    stats.frameTime = std::chrono::duration<double>(endTime - startTime).count();
}

void RenderingSubsystem::EndFrame() {
    if (!window) return;
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Execute all render commands
    ExecuteRenderCommands();
    
    if (debugMode) {
        RenderDebugInfo();
    }
    
    if (postProcessor->IsEnabled()) {
        postProcessor->EndCapture();
        // Reset window to actual render window
        window = window; // This needs proper handling in real implementation
        postProcessor->Render(*window);
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    stats.frameTime += std::chrono::duration<double>(endTime - startTime).count();
    
    UpdateStats();
}

void RenderingSubsystem::Present() {
    if (window) {
        window->display();
    }
}

void RenderingSubsystem::SubmitRenderCommand(RenderLayer layer, int order, std::function<void(sf::RenderWindow&)> renderFunc) {
    renderCommands.emplace_back(layer, order, renderFunc);
}

void RenderingSubsystem::SubmitRenderable(const IRenderable& renderable) {
    SubmitRenderCommand(renderable.GetRenderLayer(), renderable.GetRenderOrder(),
                       [&renderable](sf::RenderWindow& window) {
                           renderable.Render(window);
                       });
}

void RenderingSubsystem::RenderGameObject(const GameObject& obj) {
    SubmitRenderCommand(RenderLayer::GameObjects, 0,
                       [&obj](sf::RenderWindow& window) {
                           obj.Draw(window);
                       });
}

void RenderingSubsystem::RenderGameObject3D(const GameObject3D& obj) {
    SubmitRenderCommand(RenderLayer::GameObjects, 0,
                       [&obj](sf::RenderWindow& window) {
                           obj.Draw(window);
                       });
}

void RenderingSubsystem::RenderSprite(const sf::Sprite& sprite, RenderLayer layer) {
    SubmitRenderCommand(layer, 0,
                       [&sprite](sf::RenderWindow& window) {
                           window.draw(sprite);
                       });
}

void RenderingSubsystem::RenderText(const sf::Text& text, RenderLayer layer) {
    SubmitRenderCommand(layer, 0,
                       [&text](sf::RenderWindow& window) {
                           window.draw(text);
                       });
}

void RenderingSubsystem::RenderShape(const sf::Shape& shape, RenderLayer layer) {
    SubmitRenderCommand(layer, 0,
                       [&shape](sf::RenderWindow& window) {
                           window.draw(shape);
                       });
}

void RenderingSubsystem::SetVSync(bool enabled) {
    vsyncEnabled = enabled;
    if (window) {
        window->setVerticalSyncEnabled(enabled);
    }
}

sf::Vector2f RenderingSubsystem::WorldToScreen(const sf::Vector2f& worldPos) const {
    if (!window) return worldPos;
    return window->mapCoordsToPixel(worldPos);
}

sf::Vector2f RenderingSubsystem::ScreenToWorld(const sf::Vector2f& screenPos) const {
    if (!window) return screenPos;
    return window->mapPixelToCoords(sf::Vector2i(screenPos.x, screenPos.y));
}

void RenderingSubsystem::ExecuteRenderCommands() {
    SortRenderCommands();
    
    for (const auto& command : renderCommands) {
        command.renderFunc(*window);
        stats.drawCalls++;
    }
}

void RenderingSubsystem::SortRenderCommands() {
    std::sort(renderCommands.begin(), renderCommands.end(),
              [](const RenderCommand& a, const RenderCommand& b) {
                  if (a.layer != b.layer) {
                      return static_cast<int>(a.layer) < static_cast<int>(b.layer);
                  }
                  return a.order < b.order;
              });
}

void RenderingSubsystem::RenderDebugInfo() {
    // Render debug information
    sf::Text debugText;
    debugText.setCharacterSize(16);
    debugText.setFillColor(sf::Color::Yellow);
    debugText.setPosition(10, window->getSize().y - 100);
    
    std::string debugInfo = "Draw Calls: " + std::to_string(stats.drawCalls) + "\n";
    debugInfo += "Frame Time: " + std::to_string(stats.frameTime * 1000) + "ms\n";
    debugInfo += "Render Commands: " + std::to_string(renderCommands.size());
    
    debugText.setString(debugInfo);
    window->draw(debugText);
}

void RenderingSubsystem::UpdateStats() {
    // Update rendering statistics
    // This would typically include more detailed metrics
}

// Global functions
void Initialize(sf::RenderWindow* window) {
    g_RenderingSubsystem = std::make_unique<RenderingSubsystem>();
    g_RenderingSubsystem->Initialize(window);
}

void Shutdown() {
    g_RenderingSubsystem.reset();
}

RenderingSubsystem& GetRenderer() {
    if (!g_RenderingSubsystem) {
        throw std::runtime_error("RenderingSubsystem not initialized");
    }
    return *g_RenderingSubsystem;
}

// Built-in shader sources
namespace Shaders {
    const char* DefaultVertexShader = R"(
        #version 120
        void main() {
            gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
            gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
            gl_FrontColor = gl_Color;
        }
    )";
    
    const char* DefaultFragmentShader = R"(
        #version 120
        uniform sampler2D texture;
        void main() {
            gl_FragColor = gl_Color * texture2D(texture, gl_TexCoord[0].xy);
        }
    )";
    
    const char* BlurFragmentShader = R"(
        #version 120
        uniform sampler2D texture;
        uniform float blur_radius;
        void main() {
            vec2 texCoord = gl_TexCoord[0].xy;
            vec4 color = vec4(0.0);
            float total = 0.0;
            
            for (float x = -blur_radius; x <= blur_radius; x += 1.0) {
                for (float y = -blur_radius; y <= blur_radius; y += 1.0) {
                    vec2 offset = vec2(x, y) / textureSize(texture, 0);
                    color += texture2D(texture, texCoord + offset);
                    total += 1.0;
                }
            }
            
            gl_FragColor = color / total;
        }
    )";
    
    const char* BloomFragmentShader = R"(
        #version 120
        uniform sampler2D texture;
        uniform float threshold;
        uniform float intensity;
        void main() {
            vec4 color = texture2D(texture, gl_TexCoord[0].xy);
            float brightness = dot(color.rgb, vec3(0.299, 0.587, 0.114));
            
            if (brightness > threshold) {
                gl_FragColor = color * intensity;
            } else {
                gl_FragColor = color;
            }
        }
    )";
    
    const char* ColorGradingFragmentShader = R"(
        #version 120
        uniform sampler2D texture;
        uniform float contrast;
        uniform float brightness;
        uniform float saturation;
        void main() {
            vec4 color = texture2D(texture, gl_TexCoord[0].xy);
            
            // Apply brightness
            color.rgb += brightness;
            
            // Apply contrast
            color.rgb = (color.rgb - 0.5) * contrast + 0.5;
            
            // Apply saturation
            float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
            color.rgb = mix(vec3(gray), color.rgb, saturation);
            
            gl_FragColor = color;
        }
    )";
}

} // namespace Rendering