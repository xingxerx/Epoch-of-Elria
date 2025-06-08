// main_with_graphics.cpp - Enhanced game engine with SVG graphics and environment
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
#include "GameEnvironment.h"
#include "GameObject.h"

// --- Enhanced Player Class ---
class Player : public GameObject {
private:
    double jumpPower;
    bool onGround;
    double gravity;
    
public:
    Player(double start_x, double start_y) 
        : GameObject("Player", start_x, start_y, EnvironmentAssets::CreatePlayerTexture(50)),
          jumpPower(200.0), onGround(false), gravity(300.0) {
        std::cout << "Enhanced Player created with SVG texture!" << std::endl;
    }

    void Update(double deltaTime, GameEnvironment* environment = nullptr) override {
        // Apply gravity
        if (!onGround) {
            velocity.y += gravity * deltaTime;
        }
        
        // Simple AI movement (move right, jump occasionally)
        velocity.x = 80.0; // Move right at 80 units/second
        
        // Jump occasionally when on ground
        static double jumpTimer = 0.0;
        jumpTimer += deltaTime;
        if (onGround && jumpTimer > 3.0) { // Jump every 3 seconds
            velocity.y = -jumpPower;
            onGround = false;
            jumpTimer = 0.0;
        }
        
        // Store old position for ground detection
        Vector2D oldPos = position;
        
        // Call base class update
        GameObject::Update(deltaTime, environment);
        
        // Check if we're on ground (simple check - if Y position didn't change much and we were falling)
        if (std::abs(position.y - oldPos.y) < 1.0 && velocity.y >= 0) {
            onGround = true;
            velocity.y = 0;
        } else if (velocity.y < 0) {
            onGround = false;
        }
    }
};

// --- Enhanced Collectible Class ---
class Collectible : public GameObject {
private:
    int value;
    bool collected;
    mutable std::mutex collectible_mutex;
    double animationTime;
    double bobHeight;
    Vector2D originalPosition;
    
public:
    Collectible(double start_x, double start_y, int collect_value, const std::string& type = "coin")
        : GameObject("Collectible", start_x, start_y, 
                    type == "coin" ? EnvironmentAssets::CreateCoinTexture(20) : 
                                   EnvironmentAssets::CreateGemTexture(20, Color::BLUE)),
          value(collect_value), collected(false), animationTime(0.0), 
          bobHeight(5.0), originalPosition(start_x, start_y) {}

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

    void Update(double deltaTime, GameEnvironment* environment = nullptr) override {
        if (collected) return;
        
        // Animate bobbing motion
        animationTime += deltaTime * 2.0; // Speed up animation
        position.y = originalPosition.y + std::sin(animationTime) * bobHeight;
        
        // Simulate some work for parallel processing
        std::this_thread::sleep_for(std::chrono::microseconds(5));
        
        GameObject::Update(deltaTime, environment);
    }
};

// Function to update a range of collectibles using indices
void UpdateCollectiblesRange(std::vector<Collectible*>& collectibles, size_t start_index, size_t end_index, double deltaTime, GameEnvironment* environment) {
    for (size_t i = start_index; i < end_index; ++i) {
        if (i < collectibles.size() && !collectibles[i]->isCollected()) {
            collectibles[i]->Update(deltaTime, environment);
        }
    }
}

// --- Enhanced Game Simulation ---
void RunEnhancedGameSimulation() {
    std::cout << "\n=== Enhanced C++ Game Engine with SVG Graphics ===" << std::endl;
    std::cout << "Features: SVG Textures, Environment System, Particle Effects, Camera" << std::endl;
    
    // Create renderers
    auto consoleRenderer = std::make_unique<ConsoleRenderer>(120, 30);
    auto htmlRenderer = std::make_unique<HTMLRenderer>(1000, 600);
    
    // Create game environment
    Vector2D viewSize(1000, 600);
    Vector2D worldSize(2000, 800);
    GameEnvironment environment(viewSize, worldSize);
    
    // Setup environment
    environment.CreateProceduralBackground("sky", Color(0.5f, 0.8f, 1.0f, 1.0f), Color(0.3f, 0.7f, 0.3f, 1.0f));
    environment.CreateLevel1Platforms();
    
    // Add some decorative elements
    auto grassTexture = EnvironmentAssets::CreateGrassTexture(100, 30);
    auto cloudTexture = EnvironmentAssets::CreateCloudTexture(80, 40);
    if (grassTexture) environment.AddBackgroundLayer(grassTexture, Vector2D(0.3, 0.0), true);
    if (cloudTexture) environment.AddBackgroundLayer(cloudTexture, Vector2D(0.1, 0.05), true);
    
    // Create game objects
    Player player(100, 100);
    
    std::vector<Collectible*> collectibles;
    const int NUM_COLLECTIBLES = 200; // Reduced for better performance with graphics
    
    for (int i = 0; i < NUM_COLLECTIBLES; ++i) {
        std::string type = (i % 3 == 0) ? "gem" : "coin";
        int value = (type == "gem") ? 25 : 10;
        
        collectibles.push_back(new Collectible(
            static_cast<double>(rand() % 1800 + 100), // Random X within world bounds
            static_cast<double>(rand() % 300 + 100),  // Random Y in upper area
            value,
            type
        ));
    }
    
    // Threading setup
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 1;
    std::cout << "Using " << num_threads << " threads for collectible updates." << std::endl;
    
    double deltaTime = 1.0 / 60.0; // 60 FPS
    int totalScore = 0;
    int frameCount = 0;
    
    // Game loop
    for (int frame = 0; frame < 100; ++frame) { // Run for 100 frames
        frameCount++;
        
        std::cout << "\n--- Frame " << frame << " (Score: " << totalScore << ") ---" << std::endl;
        
        // 1. Update Player
        player.Update(deltaTime, &environment);
        
        // 2. Update Environment (camera follows player)
        environment.Update(deltaTime, player);
        
        // 3. Update Collectibles in Parallel
        std::vector<std::thread> threads;
        unsigned int effective_threads = std::min(num_threads, static_cast<unsigned int>(collectibles.size()));
        if (effective_threads == 0) effective_threads = 1;
        
        size_t collectibles_per_thread = collectibles.size() / effective_threads;
        if (collectibles_per_thread == 0) collectibles_per_thread = 1;

        for (unsigned int i = 0; i < effective_threads; ++i) {
            size_t start_index = i * collectibles_per_thread;
            size_t end_index = (i == effective_threads - 1) ? collectibles.size() : (start_index + collectibles_per_thread);

            if (start_index >= collectibles.size()) break;

            threads.emplace_back(UpdateCollectiblesRange, std::ref(collectibles), start_index, end_index, deltaTime, &environment);
        }

        // Wait for all threads to complete
        for (std::thread& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
        
        // 4. Check for collisions and create effects
        for (Collectible* collect : collectibles) {
            if (!collect->isCollected() && player.CheckCollision(*collect)) {
                totalScore += collect->getValue();
                collect->setCollected(true);
                
                // Create collection particle effect
                environment.CreateCollectionEffect(collect->getPosition());
                
                std::cout << "Player collected " << collect->getName() << "! Score: " << totalScore << std::endl;
            }
        }
        
        // 5. Render frame
        if (frame % 10 == 0) { // Render every 10th frame to reduce output
            // Console rendering
            consoleRenderer->Clear(Color::BLACK);
            environment.Draw(*consoleRenderer);
            player.Draw(*consoleRenderer, environment.GetCamera());
            
            for (Collectible* collect : collectibles) {
                if (!collect->isCollected()) {
                    collect->Draw(*consoleRenderer, environment.GetCamera());
                }
            }
            
            consoleRenderer->DrawText("Score: " + std::to_string(totalScore), Vector2D(5, 5), Color::WHITE);
            consoleRenderer->DrawText("Frame: " + std::to_string(frame), Vector2D(5, 25), Color::WHITE);
            consoleRenderer->Present();
            
            // HTML rendering (save periodically)
            if (frame % 50 == 0) {
                htmlRenderer->Clear(Color(0.5f, 0.8f, 1.0f, 1.0f)); // Sky blue background
                environment.Draw(*htmlRenderer);
                player.Draw(*htmlRenderer, environment.GetCamera());
                
                for (Collectible* collect : collectibles) {
                    if (!collect->isCollected()) {
                        collect->Draw(*htmlRenderer, environment.GetCamera());
                    }
                }
                
                htmlRenderer->DrawText("Score: " + std::to_string(totalScore), Vector2D(10, 30), Color::WHITE, 20);
                htmlRenderer->DrawText("Frame: " + std::to_string(frame), Vector2D(10, 60), Color::WHITE, 16);
                htmlRenderer->Present();
                htmlRenderer->SaveToFile("game_frame_" + std::to_string(frame) + ".html");
            }
        }
        
        // Small delay to make it visible
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    std::cout << "\n=== Game Simulation Complete ===" << std::endl;
    std::cout << "Final Score: " << totalScore << std::endl;
    std::cout << "Frames Rendered: " << frameCount << std::endl;
    
    // Final render
    htmlRenderer->Clear(Color(0.5f, 0.8f, 1.0f, 1.0f));
    environment.Draw(*htmlRenderer);
    player.Draw(*htmlRenderer, environment.GetCamera());
    
    for (Collectible* collect : collectibles) {
        if (!collect->isCollected()) {
            collect->Draw(*htmlRenderer, environment.GetCamera());
        }
    }
    
    htmlRenderer->DrawText("FINAL SCORE: " + std::to_string(totalScore), Vector2D(400, 300), Color::YELLOW, 32);
    htmlRenderer->DrawText("Game Complete!", Vector2D(420, 340), Color::WHITE, 24);
    htmlRenderer->Present();
    htmlRenderer->SaveToFile("game_final.html");
    
    // Cleanup
    for (Collectible* collect : collectibles) {
        delete collect;
    }
    collectibles.clear();
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    RunEnhancedGameSimulation();
    return 0;
}
