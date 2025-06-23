#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <chrono>
#include <cmath>
#include <sstream>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

// Forward declarations
class GameObject;
class Scene;
class GameEngine;

// Vector3D class
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
    
    virtual void render() {
        if (!active) return;
        
        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        glRotatef(rotation.y * 180.0 / M_PI, 0, 1, 0);
        glScalef(scale.x, scale.y, scale.z);
        
        // Set color based on type
        if (properties["type"] == "planet") {
            glColor3f(0.2f, 0.6f, 0.8f); // Blue for Earth
        } else if (properties["type"] == "platform") {
            glColor3f(0.6f, 0.4f, 0.2f); // Brown for platforms
        } else if (properties["type"] == "collectible") {
            glColor3f(1.0f, 0.8f, 0.2f); // Gold for crystals
        } else {
            glColor3f(0.8f, 0.8f, 0.8f); // Gray default
        }
        
        // Render based on type
        if (properties["type"] == "planet") {
            renderSphere(1.0, 20, 20);
        } else if (properties["type"] == "platform") {
            renderCube(1.0);
        } else if (properties["type"] == "collectible") {
            renderOctahedron(0.5);
        } else {
            renderCube(0.5);
        }
        
        glPopMatrix();
    }
    
private:
    void renderSphere(double radius, int slices, int stacks) {
        GLUquadric* quad = gluNewQuadric();
        gluSphere(quad, radius, slices, stacks);
        gluDeleteQuadric(quad);
    }
    
    void renderCube(double size) {
        glBegin(GL_QUADS);
        // Front face
        glVertex3f(-size, -size, size);
        glVertex3f(size, -size, size);
        glVertex3f(size, size, size);
        glVertex3f(-size, size, size);
        // Back face
        glVertex3f(-size, -size, -size);
        glVertex3f(-size, size, -size);
        glVertex3f(size, size, -size);
        glVertex3f(size, -size, -size);
        // Top face
        glVertex3f(-size, size, -size);
        glVertex3f(-size, size, size);
        glVertex3f(size, size, size);
        glVertex3f(size, size, -size);
        // Bottom face
        glVertex3f(-size, -size, -size);
        glVertex3f(size, -size, -size);
        glVertex3f(size, -size, size);
        glVertex3f(-size, -size, size);
        // Right face
        glVertex3f(size, -size, -size);
        glVertex3f(size, size, -size);
        glVertex3f(size, size, size);
        glVertex3f(size, -size, size);
        // Left face
        glVertex3f(-size, -size, -size);
        glVertex3f(-size, -size, size);
        glVertex3f(-size, size, size);
        glVertex3f(-size, size, -size);
        glEnd();
    }
    
    void renderOctahedron(double size) {
        glBegin(GL_TRIANGLES);
        // Top pyramid
        glVertex3f(0, size, 0); glVertex3f(-size, 0, size); glVertex3f(size, 0, size);
        glVertex3f(0, size, 0); glVertex3f(size, 0, size); glVertex3f(size, 0, -size);
        glVertex3f(0, size, 0); glVertex3f(size, 0, -size); glVertex3f(-size, 0, -size);
        glVertex3f(0, size, 0); glVertex3f(-size, 0, -size); glVertex3f(-size, 0, size);
        // Bottom pyramid
        glVertex3f(0, -size, 0); glVertex3f(size, 0, size); glVertex3f(-size, 0, size);
        glVertex3f(0, -size, 0); glVertex3f(size, 0, -size); glVertex3f(size, 0, size);
        glVertex3f(0, -size, 0); glVertex3f(-size, 0, -size); glVertex3f(size, 0, -size);
        glVertex3f(0, -size, 0); glVertex3f(-size, 0, size); glVertex3f(-size, 0, -size);
        glEnd();
    }
};

// Scene class
class Scene {
private:
    std::vector<std::unique_ptr<GameObject>> objects;
    std::string name;
    Vector3D cameraPos, cameraTarget;
    
public:
    Scene(const std::string& sceneName) : name(sceneName), cameraPos(0, 8, 20), cameraTarget(0, 0, 0) {}
    
    void addObject(std::unique_ptr<GameObject> obj) { objects.push_back(std::move(obj)); }
    
    GameObject* findObject(const std::string& name) {
        for (auto& obj : objects) {
            if (obj->name == name) return obj.get();
        }
        return nullptr;
    }
    
    void update(double deltaTime) {
        for (auto& obj : objects) obj->update(deltaTime);
    }
    
    void render() {
        for (auto& obj : objects) obj->render();
    }
    
    void setCameraPosition(const Vector3D& pos) { cameraPos = pos; }
    void setCameraTarget(const Vector3D& target) { cameraTarget = target; }
    Vector3D getCameraPosition() const { return cameraPos; }
    Vector3D getCameraTarget() const { return cameraTarget; }
    const std::string& getName() const { return name; }
    size_t getObjectCount() const { return objects.size(); }
};

// Main Game Engine class
class GameEngine {
private:
    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;
    bool running;
    bool editMode;
    std::unique_ptr<Scene> currentScene;
    Vector3D cameraPos;
    double cameraAngleX, cameraAngleY, cameraDistance;
    
public:
    GameEngine() : hwnd(nullptr), hdc(nullptr), hglrc(nullptr), running(false), editMode(false),
                   cameraPos(0, 8, 20), cameraAngleX(0), cameraAngleY(0), cameraDistance(20) {
        currentScene = std::make_unique<Scene>("Earth Sphere Sandbox");
        createEarthScene();
    }
    
    bool initialize(HINSTANCE hInstance) {
        // Register window class
        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = L"EpochOfElriaEngine";
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        
        if (!RegisterClass(&wc)) return false;
        
        // Create window
        hwnd = CreateWindowEx(0, L"EpochOfElriaEngine", L"🌍 Epoch of Elria - 3D Game Engine 🌍",
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                             CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
                             nullptr, nullptr, hInstance, this);
        
        if (!hwnd) return false;
        
        // Setup OpenGL
        hdc = GetDC(hwnd);
        
        PIXELFORMATDESCRIPTOR pfd = {};
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        
        int pixelFormat = ChoosePixelFormat(hdc, &pfd);
        SetPixelFormat(hdc, pixelFormat, &pfd);
        
        hglrc = wglCreateContext(hdc);
        wglMakeCurrent(hdc, hglrc);
        
        setupOpenGL();
        return true;
    }
    
    void setupOpenGL() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        
        GLfloat lightPos[] = {5.0f, 5.0f, 5.0f, 1.0f};
        GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
        GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
        
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        
        glClearColor(0.05f, 0.05f, 0.15f, 1.0f); // Dark space background
        
        setupProjection();
    }
    
    void setupProjection() {
        RECT rect;
        GetClientRect(hwnd, &rect);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, (double)width / height, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);
    }
    
    void createEarthScene() {
        // Create Earth
        auto earth = std::make_unique<GameObject>("Earth");
        earth->position = Vector3D(0, 0, 0);
        earth->scale = Vector3D(3, 3, 3);
        earth->properties["type"] = "planet";
        currentScene->addObject(std::move(earth));
        
        // Create platforms
        for (int i = 0; i < 6; ++i) {
            auto platform = std::make_unique<GameObject>("Platform_" + std::to_string(i));
            double angle = (i * 2 * M_PI) / 6;
            platform->position = Vector3D(cos(angle) * 8, sin(angle) * 2, sin(angle) * 8);
            platform->scale = Vector3D(1.5, 0.5, 1.5);
            platform->properties["type"] = "platform";
            currentScene->addObject(std::move(platform));
        }
        
        // Create crystals
        for (int i = 0; i < 4; ++i) {
            auto crystal = std::make_unique<GameObject>("Crystal_" + std::to_string(i));
            crystal->position = Vector3D(i * 3 - 4.5, 4, i * 2 - 3);
            crystal->scale = Vector3D(0.8, 0.8, 0.8);
            crystal->properties["type"] = "collectible";
            currentScene->addObject(std::move(crystal));
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
        if (currentScene) {
            currentScene->update(deltaTime);
        }

        // Update camera position
        updateCamera();
    }

    void updateCamera() {
        cameraPos.x = cameraDistance * cos(cameraAngleX) * cos(cameraAngleY);
        cameraPos.y = cameraDistance * sin(cameraAngleX);
        cameraPos.z = cameraDistance * cos(cameraAngleX) * sin(cameraAngleY);
    }

    void render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Set up camera
        gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
                  0, 0, 0,  // Look at origin
                  0, 1, 0); // Up vector

        // Render stars
        renderStars();

        // Render scene
        if (currentScene) {
            currentScene->render();
        }

        // Render UI text
        renderUI();

        SwapBuffers(hdc);
    }

    void renderStars() {
        glDisable(GL_LIGHTING);
        glPointSize(2.0f);
        glColor3f(1.0f, 1.0f, 1.0f);

        glBegin(GL_POINTS);
        for (int i = 0; i < 100; ++i) {
            double x = (rand() % 200 - 100) / 5.0;
            double y = (rand() % 200 - 100) / 5.0;
            double z = (rand() % 200 - 100) / 5.0;

            if (sqrt(x*x + y*y + z*z) > 15.0) {
                glVertex3f(x, y, z);
            }
        }
        glEnd();

        glEnable(GL_LIGHTING);
    }

    void renderUI() {
        // Switch to 2D rendering for UI
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        RECT rect;
        GetClientRect(hwnd, &rect);
        glOrtho(0, rect.right, rect.bottom, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        // Render title
        glColor3f(1.0f, 1.0f, 1.0f);
        renderText(10, 20, "Epoch of Elria - 3D Game Engine");
        renderText(10, 40, editMode ? "EDIT MODE - Build your game!" : "PLAY MODE - Explore the universe!");

        // Render controls
        renderText(10, rect.bottom - 80, "Controls:");
        renderText(10, rect.bottom - 60, "WASD - Move Camera | E - Edit Mode | X - Create Platform");
        renderText(10, rect.bottom - 40, "Mouse - Look Around | ESC - Quit");

        // Render scene info
        std::string sceneInfo = "Scene: " + currentScene->getName() + " | Objects: " + std::to_string(currentScene->getObjectCount());
        renderText(10, rect.bottom - 20, sceneInfo);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }

    void renderText(int x, int y, const std::string& text) {
        // Simple text rendering (placeholder - would need proper font rendering)
        glRasterPos2i(x, y);
        // In a real implementation, you'd use a proper font rendering library
    }

    void handleKeyboard(WPARAM key) {
        const double moveSpeed = 2.0;
        const double rotateSpeed = 0.1;

        switch (key) {
            case 'W':
                cameraDistance -= moveSpeed;
                if (cameraDistance < 5.0) cameraDistance = 5.0;
                break;
            case 'S':
                cameraDistance += moveSpeed;
                if (cameraDistance > 50.0) cameraDistance = 50.0;
                break;
            case 'A':
                cameraAngleY -= rotateSpeed;
                break;
            case 'D':
                cameraAngleY += rotateSpeed;
                break;
            case 'Q':
                cameraAngleX -= rotateSpeed;
                break;
            case 'E':
                if (!editMode) {
                    editMode = true;
                } else {
                    cameraAngleX += rotateSpeed;
                }
                break;
            case 'X':
                createWovenPlatform();
                break;
            case VK_ESCAPE:
                running = false;
                break;
        }
    }

    void createWovenPlatform() {
        auto platform = std::make_unique<GameObject>("Woven_" + std::to_string(rand() % 1000));
        platform->position = Vector3D(
            (rand() % 20 - 10),
            (rand() % 8 + 2),
            (rand() % 20 - 10)
        );
        platform->scale = Vector3D(1.0, 0.3, 1.0);
        platform->properties["type"] = "platform";
        currentScene->addObject(std::move(platform));
    }

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        GameEngine* engine = nullptr;

        if (uMsg == WM_NCCREATE) {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            engine = (GameEngine*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)engine);
        } else {
            engine = (GameEngine*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
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
            case WM_SIZE:
                setupProjection();
                return 0;
            case WM_KEYDOWN:
                handleKeyboard(wParam);
                return 0;
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    void cleanup() {
        if (hglrc) {
            wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(hglrc);
        }
        if (hdc) ReleaseDC(hwnd, hdc);
        if (hwnd) DestroyWindow(hwnd);
    }

    ~GameEngine() {
        cleanup();
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    GameEngine engine;

    if (!engine.initialize(hInstance)) {
        MessageBox(nullptr, L"Failed to initialize game engine!", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    engine.run();

    return 0;
}
