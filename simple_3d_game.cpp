// simple_3d_game.cpp - Fixed 3D World Game
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Simple Vector3D for this demo
struct SimpleVector3D {
    double x, y, z;
    SimpleVector3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    
    SimpleVector3D operator+(const SimpleVector3D& other) const {
        return SimpleVector3D(x + other.x, y + other.y, z + other.z);
    }
    
    void print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
    
    double distance(const SimpleVector3D& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        double dz = z - other.z;
        return sqrt(dx*dx + dy*dy + dz*dz);
    }
};

class Simple3DPlayer {
private:
    SimpleVector3D position;
    double health;
    int score;
    
public:
    Simple3DPlayer(SimpleVector3D pos = SimpleVector3D(0, 0, 0)) 
        : position(pos), health(100), score(0) {}
    
    void move(const SimpleVector3D& direction) {
        position = position + direction;
        std::cout << "Player moves to ";
        position.print();
        std::cout << std::endl;
    }
    
    void jump() {
        position.y += 2;
        std::cout << "Player jumps! New position: ";
        position.print();
        std::cout << std::endl;
    }
    
    bool collectItem(const SimpleVector3D& itemPos) {
        if (position.distance(itemPos) < 2.0) {
            score += 10;
            health = std::min(100.0, health + 5);
            std::cout << "Item collected! Score: " << score << " Health: " << health << std::endl;
            return true;
        }
        return false;
    }
    
    const SimpleVector3D& getPosition() const { return position; }
    double getHealth() const { return health; }
    int getScore() const { return score; }
    
    void displayStatus() const {
        std::cout << "=== PLAYER STATUS ===" << std::endl;
        std::cout << "Position: "; position.print(); std::cout << std::endl;
        std::cout << "Health: " << health << "/100" << std::endl;
        std::cout << "Score: " << score << std::endl;
    }
};

class Simple3DWorld {
private:
    std::vector<SimpleVector3D> collectibles;
    Simple3DPlayer player;
    
public:
    Simple3DWorld() : player(SimpleVector3D(0, 0, 0)) {
        // Create some collectibles
        srand(static_cast<unsigned int>(time(NULL)));
        for (int i = 0; i < 10; ++i) {
            collectibles.push_back(SimpleVector3D(
                (rand() % 20) - 10,  // -10 to 10
                rand() % 5,          // 0 to 5
                (rand() % 20) - 10   // -10 to 10
            ));
        }
        
        std::cout << "3D World created with " << collectibles.size() << " collectibles!" << std::endl;
    }
    
    void processMovement(const std::string& input) {
        SimpleVector3D movement(0, 0, 0);
        
        for (char c : input) {
            switch (std::tolower(c)) {
                case 'w': movement.z += 2; break;  // Forward
                case 's': movement.z -= 2; break;  // Backward
                case 'a': movement.x -= 2; break;  // Left
                case 'd': movement.x += 2; break;  // Right
                case 'j': player.jump(); return;   // Jump
            }
        }
        
        if (movement.x != 0 || movement.z != 0) {
            player.move(movement);
            checkCollectibles();
        }
    }
    
    void checkCollectibles() {
        for (auto it = collectibles.begin(); it != collectibles.end(); ) {
            if (player.collectItem(*it)) {
                std::cout << "Collectible found at ";
                it->print();
                std::cout << "!" << std::endl;
                it = collectibles.erase(it);
            } else {
                ++it;
            }
        }
        
        if (collectibles.empty()) {
            std::cout << "🎉 All collectibles found! You win!" << std::endl;
        }
    }
    
    void displayWorld() const {
        std::cout << "\n=== 3D WORLD STATUS ===" << std::endl;
        player.displayStatus();
        std::cout << "Collectibles remaining: " << collectibles.size() << std::endl;
        
        if (!collectibles.empty()) {
            std::cout << "Nearest collectible at: ";
            double minDist = 1000;
            SimpleVector3D nearest;
            for (const auto& item : collectibles) {
                double dist = player.getPosition().distance(item);
                if (dist < minDist) {
                    minDist = dist;
                    nearest = item;
                }
            }
            nearest.print();
            std::cout << " (distance: " << minDist << ")" << std::endl;
        }
    }
    
    bool isGameComplete() const {
        return collectibles.empty();
    }
    
    int getScore() const {
        return player.getScore();
    }
};

int main() {
    std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                    3D WORLD EXPLORER                        ║" << std::endl;
    std::cout << "║              Navigate and Collect Items                     ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
    
    Simple3DWorld world;
    std::string input;
    
    std::cout << "\nControls:" << std::endl;
    std::cout << "W - Move Forward | S - Move Backward" << std::endl;
    std::cout << "A - Move Left    | D - Move Right" << std::endl;
    std::cout << "J - Jump         | Q - Quit" << std::endl;
    std::cout << "You can combine movements: 'wd' moves forward-right" << std::endl;
    
    while (!world.isGameComplete()) {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        world.displayWorld();
        std::cout << "\nEnter movement (WASD/J) or Q to quit: ";
        
        std::getline(std::cin, input);
        
        if (input == "q" || input == "Q") {
            break;
        }
        
        if (input == "status") {
            world.displayWorld();
            continue;
        }
        
        if (!input.empty()) {
            world.processMovement(input);
        }
    }
    
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                      GAME COMPLETE                          ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
    
    if (world.isGameComplete()) {
        std::cout << "🎉 Congratulations! You collected all items!" << std::endl;
    } else {
        std::cout << "Thanks for playing!" << std::endl;
    }
    
    std::cout << "Final Score: " << world.getScore() << std::endl;
    
    return 0;
}
