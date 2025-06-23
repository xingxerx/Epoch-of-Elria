#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <fstream>
#include <sstream>
#include "Vector2D.h"

// Forward declarations
class GameObject;

// --- Color Class ---
struct Color {
    float r, g, b, a; // RGBA values (0.0 to 1.0)
    
    Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
    Color(float red, float green, float blue, float alpha = 1.0f) 
        : r(red), g(green), b(blue), a(alpha) {}
    
    // Predefined colors
    static const Color WHITE;
    static const Color BLACK;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color YELLOW;
    static const Color PURPLE;
    static const Color ORANGE;
};

// --- SVG Texture Class ---
class SVGTexture {
private:
    std::string svgContent;
    std::string filename;
    Vector2D size;
    bool loaded;

public:
    SVGTexture();
    SVGTexture(const std::string& filepath);
    
    bool LoadFromFile(const std::string& filepath);
    bool LoadFromString(const std::string& svgData);
    
    const std::string& GetSVGContent() const { return svgContent; }
    const Vector2D& GetSize() const { return size; }
    bool IsLoaded() const { return loaded; }
    
    // Simple SVG generation methods
    static std::string CreateRectangle(double width, double height, const Color& fillColor, const Color& strokeColor = Color::BLACK, double strokeWidth = 1.0);
    static std::string CreateCircle(double radius, const Color& fillColor, const Color& strokeColor = Color::BLACK, double strokeWidth = 1.0);
    static std::string CreatePolygon(const std::vector<Vector2D>& points, const Color& fillColor, const Color& strokeColor = Color::BLACK, double strokeWidth = 1.0);
};

// --- Texture Manager ---
class TextureManager {
private:
    std::map<std::string, std::shared_ptr<SVGTexture>> textures;
    static TextureManager* instance;

public:
    static TextureManager& GetInstance();
    
    std::shared_ptr<SVGTexture> LoadTexture(const std::string& name, const std::string& filepath);
    std::shared_ptr<SVGTexture> GetTexture(const std::string& name);
    std::shared_ptr<SVGTexture> CreateProceduralTexture(const std::string& name, const std::string& svgContent);
    
    void UnloadTexture(const std::string& name);
    void UnloadAllTextures();
};

// --- Renderer Interface ---
class Renderer {
public:
    virtual ~Renderer() = default;
    
    // Basic drawing operations
    virtual void Clear(const Color& clearColor = Color::BLACK) = 0;
    virtual void DrawRectangle(const Vector2D& position, const Vector2D& size, const Color& color) = 0;
    virtual void DrawCircle(const Vector2D& position, double radius, const Color& color) = 0;
    virtual void DrawTexture(const SVGTexture& texture, const Vector2D& position, const Vector2D& scale = Vector2D(1.0, 1.0)) = 0;
    virtual void DrawLine(const Vector2D& start, const Vector2D& end, const Color& color, double thickness = 1.0) = 0;
    
    // Text rendering
    virtual void DrawText(const std::string& text, const Vector2D& position, const Color& color, double fontSize = 16.0) = 0;
    
    // Frame management
    virtual void Present() = 0;
    virtual void SetViewport(int width, int height) = 0;
};

// --- Console Renderer (for debugging/testing) ---
class ConsoleRenderer : public Renderer {
private:
    int viewportWidth;
    int viewportHeight;
    std::vector<std::vector<char>> frameBuffer;
    
public:
    ConsoleRenderer(int width = 80, int height = 24);
    
    void Clear(const Color& clearColor = Color::BLACK) override;
    void DrawRectangle(const Vector2D& position, const Vector2D& size, const Color& color) override;
    void DrawCircle(const Vector2D& position, double radius, const Color& color) override;
    void DrawTexture(const SVGTexture& texture, const Vector2D& position, const Vector2D& scale = Vector2D(1.0, 1.0)) override;
    void DrawLine(const Vector2D& start, const Vector2D& end, const Color& color, double thickness = 1.0) override;
    void DrawText(const std::string& text, const Vector2D& position, const Color& color, double fontSize = 16.0) override;
    
    void Present() override;
    void SetViewport(int width, int height) override;
    
private:
    char ColorToChar(const Color& color);
    bool IsInBounds(int x, int y) const;
};

// --- HTML/SVG Renderer (for web output) ---
class HTMLRenderer : public Renderer {
private:
    std::string htmlContent;
    int viewportWidth;
    int viewportHeight;
    std::vector<std::string> drawCommands;
    
public:
    HTMLRenderer(int width = 800, int height = 600);
    
    void Clear(const Color& clearColor = Color::BLACK) override;
    void DrawRectangle(const Vector2D& position, const Vector2D& size, const Color& color) override;
    void DrawCircle(const Vector2D& position, double radius, const Color& color) override;
    void DrawTexture(const SVGTexture& texture, const Vector2D& position, const Vector2D& scale = Vector2D(1.0, 1.0)) override;
    void DrawLine(const Vector2D& start, const Vector2D& end, const Color& color, double thickness = 1.0) override;
    void DrawText(const std::string& text, const Vector2D& position, const Color& color, double fontSize = 16.0) override;
    
    void Present() override;
    void SetViewport(int width, int height) override;
    
    void SaveToFile(const std::string& filename);
    const std::string& GetHTMLContent() const { return htmlContent; }
    
private:
    std::string ColorToHex(const Color& color);
};

#endif // GRAPHICS_H
