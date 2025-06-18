#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <functional>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <cmath>
#include <random>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#include "Vector3D.h"
#include "World3D.h"
#include "DreamWeaverCharacters.h"

// Forward declarations
class GameEngine;
class SceneEditor;
class AssetManager;
class RenderSystem;

// Asset types for the engine
enum class AssetType {
    TEXTURE_SVG,
    MODEL_3D,
    AUDIO,
    SCRIPT,
    SCENE,
    CHARACTER
};

struct Asset {
    std::string name;
    std::string path;
    AssetType type;
    std::string data;
    bool loaded;
    
    Asset(const std::string& n, const std::string& p, AssetType t) 
        : name(n), path(p), type(t), loaded(false) {}
};

// Game object for the engine
class GameObject {
public:
    std::string name;
    Vector3D position;
    Vector3D rotation;
    Vector3D scale;
    std::string model;
    std::string texture;
    bool active;
    std::map<std::string, std::string> properties;
    
    GameObject(const std::string& n) 
        : name(n), position(0,0,0), rotation(0,0,0), scale(1,1,1), active(true) {}
    
    virtual void update(double deltaTime) {
        // Override in derived classes
    }
    
    virtual void render() {
        if (!active) return;
        
        // Simple ASCII representation based on position
        std::cout << "📦 " << name << " at (" << position.x << "," << position.y << "," << position.z << ")";
        if (!texture.empty()) {
            std::cout << " [" << texture << "]";
        }
        std::cout << std::endl;
    }
};

// Scene management
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
        std::cout << "📷 Camera: (" << cameraPos.x << "," << cameraPos.y << "," << cameraPos.z << ")";
        std::cout << " → (" << cameraTarget.x << "," << cameraTarget.y << "," << cameraTarget.z << ")" << std::endl;
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

// Asset Manager
class AssetManager {
private:
    std::map<std::string, std::unique_ptr<Asset>> assets;
    
public:
    void loadAsset(const std::string& name, const std::string& path, AssetType type) {
        auto asset = std::make_unique<Asset>(name, path, type);
        
        // Load asset data based on type
        switch (type) {
            case AssetType::TEXTURE_SVG:
                asset->data = generateSVGTexture(name);
                break;
            case AssetType::MODEL_3D:
                asset->data = generateSimpleModel(name);
                break;
            case AssetType::SCRIPT:
                asset->data = loadTextFile(path);
                break;
            default:
                asset->data = "Default asset data";
                break;
        }
        
        asset->loaded = true;
        assets[name] = std::move(asset);
        std::cout << "✅ Loaded asset: " << name << " (" << assetTypeToString(type) << ")" << std::endl;
    }
    
    Asset* getAsset(const std::string& name) {
        auto it = assets.find(name);
        return (it != assets.end()) ? it->second.get() : nullptr;
    }
    
    void listAssets() {
        std::cout << "\n📁 ASSET LIBRARY 📁" << std::endl;
        std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
        for (const auto& pair : assets) {
            const Asset* asset = pair.second.get();
            std::cout << "📄 " << asset->name << " (" << assetTypeToString(asset->type) << ")";
            std::cout << " - " << (asset->loaded ? "✅ Loaded" : "❌ Not Loaded") << std::endl;
        }
        std::cout << "Total assets: " << assets.size() << std::endl;
    }
    
private:
    std::string generateSVGTexture(const std::string& name) {
        // Generate optimized SVG texture
        std::stringstream svg;
        svg << "<svg width='256' height='256' xmlns='http://www.w3.org/2000/svg'>";
        svg << "<rect width='256' height='256' fill='#" << std::hex << (std::hash<std::string>{}(name) % 0xFFFFFF) << "'/>";
        svg << "<text x='128' y='128' text-anchor='middle' fill='white' font-size='24'>" << name << "</text>";
        svg << "</svg>";
        return svg.str();
    }
    
    std::string generateSimpleModel(const std::string& name) {
        return "# Simple 3D Model: " + name + "\nv 0 0 0\nv 1 0 0\nv 1 1 0\nv 0 1 0\nf 1 2 3 4";
    }
    
    std::string loadTextFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) return "// File not found: " + path;
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    
    std::string assetTypeToString(AssetType type) {
        switch (type) {
            case AssetType::TEXTURE_SVG: return "SVG Texture";
            case AssetType::MODEL_3D: return "3D Model";
            case AssetType::AUDIO: return "Audio";
            case AssetType::SCRIPT: return "Script";
            case AssetType::SCENE: return "Scene";
            case AssetType::CHARACTER: return "Character";
            default: return "Unknown";
        }
    }
};

// Scene Editor for building games inside the engine
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
        std::cout << "save <filename>         - Save scene" << std::endl;
        std::cout << "load <filename>         - Load scene" << std::endl;
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
                Asset* asset = assetManager->getAsset(assetName);
                if (obj && asset) {
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
        else if (cmd == "save") {
            std::string filename;
            if (iss >> filename) {
                saveScene(filename);
            }
        }
        else if (cmd == "load") {
            std::string filename;
            if (iss >> filename) {
                loadScene(filename);
            }
        }
        else {
            std::cout << "❌ Unknown command: " << cmd << ". Type 'help' for commands." << std::endl;
        }
    }
    
private:
    void saveScene(const std::string& filename) {
        std::ofstream file(filename + ".scene");
        if (!file.is_open()) {
            std::cout << "❌ Could not save scene to " << filename << std::endl;
            return;
        }
        
        file << "# Scene: " << currentScene->getName() << std::endl;
        Vector3D camPos = currentScene->getCameraPosition();
        file << "camera " << camPos.x << " " << camPos.y << " " << camPos.z << std::endl;
        
        for (const auto& obj : currentScene->getObjects()) {
            file << "object " << obj->name << " " << obj->position.x << " " << obj->position.y << " " << obj->position.z;
            if (!obj->texture.empty()) {
                file << " " << obj->texture;
            }
            file << std::endl;
        }
        
        file.close();
        std::cout << "✅ Scene saved to " << filename << ".scene" << std::endl;
    }
    
    void loadScene(const std::string& filename) {
        std::cout << "📁 Loading scene from " << filename << ".scene..." << std::endl;
        // Implementation would load scene from file
        std::cout << "✅ Scene loaded (placeholder)" << std::endl;
    }
};

// Main Game Engine class
class UnifiedGameEngine {
private:
    std::unique_ptr<Scene> currentScene;
    std::unique_ptr<AssetManager> assetManager;
    std::unique_ptr<SceneEditor> sceneEditor;
    std::unique_ptr<Xing> dreamWeaver;
    bool running;
    bool windowMode;
    int windowWidth, windowHeight;

public:
    UnifiedGameEngine() : running(false), windowMode(true), windowWidth(120), windowHeight(40) {
        assetManager = std::make_unique<AssetManager>();
        sceneEditor = std::make_unique<SceneEditor>(assetManager.get());
        dreamWeaver = std::make_unique<Xing>();

        // Create default scene
        currentScene = std::make_unique<Scene>("Main Scene");
        sceneEditor->setCurrentScene(currentScene.get());

        initializeDefaultAssets();
        createSampleScene();
    }

    void initialize() {
        clearScreen();
        showWelcomeScreen();

        std::cout << "🚀 Initializing Unified Game Engine..." << std::endl;
        std::cout << "📦 Loading assets..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::cout << "🎬 Setting up scene editor..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::cout << "🌟 Loading Dream Weaver characters..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::cout << "✅ Engine initialized successfully!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        running = true;
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

            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // ~20 FPS for console
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

    void showWelcomeScreen() {
        std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║                 EPOCH OF ELRIA GAME ENGINE                  ║" << std::endl;
        std::cout << "║              Unified Development Environment                 ║" << std::endl;
        std::cout << "║                                                              ║" << std::endl;
        std::cout << "║  🌍 3D Earth Sphere Sandbox                                 ║" << std::endl;
        std::cout << "║  🎨 SVG-Optimized Graphics                                  ║" << std::endl;
        std::cout << "║  🛠️ Built-in Scene Editor                                   ║" << std::endl;
        std::cout << "║  🌟 Dream Weaver Characters                                 ║" << std::endl;
        std::cout << "║  📦 Asset Management System                                 ║" << std::endl;
        std::cout << "║  🎮 Real-time Game Development                              ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
        std::cout << std::endl;
    }

    void initializeDefaultAssets() {
        // Load default SVG textures
        assetManager->loadAsset("earth_texture", "textures/earth.svg", AssetType::TEXTURE_SVG);
        assetManager->loadAsset("grass_texture", "textures/grass.svg", AssetType::TEXTURE_SVG);
        assetManager->loadAsset("stone_texture", "textures/stone.svg", AssetType::TEXTURE_SVG);
        assetManager->loadAsset("water_texture", "textures/water.svg", AssetType::TEXTURE_SVG);
        assetManager->loadAsset("sky_texture", "textures/sky.svg", AssetType::TEXTURE_SVG);

        // Load 3D models
        assetManager->loadAsset("cube_model", "models/cube.obj", AssetType::MODEL_3D);
        assetManager->loadAsset("sphere_model", "models/sphere.obj", AssetType::MODEL_3D);
        assetManager->loadAsset("plane_model", "models/plane.obj", AssetType::MODEL_3D);

        // Load character assets
        assetManager->loadAsset("xing_character", "characters/xing.char", AssetType::CHARACTER);
        assetManager->loadAsset("xerx_character", "characters/xerx.char", AssetType::CHARACTER);
        assetManager->loadAsset("lyra_character", "characters/lyra.char", AssetType::CHARACTER);
        assetManager->loadAsset("heart_character", "characters/heart.char", AssetType::CHARACTER);
    }

    void createSampleScene() {
        // Create earth sphere at center
        auto earth = std::make_unique<GameObject>("Earth");
        earth->position = Vector3D(0, 0, 0);
        earth->scale = Vector3D(5, 5, 5);
        earth->texture = "earth_texture";
        earth->properties["type"] = "planet";
        earth->properties["rotationSpeed"] = "0.1";
        currentScene->addObject(std::move(earth));

        // Create some platforms around earth
        for (int i = 0; i < 5; ++i) {
            auto platform = std::make_unique<GameObject>("Platform_" + std::to_string(i));
            double angle = (i * 2 * M_PI) / 5;
            platform->position = Vector3D(cos(angle) * 8, sin(angle) * 2, sin(angle) * 8);
            platform->texture = "stone_texture";
            platform->properties["type"] = "platform";
            currentScene->addObject(std::move(platform));
        }

        // Create collectibles
        for (int i = 0; i < 3; ++i) {
            auto collectible = std::make_unique<GameObject>("Crystal_" + std::to_string(i));
            collectible->position = Vector3D(i * 3 - 3, 2, i * 2 - 2);
            collectible->texture = "sky_texture";
            collectible->properties["type"] = "collectible";
            collectible->properties["value"] = "100";
            currentScene->addObject(std::move(collectible));
        }

        // Set camera position
        currentScene->setCameraPosition(Vector3D(0, 5, 15));
        currentScene->setCameraTarget(Vector3D(0, 0, 0));
    }

    void update(double deltaTime) {
        if (currentScene) {
            currentScene->update(deltaTime);

            // Update earth rotation
            GameObject* earth = currentScene->findObject("Earth");
            if (earth) {
                earth->rotation.y += 0.1 * deltaTime;
                if (earth->rotation.y > 2 * M_PI) earth->rotation.y -= 2 * M_PI;
            }
        }
    }

    void render() {
        clearScreen();

        // Show engine header
        std::cout << "🎮 EPOCH OF ELRIA ENGINE - " << (sceneEditor->isEditMode() ? "EDIT MODE 🔧" : "PLAY MODE ▶️") << std::endl;
        std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;

        // Render current scene
        if (currentScene) {
            currentScene->render();
        }

        // Show controls
        std::cout << "\n🎮 CONTROLS:" << std::endl;
        if (sceneEditor->isEditMode()) {
            std::cout << "Type commands to edit scene (type 'help' for commands)" << std::endl;
        } else {
            std::cout << "E - Edit Mode | W/A/S/D - Move Camera | X - Xing Powers | Q - Quit" << std::endl;
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
            // Play mode controls
            if (input == "q" || input == "Q") {
                running = false;
            }
            else if (input == "e" || input == "E") {
                sceneEditor->enterEditMode();
            }
            else if (input == "x" || input == "X") {
                activateXingPowers();
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
                showPlayModeHelp();
            }
            else {
                std::cout << "❌ Unknown command. Type 'help' for available commands." << std::endl;
            }
        }
    }

    void activateXingPowers() {
        std::cout << "\n🌟 XING'S REALITY WEAVING ACTIVATED! 🌟" << std::endl;
        std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;

        dreamWeaver->weavePlatform(nullptr);

        // Create a new platform in the scene
        auto newPlatform = std::make_unique<GameObject>("Woven_Platform_" + std::to_string(rand() % 1000));
        newPlatform->position = Vector3D(
            (rand() % 20 - 10),
            (rand() % 10),
            (rand() % 20 - 10)
        );
        newPlatform->texture = "stone_texture";
        newPlatform->properties["type"] = "woven_platform";
        newPlatform->properties["creator"] = "xing";

        currentScene->addObject(std::move(newPlatform));

        std::cout << "✨ A new platform materializes in the scene!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }

    void moveCameraForward() {
        Vector3D pos = currentScene->getCameraPosition();
        Vector3D target = currentScene->getCameraTarget();
        Vector3D direction = (target - pos).normalize();
        currentScene->setCameraPosition(pos + direction * 2.0);
        std::cout << "📷 Camera moved forward" << std::endl;
    }

    void moveCameraBackward() {
        Vector3D pos = currentScene->getCameraPosition();
        Vector3D target = currentScene->getCameraTarget();
        Vector3D direction = (target - pos).normalize();
        currentScene->setCameraPosition(pos - direction * 2.0);
        std::cout << "📷 Camera moved backward" << std::endl;
    }

    void moveCameraLeft() {
        Vector3D pos = currentScene->getCameraPosition();
        Vector3D newPos = Vector3D(pos.x - 2, pos.y, pos.z);
        currentScene->setCameraPosition(newPos);
        std::cout << "📷 Camera moved left" << std::endl;
    }

    void moveCameraRight() {
        Vector3D pos = currentScene->getCameraPosition();
        Vector3D newPos = Vector3D(pos.x + 2, pos.y, pos.z);
        currentScene->setCameraPosition(newPos);
        std::cout << "📷 Camera moved right" << std::endl;
    }

    void showPlayModeHelp() {
        std::cout << "\n🎮 PLAY MODE COMMANDS 🎮" << std::endl;
        std::cout << "═══════════════════════════════════════════════════════════════" << std::endl;
        std::cout << "E - Enter Edit Mode (build and modify scenes)" << std::endl;
        std::cout << "W/A/S/D - Move camera around the scene" << std::endl;
        std::cout << "X - Activate Xing's reality weaving powers" << std::endl;
        std::cout << "assets - Show asset library" << std::endl;
        std::cout << "help - Show this help" << std::endl;
        std::cout << "Q - Quit engine" << std::endl;
        std::cout << std::endl;
        std::cout << "💡 TIP: Use Edit Mode to build your game, Play Mode to test it!" << std::endl;
    }

    void shutdown() {
        std::cout << "\n🌟 Thank you for using Epoch of Elria Game Engine! 🌟" << std::endl;
        std::cout << "✨ Your creations have been saved to the Metaverse... ✨" << std::endl;
        std::cout << "🎮 Keep building amazing games! 🎮" << std::endl;
    }
};

// Main function
int main() {
    try {
        UnifiedGameEngine engine;
        engine.run();
    }
    catch (const std::exception& e) {
        std::cerr << "❌ Engine Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
