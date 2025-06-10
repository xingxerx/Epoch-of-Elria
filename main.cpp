// main.cpp - Epoch of Elria Game Engine
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <memory>

// --- 1. Vector2D Class ---
// Represents a 2D point or vector in space.
// Essential for positions, velocities, and sizes in games.
class Vector2D {
public:
    double x; // X-coordinate
    double y; // Y-coordinate

    // Default constructor
    Vector2D() : x(0.0), y(0.0) {}

    // Parameterized constructor
    Vector2D(double in_x, double in_y) : x(in_x), y(in_y) {}

    // Overload the + operator for vector addition
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    // Overload the - operator for vector subtraction
    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    // Overload the * operator for scalar multiplication
    Vector2D operator*(double scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    // Overload the / operator for scalar division
    Vector2D operator/(double scalar) const {
        if (scalar != 0) {
            return Vector2D(x / scalar, y / scalar);
        }
        return Vector2D(0.0, 0.0); // Avoid division by zero
    }

    // Calculate the magnitude (length) of the vector
    double magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    // Normalize the vector (make its magnitude 1)
    Vector2D normalize() const {
        double mag = magnitude();
        if (mag != 0) {
            return *this / mag;
        }
        return Vector2D(0.0, 0.0); // Return zero vector if magnitude is zero
    }

    // Print the vector (for debugging)
    void print() const {
        std::cout << "(" << x << ", " << y << ")";
    }
};

// --- 2. GameObject Class ---
// A base class for any entity in the game world (player, enemy, projectile, etc.).
class GameObject {
protected:
    Vector2D position;  // Current position in the game world
    Vector2D velocity;  // Current velocity (how fast and in which direction it's moving)
    double width;       // Width of the object
    double height;      // Height of the object
    std::string name;   // Name of the object (e.g., "Player", "Enemy1")
    sf::Color color;    // Color for SFML rendering

public:
    // Constructor
    GameObject(std::string obj_name, double start_x, double start_y, double obj_width, double obj_height, sf::Color obj_color = sf::Color::White)
        : name(obj_name), position(start_x, start_y), width(obj_width), height(obj_height), velocity(0.0, 0.0), color(obj_color) {}

    // Virtual destructor to ensure proper cleanup for derived classes
    virtual ~GameObject() = default;

    // --- Getters ---
    const Vector2D& getPosition() const { return position; }
    const Vector2D& getVelocity() const { return velocity; }
    double getWidth() const { return width; }
    double getHeight() const { return height; }
    const std::string& getName() const { return name; }
    sf::Color getColor() const { return color; }

    // --- Setters ---
    void setPosition(double x, double y) { position.x = x; position.y = y; }
    void setVelocity(double vx, double vy) { velocity.x = vx; velocity.y = vy; }
    void setColor(sf::Color new_color) { color = new_color; }

    // --- Core Game Loop Methods (virtual for overriding by derived classes) ---

    // Update method: Called every frame to update the object's state (e.g., position based on velocity).
    virtual void Update(double deltaTime) {
        position = position + (velocity * deltaTime);
    }

    // Draw method: Called every frame to render the object using SFML.
    virtual void Draw(sf::RenderWindow& window) const {
        sf::RectangleShape shape(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
        shape.setPosition(static_cast<float>(position.x), static_cast<float>(position.y));
        shape.setFillColor(color);
        shape.setOutlineThickness(1.0f);
        shape.setOutlineColor(sf::Color::Black);
        window.draw(shape);
    }

    // SVG export method for debugging/recording
    virtual void DrawSVG(std::ofstream& svg) const {
        svg << "<rect x='" << position.x << "' y='" << position.y
            << "' width='" << width << "' height='" << height
            << "' fill='rgb(" << static_cast<int>(color.r) << "," 
            << static_cast<int>(color.g) << "," << static_cast<int>(color.b) 
            << ")' stroke='black' stroke-width='1'/>\n";
    }

    // Basic collision detection (Axis-Aligned Bounding Box - AABB)
    bool CheckCollision(const GameObject& other) const {
        return (position.x < other.position.x + other.width &&
                position.x + width > other.position.x &&
                position.y < other.position.y + other.height &&
                position.y + height > other.position.y);
    }
};

// --- Example Derived Class: Player ---
class Player : public GameObject {
private:
    double speed;
    bool keys[4] = {false}; // W, A, S, D

public:
    Player(double start_x, double start_y)
        : GameObject("Player", start_x, start_y, 50, 50, sf::Color::Blue), speed(200.0) {
        std::cout << "Player created!" << std::endl;
    }

    void HandleInput(const sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::W: keys[0] = true; break;
                case sf::Keyboard::A: keys[1] = true; break;
                case sf::Keyboard::S: keys[2] = true; break;
                case sf::Keyboard::D: keys[3] = true; break;
                default: break;
            }
        } else if (event.type == sf::Event::KeyReleased) {
            switch (event.key.code) {
                case sf::Keyboard::W: keys[0] = false; break;
                case sf::Keyboard::A: keys[1] = false; break;
                case sf::Keyboard::S: keys[2] = false; break;
                case sf::Keyboard::D: keys[3] = false; break;
                default: break;
            }
        }
    }

    void Update(double deltaTime) override {
        // Update velocity based on input
        velocity.x = 0.0;
        velocity.y = 0.0;

        if (keys[0]) velocity.y -= speed; // W - Up
        if (keys[1]) velocity.x -= speed; // A - Left
        if (keys[2]) velocity.y += speed; // S - Down
        if (keys[3]) velocity.x += speed; // D - Right

        // Normalize diagonal movement
        if (velocity.magnitude() > speed) {
            velocity = velocity.normalize() * speed;
        }

        GameObject::Update(deltaTime); // Call base class Update for generic movement

        // Keep player within screen bounds
        if (position.x < 0) position.x = 0;
        if (position.y < 0) position.y = 0;
        if (position.x + width > 800) position.x = 800 - width;
        if (position.y + height > 600) position.y = 600 - height;
    }
};

// --- Example Derived Class: Collectible ---
class Collectible : public GameObject {
private:
    int value; // How many points this collectible is worth
    bool collected; // Is it collected?
    mutable std::mutex collectible_mutex; // Mutex to protect access if needed by multiple threads
    double rotation; // For visual effect

public:
    Collectible(double start_x, double start_y, int collect_value)
        : GameObject("Collectible", start_x, start_y, 20, 20, sf::Color::Yellow),
          value(collect_value), collected(false), rotation(0.0) {}

    int getValue() const { return value; }
    
    bool isCollected() const {
        std::lock_guard<std::mutex> lock(collectible_mutex); // Protect read
        return collected;
    }
    
    void setCollected(bool status) {
        std::lock_guard<std::mutex> lock(collectible_mutex); // Protect write
        collected = status;
    }

    void Update(double deltaTime) override {
        if (!isCollected()) {
            // Add some rotation for visual effect
            rotation += 90.0 * deltaTime; // 90 degrees per second
            if (rotation >= 360.0) rotation -= 360.0;
            
            // Simulate some minor "work" that would be done in parallel
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
        GameObject::Update(deltaTime);
    }

    void Draw(sf::RenderWindow& window) const override {
        if (!isCollected()) {
            sf::CircleShape shape(static_cast<float>(width / 2));
            shape.setPosition(static_cast<float>(position.x), static_cast<float>(position.y));
            shape.setFillColor(color);
            shape.setOutlineThickness(2.0f);
            shape.setOutlineColor(sf::Color::Red);
            
            // Apply rotation
            shape.setOrigin(static_cast<float>(width / 2), static_cast<float>(height / 2));
            shape.setRotation(static_cast<float>(rotation));
            
            window.draw(shape);
        }
    }

    void DrawSVG(std::ofstream& svg) const override {
        if (!isCollected()) {
            svg << "<circle cx='" << (position.x + width/2) << "' cy='" << (position.y + height/2)
                << "' r='" << (width/2) << "' fill='gold' stroke='red' stroke-width='2'/>\n";
        }
    }
};

// Function to update a range of collectibles using indices
void UpdateCollectiblesRange(std::vector<std::unique_ptr<Collectible>>& collectibles, 
                           size_t start_index, size_t end_index, double deltaTime) {
    for (size_t i = start_index; i < end_index; ++i) {
        if (i < collectibles.size() && !collectibles[i]->isCollected()) {
            collectibles[i]->Update(deltaTime);
        }
    }
}

// SVG utility functions for recording/debugging
void svg_start(std::ofstream& svgfile) {
    svgfile << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"600\">" << std::endl;
}

void svg_end(std::ofstream& svgfile) {
    svgfile << "</svg>" << std::endl;
}

// --- Main Game Class ---
class Game {
private:
    sf::RenderWindow window;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Collectible>> collectibles;
    sf::Clock clock;
    int totalScore;
    sf::Font font;
    sf::Text scoreText;
    bool recordSVG;
    int frameCount;

public:
    Game() : window(sf::VideoMode(800, 600), "Epoch of Elria - Game Engine Demo"),
             totalScore(0), recordSVG(false), frameCount(0) {
        
        // Initialize player
        player = std::make_unique<Player>(375, 275); // Center of screen
        
        // Create collectibles
        const int NUM_COLLECTIBLES = 50; // Reduced for better performance in real-time
        for (int i = 0; i < NUM_COLLECTIBLES; ++i) {
            collectibles.push_back(std::make_unique<Collectible>(
                static_cast<double>(rand() % 760), // Random X (leave margin)
                static_cast<double>(rand() % 560), // Random Y (leave margin)
                10 // Value
            ));
        }

        // Try to load font (optional)
        if (!font.loadFromFile("arial.ttf")) {
            // Use default font if arial.ttf not found
            std::cout << "Warning: Could not load arial.ttf, using default font" << std::endl;
        }
        
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10, 10);
        
        window.setFramerateLimit(60);
        std::cout << "Game initialized with " << collectibles.size() << " collectibles" << std::endl;
    }

    void Run() {
        while (window.isOpen()) {
            HandleEvents();
            Update();
            Render();
        }
    }

private:
    void HandleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R) {
                    recordSVG = !recordSVG;
                    std::cout << "SVG Recording " << (recordSVG ? "ON" : "OFF") << std::endl;
                } else if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
            
            // Pass input to player
            player->HandleInput(event);
        }
    }

    void Update() {
        double deltaTime = clock.restart().asSeconds();
        
        // Update player
        player->Update(deltaTime);
        
        // Update collectibles in parallel
        UpdateCollectiblesParallel(deltaTime);
        
        // Check collisions
        CheckCollisions();
        
        // Update score display
        scoreText.setString("Score: " + std::to_string(totalScore) + 
                           " | Collectibles: " + std::to_string(GetActiveCollectibles()) +
                           " | Press R to toggle SVG recording");
    }

    void UpdateCollectiblesParallel(double deltaTime) {
        unsigned int num_threads = std::thread::hardware_concurrency();
        if (num_threads == 0) num_threads = 1;
        
        std::vector<std::thread> threads;
        unsigned int effective_threads = std::min(num_threads, static_cast<unsigned int>(collectibles.size()));
        if (effective_threads == 0) effective_threads = 1;
        
        size_t collectibles_per_thread = collectibles.size() / effective_threads;
        if (collectibles_per_thread == 0) collectibles_per_thread = 1;
        
        for (unsigned int i = 0; i < effective_threads; ++i) {
            size_t start_index = i * collectibles_per_thread;
            size_t end_index = (i == effective_threads - 1) ? collectibles.size() : (start_index + collectibles_per_thread);
            
            if (start_index >= collectibles.size()) break;
            
            threads.emplace_back(UpdateCollectiblesRange, std::ref(collectibles), start_index, end_index, deltaTime);
        }
        
        for (std::thread& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

    void CheckCollisions() {
        for (auto& collectible : collectibles) {
            if (!collectible->isCollected() && player->CheckCollision(*collectible)) {
                totalScore += collectible->getValue();
                collectible->setCollected(true);
                std::cout << "Collected! Score: " << totalScore << std::endl;
            }
        }
    }

    int GetActiveCollectibles() const {
        int count = 0;
        for (const auto& collectible : collectibles) {
            if (!collectible->isCollected()) count++;
        }
        return count;
    }

    void Render() {
        window.clear(sf::Color::Black);
        
        // Draw all game objects
        player->Draw(window);
        for (const auto& collectible : collectibles) {
            collectible->Draw(window);
        }
        
        // Draw UI
        window.draw(scoreText);
        
        window.display();
        
        // Record SVG if enabled
        if (recordSVG) {
            RecordSVGFrame();
        }
    }

    void RecordSVGFrame() {
        std::string filename = "game_frame_" + std::to_string(frameCount++) + ".svg";
        std::ofstream svgfile(filename);
        svg_start(svgfile);
        
        player->DrawSVG(svgfile);
        for (const auto& collectible : collectibles) {
            collectible->DrawSVG(svgfile);
        }
        
        svg_end(svgfile);
        svgfile.close();
    }
};

// --- Main Function ---
int main() {
    // Seed random number generator for collectible positions
    srand(static_cast<unsigned int>(time(nullptr)));
    
    std::cout << "=== Epoch of Elria Game Engine ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD - Move player" << std::endl;
    std::cout << "  R - Toggle SVG recording" << std::endl;
    std::cout << "  ESC - Exit game" << std::endl;
    std::cout << "===================================" << std::endl;
    
    try {
        Game game;
        game.Run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}