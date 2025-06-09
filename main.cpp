// main.cpp
#include <iostream> // For console output
#include <vector>   // For dynamic arrays of game objects
#include <string>   // For object names
#include <cmath>    // For mathematical operations
#include <thread>   // For parallel processing with std::thread
#include <mutex>    // For std::mutex (though not strictly necessary for this simple example, good practice)
#include <chrono>   // For simulating work (optional, but good for demoing multithreading)
#include <cstdlib>  // For rand(), srand()
#include <ctime>    // For time()
#include <fstream>    // For file output
#include <sstream>    // For string stream

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

public:
    // Constructor
    GameObject(std::string obj_name, double start_x, double start_y, double obj_width, double obj_height)
        : name(obj_name), position(start_x, start_y), width(obj_width), height(obj_height), velocity(0.0, 0.0) {}

    // Virtual destructor to ensure proper cleanup for derived classes
    virtual ~GameObject() {
        // std::cout << "GameObject '" << name << "' destroyed." << std::endl; // Commented to reduce excessive output
    }

    // --- Getters ---
    const Vector2D& getPosition() const { return position; }
    const Vector2D& getVelocity() const { return velocity; }
    double getWidth() const { return width; }
    double getHeight() const { return height; }
    const std::string& getName() const { return name; }

    // --- Setters ---
    void setPosition(double x, double y) { position.x = x; position.y = y; }
    void setVelocity(double vx, double vy) { velocity.x = vx; velocity.y = vy; }

    // --- Core Game Loop Methods (virtual for overriding by derived classes) ---

    // Update method: Called every frame to update the object's state (e.g., position based on velocity).
    virtual void Update(double deltaTime) {
        position = position + (velocity * deltaTime);
    }

    // Draw method: Called every frame to render the object.
    virtual void Draw() const {
        // std::cout << "Drawing " << name << " at position "; // Commented to reduce excessive output
        // position.print();
        // std::cout << " with size (" << width << ", " << height << ")" << std::endl;
        // This is a placeholder. Actual drawing would use a graphics API (OpenGL, DirectX, Vulkan).
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
public:
    Player(double start_x, double start_y)
        : GameObject("Player", start_x, start_y, 50, 50) { // Player is 50x50
        std::cout << "Player created!" << std::endl;
    }

    void Update(double deltaTime) override {
        // For this console example, we'll just move it right for a bit
        // In a real game, this would read input or follow complex AI
        velocity.x = 50.0; // Move right at 50 units/second
        GameObject::Update(deltaTime); // Call base class Update for generic movement
    }

    void Draw(std::ofstream& svg) const override {
        svg << "<rect x=\"" << position.x << "\" y=\"" << position.y
            << "\" width=\"" << width << "\" height=\"" << height
            << "\" fill=\"blue\"/>\n";
    }
};

// --- Example Derived Class: Collectible ---
class Collectible : public GameObject {
private:
    int value; // How many points this collectible is worth
    bool collected; // Is it collected?
    mutable std::mutex collectible_mutex; // Mutex to protect access if needed by multiple threads

public:
    Collectible(double start_x, double start_y, int collect_value)
        : GameObject("Collectible", start_x, start_y, 20, 20), // Collectible is 20x20
          value(collect_value), collected(false) {
        // std::cout << "Collectible created at "; // Commented to reduce excessive output
        // position.print();
        // std::cout << " with value " << value << std::endl;
    }

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
        // Collectibles could also have their own movement/animation logic
        // For now, they just stay put.
        // Simulate some minor "work" that would be done in parallel
        std::this_thread::sleep_for(std::chrono::microseconds(10)); // Simulate a tiny bit of work
        GameObject::Update(deltaTime);
    }

    void Draw(std::ofstream& svg) const override {
        if (!isCollected()) {
            svg << "<rect x=\"" << position.x << "\" y=\"" << position.y
                << "\" width=\"" << width << "\" height=\"" << height
                << "\" fill=\"gold\"/>\n";
        }
    }
};


// Function to update a range of collectibles using indices
void UpdateCollectiblesRange(std::vector<Collectible*>& collectibles, size_t start_index, size_t end_index, double deltaTime) {
    for (size_t i = start_index; i < end_index; ++i) {
        if (i < collectibles.size() && !collectibles[i]->isCollected()) {
            collectibles[i]->Update(deltaTime);
        }
    }
}

// --- Main Game Simulation Loop ---
// SVG helper functions
void startSVGFile(std::ofstream& file, int width = 800, int height = 400) {
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
         << "<svg width=\"" << width << "\" height=\"" << height
         << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
}

void endSVGFile(std::ofstream& file) {
    file << "</svg>\n";
}

void RunGameSimulation() {
    std::cout << "\n--- Starting C++ Game Simulation (with Parallel Collectible Updates) ---" << std::endl;

    // Create game objects
    Player player(100, 100);

    std::vector<Collectible*> collectibles;
    // Create a good number of collectibles to demonstrate parallel processing
    const int NUM_COLLECTIBLES = 1000;
    for (int i = 0; i < NUM_COLLECTIBLES; ++i) {
        collectibles.push_back(new Collectible(
            static_cast<double>(rand() % 800), // Random X (assuming canvas width 800)
            static_cast<double>(rand() % 400), // Random Y (assuming canvas height 400)
            10 // Value
        ));
    }

    // Determine how many threads to use
    // Using std::thread::hardware_concurrency() is a good default for real systems
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 1; // Fallback if hardware_concurrency returns 0
    std::cout << "Using " << num_threads << " threads for collectible updates." << std::endl;

    double deltaTime = 1.0 / 60.0; // Simulate 60 frames per second
    int totalScore = 0;

    // Simulate a few frames of the game
    for (int frame = 0; frame < 50; ++frame) { // Simulate 50 frames
        std::cout << "\n--- Frame " << frame << " ---" << std::endl;

        // 1. Update Player (on main thread)
        player.Update(deltaTime);
        player.Draw(); // Draw player immediately after update

        // 2. Update Collectibles in Parallel (Improved Version)
        std::vector<std::thread> threads;

        // Ensure we don't create more threads than collectibles
        unsigned int effective_threads = std::min(num_threads, static_cast<unsigned int>(collectibles.size()));
        if (effective_threads == 0) effective_threads = 1;

        // Divide collectibles into chunks for each thread
        size_t collectibles_per_thread = collectibles.size() / effective_threads;
        if (collectibles_per_thread == 0) collectibles_per_thread = 1;

        for (unsigned int i = 0; i < effective_threads; ++i) {
            size_t start_index = i * collectibles_per_thread;
            size_t end_index = (i == effective_threads - 1) ? collectibles.size() : (start_index + collectibles_per_thread);

            // Skip if this thread would have no work
            if (start_index >= collectibles.size()) break;

            // Launch a thread to update this range using indices (safer approach)
            threads.emplace_back(UpdateCollectiblesRange, std::ref(collectibles), start_index, end_index, deltaTime);
        }

        // Wait for all threads to complete their updates
        for (std::thread& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
        // At this point, all collectibles' positions have been updated in parallel.

        // 3. Check for collisions (can be parallelized too, but let's keep it simple for now)
        // This is done sequentially after all updates are complete to avoid complex synchronization
        // issues if `collectibles` vector itself was modified (e.g., elements removed) during update.
        for (Collectible* collect : collectibles) {
            if (!collect->isCollected() && player.CheckCollision(*collect)) {
                totalScore += collect->getValue();
                collect->setCollected(true); // Mark as collected
                std::cout << "Player collected " << collect->getName() << " at ";
                collect->getPosition().print();
                std::cout << "! Score: " << totalScore << std::endl;
            }
        }

        // 4. Draw all game objects to SVG file
        std::stringstream filename;
        filename << "game_frame_" << frame << ".svg";
        std::ofstream svgFile(filename.str());
        startSVGFile(svgFile);

        // Draw all objects
        player.Draw(svgFile);
        for (Collectible* collect : collectibles) {
            collect->Draw(svgFile);
        }

        endSVGFile(svgFile);
        svgFile.close();
    }

    std::cout << "\n--- End of C++ Game Simulation ---" << std::endl;
    std::cout << "Final Score: " << totalScore << std::endl;

    // Clean up dynamically allocated collectibles
    for (Collectible* collect : collectibles) {
        delete collect;
    }
    collectibles.clear();
}

int main() {
    // Seed random number generator for collectible positions
    srand(static_cast<unsigned int>(time(NULL)));
    RunGameSimulation();
    return 0;
}
