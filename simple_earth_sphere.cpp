#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <chrono>
#include <thread>
#include <random>

#include "Vector3D.h"
#include "World3D.h"
#include "DreamWeaverCharacters.h"

class SimpleEarthSphere {
private:
    Vector3D position;
    double radius;
    double rotation;
    std::vector<std::string> landmasses;
    std::vector<std::string> oceans;
    
public:
    SimpleEarthSphere(const Vector3D& pos, double r) 
        : position(pos), radius(r), rotation(0.0) {
        
        // Initialize earth features using SVG-like descriptions
        landmasses = {
            "🌍 North America - Vast forests and plains",
            "🌎 South America - Amazon rainforest",
            "🌏 Europe - Ancient civilizations", 
            "🌍 Asia - Diverse landscapes",
            "🌎 Africa - Cradle of humanity",
            "🌏 Australia - Unique wildlife",
            "❄️ Antarctica - Frozen continent"
        };
        
        oceans = {
            "🌊 Pacific Ocean - Largest ocean",
            "🌊 Atlantic Ocean - Connects continents", 
            "🌊 Indian Ocean - Warm waters",
            "🌊 Arctic Ocean - Frozen seas",
            "🌊 Southern Ocean - Antarctic waters"
        };
    }
    
    void update(double deltaTime) {
        rotation += deltaTime * 0.1; // Slow earth rotation
        if (rotation > 2 * M_PI) rotation -= 2 * M_PI;
    }
    
    void render3D() {
        // Clear screen with space background
        std::cout << "\033[2J\033[H"; // Clear screen and move cursor to top
        
        // Draw space background
        std::cout << "🌌 ✨ 🌟 ⭐ 🌌 ✨ 🌟 ⭐ 🌌 ✨ 🌟 ⭐ 🌌 ✨ 🌟 ⭐" << std::endl;
        std::cout << "✨     EPOCH OF ELRIA - 3D EARTH SPHERE ENGINE     ✨" << std::endl;
        std::cout << "🌟 ⭐ 🌌 ✨ 🌟 ⭐ 🌌 ✨ 🌟 ⭐ 🌌 ✨ 🌟 ⭐ 🌌 ✨" << std::endl;
        std::cout << std::endl;
        
        // Draw earth sphere with rotation
        std::cout << "        🌍 EARTH SPHERE (Rotation: " << (int)(rotation * 180 / M_PI) << "°) 🌍" << std::endl;
        std::cout << "                    ╭─────────────╮" << std::endl;
        std::cout << "                 ╭─╯             ╰─╮" << std::endl;
        std::cout << "               ╭─╯    🌊 🏔️ 🌲    ╰─╮" << std::endl;
        std::cout << "             ╭─╯   🌊 🏜️ 🌴 🏔️ 🌊   ╰─╮" << std::endl;
        std::cout << "           ╭─╯  🌊 🏔️ 🌲 🏜️ 🌴 🏔️ 🌊  ╰─╮" << std::endl;
        std::cout << "          ╱   🌊 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🌊   ╲" << std::endl;
        std::cout << "         ╱  🌊 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌊  ╲" << std::endl;
        std::cout << "        ╱ 🌊 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🌊 ╲" << std::endl;
        std::cout << "       ╱🌊 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌊╲" << std::endl;
        std::cout << "      ╱🌊 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌊╲" << std::endl;
        std::cout << "     ╱ 🌊 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌊 ╲" << std::endl;
        std::cout << "    ╱  🌊 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌊  ╲" << std::endl;
        std::cout << "   ╱   🌊 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌊   ╲" << std::endl;
        std::cout << "  ╱    🌊 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌊    ╲" << std::endl;
        std::cout << " ╱      🌊 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌊      ╲" << std::endl;
        std::cout << "╱        🌊 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌊        ╲" << std::endl;
        std::cout << "╲        🌊 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌊        ╱" << std::endl;
        std::cout << " ╲      🌊 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌊      ╱" << std::endl;
        std::cout << "  ╲    🌊 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌊    ╱" << std::endl;
        std::cout << "   ╲   🌊 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌊   ╱" << std::endl;
        std::cout << "    ╲  🌊 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌊  ╱" << std::endl;
        std::cout << "     ╲ 🌊 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌊 ╱" << std::endl;
        std::cout << "      ╲🌊 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌊╱" << std::endl;
        std::cout << "       ╲🌊 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌊╱" << std::endl;
        std::cout << "        ╲ 🌊 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌊 ╱" << std::endl;
        std::cout << "         ╲  🌊 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌊  ╱" << std::endl;
        std::cout << "          ╲   🌊 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌊   ╱" << std::endl;
        std::cout << "           ╰─╮  🌊 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌊  ╭─╯" << std::endl;
        std::cout << "             ╰─╮   🌊 🏔️ 🌴 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️ 🌊   ╭─╯" << std::endl;
        std::cout << "               ╰─╮    🌊 🏜️ 🌲 🏔️ 🌴 🏜️ 🌲 🏔️    ╭─╯" << std::endl;
        std::cout << "                 ╰─╮             ╭─╯" << std::endl;
        std::cout << "                    ╰─────────────╯" << std::endl;
        std::cout << std::endl;
        
        // Show current visible features based on rotation
        int visibleIndex = (int)(rotation / (2 * M_PI / landmasses.size())) % landmasses.size();
        std::cout << "🌍 Currently Visible: " << landmasses[visibleIndex] << std::endl;
        std::cout << "🌊 Ocean View: " << oceans[visibleIndex % oceans.size()] << std::endl;
        std::cout << std::endl;
        
        // Show navigation info
        std::cout << "🎮 Controls: [W/S] Zoom | [A/D] Rotate | [Q] Quit | [SPACE] Explore" << std::endl;
        std::cout << "📍 Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
        std::cout << "🔄 Rotation: " << (int)(rotation * 180 / M_PI) << "° | Radius: " << radius << std::endl;
    }
    
    void zoom(double factor) {
        radius *= factor;
        if (radius < 1.0) radius = 1.0;
        if (radius > 10.0) radius = 10.0;
    }
    
    void rotate(double angle) {
        rotation += angle;
        if (rotation > 2 * M_PI) rotation -= 2 * M_PI;
        if (rotation < 0) rotation += 2 * M_PI;
    }
    
    void exploreCurrentRegion() {
        int regionIndex = (int)(rotation / (2 * M_PI / landmasses.size())) % landmasses.size();
        
        std::cout << "\n🔍 EXPLORING REGION..." << std::endl;
        std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
        std::cout << "📍 Region: " << landmasses[regionIndex] << std::endl;
        std::cout << "🌊 Adjacent Ocean: " << oceans[regionIndex % oceans.size()] << std::endl;
        std::cout << std::endl;
        
        // Generate some exploration content
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 4);
        
        switch(dis(gen)) {
            case 1:
                std::cout << "🏛️ You discover ancient ruins with mysterious inscriptions!" << std::endl;
                std::cout << "   The symbols seem to tell a story of the Dream Weavers..." << std::endl;
                break;
            case 2:
                std::cout << "🌲 A vast forest stretches before you, filled with bioluminescent plants!" << std::endl;
                std::cout << "   The trees whisper secrets of the old world..." << std::endl;
                break;
            case 3:
                std::cout << "🏔️ Towering mountains reveal hidden caves with crystal formations!" << std::endl;
                std::cout << "   Each crystal resonates with harmonic frequencies..." << std::endl;
                break;
            case 4:
                std::cout << "🌊 The ocean reveals underwater cities of an advanced civilization!" << std::endl;
                std::cout << "   Structures that defy conventional physics..." << std::endl;
                break;
        }
        
        std::cout << std::endl;
        std::cout << "Press any key to continue exploring..." << std::endl;
        std::cin.get();
    }
};

class EarthSphereGame {
private:
    SimpleEarthSphere earth;
    bool running;
    std::unique_ptr<Xing> xing;
    
public:
    EarthSphereGame() : earth(Vector3D(0, 0, 0), 5.0), running(true) {
        xing = std::make_unique<Xing>();
    }
    
    void run() {
        std::cout << "🌍 Initializing Earth Sphere 3D Engine..." << std::endl;
        std::cout << "🎮 Loading Dream Weaver characters..." << std::endl;
        std::cout << "✨ Optimizing SVG textures..." << std::endl;
        std::cout << "🚀 Engine ready!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        
        auto lastTime = std::chrono::high_resolution_clock::now();
        
        while (running) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            double deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
            lastTime = currentTime;
            
            earth.update(deltaTime);
            earth.render3D();
            
            // Simple input handling
            std::cout << "\nEnter command: ";
            char input;
            std::cin >> input;
            
            switch(input) {
                case 'w': case 'W':
                    earth.zoom(0.9);
                    break;
                case 's': case 'S':
                    earth.zoom(1.1);
                    break;
                case 'a': case 'A':
                    earth.rotate(-0.2);
                    break;
                case 'd': case 'D':
                    earth.rotate(0.2);
                    break;
                case ' ':
                    earth.exploreCurrentRegion();
                    break;
                case 'q': case 'Q':
                    running = false;
                    break;
                case 'x': case 'X':
                    // Activate Xing's abilities
                    std::cout << "\n🌟 Xing weaves reality around the Earth!" << std::endl;
                    xing->weavePlatform(nullptr);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    break;
                default:
                    std::cout << "Unknown command. Use W/S/A/D/SPACE/Q/X" << std::endl;
                    break;
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        std::cout << "\n🌍 Thank you for exploring the Earth Sphere!" << std::endl;
        std::cout << "✨ The Dream Weavers' story continues..." << std::endl;
    }
};

int main() {
    std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║              EPOCH OF ELRIA - EARTH SPHERE 3D               ║" << std::endl;
    std::cout << "║                SVG-Optimized Game Engine                    ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
    
    EarthSphereGame game;
    game.run();
    
    return 0;
}
