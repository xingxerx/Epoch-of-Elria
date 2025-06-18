#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#endif

#include "Vector3D.h"
#include "World3D.h"

class EarthSphere3D {
private:
    Vector3D position;
    double radius;
    int segments;
    std::vector<Vector3D> vertices;
    std::vector<Vector3D> normals;
    std::vector<std::pair<double, double>> texCoords;
    std::vector<std::vector<int>> faces;
    
public:
    EarthSphere3D(const Vector3D& pos, double r, int segs = 32) 
        : position(pos), radius(r), segments(segs) {
        generateSphere();
    }
    
    void generateSphere() {
        vertices.clear();
        normals.clear();
        texCoords.clear();
        faces.clear();
        
        // Generate vertices
        for (int i = 0; i <= segments; ++i) {
            double lat = M_PI * (-0.5 + (double)i / segments);
            double y = sin(lat);
            double xzRadius = cos(lat);
            
            for (int j = 0; j <= segments; ++j) {
                double lon = 2 * M_PI * (double)j / segments;
                double x = xzRadius * cos(lon);
                double z = xzRadius * sin(lon);
                
                Vector3D vertex(x * radius + position.x, 
                               y * radius + position.y, 
                               z * radius + position.z);
                Vector3D normal(x, y, z);
                
                vertices.push_back(vertex);
                normals.push_back(normal);
                texCoords.push_back({(double)j / segments, (double)i / segments});
            }
        }
        
        // Generate faces
        for (int i = 0; i < segments; ++i) {
            for (int j = 0; j < segments; ++j) {
                int first = i * (segments + 1) + j;
                int second = first + segments + 1;
                
                faces.push_back({first, second, first + 1});
                faces.push_back({second, second + 1, first + 1});
            }
        }
    }
    
    void render() {
        glPushMatrix();
        
        // Apply earth-like coloring
        glColor3f(0.2f, 0.6f, 0.8f); // Ocean blue base
        
        glBegin(GL_TRIANGLES);
        for (const auto& face : faces) {
            for (int vertexIndex : face) {
                const Vector3D& vertex = vertices[vertexIndex];
                const Vector3D& normal = normals[vertexIndex];
                const auto& texCoord = texCoords[vertexIndex];
                
                // Simple procedural earth coloring based on texture coordinates
                double lat = texCoord.second * M_PI - M_PI/2;
                double lon = texCoord.first * 2 * M_PI;
                
                // Green for land masses (simplified)
                if (sin(lat * 3) * cos(lon * 5) > 0.3) {
                    glColor3f(0.2f, 0.7f, 0.2f); // Land green
                } else {
                    glColor3f(0.1f, 0.4f, 0.8f); // Ocean blue
                }
                
                glNormal3f(normal.x, normal.y, normal.z);
                glVertex3f(vertex.x, vertex.y, vertex.z);
            }
        }
        glEnd();
        
        glPopMatrix();
    }
    
    void update(double deltaTime) {
        // Rotate the earth
        static double rotation = 0.0;
        rotation += deltaTime * 0.1; // Slow rotation
        
        glPushMatrix();
        glRotatef(rotation * 180.0 / M_PI, 0, 1, 0);
        glPopMatrix();
    }
};

class Game3DWindow {
private:
#ifdef _WIN32
    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;
#else
    Display* display;
    Window window;
    GLXContext context;
#endif
    
    int width, height;
    bool running;
    std::unique_ptr<EarthSphere3D> earth;
    Vector3D cameraPos;
    Vector3D cameraTarget;
    double cameraDistance;
    double cameraAngleX, cameraAngleY;
    
public:
    Game3DWindow(int w = 1024, int h = 768) 
        : width(w), height(h), running(false), 
          cameraPos(0, 0, 10), cameraTarget(0, 0, 0), cameraDistance(10.0),
          cameraAngleX(0), cameraAngleY(0) {
        
        earth = std::make_unique<EarthSphere3D>(Vector3D(0, 0, 0), 3.0, 64);
    }
    
    bool initialize() {
#ifdef _WIN32
        return initializeWindows();
#else
        return initializeLinux();
#endif
    }
    
#ifdef _WIN32
    bool initializeWindows() {
        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = L"EarthSphere3D";
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        
        RegisterClass(&wc);
        
        hwnd = CreateWindowEx(0, L"EarthSphere3D", L"Epoch of Elria - 3D Earth Sphere",
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                             CW_USEDEFAULT, CW_USEDEFAULT, width, height,
                             nullptr, nullptr, GetModuleHandle(nullptr), this);
        
        if (!hwnd) return false;
        
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
    
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        Game3DWindow* game = nullptr;
        
        if (uMsg == WM_NCCREATE) {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            game = (Game3DWindow*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)game);
        } else {
            game = (Game3DWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }
        
        if (game) {
            return game->handleMessage(hwnd, uMsg, wParam, lParam);
        }
        
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    
    LRESULT handleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch (uMsg) {
            case WM_CLOSE:
                running = false;
                return 0;
            case WM_SIZE:
                width = LOWORD(lParam);
                height = HIWORD(lParam);
                glViewport(0, 0, width, height);
                setupProjection();
                return 0;
            case WM_KEYDOWN:
                handleKeyboard(wParam);
                return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
#endif
    
    void setupOpenGL() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        
        // Set up lighting
        GLfloat lightPos[] = {5.0f, 5.0f, 5.0f, 1.0f};
        GLfloat lightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
        GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
        
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        
        glClearColor(0.05f, 0.05f, 0.15f, 1.0f); // Dark space background
        
        setupProjection();
    }
    
    void setupProjection() {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, (double)width / height, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);
    }
    
    void handleKeyboard(int key) {
        const double moveSpeed = 0.5;
        const double rotateSpeed = 0.1;
        
        switch (key) {
            case 'W': case 'w':
                cameraDistance -= moveSpeed;
                if (cameraDistance < 1.0) cameraDistance = 1.0;
                break;
            case 'S': case 's':
                cameraDistance += moveSpeed;
                break;
            case 'A': case 'a':
                cameraAngleY -= rotateSpeed;
                break;
            case 'D': case 'd':
                cameraAngleY += rotateSpeed;
                break;
            case 'Q': case 'q':
                cameraAngleX -= rotateSpeed;
                break;
            case 'E': case 'e':
                cameraAngleX += rotateSpeed;
                break;
            case 27: // Escape
                running = false;
                break;
        }
        
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
                  cameraTarget.x, cameraTarget.y, cameraTarget.z,
                  0, 1, 0);

        // Render earth
        earth->render();

        // Render stars
        renderStars();

#ifdef _WIN32
        SwapBuffers(hdc);
#else
        glXSwapBuffers(display, window);
#endif
    }

    void renderStars() {
        glDisable(GL_LIGHTING);
        glPointSize(2.0f);
        glColor3f(1.0f, 1.0f, 1.0f);

        glBegin(GL_POINTS);
        for (int i = 0; i < 200; ++i) {
            double x = (rand() % 200 - 100) / 10.0;
            double y = (rand() % 200 - 100) / 10.0;
            double z = (rand() % 200 - 100) / 10.0;

            if (sqrt(x*x + y*y + z*z) > 15.0) { // Don't put stars too close
                glVertex3f(x, y, z);
            }
        }
        glEnd();

        glEnable(GL_LIGHTING);
    }

    void run() {
        if (!initialize()) {
            std::cerr << "Failed to initialize 3D window!" << std::endl;
            return;
        }

        running = true;

        std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║              EPOCH OF ELRIA - 3D EARTH SPHERE               ║" << std::endl;
        std::cout << "║                   OpenGL 3D Game Engine                     ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
        std::cout << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "  W/S - Zoom in/out" << std::endl;
        std::cout << "  A/D - Rotate around Y axis" << std::endl;
        std::cout << "  Q/E - Rotate around X axis" << std::endl;
        std::cout << "  ESC - Exit" << std::endl;
        std::cout << std::endl;

        auto lastTime = std::chrono::high_resolution_clock::now();

        while (running) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            double deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
            lastTime = currentTime;

#ifdef _WIN32
            MSG msg;
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
#endif

            earth->update(deltaTime);
            render();

            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
        }

        cleanup();
    }

    void cleanup() {
#ifdef _WIN32
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(hglrc);
        ReleaseDC(hwnd, hdc);
        DestroyWindow(hwnd);
#endif
    }
};

int main() {
    std::cout << "Starting Epoch of Elria 3D Earth Sphere Engine..." << std::endl;

    Game3DWindow game(1024, 768);
    game.run();

    return 0;
}
