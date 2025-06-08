// main_walkable_simple.cpp - Simple walkable game with turn-based input
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <algorithm>
#include <cctype>

#include "Vector2D.h"
#include "graphics.h"

// Simple Player class with movement controls
class WalkablePlayer {
private:
    Vector2D position;
    std::string name;
    std::shared_ptr<SVGTexture> texture;
    double speed;
    double width, height;
    bool active;

public:
    WalkablePlayer(double start_x, double start_y) 
        : position(start_x, start_y), name("Player"), 
          speed(50.0), width(50), height(50), active(true) {
        
        // Create player texture
        std::string playerSVG = 
            "<svg width=\"50\" height=\"50\" xmlns=\"http://www.w3.org/2000/svg\">\n"
            "  <rect x=\"10\" y=\"15\" width=\"30\" height=\"25\" fill=\"#4169E1\" rx=\"5\"/>\n"
            "  <circle cx=\"25\" cy=\"10\" r=\"7.5\" fill=\"#FFE4B5\"/>\n"
            "  <circle cx=\"22.5\" cy=\"9\" r=\"2\" fill=\"black\"/>\n"
            "  <circle cx=\"27.5\" cy=\"9\" r=\"2\" fill=\"black\"/>\n"
            "  <rect x=\"5\" y=\"20\" width=\"7.5\" height=\"15\" fill=\"#FFE4B5\" rx=\"3\"/>\n"
            "  <rect x=\"37.5\" y=\"20\" width=\"7.5\" height=\"15\" fill=\"#FFE4B5\" rx=\"3\"/>\n"
            "  <rect x=\"15\" y=\"37.5\" width=\"7.5\" height=\"10\" fill=\"#4169E1\" rx=\"3\"/>\n"
            "  <rect x=\"27.5\" y=\"37.5\" width=\"7.5\" height=\"10\" fill=\"#4169E1\" rx=\"3\"/>\n"
            "</svg>";
        
        texture = TextureManager::GetInstance().CreateProceduralTexture("walkable_player", playerSVG);
        std::cout << "Walkable Player created! Use commands to move." << std::endl;
    }

    void Move(const std::string& direction, const Vector2D& worldBounds) {
        Vector2D oldPosition = position;
        
        if (direction == "w" || direction == "up") {
            position.y -= speed;
        } else if (direction == "s" || direction == "down") {
            position.y += speed;
        } else if (direction == "a" || direction == "left") {
            position.x -= speed;
        } else if (direction == "d" || direction == "right") {
            position.x += speed;
        }
        
        // Keep player within world bounds
        if (position.x < 0) position.x = 0;
        if (position.y < 0) position.y = 0;
        if (position.x + width > worldBounds.x) position.x = worldBounds.x - width;
        if (position.y + height > worldBounds.y) position.y = worldBounds.y - height;
        
        if (position.x != oldPosition.x || position.y != oldPosition.y) {
            std::cout << "Player moved to (" << (int)position.x << ", " << (int)position.y << ")" << std::endl;
        }
    }

    void Draw(Renderer& renderer, const Vector2D& cameraOffset = Vector2D(0, 0)) const {
        if (!active) return;
        
        Vector2D screenPos = position - cameraOffset;
        
        if (texture && texture->IsLoaded()) {
            renderer.DrawTexture(*texture, screenPos);
        } else {
            renderer.DrawRectangle(screenPos, Vector2D(width, height), Color::BLUE);
        }
    }

    // Getters
    const Vector2D& getPosition() const { return position; }
    double getWidth() const { return width; }
    double getHeight() const { return height; }
    bool isActive() const { return active; }
    void setActive(bool status) { active = status; }
    
    // Collision detection
    bool CheckCollision(const Vector2D& otherPos, const Vector2D& otherSize) const {
        return !(position.x >= otherPos.x + otherSize.x ||
                 position.x + width <= otherPos.x ||
                 position.y >= otherPos.y + otherSize.y ||
                 position.y + height <= otherPos.y);
    }
};

// Enhanced Collectible
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
          animationTime(0.0), bobHeight(5.0), width(20), height(20), type(collectType) {
        
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
        animationTime += deltaTime * 2.0;
        position.y = originalPosition.y + std::sin(animationTime) * bobHeight;
    }

    void Draw(Renderer& renderer, const Vector2D& cameraOffset = Vector2D(0, 0)) const {
        if (collected) return;
        
        Vector2D screenPos = position - cameraOffset;
        
        if (texture && texture->IsLoaded()) {
            renderer.DrawTexture(*texture, screenPos);
        } else {
            Color color = (type == "gem") ? Color::BLUE : Color::YELLOW;
            renderer.DrawCircle(screenPos + Vector2D(width/2, height/2), width/2, color);
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

// Simple camera system
class SimpleCamera {
private:
    Vector2D position;
    Vector2D viewSize;
    Vector2D worldSize;

public:
    SimpleCamera(const Vector2D& view, const Vector2D& world) 
        : position(0, 0), viewSize(view), worldSize(world) {}

    void CenterOnPlayer(const WalkablePlayer& player) {
        Vector2D playerPos = player.getPosition();
        position = playerPos - viewSize * 0.5;
        
        // Keep camera within world bounds
        position.x = std::max(0.0, std::min(position.x, worldSize.x - viewSize.x));
        position.y = std::max(0.0, std::min(position.y, worldSize.y - viewSize.y));
    }

    const Vector2D& getPosition() const { return position; }
};

// Main game function
void RunSimpleWalkableGame() {
    std::cout << "\n=== Simple Walkable Game ===" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  w/up    - Move up" << std::endl;
    std::cout << "  s/down  - Move down" << std::endl;
    std::cout << "  a/left  - Move left" << std::endl;
    std::cout << "  d/right - Move right" << std::endl;
    std::cout << "  look    - Show current view" << std::endl;
    std::cout << "  status  - Show game status" << std::endl;
    std::cout << "  quit    - Exit game" << std::endl;
    std::cout << "\nStarting game..." << std::endl;

    // Create renderer
    std::unique_ptr<ConsoleRenderer> consoleRenderer(new ConsoleRenderer(100, 25));
    std::unique_ptr<HTMLRenderer> htmlRenderer(new HTMLRenderer(800, 600));
    
    // Game world setup
    Vector2D worldSize(800, 600);
    Vector2D viewSize(100, 25);
    SimpleCamera camera(viewSize, worldSize);
    
    // Create player
    WalkablePlayer player(worldSize.x / 2, worldSize.y / 2);
    
    // Create collectibles
    std::vector<std::unique_ptr<WalkableCollectible>> collectibles;
    for (int i = 0; i < 15; ++i) {
        std::string type = (i % 4 == 0) ? "gem" : "coin";
        collectibles.push_back(std::unique_ptr<WalkableCollectible>(
            new WalkableCollectible(
                static_cast<double>(rand() % 700 + 50),
                static_cast<double>(rand() % 500 + 50),
                type
            )
        ));
    }
    
    int score = 0;
    int moveCount = 0;
    double gameTime = 0.0;
    
    std::cout << "\nGame started! Type commands to play." << std::endl;
    std::cout << "Type 'look' to see the game world." << std::endl;
    
    std::string command;
    while (player.isActive()) {
        std::cout << "\n> ";
        std::getline(std::cin, command);
        
        if (command.empty()) continue;
        
        // Convert to lowercase
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);
        
        if (command == "quit" || command == "exit" || command == "q") {
            break;
        } else if (command == "w" || command == "up") {
            player.Move("w", worldSize);
            moveCount++;
        } else if (command == "s" || command == "down") {
            player.Move("s", worldSize);
            moveCount++;
        } else if (command == "a" || command == "left") {
            player.Move("a", worldSize);
            moveCount++;
        } else if (command == "d" || command == "right") {
            player.Move("d", worldSize);
            moveCount++;
        } else if (command == "look" || command == "l") {
            // Update game state
            double deltaTime = 0.1;
            gameTime += deltaTime;
            
            for (auto& collectible : collectibles) {
                collectible->Update(deltaTime);
                
                // Check collision
                if (!collectible->isCollected() && 
                    player.CheckCollision(collectible->getPosition(), collectible->getSize())) {
                    score += collectible->getValue();
                    collectible->setCollected(true);
                    std::cout << "*** Collected " << collectible->getType() << "! Score: " << score << " ***" << std::endl;
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
            
            // Update camera and render
            camera.CenterOnPlayer(player);
            Vector2D cameraPos = camera.getPosition();
            
            // Console rendering
            consoleRenderer->Clear(Color::BLACK);
            
            // Draw collectibles
            for (const auto& collectible : collectibles) {
                collectible->Draw(*consoleRenderer, cameraPos);
            }
            
            // Draw player
            player.Draw(*consoleRenderer, cameraPos);
            
            // Draw UI
            consoleRenderer->DrawText("Score: " + std::to_string(score), Vector2D(5, 5), Color::WHITE);
            consoleRenderer->DrawText("Items: " + std::to_string(collectibles.size()), Vector2D(5, 7), Color::WHITE);
            consoleRenderer->DrawText("Moves: " + std::to_string(moveCount), Vector2D(5, 9), Color::WHITE);
            
            consoleRenderer->Present();
            
            // Generate HTML output occasionally
            if (moveCount % 5 == 0) {
                htmlRenderer->Clear(Color(0.2f, 0.3f, 0.5f, 1.0f));
                
                for (const auto& collectible : collectibles) {
                    collectible->Draw(*htmlRenderer);
                }
                player.Draw(*htmlRenderer);
                
                htmlRenderer->DrawText("Score: " + std::to_string(score), Vector2D(10, 30), Color::WHITE, 20);
                htmlRenderer->DrawText("Items: " + std::to_string(collectibles.size()), Vector2D(10, 60), Color::WHITE, 16);
                htmlRenderer->Present();
                htmlRenderer->SaveToFile("walkable_game_move_" + std::to_string(moveCount) + ".html");
                std::cout << "HTML snapshot saved!" << std::endl;
            }
            
        } else if (command == "status" || command == "stat") {
            std::cout << "=== Game Status ===" << std::endl;
            std::cout << "Player Position: (" << (int)player.getPosition().x << ", " << (int)player.getPosition().y << ")" << std::endl;
            std::cout << "Score: " << score << std::endl;
            std::cout << "Moves: " << moveCount << std::endl;
            std::cout << "Collectibles Remaining: " << collectibles.size() << std::endl;
            std::cout << "Game Time: " << (int)gameTime << " seconds" << std::endl;
        } else if (command == "help" || command == "h") {
            std::cout << "Available commands:" << std::endl;
            std::cout << "  w/up, s/down, a/left, d/right - Move player" << std::endl;
            std::cout << "  look/l - Show game view" << std::endl;
            std::cout << "  status/stat - Show game status" << std::endl;
            std::cout << "  help/h - Show this help" << std::endl;
            std::cout << "  quit/q - Exit game" << std::endl;
        } else {
            std::cout << "Unknown command: " << command << std::endl;
            std::cout << "Type 'help' for available commands." << std::endl;
        }
        
        // Check win condition
        if (collectibles.empty()) {
            std::cout << "\n*** CONGRATULATIONS! ***" << std::endl;
            std::cout << "You collected all items!" << std::endl;
            std::cout << "Final Score: " << score << std::endl;
            std::cout << "Total Moves: " << moveCount << std::endl;
            break;
        }
    }
    
    std::cout << "\nGame Over!" << std::endl;
    std::cout << "Final Score: " << score << std::endl;
    std::cout << "Thanks for playing!" << std::endl;
    
    // Generate final HTML
    htmlRenderer->Clear(Color(0.2f, 0.3f, 0.5f, 1.0f));
    player.Draw(*htmlRenderer);
    htmlRenderer->DrawText("GAME COMPLETE!", Vector2D(300, 250), Color::YELLOW, 32);
    htmlRenderer->DrawText("Final Score: " + std::to_string(score), Vector2D(320, 300), Color::WHITE, 24);
    htmlRenderer->DrawText("Total Moves: " + std::to_string(moveCount), Vector2D(320, 330), Color::WHITE, 20);
    htmlRenderer->Present();
    htmlRenderer->SaveToFile("walkable_game_final.html");
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    RunSimpleWalkableGame();
    return 0;
}
