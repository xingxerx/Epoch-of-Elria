#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <thread>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>

// For WSL - create an HTML5/JavaScript windowed version
class WSLWindowedEngine {
private:
    struct GameObject {
        std::string name;
        double x, y, z;
        double rotY;
        std::string type;
        std::string color;
        
        GameObject(const std::string& n, double px, double py, double pz, const std::string& t, const std::string& c)
            : name(n), x(px), y(py), z(pz), rotY(0), type(t), color(c) {}
    };
    
    std::vector<std::unique_ptr<GameObject>> objects;
    double cameraX, cameraY, cameraZ;
    bool editMode;
    bool running;
    
public:
    WSLWindowedEngine() : cameraX(0), cameraY(8), cameraZ(20), editMode(false), running(true) {
        createDefaultScene();
    }
    
    void createDefaultScene() {
        // Create Earth
        objects.push_back(std::make_unique<GameObject>("Earth", 0, 0, 0, "planet", "#4A90E2"));
        
        // Create platforms
        for (int i = 0; i < 6; ++i) {
            double angle = (i * 2 * M_PI) / 6;
            double x = cos(angle) * 8;
            double y = sin(angle) * 2;
            double z = sin(angle) * 8;
            
            std::string name = "Platform_" + std::to_string(i);
            objects.push_back(std::make_unique<GameObject>(name, x, y, z, "platform", "#8B4513"));
        }
        
        // Create crystals
        for (int i = 0; i < 4; ++i) {
            double x = i * 3 - 4.5;
            double y = 4;
            double z = i * 2 - 3;
            
            std::string name = "Crystal_" + std::to_string(i);
            objects.push_back(std::make_unique<GameObject>(name, x, y, z, "collectible", "#FFD700"));
        }
    }
    
    void generateHTML() {
        std::ofstream htmlFile("EpochOfElriaEngine.html");
        
        htmlFile << R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>🌍 Epoch of Elria - 3D Game Engine 🌍</title>
    <style>
        body {
            margin: 0;
            padding: 0;
            background: linear-gradient(to bottom, #000428, #004e92);
            font-family: 'Courier New', monospace;
            color: white;
            overflow: hidden;
        }
        
        #gameContainer {
            position: relative;
            width: 100vw;
            height: 100vh;
        }
        
        #canvas3d {
            position: absolute;
            top: 0;
            left: 0;
            background: radial-gradient(circle, #001122 0%, #000000 100%);
        }
        
        #ui {
            position: absolute;
            top: 20px;
            left: 20px;
            z-index: 100;
            background: rgba(0, 0, 0, 0.7);
            padding: 20px;
            border-radius: 10px;
            border: 2px solid #4A90E2;
        }
        
        #controls {
            position: absolute;
            bottom: 20px;
            left: 20px;
            z-index: 100;
            background: rgba(0, 0, 0, 0.7);
            padding: 15px;
            border-radius: 10px;
            border: 2px solid #FFD700;
        }
        
        #sceneInfo {
            position: absolute;
            top: 20px;
            right: 20px;
            z-index: 100;
            background: rgba(0, 0, 0, 0.7);
            padding: 15px;
            border-radius: 10px;
            border: 2px solid #8B4513;
        }
        
        .title {
            font-size: 24px;
            font-weight: bold;
            margin-bottom: 10px;
            text-align: center;
            color: #4A90E2;
        }
        
        .mode {
            font-size: 18px;
            margin-bottom: 10px;
            color: #FFD700;
        }
        
        .object {
            position: absolute;
            border-radius: 50%;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 20px;
            font-weight: bold;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.8);
            transition: all 0.3s ease;
            cursor: pointer;
        }
        
        .object:hover {
            transform: scale(1.2);
            box-shadow: 0 0 20px rgba(255, 255, 255, 0.5);
        }
        
        .planet {
            background: radial-gradient(circle, #4A90E2, #2E5BBA);
            border: 3px solid #87CEEB;
        }
        
        .platform {
            background: linear-gradient(45deg, #8B4513, #A0522D);
            border: 2px solid #D2691E;
            border-radius: 10px;
        }
        
        .collectible {
            background: radial-gradient(circle, #FFD700, #FFA500);
            border: 2px solid #FFFF00;
            animation: sparkle 2s infinite;
        }
        
        @keyframes sparkle {
            0%, 100% { box-shadow: 0 0 10px #FFD700; }
            50% { box-shadow: 0 0 30px #FFFF00, 0 0 40px #FFD700; }
        }
        
        @keyframes rotate {
            from { transform: rotate(0deg); }
            to { transform: rotate(360deg); }
        }
        
        .rotating {
            animation: rotate 10s linear infinite;
        }
        
        button {
            background: #4A90E2;
            color: white;
            border: none;
            padding: 10px 15px;
            margin: 5px;
            border-radius: 5px;
            cursor: pointer;
            font-family: inherit;
        }
        
        button:hover {
            background: #357ABD;
        }
        
        .edit-mode {
            background: #FF6B35 !important;
        }
    </style>
</head>
<body>
    <div id="gameContainer">
        <canvas id="canvas3d" width="1024" height="768"></canvas>
        
        <div id="ui">
            <div class="title">🌍 EPOCH OF ELRIA ENGINE</div>
            <div class="mode" id="modeDisplay">▶️ PLAY MODE</div>
            <div>📷 Camera: (<span id="cameraPos">0, 8, 20</span>)</div>
        </div>
        
        <div id="controls">
            <div style="margin-bottom: 10px;"><strong>🎮 CONTROLS:</strong></div>
            <div>W/A/S/D - Move Camera</div>
            <div>E - Edit Mode | X - Create Platform</div>
            <div>Space - Reset Camera | Q - Quit</div>
            <br>
            <button onclick="toggleEditMode()">Toggle Edit Mode</button>
            <button onclick="createPlatform()">Create Platform</button>
            <button onclick="resetCamera()">Reset Camera</button>
        </div>
        
        <div id="sceneInfo">
            <div><strong>🎬 SCENE: Earth Sphere Sandbox</strong></div>
            <div>Objects: <span id="objectCount">11</span></div>
            <div id="objectList"></div>
        </div>
        
        <div id="gameObjects"></div>
    </div>

    <script>
        class EpochOfElriaEngine {
            constructor() {
                this.objects = [)";
        
        // Add objects to JavaScript
        for (const auto& obj : objects) {
            htmlFile << "\n                    {name: '" << obj->name << "', x: " << obj->x 
                     << ", y: " << obj->y << ", z: " << obj->z 
                     << ", type: '" << obj->type << "', color: '" << obj->color << "'},";
        }
        
        htmlFile << R"(
                ];
                this.camera = {x: 0, y: 8, z: 20};
                this.editMode = false;
                this.running = true;
                this.setupEventListeners();
                this.render();
                this.startGameLoop();
            }
            
            setupEventListeners() {
                document.addEventListener('keydown', (e) => {
                    switch(e.key.toLowerCase()) {
                        case 'w': this.camera.z -= 2; break;
                        case 's': this.camera.z += 2; break;
                        case 'a': this.camera.x -= 2; break;
                        case 'd': this.camera.x += 2; break;
                        case 'q': this.camera.y += 2; break;
                        case 'e': this.toggleEditMode(); break;
                        case 'x': this.createPlatform(); break;
                        case ' ': this.resetCamera(); e.preventDefault(); break;
                    }
                    this.updateCameraDisplay();
                    this.render();
                });
            }
            
            toggleEditMode() {
                this.editMode = !this.editMode;
                document.getElementById('modeDisplay').textContent = 
                    this.editMode ? '🔧 EDIT MODE' : '▶️ PLAY MODE';
                document.getElementById('modeDisplay').className = 
                    this.editMode ? 'mode edit-mode' : 'mode';
            }
            
            createPlatform() {
                const newPlatform = {
                    name: `Woven_${Date.now()}`,
                    x: (Math.random() - 0.5) * 20,
                    y: Math.random() * 8 + 2,
                    z: (Math.random() - 0.5) * 20,
                    type: 'platform',
                    color: '#CD853F'
                };
                this.objects.push(newPlatform);
                this.updateObjectCount();
                this.render();
                
                // Show notification
                this.showNotification('✨ Reality Weaving Activated! New platform created!');
            }
            
            resetCamera() {
                this.camera = {x: 0, y: 8, z: 20};
                this.updateCameraDisplay();
                this.render();
            }
            
            updateCameraDisplay() {
                document.getElementById('cameraPos').textContent = 
                    `${Math.round(this.camera.x)}, ${Math.round(this.camera.y)}, ${Math.round(this.camera.z)}`;
            }
            
            updateObjectCount() {
                document.getElementById('objectCount').textContent = this.objects.length;
            }
            
            showNotification(message) {
                const notification = document.createElement('div');
                notification.style.cssText = `
                    position: fixed; top: 50%; left: 50%; transform: translate(-50%, -50%);
                    background: rgba(0,0,0,0.9); color: #FFD700; padding: 20px;
                    border-radius: 10px; border: 2px solid #FFD700; z-index: 1000;
                    font-size: 18px; text-align: center;
                `;
                notification.textContent = message;
                document.body.appendChild(notification);
                
                setTimeout(() => document.body.removeChild(notification), 3000);
            }
            
            render() {
                const container = document.getElementById('gameObjects');
                container.innerHTML = '';
                
                const centerX = window.innerWidth / 2;
                const centerY = window.innerHeight / 2;
                
                this.objects.forEach(obj => {
                    // Simple 3D to 2D projection
                    const distance = Math.sqrt(
                        Math.pow(obj.x - this.camera.x, 2) + 
                        Math.pow(obj.z - this.camera.z, 2)
                    ) + 1;
                    
                    const scale = 500 / (distance + 100);
                    const screenX = centerX + (obj.x - this.camera.x) * scale;
                    const screenY = centerY - (obj.y - this.camera.y) * scale;
                    
                    const element = document.createElement('div');
                    element.className = `object ${obj.type}`;
                    
                    let size, icon;
                    if (obj.type === 'planet') {
                        size = 60 * scale;
                        icon = '🌍';
                        element.classList.add('rotating');
                    } else if (obj.type === 'platform') {
                        size = 30 * scale;
                        icon = '🟫';
                    } else if (obj.type === 'collectible') {
                        size = 25 * scale;
                        icon = '💎';
                    }
                    
                    element.style.cssText += `
                        left: ${screenX - size/2}px;
                        top: ${screenY - size/2}px;
                        width: ${size}px;
                        height: ${size}px;
                        z-index: ${Math.round(1000 - distance)};
                    `;
                    
                    element.innerHTML = `${icon}<br><small>${obj.name}</small>`;
                    element.title = `${obj.name} at (${Math.round(obj.x)}, ${Math.round(obj.y)}, ${Math.round(obj.z)})`;
                    
                    container.appendChild(element);
                });
            }
            
            startGameLoop() {
                setInterval(() => {
                    if (this.running) {
                        // Rotate Earth
                        const earth = this.objects.find(obj => obj.type === 'planet');
                        if (earth) {
                            // Earth rotation is handled by CSS animation
                        }
                        this.render();
                    }
                }, 100);
            }
        }
        
        // Global functions for buttons
        function toggleEditMode() { engine.toggleEditMode(); }
        function createPlatform() { engine.createPlatform(); }
        function resetCamera() { engine.resetCamera(); }
        
        // Start the engine
        const engine = new EpochOfElriaEngine();
        
        // Welcome message
        setTimeout(() => {
            engine.showNotification('🌍 Welcome to Epoch of Elria Game Engine! 🌍\nUse WASD to explore, E for Edit Mode, X to create platforms!');
        }, 1000);
    </script>
</body>
</html>)";
        
        htmlFile.close();
        std::cout << "✅ HTML5 windowed application generated!" << std::endl;
    }
    
    void run() {
        std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║              EPOCH OF ELRIA GAME ENGINE                     ║" << std::endl;
        std::cout << "║                HTML5 Windowed Version                       ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
        std::cout << std::endl;
        std::cout << "🌍 Generating windowed application..." << std::endl;
        
        generateHTML();
        
        std::cout << "🚀 Opening application window..." << std::endl;
        std::cout << std::endl;
        
        // Try to open in browser (windowed application)
        system("explorer.exe EpochOfElriaEngine.html 2>/dev/null || xdg-open EpochOfElriaEngine.html 2>/dev/null || open EpochOfElriaEngine.html 2>/dev/null");
        
        std::cout << "✨ Windowed application should now be open in your browser!" << std::endl;
        std::cout << "🎮 If it didn't open automatically, open: EpochOfElriaEngine.html" << std::endl;
        std::cout << std::endl;
        std::cout << "🎮 Controls in the windowed app:" << std::endl;
        std::cout << "  • W/A/S/D - Move camera around the 3D scene" << std::endl;
        std::cout << "  • E - Toggle Edit Mode" << std::endl;
        std::cout << "  • X - Create new platforms" << std::endl;
        std::cout << "  • Space - Reset camera" << std::endl;
        std::cout << "  • Click buttons for easy control" << std::endl;
        std::cout << std::endl;
        std::cout << "🌟 Your 3D game engine is now running in a window!" << std::endl;
    }
};

int main() {
    WSLWindowedEngine engine;
    engine.run();
    return 0;
}
