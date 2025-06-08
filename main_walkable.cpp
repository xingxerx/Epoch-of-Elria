// main_walkable.cpp - Interactive game with player movement controls
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "Vector2D.h"
#include "graphics.h"

// Input handling for non-blocking keyboard input
class InputHandler {
private:
    struct termios old_tio, new_tio;
    bool initialized;

public:
    InputHandler() : initialized(false) {
        // Set terminal to non-blocking mode
        tcgetattr(STDIN_FILENO, &old_tio);
        new_tio = old_tio;
        new_tio.c_lflag &= (~ICANON & ~ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
        initialized = true;
    }

    ~InputHandler() {
        if (initialized) {
            tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
        }
    }

    char GetKey() {
        char ch = 0;
        if (read(STDIN_FILENO, &ch, 1) == 1) {
            return ch;
        }
        return 0; // No key pressed
    }
};

// Enhanced Player class with movement controls
class WalkablePlayer {
private:
    Vector2D position;
    Vector2D velocity;
    std::string name;
    std::shared_ptr<SVGTexture> texture;
    double speed;
    double width, height;
    bool active;

public:
    WalkablePlayer(double start_x, double start_y) 
        : position(start_x, start_y), velocity(0, 0), name("Player"), 
          speed(150.0), width(50), height(50), active(true) {
        
        // Create player texture
        std::string playerSVG = 
            "<svg width=\"50\" height=\"50\" xmlns=\"http://www.w3.org/2000/svg\">\n"
            "  <!-- Player body -->\n"
            "  <rect x=\"10\" y=\"15\" width=\"30\" height=\"25\" fill=\"#4169E1\" rx=\"5\"/>\n"
            "  <!-- Player head -->\n"
            "  <circle cx=\"25\" cy=\"10\" r=\"7.5\" fill=\"#FFE4B5\"/>\n"
            "  <!-- Eyes -->\n"
            "  <circle cx=\"22.5\" cy=\"9\" r=\"2\" fill=\"black\"/>\n"
            "  <circle cx=\"27.5\" cy=\"9\" r=\"2\" fill=\"black\"/>\n"
            "  <!-- Arms -->\n"
            "  <rect x=\"5\" y=\"20\" width=\"7.5\" height=\"15\" fill=\"#FFE4B5\" rx=\"3\"/>\n"
            "  <rect x=\"37.5\" y=\"20\" width=\"7.5\" height=\"15\" fill=\"#FFE4B5\" rx=\"3\"/>\n"
            "  <!-- Legs -->\n"
            "  <rect x=\"15\" y=\"37.5\" width=\"7.5\" height=\"10\" fill=\"#4169E1\" rx=\"3\"/>\n"
            "  <rect x=\"27.5\" y=\"37.5\" width=\"7.5\" height=\"10\" fill=\"#4169E1\" rx=\"3\"/>\n"
            "</svg>";
        
        texture = TextureManager::GetInstance().CreateProceduralTexture("walkable_player", playerSVG);
        std::cout << "Walkable Player created! Use WASD or arrow keys to move." << std::endl;
    }

    void HandleInput(char key) {
        velocity = Vector2D(0, 0); // Reset velocity
        
        switch (key) {
            case 'w': case 'W':
                velocity.y = -speed; // Move up
                break;
            case 's': case 'S':
                velocity.y = speed;  // Move down
                break;
            case 'a': case 'A':
                velocity.x = -speed; // Move left
                break;
            case 'd': case 'D':
                velocity.x = speed;  // Move right
                break;
            case 27: // ESC key
                active = false;
                break;
        }
    }

    void Update(double deltaTime, const Vector2D& worldBounds) {
        if (!active) return;
        
        // Update position based on velocity
        position = position + (velocity * deltaTime);
        
        // Keep player within world bounds
        if (position.x < 0) position.x = 0;
        if (position.y < 0) position.y = 0;
        if (position.x + width > worldBounds.x) position.x = worldBounds.x - width;
        if (position.y + height > worldBounds.y) position.y = worldBounds.y - height;
    }

    void Draw(Renderer& renderer) const {
        if (!active) return;
        
        if (texture && texture->IsLoaded()) {
            renderer.DrawTexture(*texture, position);
        } else {
            renderer.DrawRectangle(position, Vector2D(width, height), Color::BLUE);
        }
    }

    // Getters
    const Vector2D& getPosition() const { return position; }
    const Vector2D& getVelocity() const { return velocity; }
    double getWidth() const { return width; }
    double getHeight() const { return height; }
    bool isActive() const { return active; }
    
    // Collision detection
    bool CheckCollision(const Vector2D& otherPos, const Vector2D& otherSize) const {
        return !(position.x >= otherPos.x + otherSize.x ||
                 position.x + width <= otherPos.x ||
                 position.y >= otherPos.y + otherSize.y ||
                 position.y + height <= otherPos.y);
    }
};

// Enhanced Collectible with better visuals
class WalkableCollectible {
private:
    Vector2D position;
    Vector2D originalPosition;
    std::shared_ptr<SVGTexture> texture;
    int value;
    bool collected;
    double animationTime;
    double bobHeight;
    double width, height;
    std::string type;

public:
    WalkableCollectible(double start_x, double start_y, const std::string& collectType = "coin")
        : position(start_x, start_y), originalPosition(start_x, start_y),
          value(collectType == "gem" ? 25 : 10), collected(false),
          animationTime(0.0), bobHeight(8.0), width(20), height(20), type(collectType) {
        
        std::string svgContent;
        if (type == "coin") {
            svgContent = 
                "<svg width=\"20\" height=\"20\" xmlns=\"http://www.w3.org/2000/svg\">\n"
                "  <circle cx=\"10\" cy=\"10\" r=\"8\" fill=\"#FFD700\" stroke=\"#FFA500\" stroke-width=\"2\"/>\n"
                "  <circle cx=\"10\" cy=\"10\" r=\"5\" fill=\"none\" stroke=\"#FFA500\" stroke-width=\"1\"/>\n"
                "  <text x=\"10\" y=\"12\" text-anchor=\"middle\" font-family=\"serif\" font-size=\"8\" fill=\"#B8860B\">$</text>\n"
                "</svg>";
        } else {
            svgContent = 
                "<svg width=\"20\" height=\"20\" xmlns=\"http://www.w3.org/2000/svg\">\n"
                "  <polygon points=\"10,2 16,8 14,18 6,18 4,8\" fill=\"#0066FF\" stroke=\"#0044CC\" stroke-width=\"1\"/>\n"
                "  <polygon points=\"10,2 12,6 8,6\" fill=\"white\" opacity=\"0.6\"/>\n"
                "</svg>";
        }
        
        texture = TextureManager::GetInstance().CreateProceduralTexture(
            type + "_" + std::to_string(rand()), svgContent);
    }

    void Update(double deltaTime) {
        if (collected) return;
        
        // Animate bobbing motion
        animationTime += deltaTime * 3.0;
        position.y = originalPosition.y + std::sin(animationTime) * bobHeight;
    }

    void Draw(Renderer& renderer) const {
        if (collected) return;
        
        if (texture && texture->IsLoaded()) {
            renderer.DrawTexture(*texture, position);
        } else {
            Color color = (type == "gem") ? Color::BLUE : Color::YELLOW;
            renderer.DrawCircle(position + Vector2D(width/2, height/2), width/2, color);
        }
    }

    // Getters and setters
    const Vector2D& getPosition() const { return position; }
    Vector2D getSize() const { return Vector2D(width, height); }
    int getValue() const { return value; }
    bool isCollected() const { return collected; }
    void setCollected(bool status) { collected = status; }
    const std::string& getType() const { return type; }
};

// Game camera that follows the player
class GameCamera {
private:
    Vector2D position;
    Vector2D viewSize;
    Vector2D worldSize;

public:
    GameCamera(const Vector2D& view, const Vector2D& world) 
        : position(0, 0), viewSize(view), worldSize(world) {}

    void FollowPlayer(const WalkablePlayer& player) {
        Vector2D playerPos = player.getPosition();
        Vector2D targetPos = playerPos - viewSize * 0.5;
        
        // Smooth camera movement
        position = position + (targetPos - position) * 0.1;
        
        // Keep camera within world bounds
        position.x = std::max(0.0, std::min(position.x, worldSize.x - viewSize.x));
        position.y = std::max(0.0, std::min(position.y, worldSize.y - viewSize.y));
    }

    Vector2D WorldToScreen(const Vector2D& worldPos) const {
        return worldPos - position;
    }

    const Vector2D& getPosition() const { return position; }
    const Vector2D& getViewSize() const { return viewSize; }
};

// Main interactive game loop
void RunWalkableGame() {
    std::cout << "\n=== Interactive Walkable Game ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD - Move player" << std::endl;
    std::cout << "  ESC  - Quit game" << std::endl;
    std::cout << "  Q    - Quit game" << std::endl;
    std::cout << "\nStarting game..." << std::endl;

    // Initialize input handler
    InputHandler input;
    
    // Create renderer
    std::unique_ptr<ConsoleRenderer> renderer(new ConsoleRenderer(120, 30));
    
    // Game world setup
    Vector2D worldSize(1000, 600);
    Vector2D viewSize(120, 30);
    GameCamera camera(viewSize, worldSize);
    
    // Create player
    WalkablePlayer player(worldSize.x / 2, worldSize.y / 2);
    
    // Create collectibles
    std::vector<std::unique_ptr<WalkableCollectible>> collectibles;
    for (int i = 0; i < 20; ++i) {
        std::string type = (i % 4 == 0) ? "gem" : "coin";
        collectibles.push_back(std::unique_ptr<WalkableCollectible>(
            new WalkableCollectible(
                static_cast<double>(rand() % 900 + 50),
                static_cast<double>(rand() % 500 + 50),
                type
            )
        ));
    }
    
    int score = 0;
    double deltaTime = 1.0 / 30.0; // 30 FPS
    
    std::cout << "Game started! Move around and collect items!" << std::endl;
    
    // Game loop
    while (player.isActive()) {
        auto frameStart = std::chrono::high_resolution_clock::now();
        
        // Handle input
        char key = input.GetKey();
        if (key != 0) {
            if (key == 'q' || key == 'Q') {
                break;
            }
            player.HandleInput(key);
        }
        
        // Update game objects
        player.Update(deltaTime, worldSize);
        camera.FollowPlayer(player);
        
        for (auto& collectible : collectibles) {
            collectible->Update(deltaTime);
            
            // Check collision
            if (!collectible->isCollected() && 
                player.CheckCollision(collectible->getPosition(), collectible->getSize())) {
                score += collectible->getValue();
                collectible->setCollected(true);
                std::cout << "Collected " << collectible->getType() << "! Score: " << score << std::endl;
            }
        }
        
        // Remove collected items
        collectibles.erase(
            std::remove_if(collectibles.begin(), collectibles.end(),
                [](const std::unique_ptr<WalkableCollectible>& c) {
                    return c->isCollected();
                }),
            collectibles.end()
        );
        
        // Render
        renderer->Clear(Color::BLACK);
        
        // Draw collectibles
        for (const auto& collectible : collectibles) {
            Vector2D screenPos = camera.WorldToScreen(collectible->getPosition());
            if (screenPos.x >= -20 && screenPos.x <= viewSize.x && 
                screenPos.y >= -20 && screenPos.y <= viewSize.y) {
                // Create temporary renderer position
                Vector2D oldPos = collectible->getPosition();
                const_cast<WalkableCollectible*>(collectible.get())->Update(0); // Hack for demo
                renderer->DrawCircle(screenPos + Vector2D(10, 10), 8, 
                    collectible->getType() == "gem" ? Color::BLUE : Color::YELLOW);
            }
        }
        
        // Draw player
        Vector2D playerScreenPos = camera.WorldToScreen(player.getPosition());
        renderer->DrawRectangle(playerScreenPos, Vector2D(50, 50), Color::GREEN);
        
        // Draw UI
        renderer->DrawText("Score: " + std::to_string(score), Vector2D(5, 5), Color::WHITE);
        renderer->DrawText("Items: " + std::to_string(collectibles.size()), Vector2D(5, 7), Color::WHITE);
        renderer->DrawText("Pos: (" + std::to_string((int)player.getPosition().x) + 
                          "," + std::to_string((int)player.getPosition().y) + ")", Vector2D(5, 9), Color::WHITE);
        renderer->DrawText("Controls: WASD=Move, Q=Quit", Vector2D(5, 25), Color::WHITE);
        
        renderer->Present();
        
        // Frame rate control
        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);
        auto targetFrameTime = std::chrono::milliseconds(33); // ~30 FPS
        
        if (frameDuration < targetFrameTime) {
            std::this_thread::sleep_for(targetFrameTime - frameDuration);
        }
    }
    
    std::cout << "\nGame Over!" << std::endl;
    std::cout << "Final Score: " << score << std::endl;
    std::cout << "Thanks for playing!" << std::endl;
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    RunWalkableGame();
    return 0;
}
