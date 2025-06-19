#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

int main() {
    std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║              EPOCH OF ELRIA GAME ENGINE                     ║" << std::endl;
    std::cout << "║                Creating Windowed Application                ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
    
    // Create HTML windowed application
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
            background: linear-gradient(135deg, #0c0c0c 0%, #1a1a2e 50%, #16213e 100%);
            font-family: 'Courier New', monospace;
            color: #ffffff;
            overflow: hidden;
            height: 100vh;
        }
        
        .game-container {
            position: relative;
            width: 100vw;
            height: 100vh;
            display: flex;
            flex-direction: column;
        }
        
        .header {
            background: rgba(0, 0, 0, 0.8);
            padding: 15px;
            text-align: center;
            border-bottom: 2px solid #4A90E2;
        }
        
        .title {
            font-size: 24px;
            font-weight: bold;
            color: #4A90E2;
            margin-bottom: 5px;
        }
        
        .mode {
            font-size: 16px;
            color: #FFD700;
        }
        
        .game-world {
            flex: 1;
            position: relative;
            background: radial-gradient(ellipse at center, #001122 0%, #000000 70%);
            overflow: hidden;
        }
        
        .stars {
            position: absolute;
            width: 100%;
            height: 100%;
            background-image: 
                radial-gradient(2px 2px at 20px 30px, #eee, transparent),
                radial-gradient(2px 2px at 40px 70px, rgba(255,255,255,0.8), transparent),
                radial-gradient(1px 1px at 90px 40px, #fff, transparent),
                radial-gradient(1px 1px at 130px 80px, rgba(255,255,255,0.6), transparent),
                radial-gradient(2px 2px at 160px 30px, #ddd, transparent);
            background-repeat: repeat;
            background-size: 200px 100px;
            animation: sparkle 3s linear infinite;
        }
        
        @keyframes sparkle {
            from { transform: translateY(0px); }
            to { transform: translateY(-100px); }
        }
        
        .earth {
            position: absolute;
            width: 120px;
            height: 120px;
            background: radial-gradient(circle at 30% 30%, #4A90E2, #2E5BBA, #1E3A8A);
            border-radius: 50%;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            animation: rotate 20s linear infinite;
            box-shadow: 0 0 30px rgba(74, 144, 226, 0.5);
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 40px;
            cursor: pointer;
        }
        
        @keyframes rotate {
            from { transform: translate(-50%, -50%) rotate(0deg); }
            to { transform: translate(-50%, -50%) rotate(360deg); }
        }
        
        .platform {
            position: absolute;
            width: 60px;
            height: 40px;
            background: linear-gradient(45deg, #8B4513, #A0522D);
            border: 2px solid #D2691E;
            border-radius: 8px;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 20px;
            cursor: pointer;
            transition: all 0.3s ease;
        }
        
        .platform:hover {
            transform: scale(1.1);
            box-shadow: 0 0 15px rgba(210, 105, 30, 0.7);
        }
        
        .crystal {
            position: absolute;
            width: 40px;
            height: 40px;
            background: radial-gradient(circle, #FFD700, #FFA500);
            border: 2px solid #FFFF00;
            border-radius: 50%;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 16px;
            cursor: pointer;
            animation: sparkle-crystal 2s ease-in-out infinite;
        }
        
        @keyframes sparkle-crystal {
            0%, 100% { 
                box-shadow: 0 0 10px #FFD700;
                transform: scale(1);
            }
            50% { 
                box-shadow: 0 0 25px #FFFF00, 0 0 35px #FFD700;
                transform: scale(1.05);
            }
        }
        
        .controls {
            background: rgba(0, 0, 0, 0.9);
            padding: 15px;
            display: flex;
            justify-content: space-between;
            align-items: center;
            border-top: 2px solid #4A90E2;
        }
        
        .control-group {
            display: flex;
            gap: 10px;
            align-items: center;
        }
        
        button {
            background: #4A90E2;
            color: white;
            border: none;
            padding: 10px 15px;
            border-radius: 5px;
            cursor: pointer;
            font-family: inherit;
            font-size: 14px;
            transition: background 0.3s ease;
        }
        
        button:hover {
            background: #357ABD;
        }
        
        .edit-mode {
            background: #FF6B35 !important;
        }
        
        .info-panel {
            position: absolute;
            top: 20px;
            right: 20px;
            background: rgba(0, 0, 0, 0.8);
            padding: 15px;
            border-radius: 10px;
            border: 2px solid #8B4513;
            min-width: 200px;
        }
        
        .camera-info {
            position: absolute;
            top: 20px;
            left: 20px;
            background: rgba(0, 0, 0, 0.8);
            padding: 15px;
            border-radius: 10px;
            border: 2px solid #FFD700;
        }
        
        .notification {
            position: fixed;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            background: rgba(0, 0, 0, 0.95);
            color: #FFD700;
            padding: 20px 30px;
            border-radius: 10px;
            border: 2px solid #FFD700;
            z-index: 1000;
            font-size: 18px;
            text-align: center;
            display: none;
        }
    </style>
</head>
<body>
    <div class="game-container">
        <div class="header">
            <div class="title">🌍 EPOCH OF ELRIA GAME ENGINE 🌍</div>
            <div class="mode" id="modeDisplay">▶️ PLAY MODE</div>
        </div>
        
        <div class="game-world">
            <div class="stars"></div>
            
            <div class="earth" onclick="selectObject('Earth')" title="Earth - The center of our universe">
                🌍
            </div>
            
            <!-- Platforms positioned around Earth -->
            <div class="platform" style="top: 30%; left: 70%;" onclick="selectObject('Platform_0')" title="Platform_0">🟫</div>
            <div class="platform" style="top: 45%; left: 80%;" onclick="selectObject('Platform_1')" title="Platform_1">🟫</div>
            <div class="platform" style="top: 65%; left: 75%;" onclick="selectObject('Platform_2')" title="Platform_2">🟫</div>
            <div class="platform" style="top: 70%; left: 30%;" onclick="selectObject('Platform_3')" title="Platform_3">🟫</div>
            <div class="platform" style="top: 55%; left: 20%;" onclick="selectObject('Platform_4')" title="Platform_4">🟫</div>
            <div class="platform" style="top: 35%; left: 25%;" onclick="selectObject('Platform_5')" title="Platform_5">🟫</div>
            
            <!-- Crystals -->
            <div class="crystal" style="top: 25%; left: 40%;" onclick="selectObject('Crystal_0')" title="Crystal_0">💎</div>
            <div class="crystal" style="top: 40%; left: 55%;" onclick="selectObject('Crystal_1')" title="Crystal_1">💎</div>
            <div class="crystal" style="top: 60%; left: 45%;" onclick="selectObject('Crystal_2')" title="Crystal_2">💎</div>
            <div class="crystal" style="top: 75%; left: 60%;" onclick="selectObject('Crystal_3')" title="Crystal_3">💎</div>
        </div>
        
        <div class="camera-info">
            <div><strong>📷 Camera</strong></div>
            <div>Position: (<span id="cameraX">0</span>, <span id="cameraY">8</span>, <span id="cameraZ">20</span>)</div>
            <div>Target: (0, 0, 0)</div>
        </div>
        
        <div class="info-panel">
            <div><strong>🎬 Scene Info</strong></div>
            <div>Earth Sphere Sandbox</div>
            <div>Objects: <span id="objectCount">11</span></div>
            <div>Selected: <span id="selectedObject">None</span></div>
        </div>
        
        <div class="controls">
            <div class="control-group">
                <span><strong>🎮 Movement:</strong></span>
                <button onclick="moveCamera('w')">W ↑</button>
                <button onclick="moveCamera('a')">A ←</button>
                <button onclick="moveCamera('s')">S ↓</button>
                <button onclick="moveCamera('d')">D →</button>
            </div>
            
            <div class="control-group">
                <button id="editBtn" onclick="toggleEditMode()">E - Edit Mode</button>
                <button onclick="createPlatform()">X - Create Platform</button>
                <button onclick="resetCamera()">Reset Camera</button>
            </div>
            
            <div class="control-group">
                <span>Camera Z: <span id="cameraZDisplay">20</span></span>
            </div>
        </div>
    </div>
    
    <div class="notification" id="notification"></div>

    <script>
        let gameState = {
            camera: { x: 0, y: 8, z: 20 },
            editMode: false,
            selectedObject: null,
            objectCount: 11
        };
        
        function updateCameraDisplay() {
            document.getElementById('cameraX').textContent = Math.round(gameState.camera.x);
            document.getElementById('cameraY').textContent = Math.round(gameState.camera.y);
            document.getElementById('cameraZ').textContent = Math.round(gameState.camera.z);
            document.getElementById('cameraZDisplay').textContent = Math.round(gameState.camera.z);
        }
        
        function moveCamera(direction) {
            const speed = 2;
            switch(direction) {
                case 'w': gameState.camera.z -= speed; break;
                case 's': gameState.camera.z += speed; break;
                case 'a': gameState.camera.x -= speed; break;
                case 'd': gameState.camera.x += speed; break;
            }
            updateCameraDisplay();
            showNotification(`📷 Camera moved ${direction.toUpperCase()}`);
        }
        
        function toggleEditMode() {
            gameState.editMode = !gameState.editMode;
            const modeDisplay = document.getElementById('modeDisplay');
            const editBtn = document.getElementById('editBtn');
            
            if (gameState.editMode) {
                modeDisplay.textContent = '🔧 EDIT MODE';
                modeDisplay.style.color = '#FF6B35';
                editBtn.textContent = 'E - Play Mode';
                editBtn.classList.add('edit-mode');
                showNotification('🔧 Edit Mode Activated! Click objects to modify them.');
            } else {
                modeDisplay.textContent = '▶️ PLAY MODE';
                modeDisplay.style.color = '#FFD700';
                editBtn.textContent = 'E - Edit Mode';
                editBtn.classList.remove('edit-mode');
                showNotification('▶️ Play Mode Activated! Explore the universe.');
            }
        }
        
        function createPlatform() {
            const gameWorld = document.querySelector('.game-world');
            const newPlatform = document.createElement('div');
            newPlatform.className = 'platform';
            newPlatform.style.top = Math.random() * 70 + 15 + '%';
            newPlatform.style.left = Math.random() * 70 + 15 + '%';
            newPlatform.innerHTML = '🟫';
            newPlatform.title = `Woven_${Date.now()}`;
            newPlatform.onclick = () => selectObject(newPlatform.title);
            
            gameWorld.appendChild(newPlatform);
            gameState.objectCount++;
            document.getElementById('objectCount').textContent = gameState.objectCount;
            
            showNotification('✨ Reality Weaving Activated! New platform created!');
        }
        
        function selectObject(objectName) {
            gameState.selectedObject = objectName;
            document.getElementById('selectedObject').textContent = objectName;
            
            if (gameState.editMode) {
                showNotification(`🔧 Selected: ${objectName} - Ready for editing!`);
            } else {
                showNotification(`👁️ Viewing: ${objectName}`);
            }
        }
        
        function resetCamera() {
            gameState.camera = { x: 0, y: 8, z: 20 };
            updateCameraDisplay();
            showNotification('📷 Camera reset to default position');
        }
        
        function showNotification(message) {
            const notification = document.getElementById('notification');
            notification.textContent = message;
            notification.style.display = 'block';
            
            setTimeout(() => {
                notification.style.display = 'none';
            }, 3000);
        }
        
        // Keyboard controls
        document.addEventListener('keydown', (e) => {
            switch(e.key.toLowerCase()) {
                case 'w': moveCamera('w'); break;
                case 'a': moveCamera('a'); break;
                case 's': moveCamera('s'); break;
                case 'd': moveCamera('d'); break;
                case 'e': toggleEditMode(); break;
                case 'x': createPlatform(); break;
                case ' ': resetCamera(); e.preventDefault(); break;
                case 'q': 
                    showNotification('👋 Thanks for using Epoch of Elria Game Engine!');
                    break;
            }
        });
        
        // Initialize
        updateCameraDisplay();
        
        // Welcome message
        setTimeout(() => {
            showNotification('🌍 Welcome to Epoch of Elria Game Engine! Use WASD to explore, E for Edit Mode, X to create platforms!');
        }, 1000);
    </script>
</body>
</html>)";
    
    htmlFile.close();
    
    std::cout << "✅ Windowed application created: EpochOfElriaEngine.html" << std::endl;
    std::cout << "🚀 Opening application window..." << std::endl;
    
    // Try different methods to open the window
    int result = system("explorer.exe EpochOfElriaEngine.html 2>/dev/null");
    if (result != 0) {
        result = system("cmd.exe /c start EpochOfElriaEngine.html 2>/dev/null");
        if (result != 0) {
            result = system("xdg-open EpochOfElriaEngine.html 2>/dev/null");
            if (result != 0) {
                std::cout << "⚠️  Could not auto-open. Please manually open: EpochOfElriaEngine.html" << std::endl;
            }
        }
    }
    
    std::cout << std::endl;
    std::cout << "🎮 Your windowed game engine should now be open!" << std::endl;
    std::cout << "🌟 Features:" << std::endl;
    std::cout << "  • Click objects to select them" << std::endl;
    std::cout << "  • Use W/A/S/D buttons or keyboard" << std::endl;
    std::cout << "  • Toggle Edit Mode with E button" << std::endl;
    std::cout << "  • Create platforms with X button" << std::endl;
    std::cout << "  • Real-time camera and object info" << std::endl;
    std::cout << std::endl;
    std::cout << "📁 File created: EpochOfElriaEngine.html" << std::endl;
    
    return 0;
}
