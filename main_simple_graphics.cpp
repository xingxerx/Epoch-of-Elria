// main_simple_graphics.cpp - Simple demonstration of SVG graphics system
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

#include "Vector2D.h"
#include "graphics.h"

// Simple GameObject class for this demo
class SimpleGameObject {
protected:
    Vector2D position;
    Vector2D velocity;
    double width, height;
    std::string name;
    std::shared_ptr<SVGTexture> texture;
    Color tintColor;
    bool active;

public:
    SimpleGameObject(std::string obj_name, double start_x, double start_y, double obj_width, double obj_height)
        : name(obj_name), position(start_x, start_y), width(obj_width), height(obj_height), 
          velocity(0.0, 0.0), tintColor(Color::WHITE), active(true) {}

    virtual ~SimpleGameObject() = default;

    // Getters
    const Vector2D& getPosition() const { return position; }
    const Vector2D& getVelocity() const { return velocity; }
    double getWidth() const { return width; }
    double getHeight() const { return height; }
    const std::string& getName() const { return name; }
    bool isActive() const { return active; }

    // Setters
    void setPosition(double x, double y) { position.x = x; position.y = y; }
    void setVelocity(double vx, double vy) { velocity.x = vx; velocity.y = vy; }
    void setTexture(std::shared_ptr<SVGTexture> tex) { texture = tex; }
    void setActive(bool isActive) { active = isActive; }

    // Update method
    virtual void Update(double deltaTime) {
        if (!active) return;
        position = position + (velocity * deltaTime);
    }

    // Draw method
    virtual void Draw(Renderer& renderer) const {
        if (!active) return;
        
        if (texture && texture->IsLoaded()) {
            renderer.DrawTexture(*texture, position);
        } else {
            renderer.DrawRectangle(position, Vector2D(width, height), tintColor);
        }
    }

    // Collision detection
    bool CheckCollision(const SimpleGameObject& other) const {
        if (!active || !other.active) return false;
        
        return !(position.x >= other.position.x + other.width ||
                 position.x + width <= other.position.x ||
                 position.y >= other.position.y + other.height ||
                 position.y + height <= other.position.y);
    }
};

// Simple Player class
class SimplePlayer : public SimpleGameObject {
public:
    SimplePlayer(double start_x, double start_y) 
        : SimpleGameObject("Player", start_x, start_y, 50, 50) {
        
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
        
        texture = TextureManager::GetInstance().CreateProceduralTexture("player", playerSVG);
        std::cout << "Simple Player created with SVG texture!" << std::endl;
    }

    void Update(double deltaTime) override {
        velocity.x = 80.0; // Move right
        SimpleGameObject::Update(deltaTime);
        
        // Wrap around screen
        if (position.x > 1000) {
            position.x = -width;
        }
    }
};

// Simple Collectible class
class SimpleCollectible : public SimpleGameObject {
private:
    int value;
    bool collected;
    mutable std::mutex collectible_mutex;
    double animationTime;
    Vector2D originalPosition;
    
public:
    SimpleCollectible(double start_x, double start_y, int collect_value, const std::string& type = "coin")
        : SimpleGameObject("Collectible", start_x, start_y, 20, 20),
          value(collect_value), collected(false), animationTime(0.0), originalPosition(start_x, start_y) {
        
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
        
        texture = TextureManager::GetInstance().CreateProceduralTexture(type + "_" + std::to_string(rand()), svgContent);
    }

    int getValue() const { return value; }
    
    bool isCollected() const {
        std::lock_guard<std::mutex> lock(collectible_mutex);
        return collected;
    }
    
    void setCollected(bool status) {
        std::lock_guard<std::mutex> lock(collectible_mutex);
        collected = status;
        if (collected) {
            active = false;
        }
    }

    void Update(double deltaTime) override {
        if (collected) return;
        
        // Animate bobbing motion
        animationTime += deltaTime * 2.0;
        position.y = originalPosition.y + std::sin(animationTime) * 5.0;
        
        SimpleGameObject::Update(deltaTime);
    }
};

// Function to update collectibles in parallel
void UpdateCollectiblesRange(std::vector<SimpleCollectible*>& collectibles, size_t start_index, size_t end_index, double deltaTime) {
    for (size_t i = start_index; i < end_index; ++i) {
        if (i < collectibles.size() && !collectibles[i]->isCollected()) {
            collectibles[i]->Update(deltaTime);
        }
    }
}

// Main demonstration
void RunSimpleGraphicsDemo() {
    std::cout << "\n=== Simple SVG Graphics Demo ===" << std::endl;
    
    // Create renderers
    std::unique_ptr<ConsoleRenderer> consoleRenderer(new ConsoleRenderer(100, 25));
    std::unique_ptr<HTMLRenderer> htmlRenderer(new HTMLRenderer(1000, 600));
    
    // Create game objects
    SimplePlayer player(100, 300);
    
    std::vector<SimpleCollectible*> collectibles;
    const int NUM_COLLECTIBLES = 50;
    
    for (int i = 0; i < NUM_COLLECTIBLES; ++i) {
        std::string type = (i % 3 == 0) ? "gem" : "coin";
        int value = (type == "gem") ? 25 : 10;
        
        collectibles.push_back(new SimpleCollectible(
            static_cast<double>(rand() % 900 + 50),
            static_cast<double>(rand() % 200 + 100),
            value,
            type
        ));
    }
    
    // Threading setup
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 1;
    std::cout << "Using " << num_threads << " threads for collectible updates." << std::endl;
    
    double deltaTime = 1.0 / 60.0;
    int totalScore = 0;
    
    // Game loop
    for (int frame = 0; frame < 60; ++frame) {
        std::cout << "\n--- Frame " << frame << " (Score: " << totalScore << ") ---" << std::endl;
        
        // Update player
        player.Update(deltaTime);
        
        // Update collectibles in parallel
        std::vector<std::thread> threads;
        unsigned int effective_threads = std::min(num_threads, static_cast<unsigned int>(collectibles.size()));
        if (effective_threads == 0) effective_threads = 1;
        
        size_t collectibles_per_thread = collectibles.size() / effective_threads;
        if (collectibles_per_thread == 0) collectibles_per_thread = 1;

        for (unsigned int i = 0; i < effective_threads; ++i) {
            size_t start_index = i * collectibles_per_thread;
            size_t end_index = (i == effective_threads - 1) ? collectibles.size() : (start_index + collectibles_per_thread);

            if (start_index >= collectibles.size()) break;
            threads.emplace_back(UpdateCollectiblesRange, std::ref(collectibles), start_index, end_index, deltaTime);
        }

        for (std::thread& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
        
        // Check collisions
        for (SimpleCollectible* collect : collectibles) {
            if (!collect->isCollected() && player.CheckCollision(*collect)) {
                totalScore += collect->getValue();
                collect->setCollected(true);
                std::cout << "Player collected " << collect->getName() << "! Score: " << totalScore << std::endl;
            }
        }
        
        // Render every 10th frame
        if (frame % 10 == 0) {
            // Console rendering
            consoleRenderer->Clear(Color::BLACK);
            player.Draw(*consoleRenderer);
            
            for (SimpleCollectible* collect : collectibles) {
                if (!collect->isCollected()) {
                    collect->Draw(*consoleRenderer);
                }
            }
            
            consoleRenderer->DrawText("Score: " + std::to_string(totalScore), Vector2D(5, 5), Color::WHITE);
            consoleRenderer->Present();
            
            // HTML rendering
            if (frame % 30 == 0) {
                htmlRenderer->Clear(Color(0.5f, 0.8f, 1.0f, 1.0f));
                player.Draw(*htmlRenderer);
                
                for (SimpleCollectible* collect : collectibles) {
                    if (!collect->isCollected()) {
                        collect->Draw(*htmlRenderer);
                    }
                }
                
                htmlRenderer->DrawText("Score: " + std::to_string(totalScore), Vector2D(10, 30), Color::WHITE, 20);
                htmlRenderer->Present();
                htmlRenderer->SaveToFile("simple_game_frame_" + std::to_string(frame) + ".html");
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    std::cout << "Final Score: " << totalScore << std::endl;
    
    // Final render
    htmlRenderer->Clear(Color(0.5f, 0.8f, 1.0f, 1.0f));
    player.Draw(*htmlRenderer);
    
    for (SimpleCollectible* collect : collectibles) {
        if (!collect->isCollected()) {
            collect->Draw(*htmlRenderer);
        }
    }
    
    htmlRenderer->DrawText("FINAL SCORE: " + std::to_string(totalScore), Vector2D(400, 300), Color::YELLOW, 32);
    htmlRenderer->Present();
    htmlRenderer->SaveToFile("simple_game_final.html");
    
    // Cleanup
    for (SimpleCollectible* collect : collectibles) {
        delete collect;
    }
    collectibles.clear();
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    RunSimpleGraphicsDemo();
    return 0;
}
