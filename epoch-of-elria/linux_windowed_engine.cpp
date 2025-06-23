#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <thread>
#include <cmath>
#include <cstdlib>

// Game Object class
class GameObject {
public:
    std::string name;
    double x, y, z;
    double rotX, rotY, rotZ;
    double scaleX, scaleY, scaleZ;
    std::string type;
    float r, g, b; // Color
    
    GameObject(const std::string& n, double px, double py, double pz, const std::string& t)
        : name(n), x(px), y(py), z(pz), rotX(0), rotY(0), rotZ(0), 
          scaleX(1), scaleY(1), scaleZ(1), type(t), r(1), g(1), b(1) {
        
        // Set colors based on type
        if (type == "planet") {
            r = 0.2f; g = 0.6f; b = 0.8f; // Blue for Earth
        } else if (type == "platform") {
            r = 0.6f; g = 0.4f; b = 0.2f; // Brown for platforms
        } else if (type == "collectible") {
            r = 1.0f; g = 0.8f; b = 0.2f; // Gold for crystals
        }
    }
    
    void update(double deltaTime) {
        if (type == "planet") {
            rotY += 30.0 * deltaTime; // Rotate Earth
            if (rotY > 360.0) rotY -= 360.0;
        }
    }
    
    void render() {
        glPushMatrix();
        
        glTranslatef(x, y, z);
        glRotatef(rotX, 1, 0, 0);
        glRotatef(rotY, 0, 1, 0);
        glRotatef(rotZ, 0, 0, 1);
        glScalef(scaleX, scaleY, scaleZ);
        
        glColor3f(r, g, b);
        
        if (type == "planet") {
            renderSphere(1.0, 20, 20);
        } else if (type == "platform") {
            renderCube(1.0);
        } else if (type == "collectible") {
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

// Linux Windowed Game Engine
class LinuxWindowedEngine {
private:
    Display* display;
    Window window;
    GLXContext glContext;
    XWindowAttributes windowAttribs;
    bool running;
    bool editMode;
    
    std::vector<std::unique_ptr<GameObject>> objects;
    double cameraX, cameraY, cameraZ;
    double cameraAngleX, cameraAngleY;
    
public:
    LinuxWindowedEngine() : display(nullptr), window(0), glContext(nullptr), 
                           running(false), editMode(false),
                           cameraX(0), cameraY(5), cameraZ(15), 
                           cameraAngleX(0), cameraAngleY(0) {
        createDefaultScene();
    }
    
    bool initialize() {
        // Open display
        display = XOpenDisplay(nullptr);
        if (!display) {
            std::cerr << "Cannot open X display" << std::endl;
            return false;
        }
        
        // Get default screen
        int screen = DefaultScreen(display);
        
        // Choose visual
        GLint attributes[] = {
            GLX_RGBA,
            GLX_DEPTH_SIZE, 24,
            GLX_DOUBLEBUFFER,
            None
        };
        
        XVisualInfo* visual = glXChooseVisual(display, screen, attributes);
        if (!visual) {
            std::cerr << "Cannot choose visual" << std::endl;
            return false;
        }
        
        // Create colormap
        Colormap colormap = XCreateColormap(display, RootWindow(display, screen), 
                                          visual->visual, AllocNone);
        
        // Create window
        XSetWindowAttributes windowAttribs;
        windowAttribs.colormap = colormap;
        windowAttribs.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;
        
        window = XCreateWindow(display, RootWindow(display, screen),
                              100, 100, 1024, 768, 0,
                              visual->depth, InputOutput, visual->visual,
                              CWColormap | CWEventMask, &windowAttribs);
        
        // Set window title
        XStoreName(display, window, "🌍 Epoch of Elria - 3D Game Engine 🌍");
        
        // Create OpenGL context
        glContext = glXCreateContext(display, visual, nullptr, GL_TRUE);
        if (!glContext) {
            std::cerr << "Cannot create OpenGL context" << std::endl;
            return false;
        }
        
        // Map window
        XMapWindow(display, window);
        
        // Make context current
        glXMakeCurrent(display, window, glContext);
        
        // Setup OpenGL
        setupOpenGL();
        
        return true;
    }
    
    void setupOpenGL() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        
        // Set up lighting
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
        XGetWindowAttributes(display, window, &windowAttribs);
        
        glViewport(0, 0, windowAttribs.width, windowAttribs.height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, (double)windowAttribs.width / windowAttribs.height, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);
    }
    
    void createDefaultScene() {
        // Create Earth
        auto earth = std::make_unique<GameObject>("Earth", 0, 0, 0, "planet");
        earth->scaleX = earth->scaleY = earth->scaleZ = 3.0;
        objects.push_back(std::move(earth));
        
        // Create platforms
        for (int i = 0; i < 6; ++i) {
            double angle = (i * 2 * M_PI) / 6;
            auto platform = std::make_unique<GameObject>(
                "Platform_" + std::to_string(i),
                cos(angle) * 8, sin(angle) * 2, sin(angle) * 8,
                "platform"
            );
            platform->scaleX = 1.5; platform->scaleY = 0.5; platform->scaleZ = 1.5;
            objects.push_back(std::move(platform));
        }
        
        // Create crystals
        for (int i = 0; i < 4; ++i) {
            auto crystal = std::make_unique<GameObject>(
                "Crystal_" + std::to_string(i),
                i * 3 - 4.5, 4, i * 2 - 3,
                "collectible"
            );
            crystal->scaleX = crystal->scaleY = crystal->scaleZ = 0.8;
            objects.push_back(std::move(crystal));
        }
    }
    
    void run() {
        running = true;
        
        // Show welcome message
        std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║              EPOCH OF ELRIA GAME ENGINE                     ║" << std::endl;
        std::cout << "║                   3D Window Opened!                         ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
        std::cout << std::endl;
        std::cout << "🎮 Controls:" << std::endl;
        std::cout << "  W/A/S/D - Move Camera" << std::endl;
        std::cout << "  Q/E - Move Up/Down" << std::endl;
        std::cout << "  X - Create Platform" << std::endl;
        std::cout << "  Space - Edit Mode" << std::endl;
        std::cout << "  ESC - Quit" << std::endl;
        std::cout << std::endl;
        std::cout << "🌍 3D Window is now open! Check your screen." << std::endl;
        
        auto lastTime = std::chrono::high_resolution_clock::now();
        
        while (running) {
            // Handle events
            while (XPending(display)) {
                XEvent event;
                XNextEvent(display, &event);
                handleEvent(event);
            }
            
            auto currentTime = std::chrono::high_resolution_clock::now();
            double deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
            lastTime = currentTime;
            
            update(deltaTime);
            render();
            
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
        }
    }
    
    void handleEvent(const XEvent& event) {
        switch (event.type) {
            case Expose:
                render();
                break;
            case ConfigureNotify:
                setupProjection();
                break;
            case KeyPress: {
                KeySym key = XLookupKeysym((XKeyEvent*)&event, 0);
                handleKeyboard(key);
                break;
            }
            case ButtonPress:
                if (editMode) {
                    createWovenPlatform();
                }
                break;
        }
    }
    
    void handleKeyboard(KeySym key) {
        const double moveSpeed = 2.0;
        const double rotateSpeed = 0.1;
        
        switch (key) {
            case XK_w:
            case XK_W:
                cameraZ -= moveSpeed;
                break;
            case XK_s:
            case XK_S:
                cameraZ += moveSpeed;
                break;
            case XK_a:
            case XK_A:
                cameraX -= moveSpeed;
                break;
            case XK_d:
            case XK_D:
                cameraX += moveSpeed;
                break;
            case XK_q:
            case XK_Q:
                cameraY += moveSpeed;
                break;
            case XK_e:
            case XK_E:
                cameraY -= moveSpeed;
                break;
            case XK_x:
            case XK_X:
                createWovenPlatform();
                break;
            case XK_space:
                editMode = !editMode;
                std::cout << (editMode ? "🔧 Edit Mode Activated!" : "▶️ Play Mode Activated!") << std::endl;
                break;
            case XK_Escape:
                running = false;
                break;
        }
    }
    
    void createWovenPlatform() {
        static int platformCount = 6;
        auto platform = std::make_unique<GameObject>(
            "Woven_" + std::to_string(platformCount++),
            (rand() % 20 - 10), (rand() % 8 + 2), (rand() % 20 - 10),
            "platform"
        );
        platform->r = 0.8f; platform->g = 0.6f; platform->b = 0.4f; // Different color for woven platforms
        objects.push_back(std::move(platform));
        
        std::cout << "✨ Reality Weaving Activated! New platform created!" << std::endl;
    }
    
    void update(double deltaTime) {
        for (auto& obj : objects) {
            obj->update(deltaTime);
        }
    }
    
    void render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        
        // Set up camera
        gluLookAt(cameraX, cameraY, cameraZ,
                  0, 0, 0,  // Look at origin
                  0, 1, 0); // Up vector
        
        // Render stars
        renderStars();
        
        // Render all objects
        for (auto& obj : objects) {
            obj->render();
        }
        
        glXSwapBuffers(display, window);
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
    
    ~LinuxWindowedEngine() {
        if (glContext) {
            glXMakeCurrent(display, None, nullptr);
            glXDestroyContext(display, glContext);
        }
        if (window) XDestroyWindow(display, window);
        if (display) XCloseDisplay(display);
    }
};

int main() {
    LinuxWindowedEngine engine;
    
    if (!engine.initialize()) {
        std::cerr << "Failed to initialize game engine!" << std::endl;
        return 1;
    }
    
    engine.run();
    
    return 0;
}
