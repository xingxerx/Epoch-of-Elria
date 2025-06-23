#include <windows.h>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <thread>
#include <sstream>
#include <cmath>

// Simple windowed game engine using Windows API only
class SimpleWindowedEngine {
private:
    HWND hwnd;
    HDC hdc;
    HBRUSH backgroundBrush;
    bool running;
    bool editMode;
    int windowWidth, windowHeight;
    
    // Game state
    struct GameObject {
        std::string name;
        int x, y, z;
        std::string type;
        COLORREF color;
        
        GameObject(const std::string& n, int px, int py, int pz, const std::string& t, COLORREF c)
            : name(n), x(px), y(py), z(pz), type(t), color(c) {}
    };
    
    std::vector<std::unique_ptr<GameObject>> objects;
    int cameraX, cameraY, cameraZ;
    
public:
    SimpleWindowedEngine() : hwnd(nullptr), hdc(nullptr), running(false), editMode(false),
                            windowWidth(1024), windowHeight(768), cameraX(0), cameraY(0), cameraZ(20) {
        backgroundBrush = CreateSolidBrush(RGB(5, 5, 25)); // Dark space background
        createDefaultScene();
    }
    
    bool initialize(HINSTANCE hInstance) {
        // Register window class
        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = L"EpochOfElriaEngine";
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = backgroundBrush;
        wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        
        if (!RegisterClass(&wc)) return false;
        
        // Create window
        hwnd = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            L"EpochOfElriaEngine",
            L"🌍 Epoch of Elria - 3D Game Engine 🌍",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT,
            windowWidth, windowHeight,
            nullptr, nullptr, hInstance, this
        );
        
        if (!hwnd) return false;
        
        hdc = GetDC(hwnd);
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
        
        return true;
    }
    
    void createDefaultScene() {
        // Create Earth
        objects.push_back(std::make_unique<GameObject>("Earth", 0, 0, 0, "planet", RGB(50, 150, 200)));
        
        // Create platforms
        for (int i = 0; i < 6; ++i) {
            double angle = (i * 2 * M_PI) / 6;
            int x = (int)(cos(angle) * 150);
            int z = (int)(sin(angle) * 150);
            int y = (int)(sin(angle) * 30);
            
            std::string name = "Platform_" + std::to_string(i);
            objects.push_back(std::make_unique<GameObject>(name, x, y, z, "platform", RGB(150, 100, 50)));
        }
        
        // Create crystals
        for (int i = 0; i < 4; ++i) {
            int x = i * 80 - 120;
            int y = 60;
            int z = i * 40 - 60;
            
            std::string name = "Crystal_" + std::to_string(i);
            objects.push_back(std::make_unique<GameObject>(name, x, y, z, "collectible", RGB(255, 200, 50)));
        }
    }
    
    void run() {
        running = true;
        
        auto lastTime = std::chrono::high_resolution_clock::now();
        
        while (running) {
            MSG msg;
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            
            auto currentTime = std::chrono::high_resolution_clock::now();
            double deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
            lastTime = currentTime;
            
            update(deltaTime);
            render();
            
            Sleep(16); // ~60 FPS
        }
    }
    
    void update(double deltaTime) {
        // Rotate Earth
        static double earthRotation = 0;
        earthRotation += deltaTime * 50; // degrees per second
        if (earthRotation > 360) earthRotation -= 360;
        
        // Update Earth object rotation (for display purposes)
        if (!objects.empty() && objects[0]->type == "planet") {
            // Earth rotation is handled in rendering
        }
    }
    
    void render() {
        RECT rect;
        GetClientRect(hwnd, &rect);
        
        // Clear background
        FillRect(hdc, &rect, backgroundBrush);
        
        // Set text color to white
        SetTextColor(hdc, RGB(255, 255, 255));
        SetBkMode(hdc, TRANSPARENT);
        
        // Render title
        RECT titleRect = {10, 10, rect.right - 10, 50};
        DrawText(hdc, L"🌍 EPOCH OF ELRIA - 3D GAME ENGINE 🌍", -1, &titleRect, DT_LEFT);
        
        // Render mode
        RECT modeRect = {10, 35, rect.right - 10, 60};
        std::wstring mode = editMode ? L"🔧 EDIT MODE - Build your game!" : L"▶️ PLAY MODE - Explore the universe!";
        DrawText(hdc, mode.c_str(), -1, &modeRect, DT_LEFT);
        
        // Render scene info
        RECT sceneRect = {10, 60, rect.right - 10, 85};
        std::wstring sceneInfo = L"🎬 Scene: Earth Sphere Sandbox | Objects: " + std::to_wstring(objects.size());
        DrawText(hdc, sceneInfo.c_str(), -1, &sceneRect, DT_LEFT);
        
        // Render camera info
        RECT cameraRect = {10, 85, rect.right - 10, 110};
        std::wstring cameraInfo = L"📷 Camera: (" + std::to_wstring(cameraX) + L", " + 
                                 std::to_wstring(cameraY) + L", " + std::to_wstring(cameraZ) + L")";
        DrawText(hdc, cameraInfo.c_str(), -1, &cameraRect, DT_LEFT);
        
        // Render 3D objects (pseudo-3D projection)
        int centerX = rect.right / 2;
        int centerY = rect.bottom / 2;
        
        for (const auto& obj : objects) {
            // Simple 3D to 2D projection
            double distance = sqrt((obj->x - cameraX) * (obj->x - cameraX) + 
                                 (obj->z - cameraZ) * (obj->z - cameraZ));
            
            if (distance < 1) distance = 1; // Avoid division by zero
            
            double scale = 500.0 / (distance + 100); // Perspective scaling
            
            int screenX = centerX + (int)((obj->x - cameraX) * scale);
            int screenY = centerY - (int)((obj->y - cameraY) * scale);
            
            // Create brush for object
            HBRUSH objBrush = CreateSolidBrush(obj->color);
            HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, objBrush);
            
            // Render based on type
            if (obj->type == "planet") {
                // Draw Earth as a large circle
                int radius = (int)(30 * scale);
                Ellipse(hdc, screenX - radius, screenY - radius, screenX + radius, screenY + radius);
                
                // Add Earth label
                RECT labelRect = {screenX - 30, screenY + radius + 5, screenX + 30, screenY + radius + 25};
                DrawText(hdc, L"🌍 Earth", -1, &labelRect, DT_CENTER);
            }
            else if (obj->type == "platform") {
                // Draw platform as rectangle
                int size = (int)(15 * scale);
                Rectangle(hdc, screenX - size, screenY - size, screenX + size, screenY + size);
                
                // Add platform label
                RECT labelRect = {screenX - 25, screenY + size + 2, screenX + 25, screenY + size + 15};
                DrawText(hdc, L"🟫", -1, &labelRect, DT_CENTER);
            }
            else if (obj->type == "collectible") {
                // Draw crystal as diamond
                POINT diamond[4] = {
                    {screenX, screenY - (int)(10 * scale)},
                    {screenX + (int)(8 * scale), screenY},
                    {screenX, screenY + (int)(10 * scale)},
                    {screenX - (int)(8 * scale), screenY}
                };
                Polygon(hdc, diamond, 4);
                
                // Add crystal label
                RECT labelRect = {screenX - 15, screenY + 12, screenX + 15, screenY + 25};
                DrawText(hdc, L"💎", -1, &labelRect, DT_CENTER);
            }
            
            SelectObject(hdc, oldBrush);
            DeleteObject(objBrush);
        }
        
        // Render controls
        RECT controlsRect = {10, rect.bottom - 120, rect.right - 10, rect.bottom - 100};
        DrawText(hdc, L"🎮 CONTROLS:", -1, &controlsRect, DT_LEFT);
        
        RECT controls1Rect = {10, rect.bottom - 100, rect.right - 10, rect.bottom - 80};
        DrawText(hdc, L"W/A/S/D - Move Camera | E - Edit Mode | X - Create Platform", -1, &controls1Rect, DT_LEFT);
        
        RECT controls2Rect = {10, rect.bottom - 80, rect.right - 10, rect.bottom - 60};
        DrawText(hdc, L"Mouse Click - Interact | ESC - Quit | Space - Reset Camera", -1, &controls2Rect, DT_LEFT);
        
        RECT statusRect = {10, rect.bottom - 40, rect.right - 10, rect.bottom - 20};
        std::wstring status = editMode ? L"💡 Edit Mode: Click to add objects, use commands to modify scene" :
                                       L"💡 Play Mode: Explore the 3D world, press E to start building";
        DrawText(hdc, status.c_str(), -1, &statusRect, DT_LEFT);
    }
    
    void handleKeyboard(WPARAM key) {
        const int moveSpeed = 10;
        
        switch (key) {
            case 'W':
                cameraZ -= moveSpeed;
                break;
            case 'S':
                cameraZ += moveSpeed;
                break;
            case 'A':
                cameraX -= moveSpeed;
                break;
            case 'D':
                cameraX += moveSpeed;
                break;
            case 'Q':
                cameraY += moveSpeed;
                break;
            case 'E':
                if (!editMode) {
                    editMode = true;
                    MessageBox(hwnd, L"Edit Mode Activated!\n\nYou can now build your game:\n- Click to add objects\n- Use keyboard shortcuts\n- Press E again to return to Play Mode", L"Edit Mode", MB_OK | MB_ICONINFORMATION);
                } else {
                    cameraY -= moveSpeed;
                }
                break;
            case 'X':
                createWovenPlatform();
                break;
            case VK_SPACE:
                // Reset camera
                cameraX = 0; cameraY = 0; cameraZ = 20;
                break;
            case VK_ESCAPE:
                running = false;
                break;
        }
        
        InvalidateRect(hwnd, nullptr, TRUE);
    }
    
    void createWovenPlatform() {
        static int platformCount = 6;
        std::string name = "Woven_" + std::to_string(platformCount++);
        
        int x = (rand() % 300) - 150;
        int y = (rand() % 100) + 20;
        int z = (rand() % 300) - 150;
        
        objects.push_back(std::make_unique<GameObject>(name, x, y, z, "platform", RGB(200, 150, 100)));
        
        MessageBox(hwnd, L"✨ Reality Weaving Activated!\n\nA new platform has materialized from pure narrative energy!", L"Xing's Power", MB_OK | MB_ICONINFORMATION);
        InvalidateRect(hwnd, nullptr, TRUE);
    }
    
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        SimpleWindowedEngine* engine = nullptr;
        
        if (uMsg == WM_NCCREATE) {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            engine = (SimpleWindowedEngine*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)engine);
        } else {
            engine = (SimpleWindowedEngine*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }
        
        if (engine) {
            return engine->handleMessage(hwnd, uMsg, wParam, lParam);
        }
        
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    
    LRESULT handleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch (uMsg) {
            case WM_CLOSE:
                running = false;
                return 0;
            case WM_PAINT: {
                PAINTSTRUCT ps;
                BeginPaint(hwnd, &ps);
                render();
                EndPaint(hwnd, &ps);
                return 0;
            }
            case WM_KEYDOWN:
                handleKeyboard(wParam);
                return 0;
            case WM_LBUTTONDOWN: {
                if (editMode) {
                    int x = LOWORD(lParam) - windowWidth/2;
                    int y = windowHeight/2 - HIWORD(lParam);
                    
                    static int objCount = 0;
                    std::string name = "CustomObj_" + std::to_string(objCount++);
                    objects.push_back(std::make_unique<GameObject>(name, x/2, y/2, 0, "platform", RGB(100, 200, 100)));
                    
                    InvalidateRect(hwnd, nullptr, TRUE);
                }
                return 0;
            }
            case WM_SIZE:
                windowWidth = LOWORD(lParam);
                windowHeight = HIWORD(lParam);
                InvalidateRect(hwnd, nullptr, TRUE);
                return 0;
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    
    ~SimpleWindowedEngine() {
        if (backgroundBrush) DeleteObject(backgroundBrush);
        if (hdc) ReleaseDC(hwnd, hdc);
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SimpleWindowedEngine engine;
    
    if (!engine.initialize(hInstance)) {
        MessageBox(nullptr, L"Failed to initialize Epoch of Elria Game Engine!", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }
    
    // Show welcome message
    MessageBox(nullptr, 
        L"🌍 Welcome to Epoch of Elria Game Engine! 🌍\n\n"
        L"🎮 Controls:\n"
        L"• W/A/S/D - Move camera\n"
        L"• E - Toggle Edit Mode\n"
        L"• X - Create platform\n"
        L"• Space - Reset camera\n"
        L"• ESC - Quit\n\n"
        L"✨ Press E to start building your game!",
        L"Epoch of Elria", MB_OK | MB_ICONINFORMATION);
    
    engine.run();
    
    return 0;
}
