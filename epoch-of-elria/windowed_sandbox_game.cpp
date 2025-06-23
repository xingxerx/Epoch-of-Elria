// windowed_sandbox_game.cpp - Windowed Open World Sandbox with FPS Counter
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>

// For now, we'll create a console-based "windowed" version that simulates graphics
// This can be easily upgraded to SDL2/OpenGL later

class FrameCounter {
private:
    std::chrono::high_resolution_clock::time_point lastTime;
    std::chrono::high_resolution_clock::time_point startTime;
    int frameCount;
    double fps;
    double deltaTime;
    
public:
    FrameCounter() : frameCount(0), fps(0.0), deltaTime(0.0) {
        lastTime = std::chrono::high_resolution_clock::now();
        startTime = lastTime;
    }
    
    void update() {
        frameCount++;
        auto currentTime = std::chrono::high_resolution_clock::now();
        
        // Calculate delta time
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime);
        deltaTime = duration.count() / 1000000.0; // Convert to seconds
        
        // Calculate FPS every second
        auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
        if (totalDuration.count() >= 1000) {
            fps = frameCount / (totalDuration.count() / 1000.0);
            frameCount = 0;
            startTime = currentTime;
        }
        
        lastTime = currentTime;
    }
    
    double getFPS() const { return fps; }
    double getDeltaTime() const { return deltaTime; }
};

struct Vector3D {
    double x, y, z;
    Vector3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    
    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3D operator*(double scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }
    
    double distance(const Vector3D& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        double dz = z - other.z;
        return sqrt(dx*dx + dy*dy + dz*dz);
    }
    
    void print() const {
        std::cout << "(" << std::fixed << std::setprecision(1) << x << ", " << y << ", " << z << ")";
    }
};

class SandboxPlayer {
private:
    Vector3D position;
    Vector3D velocity;
    double health;
    int score;
    double moveSpeed;
    bool flying;
    
public:
    SandboxPlayer() : position(0, 5, 0), velocity(0, 0, 0), health(100), score(0), moveSpeed(10.0), flying(false) {}
    
    void update(double deltaTime) {
        // Apply gravity if not flying
        if (!flying && position.y > 0) {
            velocity.y -= 20.0 * deltaTime; // Gravity
        }
        
        // Apply velocity
        position = position + velocity * deltaTime;
        
        // Ground collision
        if (position.y <= 0) {
            position.y = 0;
            velocity.y = 0;
        }
        
        // Damping
        velocity = velocity * 0.95;
    }
    
    void move(const Vector3D& direction, double deltaTime) {
        Vector3D movement = direction * moveSpeed * deltaTime;
        if (flying) {
            position = position + movement;
        } else {
            velocity.x += movement.x;
            velocity.z += movement.z;
        }
    }
    
    void jump() {
        if (!flying && position.y <= 0.1) {
            velocity.y = 15.0;
        }
    }
    
    void toggleFly() {
        flying = !flying;
        if (flying) {
            velocity.y = 0;
        }
    }
    
    void collectItem() {
        score += 10;
        health = std::min(100.0, health + 5);
    }
    
    // Getters
    const Vector3D& getPosition() const { return position; }
    double getHealth() const { return health; }
    int getScore() const { return score; }
    bool isFlying() const { return flying; }
    const Vector3D& getVelocity() const { return velocity; }
};

class SandboxWorld {
private:
    std::vector<Vector3D> collectibles;
    std::vector<Vector3D> platforms;
    SandboxPlayer player;
    FrameCounter frameCounter;
    int worldSize;
    
public:
    SandboxWorld() : worldSize(50) {
        generateWorld();
    }
    
    void generateWorld() {
        srand(static_cast<unsigned int>(time(NULL)));
        
        // Generate collectibles
        collectibles.clear();
        for (int i = 0; i < 20; ++i) {
            collectibles.push_back(Vector3D(
                (rand() % (worldSize * 2)) - worldSize,
                rand() % 20 + 1,
                (rand() % (worldSize * 2)) - worldSize
            ));
        }
        
        // Generate platforms
        platforms.clear();
        for (int i = 0; i < 15; ++i) {
            platforms.push_back(Vector3D(
                (rand() % (worldSize * 2)) - worldSize,
                rand() % 15 + 5,
                (rand() % (worldSize * 2)) - worldSize
            ));
        }
    }
    
    void update(double deltaTime) {
        player.update(deltaTime);
        frameCounter.update();
        
        // Check collectible collection
        Vector3D playerPos = player.getPosition();
        for (auto it = collectibles.begin(); it != collectibles.end(); ) {
            if (playerPos.distance(*it) < 3.0) {
                player.collectItem();
                it = collectibles.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    void processInput(const std::string& input, double deltaTime) {
        Vector3D movement(0, 0, 0);
        
        for (char c : input) {
            switch (std::tolower(c)) {
                case 'w': movement.z += 1; break;  // Forward
                case 's': movement.z -= 1; break;  // Backward
                case 'a': movement.x -= 1; break;  // Left
                case 'd': movement.x += 1; break;  // Right
                case 'q': movement.y += 1; break;  // Up (if flying)
                case 'e': movement.y -= 1; break;  // Down (if flying)
                case 'j': player.jump(); break;    // Jump
                case 'f': player.toggleFly(); break; // Toggle fly mode
                case 'r': generateWorld(); break;   // Regenerate world
            }
        }
        
        if (movement.x != 0 || movement.y != 0 || movement.z != 0) {
            player.move(movement, deltaTime);
        }
    }
    
    void render() const {
        // Clear screen (simulate window)
        system("clear || cls");
        
        // Draw title bar
        std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║              DREAM WEAVER'S SANDBOX WORLD                   ║" << std::endl;
        std::cout << "║                  FPS: " << std::setw(6) << std::fixed << std::setprecision(1) 
                  << frameCounter.getFPS() << " | DeltaTime: " << std::setw(6) << std::setprecision(4) 
                  << frameCounter.getDeltaTime() << "s              ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
        
        // Player status
        std::cout << "\n=== PLAYER STATUS ===" << std::endl;
        std::cout << "Position: "; player.getPosition().print(); std::cout << std::endl;
        std::cout << "Velocity: "; player.getVelocity().print(); std::cout << std::endl;
        std::cout << "Health: " << player.getHealth() << "/100 | Score: " << player.getScore() << std::endl;
        std::cout << "Mode: " << (player.isFlying() ? "FLYING" : "WALKING") << std::endl;
        
        // World status
        std::cout << "\n=== WORLD STATUS ===" << std::endl;
        std::cout << "Collectibles: " << collectibles.size() << " remaining" << std::endl;
        std::cout << "Platforms: " << platforms.size() << " in world" << std::endl;
        std::cout << "World Size: " << worldSize << "x" << worldSize << " units" << std::endl;
        
        // Nearby objects
        Vector3D playerPos = player.getPosition();
        std::cout << "\n=== NEARBY OBJECTS ===" << std::endl;
        
        // Find nearest collectible
        if (!collectibles.empty()) {
            double minDist = 1000;
            Vector3D nearest;
            for (const auto& item : collectibles) {
                double dist = playerPos.distance(item);
                if (dist < minDist) {
                    minDist = dist;
                    nearest = item;
                }
            }
            std::cout << "Nearest collectible: "; nearest.print(); 
            std::cout << " (distance: " << std::fixed << std::setprecision(1) << minDist << ")" << std::endl;
        }
        
        // Find nearest platform
        if (!platforms.empty()) {
            double minDist = 1000;
            Vector3D nearest;
            for (const auto& platform : platforms) {
                double dist = playerPos.distance(platform);
                if (dist < minDist) {
                    minDist = dist;
                    nearest = platform;
                }
            }
            std::cout << "Nearest platform: "; nearest.print(); 
            std::cout << " (distance: " << std::fixed << std::setprecision(1) << minDist << ")" << std::endl;
        }
        
        // Mini-map (ASCII representation)
        std::cout << "\n=== MINI-MAP (10x10 area around player) ===" << std::endl;
        int mapSize = 10;
        for (int z = mapSize/2; z >= -mapSize/2; z--) {
            for (int x = -mapSize/2; x <= mapSize/2; x++) {
                Vector3D checkPos = playerPos + Vector3D(x*2, 0, z*2);
                
                if (x == 0 && z == 0) {
                    std::cout << (player.isFlying() ? "✈" : "P"); // Player
                } else {
                    bool hasCollectible = false;
                    bool hasPlatform = false;
                    
                    for (const auto& item : collectibles) {
                        if (checkPos.distance(item) < 2.0) {
                            hasCollectible = true;
                            break;
                        }
                    }
                    
                    for (const auto& platform : platforms) {
                        if (checkPos.distance(platform) < 2.0) {
                            hasPlatform = true;
                            break;
                        }
                    }
                    
                    if (hasCollectible) std::cout << "●";
                    else if (hasPlatform) std::cout << "■";
                    else std::cout << "·";
                }
            }
            std::cout << std::endl;
        }
        
        // Controls
        std::cout << "\n=== CONTROLS ===" << std::endl;
        std::cout << "WASD - Move | QE - Up/Down (flying) | J - Jump | F - Toggle Fly" << std::endl;
        std::cout << "R - Regenerate World | X - Exit | Enter - Continue" << std::endl;
    }
    
    bool isGameRunning() const {
        return true; // Sandbox mode - runs indefinitely
    }
    
    double getFPS() const {
        return frameCounter.getFPS();
    }
};

int main() {
    std::cout << "Initializing Dream Weaver's Sandbox World..." << std::endl;
    std::cout << "Creating windowed environment..." << std::endl;
    
    SandboxWorld world;
    std::string input;
    
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    
    std::cout << "\nSandbox World loaded! Press Enter to start..." << std::endl;
    std::getline(std::cin, input);
    
    while (world.isGameRunning()) {
        // Calculate delta time
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastFrameTime);
        double deltaTime = duration.count() / 1000000.0;
        lastFrameTime = currentTime;
        
        // Update world
        world.update(deltaTime);
        
        // Render frame
        world.render();
        
        // Get input
        std::cout << "\nCommand: ";
        std::getline(std::cin, input);
        
        if (input == "x" || input == "exit") {
            break;
        }
        
        // Process input
        world.processInput(input, deltaTime);
        
        // Simulate frame rate (remove this for real-time)
        // std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }
    
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                    SANDBOX SESSION ENDED                    ║" << std::endl;
    std::cout << "║                  Final FPS: " << std::setw(6) << std::fixed << std::setprecision(1) 
              << world.getFPS() << "                        ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
    
    return 0;
}
