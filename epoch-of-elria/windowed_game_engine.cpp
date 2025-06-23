#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <chrono>
#include <thread>
#include <sstream>
#include <cmath>
#include <random>
#include <algorithm>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif

// Simple Vector3D implementation
struct Vector3D {
    double x, y, z;
    Vector3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    Vector3D operator+(const Vector3D& other) const { return Vector3D(x + other.x, y + other.y, z + other.z); }
    Vector3D operator-(const Vector3D& other) const { return Vector3D(x - other.x, y - other.y, z - other.z); }
    Vector3D operator*(double scalar) const { return Vector3D(x * scalar, y * scalar, z * scalar); }
    double length() const { return sqrt(x*x + y*y + z*z); }
    Vector3D normalize() const { double len = length(); return len == 0 ? Vector3D(0,0,0) : Vector3D(x/len, y/len, z/len); }
};

// Game Object class
class GameObject {
public:
    std::string name;
    Vector3D position, rotation, scale;
    std::string texture;
    bool active;
    std::map<std::string, std::string> properties;
    
    GameObject(const std::string& n) : name(n), position(0,0,0), rotation(0,0,0), scale(1,1,1), active(true) {}
    
    virtual void update(double deltaTime) {
        if (properties["type"] == "planet") {
            rotation.y += 0.5 * deltaTime;
            if (rotation.y > 2 * M_PI) rotation.y -= 2 * M_PI;
        }
    }
    
    virtual std::string render() {
        if (!active) return "";
        
        std::string icon = "📦";
        if (properties["type"] == "planet") icon = "🌍";
        else if (properties["type"] == "platform") icon = "🟫";
        else if (properties["type"] == "collectible") icon = "💎";
        else if (properties["type"] == "woven_platform") icon = "✨";
        
        std::ostringstream oss;
        oss << icon << " " << name << " at (" << (int)position.x << "," << (int)position.y << "," << (int)position.z << ")";
        
        if (!texture.empty()) oss << " [" << texture << "]";
        if (properties["type"] == "planet") oss << " (rotating " << (int)(rotation.y * 180 / M_PI) << "°)";
        
        return oss.str();
    }
};

// Window Manager for cross-platform windowing
class WindowManager {
private:
    int width, height;
    std::string title;
    
public:
    WindowManager(int w = 120, int h = 40, const std::string& t = "Epoch of Elria Game Engine") 
        : width(w), height(h), title(t) {}
    
    void initialize() {
        clearScreen();
        setWindowTitle();
        resizeWindow();
    }
    
    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
    
    void setWindowTitle() {
#ifdef _WIN32
        SetConsoleTitle(title.c_str());
#else
        std::cout << "\033]0;" << title << "\007";
#endif
    }
    
    void resizeWindow() {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD bufferSize = {(SHORT)width, (SHORT)height};
        SMALL_RECT windowSize = {0, 0, (SHORT)(width-1), (SHORT)(height-1)};
        SetConsoleScreenBufferSize(hOut, bufferSize);
        SetConsoleWindowInfo(hOut, TRUE, &windowSize);
#else
        // Linux terminal resizing (best effort)
        std::cout << "\033[8;" << height << ";" << width << "t";
#endif
    }
    
    void drawBorder() {
        std::cout << "╔";
        for (int i = 0; i < width-2; ++i) std::cout << "═";
        std::cout << "╗\n";
        
        for (int i = 0; i < height-2; ++i) {
            std::cout << "║";
            for (int j = 0; j < width-2; ++j) std::cout << " ";
            std::cout << "║\n";
        }
        
        std::cout << "╚";
        for (int i = 0; i < width-2; ++i) std::cout << "═";
        std::cout << "╝\n";
    }
    
    void setCursorPosition(int x, int y) {
#ifdef _WIN32
        COORD coord = {(SHORT)x, (SHORT)y};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
        std::cout << "\033[" << (y+1) << ";" << (x+1) << "H";
#endif
    }
    
    void hideCursor() {
#ifdef _WIN32
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
#else
        std::cout << "\033[?25l";
#endif
    }
    
    void showCursor() {
#ifdef _WIN32
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
        cursorInfo.bVisible = true;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
#else
        std::cout << "\033[?25h";
#endif
    }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

// Scene class
class Scene {
private:
    std::vector<std::unique_ptr<GameObject>> objects;
    std::string name;
    Vector3D cameraPos, cameraTarget;
    
public:
    Scene(const std::string& sceneName) : name(sceneName), cameraPos(0,8,20), cameraTarget(0,0,0) {}
    
    void addObject(std::unique_ptr<GameObject> obj) { objects.push_back(std::move(obj)); }
    
    void removeObject(const std::string& name) {
        objects.erase(std::remove_if(objects.begin(), objects.end(),
            [&name](const std::unique_ptr<GameObject>& obj) { return obj->name == name; }), objects.end());
    }
    
    GameObject* findObject(const std::string& name) {
        for (auto& obj : objects) {
            if (obj->name == name) return obj.get();
        }
        return nullptr;
    }
    
    void update(double deltaTime) {
        for (auto& obj : objects) obj->update(deltaTime);
    }
    
    std::vector<std::string> render() {
        std::vector<std::string> lines;
        lines.push_back("🎬 SCENE: " + name + " 🎬");
        lines.push_back("📷 Camera: (" + std::to_string((int)cameraPos.x) + "," + 
                       std::to_string((int)cameraPos.y) + "," + std::to_string((int)cameraPos.z) + 
                       ") → (" + std::to_string((int)cameraTarget.x) + "," + 
                       std::to_string((int)cameraTarget.y) + "," + std::to_string((int)cameraTarget.z) + ")");
        lines.push_back("═══════════════════════════════════════════════════════════════");
        
        for (auto& obj : objects) {
            std::string objLine = obj->render();
            if (!objLine.empty()) lines.push_back(objLine);
        }
        
        lines.push_back("═══════════════════════════════════════════════════════════════");
        lines.push_back("Objects in scene: " + std::to_string(objects.size()));
        
        return lines;
    }
    
    void setCameraPosition(const Vector3D& pos) { cameraPos = pos; }
    void setCameraTarget(const Vector3D& target) { cameraTarget = target; }
    Vector3D getCameraPosition() const { return cameraPos; }
    Vector3D getCameraTarget() const { return cameraTarget; }
    const std::string& getName() const { return name; }
    size_t getObjectCount() const { return objects.size(); }
};

// Windowed Game Engine
class WindowedGameEngine {
private:
    std::unique_ptr<WindowManager> window;
    std::unique_ptr<Scene> currentScene;
    bool running, editMode;
    std::map<std::string, std::string> assets;
    
public:
    WindowedGameEngine() : running(false), editMode(false) {
        window = std::make_unique<WindowManager>(100, 30, "🌍 Epoch of Elria - 3D Game Engine 🌍");
        currentScene = std::make_unique<Scene>("Earth Sphere Sandbox");
        initializeAssets();
        createEarthScene();
    }
    
    void run() {
        initialize();
        
        auto lastTime = std::chrono::high_resolution_clock::now();
        
        while (running) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            double deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
            lastTime = currentTime;
            
            update(deltaTime);
            render();
            handleInput();
            
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        shutdown();
    }
    
private:
    void initialize() {
        window->initialize();
        window->hideCursor();
        
        showSplashScreen();
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        
        running = true;
    }
    
    void showSplashScreen() {
        window->clearScreen();
        window->setCursorPosition(10, 5);
        std::cout << "╔══════════════════════════════════════════════════════════════╗";
        window->setCursorPosition(10, 6);
        std::cout << "║                 EPOCH OF ELRIA GAME ENGINE                  ║";
        window->setCursorPosition(10, 7);
        std::cout << "║              🌍 3D Earth Sphere Sandbox 🌍                  ║";
        window->setCursorPosition(10, 8);
        std::cout << "║                                                              ║";
        window->setCursorPosition(10, 9);
        std::cout << "║  🎨 SVG-Optimized Graphics Engine                           ║";
        window->setCursorPosition(10, 10);
        std::cout << "║  🛠️ Built-in Scene Editor                                   ║";
        window->setCursorPosition(10, 11);
        std::cout << "║  🌟 Dream Weaver Integration                                ║";
        window->setCursorPosition(10, 12);
        std::cout << "║  📦 Real-time Asset Management                              ║";
        window->setCursorPosition(10, 13);
        std::cout << "║  🎮 Interactive Game Development                            ║";
        window->setCursorPosition(10, 14);
        std::cout << "╚══════════════════════════════════════════════════════════════╝";
        
        window->setCursorPosition(25, 16);
        std::cout << "🚀 Initializing engine...";
        std::cout.flush();
    }
    
    void initializeAssets() {
        assets["earth_texture"] = "SVG Earth Texture";
        assets["stone_texture"] = "SVG Stone Texture";
        assets["crystal_texture"] = "SVG Crystal Texture";
        assets["platform_texture"] = "SVG Platform Texture";
    }
    
    void createEarthScene() {
        // Create Earth
        auto earth = std::make_unique<GameObject>("Earth");
        earth->position = Vector3D(0, 0, 0);
        earth->scale = Vector3D(5, 5, 5);
        earth->texture = "earth_texture";
        earth->properties["type"] = "planet";
        currentScene->addObject(std::move(earth));
        
        // Create platforms
        for (int i = 0; i < 6; ++i) {
            auto platform = std::make_unique<GameObject>("Platform_" + std::to_string(i));
            double angle = (i * 2 * M_PI) / 6;
            platform->position = Vector3D(cos(angle) * 10, sin(angle) * 3, sin(angle) * 10);
            platform->texture = "stone_texture";
            platform->properties["type"] = "platform";
            currentScene->addObject(std::move(platform));
        }
        
        // Create crystals
        for (int i = 0; i < 4; ++i) {
            auto crystal = std::make_unique<GameObject>("Crystal_" + std::to_string(i));
            crystal->position = Vector3D(i * 4 - 6, 3, i * 3 - 4);
            crystal->texture = "crystal_texture";
            crystal->properties["type"] = "collectible";
            currentScene->addObject(std::move(crystal));
        }
    }
    
    void update(double deltaTime) {
        if (currentScene) currentScene->update(deltaTime);
    }
    
    void render() {
        window->clearScreen();
        
        // Header
        window->setCursorPosition(2, 1);
        std::cout << "🎮 EPOCH OF ELRIA ENGINE - " << (editMode ? "EDIT MODE 🔧" : "PLAY MODE ▶️");
        
        // Scene content
        auto sceneLines = currentScene->render();
        int startY = 3;
        for (size_t i = 0; i < sceneLines.size() && i < 20; ++i) {
            window->setCursorPosition(2, startY + i);
            std::cout << sceneLines[i];
        }
        
        // Controls
        window->setCursorPosition(2, 25);
        std::cout << "🎮 CONTROLS:";
        window->setCursorPosition(2, 26);
        if (editMode) {
            std::cout << "Type editor commands (add/move/remove/texture/camera/play/help)";
        } else {
            std::cout << "E - Edit Mode | W/A/S/D - Move Camera | X - Create Platform | Q - Quit";
        }
        
        window->setCursorPosition(2, 28);
        std::cout << "> ";
        window->showCursor();
        std::cout.flush();
    }

    void handleInput() {
        std::string input;
        std::getline(std::cin, input);

        if (input.empty()) return;

        window->hideCursor();

        if (editMode) {
            processEditCommand(input);
        } else {
            processPlayCommand(input);
        }
    }

    void processPlayCommand(const std::string& input) {
        if (input == "q" || input == "Q") {
            running = false;
        }
        else if (input == "e" || input == "E") {
            editMode = true;
        }
        else if (input == "x" || input == "X") {
            createWovenPlatform();
        }
        else if (input == "w" || input == "W") {
            Vector3D pos = currentScene->getCameraPosition();
            Vector3D target = currentScene->getCameraTarget();
            Vector3D direction = (target - pos).normalize();
            currentScene->setCameraPosition(pos + direction * 3.0);
        }
        else if (input == "s" || input == "S") {
            Vector3D pos = currentScene->getCameraPosition();
            Vector3D target = currentScene->getCameraTarget();
            Vector3D direction = (target - pos).normalize();
            currentScene->setCameraPosition(pos - direction * 3.0);
        }
        else if (input == "a" || input == "A") {
            Vector3D pos = currentScene->getCameraPosition();
            currentScene->setCameraPosition(Vector3D(pos.x - 3, pos.y, pos.z));
        }
        else if (input == "d" || input == "D") {
            Vector3D pos = currentScene->getCameraPosition();
            currentScene->setCameraPosition(Vector3D(pos.x + 3, pos.y, pos.z));
        }
    }

    void processEditCommand(const std::string& command) {
        std::istringstream iss(command);
        std::string cmd;
        iss >> cmd;

        if (cmd == "add") {
            std::string name;
            float x, y, z;
            if (iss >> name >> x >> y >> z) {
                auto obj = std::make_unique<GameObject>(name);
                obj->position = Vector3D(x, y, z);
                obj->properties["type"] = "custom";
                currentScene->addObject(std::move(obj));
            }
        }
        else if (cmd == "move") {
            std::string name;
            float x, y, z;
            if (iss >> name >> x >> y >> z) {
                GameObject* obj = currentScene->findObject(name);
                if (obj) obj->position = Vector3D(x, y, z);
            }
        }
        else if (cmd == "remove") {
            std::string name;
            if (iss >> name) {
                currentScene->removeObject(name);
            }
        }
        else if (cmd == "texture") {
            std::string objName, assetName;
            if (iss >> objName >> assetName) {
                GameObject* obj = currentScene->findObject(objName);
                if (obj && assets.find(assetName) != assets.end()) {
                    obj->texture = assetName;
                }
            }
        }
        else if (cmd == "camera") {
            float x, y, z;
            if (iss >> x >> y >> z) {
                currentScene->setCameraPosition(Vector3D(x, y, z));
            }
        }
        else if (cmd == "play") {
            editMode = false;
        }
    }

    void createWovenPlatform() {
        auto platform = std::make_unique<GameObject>("Woven_" + std::to_string(rand() % 1000));
        platform->position = Vector3D((rand() % 20 - 10), (rand() % 8 + 2), (rand() % 20 - 10));
        platform->texture = "platform_texture";
        platform->properties["type"] = "woven_platform";
        currentScene->addObject(std::move(platform));
    }

    void shutdown() {
        window->showCursor();
        window->clearScreen();
        window->setCursorPosition(20, 10);
        std::cout << "🌟 Thank you for using Epoch of Elria Game Engine! 🌟";
        window->setCursorPosition(20, 11);
        std::cout << "✨ Your 3D creations live on in the digital realm... ✨";
        window->setCursorPosition(0, 15);
    }
};

int main() {
    try {
        WindowedGameEngine engine;
        engine.run();
    }
    catch (const std::exception& e) {
        std::cerr << "❌ Engine Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
