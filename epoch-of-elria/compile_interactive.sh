#!/bin/bash

# Compilation script for the interactive SFML version
# This script attempts to compile the SFML version with different library configurations

CXX="${CXX:-g++}" # Use CXX environment variable if set, otherwise default to g++
CPP_STD="-std=c++11"
PTHREAD_FLAG="-pthread"
COMMON_COMPILE_FLAGS="$CPP_STD $PTHREAD_FLAG"
SOURCE_FILES="main_interactive.cpp Vector2D.cpp"
OUTPUT_BINARY="game_interactive"
OUTPUT_BINARY_SIMPLE="game_interactive_simple"

echo "=== Compiling Interactive Game with SFML ==="

SFML_FLAGS=""
SFML_FLAGS_CMD=""

# Try to get SFML flags using pkg-config
if pkg-config --exists sfml-all; then
    echo "SFML found via pkg-config (sfml-all)."
    SFML_FLAGS_CMD="pkg-config --cflags --libs sfml-all"
elif pkg-config --exists sfml-graphics && pkg-config --exists sfml-window && pkg-config --exists sfml-system; then
    echo "SFML found via pkg-config (sfml-graphics, sfml-window, sfml-system)."
    SFML_FLAGS_CMD="pkg-config --cflags --libs sfml-graphics sfml-window sfml-system"
else
    echo "SFML not found via pkg-config. Please ensure SFML development libraries are installed and pkg-config is aware of them."
    echo "  Ubuntu/Debian: sudo apt-get install libsfml-dev"
    echo "  Fedora: sudo dnf install SFML-devel"
    echo "  Arch: sudo pacman -S sfml"
    echo ""
    echo "If SFML is installed in a custom location, you may need to set PKG_CONFIG_PATH."
    echo "Attempting compilation with common SFML linker flags as a fallback (may not work if headers are not in default paths)."
    SFML_FLAGS="-lsfml-graphics -lsfml-window -lsfml-system" # Fallback, less reliable
fi

if [ -n "$SFML_FLAGS_CMD" ]; then
    SFML_FLAGS=$($SFML_FLAGS_CMD)
    if [ $? -ne 0 ]; then
        echo "Error: pkg-config command failed: $SFML_FLAGS_CMD"
        SFML_FLAGS="" # Clear flags if command failed
    fi
fi

if [ -z "$SFML_FLAGS" ]; then
    echo "Could not determine SFML flags. Aborting."
    exit 1
fi

echo "Attempting to compile: $CXX $SOURCE_FILES -o $OUTPUT_BINARY $SFML_FLAGS $COMMON_COMPILE_FLAGS"
if $CXX $SOURCE_FILES -o $OUTPUT_BINARY $SFML_FLAGS $COMMON_COMPILE_FLAGS; then
    echo "Compilation successful!"
    echo "Run with: ./game_interactive"
else
    echo "Compilation failed. Trying without ImGui..."
    # Create a simplified version without ImGui
    echo "Creating simplified SFML version..."
    
    # The C++ code for main_interactive_simple.cpp is embedded here. Its content remains unchanged.
    cat > main_interactive_simple.cpp << 'EOF'
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "Vector2D.h"

class SimplePlayer {
private:
    Vector2D position;
    Vector2D velocity;
    double speed;
    sf::RectangleShape sprite;
    
public:
    SimplePlayer(double x, double y) : position(x, y), speed(200.0) {
        sprite.setSize(sf::Vector2f(50, 50));
        sprite.setFillColor(sf::Color::Blue);
        sprite.setPosition(static_cast<float>(x), static_cast<float>(y));
    }
    
    void HandleInput() {
        velocity = Vector2D(0, 0);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) velocity.y = -speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) velocity.y = speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) velocity.x = -speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) velocity.x = speed;
    }
    
    void Update(double deltaTime) {
        HandleInput();
        position = position + (velocity * deltaTime);
        
        // Keep on screen
        if (position.x < 0) position.x = 0;
        if (position.y < 0) position.y = 0;
        if (position.x > 1230) position.x = 1230;
        if (position.y > 670) position.y = 670;
        
        sprite.setPosition(static_cast<float>(position.x), static_cast<float>(position.y));
    }
    
    void Draw(sf::RenderTarget& target) {
        target.draw(sprite);
    }
    
    const Vector2D& getPosition() const { return position; }
    bool CheckCollision(const Vector2D& otherPos, const Vector2D& otherSize) const {
        return !(position.x >= otherPos.x + otherSize.x ||
                 position.x + 50 <= otherPos.x ||
                 position.y >= otherPos.y + otherSize.y ||
                 position.y + 50 <= otherPos.y);
    }
};

class SimpleCollectible {
private:
    Vector2D position;
    bool collected;
    sf::CircleShape sprite;
    
public:
    SimpleCollectible(double x, double y) : position(x, y), collected(false) {
        sprite.setRadius(10);
        sprite.setFillColor(sf::Color::Yellow);
        sprite.setPosition(static_cast<float>(x), static_cast<float>(y));
    }
    
    void Draw(sf::RenderTarget& target) {
        if (!collected) target.draw(sprite);
    }
    
    const Vector2D& getPosition() const { return position; }
    bool isCollected() const { return collected; }
    void setCollected(bool c) { collected = c; }
    Vector2D getSize() const { return Vector2D(20, 20); }
};

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Walkable Game - SFML");
    window.setFramerateLimit(60);
    
    SimplePlayer player(400, 300);
    std::vector<std::unique_ptr<SimpleCollectible>> collectibles;
    
    // Create initial collectibles
    for (int i = 0; i < 10; ++i) {
        collectibles.push_back(std::make_unique<SimpleCollectible>(
            rand() % 1200 + 40, rand() % 680 + 40));
    }
    
    sf::Clock clock;
    int score = 0;
    
    while (window.isOpen()) {
        double deltaTime = clock.restart().asSeconds();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();
        }
        
        player.Update(deltaTime);
        
        // Check collisions
        for (auto& collectible : collectibles) {
            if (!collectible->isCollected() && 
                player.CheckCollision(collectible->getPosition(), collectible->getSize())) {
                score += 10;
                collectible->setCollected(true);
                std::cout << "Score: " << score << std::endl;
            }
        }
        
        window.clear(sf::Color(30, 30, 60));
        
        player.Draw(window);
        for (const auto& collectible : collectibles) {
            collectible->Draw(window);
        }
        
        window.display();
    }
    
    std::cout << "Final Score: " << score << std::endl;
    return 0;
}
EOF
    
    echo "Compiling simplified version..."
    # Use the same SFML_FLAGS and COMMON_COMPILE_FLAGS obtained earlier
    echo "Attempting to compile: $CXX main_interactive_simple.cpp Vector2D.cpp -o $OUTPUT_BINARY_SIMPLE $SFML_FLAGS $COMMON_COMPILE_FLAGS"
    if $CXX main_interactive_simple.cpp Vector2D.cpp -o $OUTPUT_BINARY_SIMPLE $SFML_FLAGS $COMMON_COMPILE_FLAGS; then
        echo "Simplified version compiled successfully!"
        echo "Run with: ./$OUTPUT_BINARY_SIMPLE"
    else
        echo "Compilation of simplified SFML version also failed."
        echo "Please check compiler errors and ensure SFML is correctly installed."
        echo "  (e.g., Ubuntu/Debian: sudo apt-get install libsfml-dev)"
        echo "If a console version is available, you might try that: ./game_walkable_simple" # This refers to a non-SFML alternative
    fi
fi
