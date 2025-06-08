#include "graphics.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

// --- Color Constants ---
const Color Color::WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const Color Color::BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const Color Color::RED(1.0f, 0.0f, 0.0f, 1.0f);
const Color Color::GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const Color Color::BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const Color Color::YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const Color Color::PURPLE(1.0f, 0.0f, 1.0f, 1.0f);
const Color Color::ORANGE(1.0f, 0.5f, 0.0f, 1.0f);

// --- SVGTexture Implementation ---
SVGTexture::SVGTexture() : loaded(false), size(0, 0) {}

SVGTexture::SVGTexture(const std::string& filepath) : loaded(false), size(0, 0) {
    LoadFromFile(filepath);
}

bool SVGTexture::LoadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open SVG file: " << filepath << std::endl;
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    svgContent = buffer.str();
    filename = filepath;
    loaded = true;
    
    // Simple size extraction (look for width and height attributes)
    // This is a basic implementation - a full SVG parser would be more robust
    size_t widthPos = svgContent.find("width=\"");
    size_t heightPos = svgContent.find("height=\"");
    
    if (widthPos != std::string::npos && heightPos != std::string::npos) {
        widthPos += 7; // Skip 'width="'
        size_t widthEnd = svgContent.find("\"", widthPos);
        if (widthEnd != std::string::npos) {
            std::string widthStr = svgContent.substr(widthPos, widthEnd - widthPos);
            size.x = std::stod(widthStr);
        }
        
        heightPos += 8; // Skip 'height="'
        size_t heightEnd = svgContent.find("\"", heightPos);
        if (heightEnd != std::string::npos) {
            std::string heightStr = svgContent.substr(heightPos, heightEnd - heightPos);
            size.y = std::stod(heightStr);
        }
    }
    
    return true;
}

bool SVGTexture::LoadFromString(const std::string& svgData) {
    svgContent = svgData;
    loaded = true;
    filename = "procedural";
    
    // Extract size from SVG content
    size_t widthPos = svgContent.find("width=\"");
    size_t heightPos = svgContent.find("height=\"");
    
    if (widthPos != std::string::npos && heightPos != std::string::npos) {
        widthPos += 7;
        size_t widthEnd = svgContent.find("\"", widthPos);
        if (widthEnd != std::string::npos) {
            std::string widthStr = svgContent.substr(widthPos, widthEnd - widthPos);
            size.x = std::stod(widthStr);
        }
        
        heightPos += 8;
        size_t heightEnd = svgContent.find("\"", heightPos);
        if (heightEnd != std::string::npos) {
            std::string heightStr = svgContent.substr(heightPos, heightEnd - heightPos);
            size.y = std::stod(heightStr);
        }
    }
    
    return true;
}

std::string SVGTexture::CreateRectangle(double width, double height, const Color& fillColor, const Color& strokeColor, double strokeWidth) {
    std::stringstream svg;
    svg << "<svg width=\"" << width << "\" height=\"" << height << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    svg << "  <rect x=\"0\" y=\"0\" width=\"" << width << "\" height=\"" << height << "\" ";
    svg << "fill=\"rgb(" << (int)(fillColor.r * 255) << "," << (int)(fillColor.g * 255) << "," << (int)(fillColor.b * 255) << ")\" ";
    svg << "stroke=\"rgb(" << (int)(strokeColor.r * 255) << "," << (int)(strokeColor.g * 255) << "," << (int)(strokeColor.b * 255) << ")\" ";
    svg << "stroke-width=\"" << strokeWidth << "\"/>\n";
    svg << "</svg>";
    return svg.str();
}

std::string SVGTexture::CreateCircle(double radius, const Color& fillColor, const Color& strokeColor, double strokeWidth) {
    double diameter = radius * 2;
    std::stringstream svg;
    svg << "<svg width=\"" << diameter << "\" height=\"" << diameter << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    svg << "  <circle cx=\"" << radius << "\" cy=\"" << radius << "\" r=\"" << radius << "\" ";
    svg << "fill=\"rgb(" << (int)(fillColor.r * 255) << "," << (int)(fillColor.g * 255) << "," << (int)(fillColor.b * 255) << ")\" ";
    svg << "stroke=\"rgb(" << (int)(strokeColor.r * 255) << "," << (int)(strokeColor.g * 255) << "," << (int)(strokeColor.b * 255) << ")\" ";
    svg << "stroke-width=\"" << strokeWidth << "\"/>\n";
    svg << "</svg>";
    return svg.str();
}

std::string SVGTexture::CreatePolygon(const std::vector<Vector2D>& points, const Color& fillColor, const Color& strokeColor, double strokeWidth) {
    if (points.empty()) return "";
    
    // Calculate bounding box
    double minX = points[0].x, maxX = points[0].x;
    double minY = points[0].y, maxY = points[0].y;
    for (const auto& point : points) {
        minX = std::min(minX, point.x);
        maxX = std::max(maxX, point.x);
        minY = std::min(minY, point.y);
        maxY = std::max(maxY, point.y);
    }
    
    double width = maxX - minX;
    double height = maxY - minY;
    
    std::stringstream svg;
    svg << "<svg width=\"" << width << "\" height=\"" << height << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    svg << "  <polygon points=\"";
    for (size_t i = 0; i < points.size(); ++i) {
        if (i > 0) svg << " ";
        svg << (points[i].x - minX) << "," << (points[i].y - minY);
    }
    svg << "\" ";
    svg << "fill=\"rgb(" << (int)(fillColor.r * 255) << "," << (int)(fillColor.g * 255) << "," << (int)(fillColor.b * 255) << ")\" ";
    svg << "stroke=\"rgb(" << (int)(strokeColor.r * 255) << "," << (int)(strokeColor.g * 255) << "," << (int)(strokeColor.b * 255) << ")\" ";
    svg << "stroke-width=\"" << strokeWidth << "\"/>\n";
    svg << "</svg>";
    return svg.str();
}

// --- TextureManager Implementation ---
TextureManager* TextureManager::instance = nullptr;

TextureManager& TextureManager::GetInstance() {
    if (!instance) {
        instance = new TextureManager();
    }
    return *instance;
}

std::shared_ptr<SVGTexture> TextureManager::LoadTexture(const std::string& name, const std::string& filepath) {
    auto texture = std::make_shared<SVGTexture>(filepath);
    if (texture->IsLoaded()) {
        textures[name] = texture;
        return texture;
    }
    return nullptr;
}

std::shared_ptr<SVGTexture> TextureManager::GetTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<SVGTexture> TextureManager::CreateProceduralTexture(const std::string& name, const std::string& svgContent) {
    auto texture = std::make_shared<SVGTexture>();
    if (texture->LoadFromString(svgContent)) {
        textures[name] = texture;
        return texture;
    }
    return nullptr;
}

void TextureManager::UnloadTexture(const std::string& name) {
    textures.erase(name);
}

void TextureManager::UnloadAllTextures() {
    textures.clear();
}

// --- ConsoleRenderer Implementation ---
ConsoleRenderer::ConsoleRenderer(int width, int height) : viewportWidth(width), viewportHeight(height) {
    frameBuffer.resize(height, std::vector<char>(width, ' '));
}

void ConsoleRenderer::Clear(const Color& clearColor) {
    char clearChar = ColorToChar(clearColor);
    for (auto& row : frameBuffer) {
        std::fill(row.begin(), row.end(), clearChar);
    }
}

void ConsoleRenderer::DrawRectangle(const Vector2D& position, const Vector2D& size, const Color& color) {
    char ch = ColorToChar(color);
    int startX = static_cast<int>(position.x);
    int startY = static_cast<int>(position.y);
    int endX = static_cast<int>(position.x + size.x);
    int endY = static_cast<int>(position.y + size.y);

    for (int y = startY; y < endY && y < viewportHeight; ++y) {
        for (int x = startX; x < endX && x < viewportWidth; ++x) {
            if (IsInBounds(x, y)) {
                frameBuffer[y][x] = ch;
            }
        }
    }
}

void ConsoleRenderer::DrawCircle(const Vector2D& position, double radius, const Color& color) {
    char ch = ColorToChar(color);
    int centerX = static_cast<int>(position.x);
    int centerY = static_cast<int>(position.y);
    int r = static_cast<int>(radius);

    for (int y = centerY - r; y <= centerY + r; ++y) {
        for (int x = centerX - r; x <= centerX + r; ++x) {
            if (IsInBounds(x, y)) {
                double dx = x - centerX;
                double dy = y - centerY;
                if (dx * dx + dy * dy <= r * r) {
                    frameBuffer[y][x] = ch;
                }
            }
        }
    }
}

void ConsoleRenderer::DrawTexture(const SVGTexture& texture, const Vector2D& position, const Vector2D& scale) {
    // For console rendering, we'll just draw a placeholder rectangle
    Vector2D scaledSize = texture.GetSize();
    scaledSize.x *= scale.x;
    scaledSize.y *= scale.y;
    DrawRectangle(position, scaledSize, Color::WHITE);
}

void ConsoleRenderer::DrawLine(const Vector2D& start, const Vector2D& end, const Color& color, double thickness) {
    char ch = ColorToChar(color);
    int x0 = static_cast<int>(start.x);
    int y0 = static_cast<int>(start.y);
    int x1 = static_cast<int>(end.x);
    int y1 = static_cast<int>(end.y);

    // Bresenham's line algorithm
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    while (true) {
        if (IsInBounds(x0, y0)) {
            frameBuffer[y0][x0] = ch;
        }

        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void ConsoleRenderer::DrawText(const std::string& text, const Vector2D& position, const Color& color, double fontSize) {
    int x = static_cast<int>(position.x);
    int y = static_cast<int>(position.y);

    for (size_t i = 0; i < text.length() && x + i < viewportWidth; ++i) {
        if (IsInBounds(x + i, y)) {
            frameBuffer[y][x + i] = text[i];
        }
    }
}

void ConsoleRenderer::Present() {
    // Clear screen (simple approach)
    system("clear || cls");

    // Draw frame buffer
    for (const auto& row : frameBuffer) {
        for (char ch : row) {
            std::cout << ch;
        }
        std::cout << '\n';
    }
    std::cout << std::flush;
}

void ConsoleRenderer::SetViewport(int width, int height) {
    viewportWidth = width;
    viewportHeight = height;
    frameBuffer.resize(height, std::vector<char>(width, ' '));
}

char ConsoleRenderer::ColorToChar(const Color& color) {
    // Convert color to ASCII character based on brightness
    float brightness = (color.r + color.g + color.b) / 3.0f;
    if (brightness < 0.1f) return ' ';
    else if (brightness < 0.3f) return '.';
    else if (brightness < 0.5f) return ':';
    else if (brightness < 0.7f) return '=';
    else if (brightness < 0.9f) return '#';
    else return '@';
}

bool ConsoleRenderer::IsInBounds(int x, int y) const {
    return x >= 0 && x < viewportWidth && y >= 0 && y < viewportHeight;
}
