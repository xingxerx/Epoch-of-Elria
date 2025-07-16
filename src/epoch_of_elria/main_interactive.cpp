// main_interactive.cpp - SFML Window with ImGui Terminal
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "imgui.h"
#include "imgui-SFML.h" // Needs to be included after imgui.h and SFML headers

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream> // For ostringstream
#include <memory>  // For std::shared_ptr, std::make_unique

// --- Basic Logging System for ImGui ---
struct AppLog {
    ImGuiTextBuffer Buf;
    ImVector<int> LineOffsets; // Index to lines offset. We maintain this with AddLog().
    bool AutoScroll;

    AppLog() {
        AutoScroll = true;
        Clear();
    }

    void Clear() {
        Buf.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void AddLog(const char* fmt, ...) IM_FMTARGS(2) {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
    }

    void Draw(const char* title, bool* p_open = NULL) {
        if (!ImGui::Begin(title, p_open)) {
            ImGui::End();
            return;
        }
        if (ImGui::Button("Clear")) Clear();
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();
        ImGui::Checkbox("Auto-scroll", &AutoScroll);

        ImGui::Separator();
        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (copy) ImGui::LogToClipboard();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf = Buf.begin();
        const char* buf_end = Buf.end();
        
        // Traditional way to display the log:
        // ImGui::TextUnformatted(buf, buf_end);

        // More performant way for very long logs:
        ImGuiListClipper clipper;
        clipper.Begin(LineOffsets.Size);
        while (clipper.Step()) {
            for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
                const char* line_start = buf + LineOffsets[line_no];
                const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                ImGui::TextUnformatted(line_start, line_end);
            }
        }
        clipper.End();
        ImGui::PopStyleVar();

        if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
        ImGui::End();
    }
};

// Global log instance
AppLog gameLog;

// Custom stream buffer to redirect std::cout
class ImGuiStream : public std::stringbuf {
public:
    ~ImGuiStream() { pubsync(); } // Flush on destruction
    int sync() override {
        if (!str().empty()) {
            gameLog.AddLog("%s", str().c_str());
            str(""); // Clear the buffer
        }
        return 0;
    }
};

ImGuiStream imgui_out_buffer;
std::streambuf* original_cout_buffer = nullptr;

void RedirectStdOutToImGui() {
    original_cout_buffer = std::cout.rdbuf();
    std::cout.rdbuf(&imgui_out_buffer);
}

void RestoreStdOut() {
    if (original_cout_buffer) {
        std::cout.rdbuf(original_cout_buffer);
    }
}

// --- Simplified Game Classes (from your main.cpp for this example) ---
// (Ideally, these would be in their own .h/.cpp files and included)
class Vector2D {
public:
    double x, y;
    Vector2D(double x = 0, double y = 0) : x(x), y(y) {}
    Vector2D operator+(const Vector2D& other) const { return Vector2D(x + other.x, y + other.y); }
    Vector2D operator*(double scalar) const { return Vector2D(x * scalar, y * scalar); }
    void print() const { std::cout << "(" << x << ", " << y << ")"; } // Will now print to ImGui console
};

class GameObject {
protected:
    Vector2D position;
    Vector2D velocity;
    std::string name;
public:
    GameObject(std::string n, double px, double py) : name(n), position(px, py) {}
    virtual ~GameObject() = default;
    virtual void Update(double deltaTime) { position = position + (velocity * deltaTime); }
    // Draw method would need to take sf::RenderTarget& for SFML rendering
    virtual void Draw() const {
        std::cout << "Drawing " << name << " at ";
        position.print();
        std::cout << std::endl;
    }
    const Vector2D& getPosition() const { return position; }
    bool CheckCollision(const GameObject& other) const { /* Basic AABB collision */
        // Simplified for brevity, assuming fixed size or use width/height members
        double size = 20; // Placeholder
        return (position.x < other.position.x + size &&
                position.x + size > other.position.x &&
                position.y < other.position.y + size &&
                position.y + size > other.position.y);
    }
};

class Player : public GameObject {
private:
    double speed;
    sf::RectangleShape sprite;

public:
    Player(double x, double y) : GameObject("Player", x, y), speed(200.0) {
        std::cout << "Player created!" << std::endl;
        velocity = Vector2D(0, 0); // Start stationary

        // Create visual sprite
        sprite.setSize(sf::Vector2f(50, 50));
        sprite.setFillColor(sf::Color::Blue);
        sprite.setPosition(static_cast<float>(x), static_cast<float>(y));
    }

    void HandleInput() {
        velocity = Vector2D(0, 0); // Reset velocity

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            velocity.y = -speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            velocity.y = speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            velocity.x = -speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            velocity.x = speed;
        }
    }

    void Update(double deltaTime) override {
        HandleInput();
        GameObject::Update(deltaTime);

        // Keep player on screen
        if (position.x < 0) position.x = 0;
        if (position.y < 0) position.y = 0;
        if (position.x > 1230) position.x = 1230; // 1280 - 50
        if (position.y > 670) position.y = 670;   // 720 - 50

        // Update sprite position
        sprite.setPosition(static_cast<float>(position.x), static_cast<float>(position.y));
    }

    void DrawSFML(sf::RenderTarget& target) const {
        target.draw(sprite);
    }

    void Draw() const override {
        std::cout << "PLAYER at ";
        position.print();
        std::cout << std::endl;
    }
};

class Collectible : public GameObject {
    bool collected = false;
    int value = 10;
    sf::CircleShape sprite;
    double animationTime = 0.0;
    Vector2D originalPosition;

public:
    Collectible(double x, double y) : GameObject("Collectible", x, y), originalPosition(x, y) {
        sprite.setRadius(10);
        sprite.setFillColor(sf::Color::Yellow);
        sprite.setPosition(static_cast<float>(x), static_cast<float>(y));
    }

    bool isCollected() const { return collected; }
    void setCollected(bool c) { collected = c; }
    int getValue() const { return value; }

    void Update(double deltaTime) override {
        if (!collected) {
            // Animate bobbing motion
            animationTime += deltaTime * 2.0;
            position.y = originalPosition.y + std::sin(animationTime) * 5.0;
            sprite.setPosition(static_cast<float>(position.x), static_cast<float>(position.y));
        }
        GameObject::Update(deltaTime);
    }

    void DrawSFML(sf::RenderTarget& target) const {
        if (!collected) {
            target.draw(sprite);
        }
    }

    void Draw() const override {
        if (!collected) {
            std::cout << "COLLECTIBLE (value: " << value << ") at ";
            position.print();
            std::cout << std::endl;
        }
    }
};

int main() {
    RedirectStdOutToImGui(); // All std::cout will go to our ImGui console

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Epoch of Elria - Interactive Test");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    std::cout << "SFML Window and ImGui Initialized." << std::endl;
    std::cout << "Welcome to the Epoch of Elria interactive test environment!" << std::endl;

    // --- Game Specific Setup ---
    Player player(100, 100);
    std::vector<std::unique_ptr<Collectible>> collectibles;
    for (int i = 0; i < 5; ++i) {
        collectibles.push_back(std::make_unique<Collectible>(rand() % 1000 + 100.0, rand() % 500 + 100.0));
    }
    int score = 0;
    // --- End Game Specific Setup ---

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        double deltaTime = deltaClock.restart().asSeconds();
        ImGui::SFML::Update(window, sf::seconds(static_cast<float>(deltaTime))); // Update ImGui

        // --- ImGui Windows ---
        ImGui::Begin("Game Controls");
        ImGui::Text("DeltaTime: %.3f ms (%.1f FPS)", deltaTime * 1000.0, 1.0 / deltaTime);
        ImGui::Text("Score: %d", score);
        ImGui::Text("Collectibles: %zu", collectibles.size());
        ImGui::Separator();
        ImGui::Text("Controls:");
        ImGui::Text("WASD or Arrow Keys - Move Player");
        ImGui::Text("ESC - Exit Game");
        ImGui::Separator();
        if (ImGui::Button("Add Collectible")) {
             collectibles.push_back(std::make_unique<Collectible>(rand() % 1000 + 100.0, rand() % 500 + 100.0));
             std::cout << "New collectible added." << std::endl;
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset Game")) {
            score = 0;
            collectibles.clear();
            player = Player(400, 300); // Reset player position
            std::cout << "Game reset!" << std::endl;
        }
        ImGui::End();

        gameLog.Draw("Game Console"); // Draw our custom log window

        // --- Game Logic Update ---
        player.Update(deltaTime);

        for (auto& collectible : collectibles) {
            if (collectible) {
                collectible->Update(deltaTime);
                if (!collectible->isCollected() && player.CheckCollision(*collectible)) {
                    score += collectible->getValue();
                    collectible->setCollected(true);
                    std::cout << "Player collected a " << collectible->getName() << "! Score: " << score << std::endl;
                }
            }
        }
        // Remove collected items (basic way)
        collectibles.erase(std::remove_if(collectibles.begin(), collectibles.end(),
            [](const std::unique_ptr<Collectible>& c) { return c->isCollected(); }), collectibles.end());
        // --- End Game Logic Update ---


        // --- Rendering ---
        window.clear(sf::Color(70, 70, 90)); // Clear window with a dark color

        // --- Game Object Rendering ---
        // Draw player
        player.DrawSFML(window);

        // Draw collectibles
        for (const auto& collectible : collectibles) {
            if (collectible) {
                collectible->DrawSFML(window);
            }
        }
        // --- End Game Object Rendering ---

        ImGui::SFML::Render(window); // Render ImGui
        window.display();
    }

    ImGui::SFML::Shutdown();
    RestoreStdOut(); // Restore std::cout before exiting
    return 0;
}