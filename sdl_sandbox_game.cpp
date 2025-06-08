// sdl_sandbox_game.cpp - Real Windowed Game with SDL2
// Note: Requires SDL2 library to be installed

#ifdef USE_SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <sstream>
#include <iomanip>

// Fallback version without SDL2
#ifndef USE_SDL2

class MockSDLGame {
public:
    void run() {
        std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║                  SDL2 WINDOWED GAME                         ║" << std::endl;
        std::cout << "║                                                              ║" << std::endl;
        std::cout << "║  To enable real windowed graphics, install SDL2:            ║" << std::endl;
        std::cout << "║                                                              ║" << std::endl;
        std::cout << "║  Ubuntu/Debian: sudo apt install libsdl2-dev libsdl2-ttf-dev║" << std::endl;
        std::cout << "║  Arch Linux:    sudo pacman -S sdl2 sdl2_ttf                ║" << std::endl;
        std::cout << "║  Windows:       Download from https://libsdl.org            ║" << std::endl;
        std::cout << "║                                                              ║" << std::endl;
        std::cout << "║  Then compile with:                                          ║" << std::endl;
        std::cout << "║  g++ -DUSE_SDL2 sdl_sandbox_game.cpp -lSDL2 -lSDL2_ttf      ║" << std::endl;
        std::cout << "║                                                              ║" << std::endl;
        std::cout << "║  For now, running console-based sandbox...                  ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
        
        // Run the console version
        system("./windowed_sandbox_game");
    }
};

int main() {
    MockSDLGame game;
    game.run();
    return 0;
}

#else

// Real SDL2 implementation
struct Vector2D {
    float x, y;
    Vector2D(float x = 0, float y = 0) : x(x), y(y) {}
    Vector2D operator+(const Vector2D& other) const { return Vector2D(x + other.x, y + other.y); }
    Vector2D operator*(float scalar) const { return Vector2D(x * scalar, y * scalar); }
};

class SDLSandboxGame {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    bool running;
    
    // Game state
    Vector2D playerPos;
    std::vector<Vector2D> collectibles;
    int score;
    
    // Frame timing
    Uint32 lastFrameTime;
    float deltaTime;
    float fps;
    int frameCount;
    Uint32 fpsTimer;
    
    const int WINDOW_WIDTH = 1024;
    const int WINDOW_HEIGHT = 768;
    
public:
    SDLSandboxGame() : window(nullptr), renderer(nullptr), font(nullptr), running(false),
                       playerPos(WINDOW_WIDTH/2, WINDOW_HEIGHT/2), score(0),
                       lastFrameTime(0), deltaTime(0), fps(0), frameCount(0), fpsTimer(0) {}
    
    bool initialize() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }
        
        if (TTF_Init() == -1) {
            std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
            return false;
        }
        
        window = SDL_CreateWindow("Dream Weaver's Sandbox World",
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                WINDOW_WIDTH, WINDOW_HEIGHT,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        
        if (window == nullptr) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }
        
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) {
            std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }
        
        font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 16);
        if (font == nullptr) {
            // Try alternative font paths
            font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 16);
            if (font == nullptr) {
                std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
                return false;
            }
        }
        
        // Generate initial collectibles
        srand(static_cast<unsigned int>(time(NULL)));
        for (int i = 0; i < 20; ++i) {
            collectibles.push_back(Vector2D(
                rand() % WINDOW_WIDTH,
                rand() % WINDOW_HEIGHT
            ));
        }
        
        running = true;
        lastFrameTime = SDL_GetTicks();
        fpsTimer = lastFrameTime;
        
        return true;
    }
    
    void handleEvents() {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }
        
        // Handle continuous key presses
        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        Vector2D movement(0, 0);
        float speed = 300.0f; // pixels per second
        
        if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP])    movement.y -= speed;
        if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN])  movement.y += speed;
        if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT])  movement.x -= speed;
        if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) movement.x += speed;
        
        // Apply movement
        playerPos = playerPos + movement * deltaTime;
        
        // Keep player in bounds
        if (playerPos.x < 0) playerPos.x = 0;
        if (playerPos.x > WINDOW_WIDTH) playerPos.x = WINDOW_WIDTH;
        if (playerPos.y < 0) playerPos.y = 0;
        if (playerPos.y > WINDOW_HEIGHT) playerPos.y = WINDOW_HEIGHT;
    }
    
    void update() {
        // Update timing
        Uint32 currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastFrameTime) / 1000.0f;
        lastFrameTime = currentTime;
        
        // Update FPS
        frameCount++;
        if (currentTime - fpsTimer >= 1000) {
            fps = frameCount / ((currentTime - fpsTimer) / 1000.0f);
            frameCount = 0;
            fpsTimer = currentTime;
        }
        
        // Check collectible collection
        for (auto it = collectibles.begin(); it != collectibles.end(); ) {
            Vector2D diff = playerPos + Vector2D(-it->x, -it->y);
            float distance = sqrt(diff.x * diff.x + diff.y * diff.y);
            
            if (distance < 30.0f) {
                score += 10;
                it = collectibles.erase(it);
            } else {
                ++it;
            }
        }
        
        // Regenerate collectibles if all collected
        if (collectibles.empty()) {
            for (int i = 0; i < 20; ++i) {
                collectibles.push_back(Vector2D(
                    rand() % WINDOW_WIDTH,
                    rand() % WINDOW_HEIGHT
                ));
            }
        }
    }
    
    void renderText(const std::string& text, int x, int y, SDL_Color color = {255, 255, 255, 255}) {
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (textSurface != nullptr) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture != nullptr) {
                SDL_Rect renderQuad = {x, y, textSurface->w, textSurface->h};
                SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }
    }
    
    void render() {
        // Clear screen with dark blue
        SDL_SetRenderDrawColor(renderer, 20, 30, 60, 255);
        SDL_RenderClear(renderer);
        
        // Draw collectibles (yellow circles)
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        for (const auto& collectible : collectibles) {
            SDL_Rect rect = {(int)collectible.x - 10, (int)collectible.y - 10, 20, 20};
            SDL_RenderFillRect(renderer, &rect);
        }
        
        // Draw player (white circle)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect playerRect = {(int)playerPos.x - 15, (int)playerPos.y - 15, 30, 30};
        SDL_RenderFillRect(renderer, &playerRect);
        
        // Draw UI
        std::stringstream ss;
        ss << "FPS: " << std::fixed << std::setprecision(1) << fps;
        renderText(ss.str(), 10, 10);
        
        ss.str("");
        ss << "Score: " << score;
        renderText(ss.str(), 10, 30);
        
        ss.str("");
        ss << "Collectibles: " << collectibles.size();
        renderText(ss.str(), 10, 50);
        
        ss.str("");
        ss << "Position: (" << (int)playerPos.x << ", " << (int)playerPos.y << ")";
        renderText(ss.str(), 10, 70);
        
        renderText("WASD to move", 10, WINDOW_HEIGHT - 40);
        renderText("Collect yellow squares!", 10, WINDOW_HEIGHT - 20);
        
        // Present the frame
        SDL_RenderPresent(renderer);
    }
    
    void run() {
        if (!initialize()) {
            std::cerr << "Failed to initialize!" << std::endl;
            return;
        }
        
        std::cout << "SDL2 Sandbox Game started!" << std::endl;
        std::cout << "Window: " << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << std::endl;
        std::cout << "Use WASD to move, collect yellow squares!" << std::endl;
        
        while (running) {
            handleEvents();
            update();
            render();
        }
        
        cleanup();
    }
    
    void cleanup() {
        if (font != nullptr) {
            TTF_CloseFont(font);
            font = nullptr;
        }
        
        if (renderer != nullptr) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }
        
        if (window != nullptr) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }
        
        TTF_Quit();
        SDL_Quit();
    }
};

int main() {
    SDLSandboxGame game;
    game.run();
    return 0;
}

#endif
