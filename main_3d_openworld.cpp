// main_3d_openworld.cpp - 3D Open World Game with WASD Movement
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cctype>

#include "Vector3D.h"
#include "GameObject3D.h"
#include "World3D.h"

// --- Input Handler for 3D Movement ---
class InputHandler3D {
private:
    bool keys[256];
    Vector3D movementInput;
    Vector3D mouseInput;
    bool mouseLook;

public:
    InputHandler3D() : mouseLook(false) {
        for (int i = 0; i < 256; ++i) {
            keys[i] = false;
        }
    }

    void setKey(char key, bool pressed) {
        if (key >= 0 && key < 256) {
            keys[key] = pressed;
        }
    }

    void updateMovementInput() {
        movementInput = Vector3D::Zero();
        
        // WASD movement
        if (keys['w'] || keys['W']) movementInput.z += 1.0;  // Forward
        if (keys['s'] || keys['S']) movementInput.z -= 1.0;  // Backward
        if (keys['a'] || keys['A']) movementInput.x -= 1.0;  // Left
        if (keys['d'] || keys['D']) movementInput.x += 1.0;  // Right
        
        // Normalize diagonal movement
        if (movementInput.magnitude() > 1.0) {
            movementInput = movementInput.normalize();
        }
    }

    const Vector3D& getMovementInput() const { return movementInput; }
    bool isKeyPressed(char key) const { 
        return (key >= 0 && key < 256) ? keys[key] : false; 
    }
};

// --- 3D Game Manager ---
class Game3D {
private:
    World3D world;
    std::unique_ptr<Player3D> player;
    InputHandler3D input;
    
    int score;
    double gameTime;
    bool running;
    bool showHelp;

public:
    Game3D() : score(0), gameTime(0.0), running(true), showHelp(true) {
        // Create world bounds (100x100x100 world)
        WorldBounds3D bounds(Vector3D(-50, -5, -50), Vector3D(50, 50, 50));
        world = World3D(bounds);
        
        // Create player at spawn point
        player = std::unique_ptr<Player3D>(new Player3D(Vector3D(0, 2, 0)));
        
        // Setup world
        setupWorld();
        
        std::cout << "\n=== 3D OPEN WORLD GAME ===\n";
        std::cout << "Welcome to the 3D Open World!\n";
        std::cout << "Use WASD to move around and explore!\n";
        std::cout << "Collect items to increase your score!\n\n";
    }

    void setupWorld() {
        // Create ground
        world.createGround(0.0);
        
        // Create platforms
        world.createPlatforms();
        
        // Create collectibles
        world.createRandomCollectibles(20);
        
        // Set camera to follow player
        world.getCamera().setPosition(player->getPosition() + Vector3D(0, 5, -10));
        world.getCamera().lookAt(player->getPosition());
    }

    void processInput(const std::string& command) {
        if (command.empty()) return;
        
        // Convert to lowercase
        std::string cmd = command;
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
        
        if (cmd == "quit" || cmd == "exit" || cmd == "q") {
            running = false;
        } else if (cmd == "help" || cmd == "h") {
            showHelp = !showHelp;
            printHelp();
        } else if (cmd == "jump" || cmd == "j" || cmd == " ") {
            player->jump();
            std::cout << "Player jumps!\n";
        } else if (cmd == "look" || cmd == "l") {
            world.draw();
        } else if (cmd == "status" || cmd == "stat") {
            printStatus();
        } else if (cmd.length() == 1) {
            // Single character movement commands
            char key = cmd[0];
            handleMovementCommand(key);
        } else {
            // Multi-character movement commands
            if (cmd == "forward" || cmd == "w") handleMovementCommand('w');
            else if (cmd == "backward" || cmd == "back" || cmd == "s") handleMovementCommand('s');
            else if (cmd == "left" || cmd == "a") handleMovementCommand('a');
            else if (cmd == "right" || cmd == "d") handleMovementCommand('d');
            else {
                std::cout << "Unknown command: " << command << "\n";
                std::cout << "Type 'help' for available commands.\n";
            }
        }
    }

    void handleMovementCommand(char key) {
        Vector3D inputDir = Vector3D::Zero();
        
        switch (key) {
            case 'w': inputDir.z = 1.0; break;  // Forward
            case 's': inputDir.z = -1.0; break; // Backward
            case 'a': inputDir.x = -1.0; break; // Left
            case 'd': inputDir.x = 1.0; break;  // Right
        }
        
        if (inputDir.magnitude() > 0) {
            player->setInputDirection(inputDir);
            
            // Simulate one frame of movement
            double deltaTime = 0.1; // 100ms movement step
            update(deltaTime);
            
            std::cout << "Player moved ";
            switch (key) {
                case 'w': std::cout << "forward"; break;
                case 's': std::cout << "backward"; break;
                case 'a': std::cout << "left"; break;
                case 'd': std::cout << "right"; break;
            }
            std::cout << " to position ";
            player->getPosition().print();
            std::cout << "\n";
        }
    }

    void update(double deltaTime) {
        gameTime += deltaTime;
        
        // Update player
        player->Update(deltaTime, &world);
        
        // Update world
        world.update(deltaTime);
        
        // Check collectible collisions
        auto nearbyCollectibles = world.getNearbyCollectibles(player->getPosition(), 2.0);
        for (auto* collectible : nearbyCollectibles) {
            if (player->CheckCollision(*collectible)) {
                score += collectible->getValue();
                collectible->setCollected(true);
                std::cout << "*** COLLECTED ITEM! Score: " << score << " ***\n";
            }
        }
        
        // Update camera to follow player
        Vector3D playerPos = player->getPosition();
        Vector3D cameraOffset(0, 8, -12);
        world.getCamera().setPosition(playerPos + cameraOffset);
        world.getCamera().lookAt(playerPos + Vector3D(0, 1, 0));
    }

    void printHelp() {
        if (!showHelp) return;
        
        std::cout << "\n=== CONTROLS ===\n";
        std::cout << "Movement:\n";
        std::cout << "  w/forward  - Move forward\n";
        std::cout << "  s/backward - Move backward\n";
        std::cout << "  a/left     - Move left\n";
        std::cout << "  d/right    - Move right\n";
        std::cout << "  jump/j     - Jump\n";
        std::cout << "\nCommands:\n";
        std::cout << "  look/l     - Show world view\n";
        std::cout << "  status     - Show game status\n";
        std::cout << "  help/h     - Toggle this help\n";
        std::cout << "  quit/q     - Exit game\n";
        std::cout << "================\n\n";
    }

    void printStatus() {
        std::cout << "\n=== GAME STATUS ===\n";
        std::cout << "Player Position: ";
        player->getPosition().print();
        std::cout << "\nPlayer Velocity: ";
        player->getVelocity().print();
        std::cout << "\nOn Ground: " << (player->isOnGround() ? "Yes" : "No") << "\n";
        std::cout << "Score: " << score << "\n";
        std::cout << "Game Time: " << (int)gameTime << " seconds\n";
        std::cout << "Collectibles Remaining: " << world.getCollectibleCount() << "\n";
        std::cout << "World Bounds: ";
        world.getBounds().min.print();
        std::cout << " to ";
        world.getBounds().max.print();
        std::cout << "\n==================\n\n";
    }

    void run() {
        printHelp();
        std::cout << "Type 'look' to see the world, or movement commands to explore!\n\n";
        
        std::string command;
        while (running) {
            std::cout << "> ";
            std::getline(std::cin, command);
            
            if (!command.empty()) {
                processInput(command);
                
                // Check win condition
                if (world.getCollectibleCount() == 0) {
                    std::cout << "\n*** CONGRATULATIONS! ***\n";
                    std::cout << "You collected all items in the 3D world!\n";
                    std::cout << "Final Score: " << score << "\n";
                    std::cout << "Total Time: " << (int)gameTime << " seconds\n";
                    break;
                }
            }
        }
        
        std::cout << "\nThanks for playing the 3D Open World Game!\n";
        std::cout << "Final Score: " << score << "\n";
    }
};

// --- Real-time 3D Game Loop (Alternative) ---
void runRealTime3D() {
    std::cout << "\n=== REAL-TIME 3D MODE ===\n";
    std::cout << "This would run a real-time 3D game with continuous WASD input.\n";
    std::cout << "For this demo, we'll simulate a few seconds of movement.\n\n";
    
    Game3D game;
    
    // Simulate some automatic movement for demonstration
    std::vector<char> movements = {'w', 'w', 'd', 'd', 'w', 's', 'a', 'a', 'w'};
    
    for (char move : movements) {
        std::cout << "Auto-movement: " << move << "\n";
        game.handleMovementCommand(move);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    std::cout << "\nReal-time demo complete!\n";
    std::cout << "In a full implementation, this would use:\n";
    std::cout << "- OpenGL/DirectX for 3D rendering\n";
    std::cout << "- Real-time input handling\n";
    std::cout << "- Continuous game loop at 60+ FPS\n";
    std::cout << "- 3D models and textures\n";
    std::cout << "- Sound effects and music\n\n";
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    
    std::cout << "Choose game mode:\n";
    std::cout << "1. Turn-based 3D exploration\n";
    std::cout << "2. Real-time 3D demo\n";
    std::cout << "Enter choice (1 or 2): ";
    
    std::string choice;
    std::getline(std::cin, choice);
    
    if (choice == "2") {
        runRealTime3D();
    } else {
        Game3D game;
        game.run();
    }
    
    return 0;
}
