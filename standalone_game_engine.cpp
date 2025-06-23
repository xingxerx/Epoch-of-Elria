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

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

// Simple Vector3D implementation
struct Vector3D {
    double x, y, z;
    
    Vector3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    
    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3D operator-(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }
    
    Vector3D operator*(double scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }
    
    double length() const {
        return sqrt(x*x + y*y + z*z);
    }
    
    Vector3D normalize() const {
        double len = length();
        if (len == 0) return Vector3D(0, 0, 0);
        return Vector3D(x/len, y/len, z/len);
    }
};

// Game Object class
class GameObject {
public:
    std::string name;
    Vector3D position;
    Vector3D rotation;
    Vector3D scale;
    std::string texture;
    bool active;
    std::map<std::string, std::string> properties;
    
    GameObject(const std::string& n) 
        : name(n), position(0,0,0), rotation(0,0,0), scale(1,1,1), active(true) {}
    
    virtual void update(double deltaTime) {
        // Basic rotation for demonstration
        if (properties["type"] == "planet") {
            rotation.y += 0.1 * deltaTime;
        }
    }
    
    virtual void render() {
        if (!active) return;
        
        std::string icon = "📦";
        if (properties["type"] == "planet") icon = "🌍";
        else if (properties["type"] == "platform") icon = "🟫";
        else if (properties["type"] == "collectible") icon = "💎";
        else if (properties["type"] == "woven_platform") icon = "✨";
        
        std::cout << icon << " " << name << " at (" 
                  << (int)position.x << "," << (int)position.y << "," << (int)position.z << ")";
        
        if (!texture.empty()) {
            std::cout << " [" << texture << "]";
        }
        
        if (properties["type"] == "planet") {
            std::cout << " (rotating " << (int)(rotation.y * 180 / M_PI) << "°)";
        }
        
        std::cout << std::endl;
    }
};

// Scene class
class Scene {
private:
    std::vector<std::unique_ptr<GameObject>> objects;
    std::string name;
    Vector3D cameraPos;
    Vector3D cameraTarget;
    
public:
    Scene(const std::string& sceneName) : name(sceneName), cameraPos(0,0,10), cameraTarget(0,0,0) {}
    
    void addObject(std::unique_ptr<GameObject> obj) {
        objects.push_back(std::move(obj));
    }
    
    void removeObject(const std::string& name) {
        objects.erase(
            std::remove_if(objects.begin(), objects.end(),
                [&name](const std::unique_ptr<GameObject>& obj) {
                    return obj->name == name;
                }),
            objects.end()
        );
    }
    
    GameObject* findObject(const std::string& name) {
        for (auto& obj : objects) {
            if (obj->name == name) return obj.get();
        }
        return nullptr;
    }
    
    void update(double deltaTime) {
        for (auto& obj : objects) {
            obj->update(deltaTime);
        }
    }
    
    void render() {
        std::cout << "\n🎬 SCENE: " << name << " 🎬" << std::endl;
        std::cout << "📷 Camera: (" << (int)cameraPos.x << "," << (int)cameraPos.y << "," << (int)cameraPos.z << ")";
        std::cout << " → (" << (int)cameraTarget.x << "," << (int)cameraTarget.y << "," << (int)cameraTarget.z << ")" << std::endl;
        std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
        
        for (auto& obj : objects) {
            obj->render();
        }
        
        std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
        std::cout << "Objects in scene: " << objects.size() << std::endl;
    }
    
    void setCameraPosition(const Vector3D& pos) { cameraPos = pos; }
    void setCameraTarget(const Vector3D& target) { cameraTarget = target; }
    Vector3D getCameraPosition() const { return cameraPos; }
    Vector3D getCameraTarget() const { return cameraTarget; }
    const std::vector<std::unique_ptr<GameObject>>& getObjects() const { return objects; }
    const std::string& getName() const { return name; }
};

// Simple Asset Manager
class AssetManager {
private:
    std::map<std::string, std::string> assets;
    
public:
    void loadAsset(const std::string& name, const std::string& type) {
        assets[name] = type;
        std::cout << "✅ Loaded asset: " << name << " (" << type << ")" << std::endl;
    }
    
    bool hasAsset(const std::string& name) {
        return assets.find(name) != assets.end();
    }
    
    void listAssets() {
        std::cout << "\n📁 ASSET LIBRARY 📁" << std::endl;
        std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
        for (const auto& pair : assets) {
            std::cout << "📄 " << pair.first << " (" << pair.second << ")" << std::endl;
        }
        std::cout << "Total assets: " << assets.size() << std::endl;
    }
};

// Scene Editor
class SceneEditor {
private:
    Scene* currentScene;
    AssetManager* assetManager;
    bool editMode;
    
public:
    SceneEditor(AssetManager* am) : currentScene(nullptr), assetManager(am), editMode(false) {}
    
    void setCurrentScene(Scene* scene) {
        currentScene = scene;
        std::cout << "🎬 Now editing scene: " << scene->getName() << std::endl;
    }
    
    void enterEditMode() {
        editMode = true;
        std::cout << "🔧 EDIT MODE ACTIVATED 🔧" << std::endl;
        showEditCommands();
    }
    
    void exitEditMode() {
        editMode = false;
        std::cout << "▶️ PLAY MODE ACTIVATED ▶️" << std::endl;
    }
    
    bool isEditMode() const { return editMode; }
    
    void showEditCommands() {
        std::cout << "\n🛠️ SCENE EDITOR COMMANDS 🛠️" << std::endl;
        std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
        std::cout << "add <name> <x> <y> <z>  - Add object at position" << std::endl;
        std::cout << "remove <name>           - Remove object" << std::endl;
        std::cout << "move <name> <x> <y> <z> - Move object" << std::endl;
        std::cout << "texture <name> <asset>  - Apply texture to object" << std::endl;
        std::cout << "camera <x> <y> <z>      - Set camera position" << std::endl;
        std::cout << "list                    - List all objects" << std::endl;
        std::cout << "assets                  - Show asset library" << std::endl;
        std::cout << "play                    - Exit edit mode" << std::endl;
        std::cout << "help                    - Show this help" << std::endl;
    }
    
    void processCommand(const std::string& command) {
        if (!currentScene) {
            std::cout << "❌ No scene loaded!" << std::endl;
            return;
        }
        
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
                std::cout << "✅ Added object: " << name << " at (" << x << "," << y << "," << z << ")" << std::endl;
            } else {
                std::cout << "❌ Usage: add <name> <x> <y> <z>" << std::endl;
            }
        }
        else if (cmd == "remove") {
            std::string name;
            if (iss >> name) {
                currentScene->removeObject(name);
                std::cout << "✅ Removed object: " << name << std::endl;
            }
        }
        else if (cmd == "move") {
            std::string name;
            float x, y, z;
            if (iss >> name >> x >> y >> z) {
                GameObject* obj = currentScene->findObject(name);
                if (obj) {
                    obj->position = Vector3D(x, y, z);
                    std::cout << "✅ Moved " << name << " to (" << x << "," << y << "," << z << ")" << std::endl;
                } else {
                    std::cout << "❌ Object not found: " << name << std::endl;
                }
            }
        }
        else if (cmd == "texture") {
            std::string objName, assetName;
            if (iss >> objName >> assetName) {
                GameObject* obj = currentScene->findObject(objName);
                if (obj && assetManager->hasAsset(assetName)) {
                    obj->texture = assetName;
                    std::cout << "✅ Applied texture " << assetName << " to " << objName << std::endl;
                } else {
                    std::cout << "❌ Object or asset not found!" << std::endl;
                }
            }
        }
        else if (cmd == "camera") {
            float x, y, z;
            if (iss >> x >> y >> z) {
                currentScene->setCameraPosition(Vector3D(x, y, z));
                std::cout << "✅ Camera moved to (" << x << "," << y << "," << z << ")" << std::endl;
            }
        }
        else if (cmd == "list") {
            currentScene->render();
        }
        else if (cmd == "assets") {
            assetManager->listAssets();
        }
        else if (cmd == "play") {
            exitEditMode();
        }
        else if (cmd == "help") {
            showEditCommands();
        }
        else {
            std::cout << "❌ Unknown command: " << cmd << ". Type 'help' for commands." << std::endl;
        }
    }
};

// Main Game Engine
class StandaloneGameEngine {
private:
    std::unique_ptr<Scene> currentScene;
    std::unique_ptr<AssetManager> assetManager;
    std::unique_ptr<SceneEditor> sceneEditor;
    bool running;

public:
    StandaloneGameEngine() : running(false) {
        assetManager = std::make_unique<AssetManager>();
        sceneEditor = std::make_unique<SceneEditor>(assetManager.get());

        // Create default scene
        currentScene = std::make_unique<Scene>("Earth Sphere Sandbox");
        sceneEditor->setCurrentScene(currentScene.get());

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
    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void initialize() {
        clearScreen();
        std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║                 EPOCH OF ELRIA GAME ENGINE                  ║" << std::endl;
        std::cout << "║              🌍 3D Earth Sphere Sandbox 🌍                  ║" << std::endl;
        std::cout << "║                                                              ║" << std::endl;
        std::cout << "║  🎨 SVG-Optimized Graphics Engine                           ║" << std::endl;
        std::cout << "║  🛠️ Built-in Scene Editor                                   ║" << std::endl;
        std::cout << "║  🌟 Dream Weaver Integration                                ║" << std::endl;
        std::cout << "║  📦 Real-time Asset Management                              ║" << std::endl;
        std::cout << "║  🎮 Interactive Game Development                            ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
        std::cout << std::endl;

        std::cout << "🚀 Initializing engine..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::cout << "📦 Loading SVG assets..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::cout << "🌍 Creating Earth sphere..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::cout << "✅ Engine ready!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        running = true;
    }

    void initializeAssets() {
        assetManager->loadAsset("earth_texture", "SVG Texture");
        assetManager->loadAsset("grass_texture", "SVG Texture");
        assetManager->loadAsset("stone_texture", "SVG Texture");
        assetManager->loadAsset("water_texture", "SVG Texture");
        assetManager->loadAsset("crystal_texture", "SVG Texture");
        assetManager->loadAsset("platform_texture", "SVG Texture");
        assetManager->loadAsset("sphere_model", "3D Model");
        assetManager->loadAsset("cube_model", "3D Model");
    }

    void createEarthScene() {
        // Create Earth at center
        auto earth = std::make_unique<GameObject>("Earth");
        earth->position = Vector3D(0, 0, 0);
        earth->scale = Vector3D(5, 5, 5);
        earth->texture = "earth_texture";
        earth->properties["type"] = "planet";
        currentScene->addObject(std::move(earth));

        // Create orbital platforms
        for (int i = 0; i < 6; ++i) {
            auto platform = std::make_unique<GameObject>("Platform_" + std::to_string(i));
            double angle = (i * 2 * M_PI) / 6;
            platform->position = Vector3D(cos(angle) * 10, sin(angle) * 3, sin(angle) * 10);
            platform->texture = "stone_texture";
            platform->properties["type"] = "platform";
            currentScene->addObject(std::move(platform));
        }

        // Create collectible crystals
        for (int i = 0; i < 4; ++i) {
            auto crystal = std::make_unique<GameObject>("Crystal_" + std::to_string(i));
            crystal->position = Vector3D(i * 4 - 6, 3, i * 3 - 4);
            crystal->texture = "crystal_texture";
            crystal->properties["type"] = "collectible";
            crystal->properties["value"] = "150";
            currentScene->addObject(std::move(crystal));
        }

        // Set initial camera
        currentScene->setCameraPosition(Vector3D(0, 8, 20));
        currentScene->setCameraTarget(Vector3D(0, 0, 0));
    }

    void update(double deltaTime) {
        if (currentScene) {
            currentScene->update(deltaTime);
        }
    }

    void render() {
        clearScreen();

        // Engine header
        std::cout << "🎮 EPOCH OF ELRIA ENGINE - " << (sceneEditor->isEditMode() ? "EDIT MODE 🔧" : "PLAY MODE ▶️") << std::endl;
        std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;

        // Render scene
        if (currentScene) {
            currentScene->render();
        }

        // Show controls
        std::cout << "\n🎮 CONTROLS:" << std::endl;
        if (sceneEditor->isEditMode()) {
            std::cout << "Type editor commands (type 'help' for list)" << std::endl;
        } else {
            std::cout << "E - Edit Mode | W/A/S/D - Move Camera | X - Create Platform | Q - Quit" << std::endl;
        }

        std::cout << "\n> ";
    }

    void handleInput() {
        std::string input;
        std::getline(std::cin, input);

        if (input.empty()) return;

        if (sceneEditor->isEditMode()) {
            sceneEditor->processCommand(input);
        } else {
            if (input == "q" || input == "Q") {
                running = false;
            }
            else if (input == "e" || input == "E") {
                sceneEditor->enterEditMode();
            }
            else if (input == "x" || input == "X") {
                createWovenPlatform();
            }
            else if (input == "w" || input == "W") {
                moveCameraForward();
            }
            else if (input == "s" || input == "S") {
                moveCameraBackward();
            }
            else if (input == "a" || input == "A") {
                moveCameraLeft();
            }
            else if (input == "d" || input == "D") {
                moveCameraRight();
            }
            else if (input == "assets") {
                assetManager->listAssets();
            }
            else if (input == "help") {
                showHelp();
            }
        }
    }

    void createWovenPlatform() {
        std::cout << "\n🌟 WEAVING REALITY... 🌟" << std::endl;

        auto platform = std::make_unique<GameObject>("Woven_" + std::to_string(rand() % 1000));
        platform->position = Vector3D(
            (rand() % 20 - 10),
            (rand() % 8 + 2),
            (rand() % 20 - 10)
        );
        platform->texture = "platform_texture";
        platform->properties["type"] = "woven_platform";

        currentScene->addObject(std::move(platform));

        std::cout << "✨ A new platform materializes from pure narrative energy!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }

    void moveCameraForward() {
        Vector3D pos = currentScene->getCameraPosition();
        Vector3D target = currentScene->getCameraTarget();
        Vector3D direction = (target - pos).normalize();
        currentScene->setCameraPosition(pos + direction * 3.0);
        std::cout << "📷 Camera moved forward" << std::endl;
    }

    void moveCameraBackward() {
        Vector3D pos = currentScene->getCameraPosition();
        Vector3D target = currentScene->getCameraTarget();
        Vector3D direction = (target - pos).normalize();
        currentScene->setCameraPosition(pos - direction * 3.0);
        std::cout << "📷 Camera moved backward" << std::endl;
    }

    void moveCameraLeft() {
        Vector3D pos = currentScene->getCameraPosition();
        currentScene->setCameraPosition(Vector3D(pos.x - 3, pos.y, pos.z));
        std::cout << "📷 Camera moved left" << std::endl;
    }

    void moveCameraRight() {
        Vector3D pos = currentScene->getCameraPosition();
        currentScene->setCameraPosition(Vector3D(pos.x + 3, pos.y, pos.z));
        std::cout << "📷 Camera moved right" << std::endl;
    }

    void showHelp() {
        std::cout << "\n🎮 GAME ENGINE HELP 🎮" << std::endl;
        std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
        std::cout << "PLAY MODE:" << std::endl;
        std::cout << "  E - Enter Edit Mode to build your game" << std::endl;
        std::cout << "  W/A/S/D - Move camera around the 3D scene" << std::endl;
        std::cout << "  X - Create a new platform using reality weaving" << std::endl;
        std::cout << "  assets - View all loaded assets" << std::endl;
        std::cout << "  Q - Quit the engine" << std::endl;
        std::cout << std::endl;
        std::cout << "EDIT MODE:" << std::endl;
        std::cout << "  add <name> <x> <y> <z> - Add new object" << std::endl;
        std::cout << "  move <name> <x> <y> <z> - Move existing object" << std::endl;
        std::cout << "  remove <name> - Remove object" << std::endl;
        std::cout << "  texture <object> <asset> - Apply texture" << std::endl;
        std::cout << "  camera <x> <y> <z> - Position camera" << std::endl;
        std::cout << "  play - Return to Play Mode" << std::endl;
        std::cout << std::endl;
        std::cout << "💡 Build your game in Edit Mode, test it in Play Mode!" << std::endl;
    }

    void shutdown() {
        std::cout << "\n🌟 Thank you for using Epoch of Elria Game Engine! 🌟" << std::endl;
        std::cout << "✨ Your 3D creations live on in the digital realm... ✨" << std::endl;
    }
};

int main() {
    try {
        StandaloneGameEngine engine;
        engine.run();
    }
    catch (const std::exception& e) {
        std::cerr << "❌ Engine Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
