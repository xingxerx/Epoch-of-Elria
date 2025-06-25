#include "GameEnvironment.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>

// GameEnvironment implementation
GameEnvironment::GameEnvironment(float worldWidth, float worldHeight)
    : camera(worldWidth, worldHeight), particleSystem(sf::Vector2f(worldWidth/2, worldHeight/2)),
      backgroundColor(sf::Color::Black), worldSize(worldWidth, worldHeight), gravity(0, 981.0f),
      lastUpdateTime(0), lastRenderTime(0), frameCount(0), fps(0) {
    
    numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 1;
    
    std::cout << "GameEnvironment created: " << worldWidth << "x" << worldHeight 
              << " with " << numThreads << " threads" << std::endl;
}

GameEnvironment::~GameEnvironment() {
    Clear();
}

void GameEnvironment::AddGameObject(std::unique_ptr<GameObject> obj) {
    std::lock_guard<std::mutex> lock(objectMutex);
    gameObjects2D.push_back(std::move(obj));
}

void GameEnvironment::AddGameObject3D(std::unique_ptr<GameObject3D> obj) {
    std::lock_guard<std::mutex> lock(objectMutex);
    gameObjects3D.push_back(std::move(obj));
}

void GameEnvironment::RemoveInactiveObjects() {
    std::lock_guard<std::mutex> lock(objectMutex);
    
    gameObjects2D.erase(
        std::remove_if(gameObjects2D.begin(), gameObjects2D.end(),
                      [](const std::unique_ptr<GameObject>& obj) { return !obj->isActive(); }),
        gameObjects2D.end()
    );
    
    gameObjects3D.erase(
        std::remove_if(gameObjects3D.begin(), gameObjects3D.end(),
                      [](const std::unique_ptr<GameObject3D>& obj) { return !obj->isActive(); }),
        gameObjects3D.end()
    );
}

void GameEnvironment::Update(double deltaTime) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Update 2D objects in parallel
    UpdateObjects2DParallel(deltaTime);
    
    // Update 3D objects in parallel
    UpdateObjects3DParallel(deltaTime);
    
    // Update particle system
    particleSystem.Update(static_cast<float>(deltaTime));
    
    // Update camera
    camera.Update(static_cast<float>(deltaTime));
    
    // Apply physics
    ApplyGravityToAll();
    BoundsCheckAll();
    
    // Remove inactive objects
    RemoveInactiveObjects();
    
    auto endTime = std::chrono::high_resolution_clock::now();
    lastUpdateTime = std::chrono::duration<double>(endTime - startTime).count();
    
    CalculateFPS(deltaTime);
}

void GameEnvironment::Render(sf::RenderWindow& window) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    window.clear(backgroundColor);
    
    // Apply camera
    camera.Apply(window);
    
    // Render 2D objects
    {
        std::lock_guard<std::mutex> lock(objectMutex);
        for (const auto& obj : gameObjects2D) {
            if (obj->isActive()) {
                obj->Draw(window);
            }
        }
    }
    
    // Render 3D objects
    {
        std::lock_guard<std::mutex> lock(objectMutex);
        for (const auto& obj : gameObjects3D) {
            if (obj->isActive()) {
                obj->Draw(window);
            }
        }
    }
    
    // Render particles
    particleSystem.Draw(window);
    
    // Reset view for UI
    window.setView(window.getDefaultView());
    
    // Render debug info
    Graphics::GraphicsManager::GetInstance().GetTextRenderer().DrawText(
        window, "FPS: " + std::to_string(static_cast<int>(fps)), sf::Vector2f(10, 10), 16, sf::Color::White
    );
    
    Graphics::GraphicsManager::GetInstance().GetTextRenderer().DrawText(
        window, "Objects 2D: " + std::to_string(gameObjects2D.size()), sf::Vector2f(10, 30), 16, sf::Color::White
    );
    
    Graphics::GraphicsManager::GetInstance().GetTextRenderer().DrawText(
        window, "Objects 3D: " + std::to_string(gameObjects3D.size()), sf::Vector2f(10, 50), 16, sf::Color::White
    );
    
    Graphics::GraphicsManager::GetInstance().GetTextRenderer().DrawText(
        window, "Particles: " + std::to_string(particleSystem.GetParticleCount()), sf::Vector2f(10, 70), 16, sf::Color::White
    );
    
    auto endTime = std::chrono::high_resolution_clock::now();
    lastRenderTime = std::chrono::duration<double>(endTime - startTime).count();
}

void GameEnvironment::UpdateObjects2DParallel(double deltaTime) {
    if (gameObjects2D.empty()) return;
    
    std::vector<std::thread> threads;
    size_t objectsPerThread = std::max(size_t(1), gameObjects2D.size() / numThreads);
    
    for (unsigned int i = 0; i < numThreads; ++i) {
        size_t start = i * objectsPerThread;
        size_t end = (i == numThreads - 1) ? gameObjects2D.size() : start + objectsPerThread;
        
        if (start >= gameObjects2D.size()) break;
        
        threads.emplace_back(&GameEnvironment::UpdateRange2D, this, start, end, deltaTime);
    }
    
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void GameEnvironment::UpdateObjects3DParallel(double deltaTime) {
    if (gameObjects3D.empty()) return;
    
    std::vector<std::thread> threads;
    size_t objectsPerThread = std::max(size_t(1), gameObjects3D.size() / numThreads);
    
    for (unsigned int i = 0; i < numThreads; ++i) {
        size_t start = i * objectsPerThread;
        size_t end = (i == numThreads - 1) ? gameObjects3D.size() : start + objectsPerThread;
        
        if (start >= gameObjects3D.size()) break;
        
        threads.emplace_back(&GameEnvironment::UpdateRange3D, this, start, end, deltaTime);
    }
    
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void GameEnvironment::UpdateRange2D(size_t start, size_t end, double deltaTime) {
    for (size_t i = start; i < end && i < gameObjects2D.size(); ++i) {
        if (gameObjects2D[i]->isActive()) {
            gameObjects2D[i]->Update(deltaTime);
        }
    }
}

void GameEnvironment::UpdateRange3D(size_t start, size_t end, double deltaTime) {
    for (size_t i = start; i < end && i < gameObjects3D.size(); ++i) {
        if (gameObjects3D[i]->isActive()) {
            gameObjects3D[i]->Update(deltaTime);
        }
    }
}

std::vector<std::pair<GameObject*, GameObject*>> GameEnvironment::DetectCollisions2D() {
    std::vector<std::pair<GameObject*, GameObject*>> collisions;
    std::lock_guard<std::mutex> lock(objectMutex);
    
    for (size_t i = 0; i < gameObjects2D.size(); ++i) {
        for (size_t j = i + 1; j < gameObjects2D.size(); ++j) {
            if (gameObjects2D[i]->isActive() && gameObjects2D[j]->isActive() &&
                gameObjects2D[i]->CheckCollision(*gameObjects2D[j])) {
                collisions.emplace_back(gameObjects2D[i].get(), gameObjects2D[j].get());
            }
        }
    }
    
    return collisions;
}

std::vector<std::pair<GameObject3D*, GameObject3D*>> GameEnvironment::DetectCollisions3D(double threshold) {
    std::vector<std::pair<GameObject3D*, GameObject3D*>> collisions;
    std::lock_guard<std::mutex> lock(objectMutex);
    
    for (size_t i = 0; i < gameObjects3D.size(); ++i) {
        for (size_t j = i + 1; j < gameObjects3D.size(); ++j) {
            if (gameObjects3D[i]->isActive() && gameObjects3D[j]->isActive() &&
                gameObjects3D[i]->IsNear(*gameObjects3D[j], threshold)) {
                collisions.emplace_back(gameObjects3D[i].get(), gameObjects3D[j].get());
            }
        }
    }
    
    return collisions;
}

std::vector<GameObject*> GameEnvironment::GetObjectsInRadius2D(const Vector2D& center, double radius) {
    std::vector<GameObject*> result;
    std::lock_guard<std::mutex> lock(objectMutex);
    
    for (const auto& obj : gameObjects2D) {
        if (obj->isActive()) {
            Vector2D objCenter = obj->GetCenter();
            double distance = (objCenter - center).magnitude();
            if (distance <= radius) {
                result.push_back(obj.get());
            }
        }
    }
    
    return result;
}

std::vector<GameObject3D*> GameEnvironment::GetObjectsInRadius3D(const Vector3D& center, double radius) {
    std::vector<GameObject3D*> result;
    std::lock_guard<std::mutex> lock(objectMutex);
    
    for (const auto& obj : gameObjects3D) {
        if (obj->isActive()) {
            double distance = obj->DistanceTo(GameObject3D("temp", center));
            if (distance <= radius) {
                result.push_back(obj.get());
            }
        }
    }
    
    return result;
}

void GameEnvironment::Clear() {
    std::lock_guard<std::mutex> lock(objectMutex);
    gameObjects2D.clear();
    gameObjects3D.clear();
    particleSystem.Clear();
}

void GameEnvironment::ApplyGravityToAll() {
    std::lock_guard<std::mutex> lock(objectMutex);
    
    for (const auto& obj : gameObjects2D) {
        if (obj->isActive()) {
            Vector2D currentVel = obj->getVelocity();
            obj->setVelocity(currentVel.x + gravity.x, currentVel.y + gravity.y);
        }
    }
}

void GameEnvironment::BoundsCheckAll() {
    std::lock_guard<std::mutex> lock(objectMutex);
    
    for (const auto& obj : gameObjects2D) {
        if (obj->isActive()) {
            obj->ClampToBounds(0, 0, worldSize.x, worldSize.y);
        }
    }
}

void GameEnvironment::CalculateFPS(double deltaTime) {
    frameCount++;
    static double timeAccumulator = 0;
    timeAccumulator += deltaTime;
    
    if (timeAccumulator >= 1.0) {
        fps = frameCount / timeAccumulator;
        frameCount = 0;
        timeAccumulator = 0;
    }
}

// PlatformerEnvironment implementation
PlatformerEnvironment::PlatformerEnvironment(float worldWidth, float worldHeight)
    : GameEnvironment(worldWidth, worldHeight) {
    SetGravity(sf::Vector2f(0, 500.0f)); // Lighter gravity for platformer
}

void PlatformerEnvironment::AddPlatform(const sf::FloatRect& platform) {
    platforms.push_back(platform);
}

void PlatformerEnvironment::RemovePlatform(size_t index) {
    if (index < platforms.size()) {
        platforms.erase(platforms.begin() + index);
    }
}

void PlatformerEnvironment::Update(double deltaTime) {
    GameEnvironment::Update(deltaTime);
    
    // Handle platform collisions
    std::lock_guard<std::mutex> lock(objectMutex);
    for (const auto& obj : gameObjects2D) {
        if (obj->isActive()) {
            for (const auto& platform : platforms) {
                // Simple platform collision (landing on top)
                Vector2D objPos = obj->getPosition();
                Vector2D objVel = obj->getVelocity();
                
                if (objPos.x + obj->getWidth() > platform.left &&
                    objPos.x < platform.left + platform.width &&
                    objPos.y + obj->getHeight() > platform.top &&
                    objPos.y < platform.top + platform.height &&
                    objVel.y > 0) {
                    
                    // Land on platform
                    obj->setPosition(objPos.x, platform.top - obj->getHeight());
                    obj->setVelocity(objVel.x, 0);
                }
            }
        }
    }
}

void PlatformerEnvironment::Render(sf::RenderWindow& window) {
    // Render platforms first
    GetCamera().Apply(window);
    
    for (const auto& platform : platforms) {
        sf::RectangleShape platformShape(sf::Vector2f(platform.width, platform.height));
        platformShape.setPosition(platform.left, platform.top);
        platformShape.setFillColor(sf::Color(100, 100, 100));
        platformShape.setOutlineColor(sf::Color::White);
        platformShape.setOutlineThickness(2.0f);
        window.draw(platformShape);
    }
    
    // Render everything else
    GameEnvironment::Render(window);
}

bool PlatformerEnvironment::IsOnPlatform(const GameObject& obj) const {
    Vector2D objPos = obj.getPosition();
    
    for (const auto& platform : platforms) {
        if (objPos.x + obj.getWidth() > platform.left &&
            objPos.x < platform.left + platform.width &&
            std::abs(objPos.y + obj.getHeight() - platform.top) < 5.0) {
            return true;
        }
    }
    return false;
}

sf::FloatRect* PlatformerEnvironment::GetPlatformBelow(const GameObject& obj) {
    Vector2D objPos = obj.getPosition();
    
    for (auto& platform : platforms) {
        if (objPos.x + obj.getWidth() > platform.left &&
            objPos.x < platform.left + platform.width &&
            platform.top > objPos.y + obj.getHeight() &&
            platform.top < objPos.y + obj.getHeight() + 100) {
            return &platform;
        }
    }
    return nullptr;
}

// SpaceEnvironment implementation
SpaceEnvironment::SpaceEnvironment(float worldWidth, float worldHeight)
    : GameEnvironment(worldWidth, worldHeight), starSpeed(50.0) {
    SetGravity(sf::Vector2f(0, 0)); // No gravity in space
    SetBackgroundColor(sf::Color::Black);
    GenerateStars(200);
}

void SpaceEnvironment::GenerateStars(int count) {
    stars.clear();
    sf::Vector2f worldSize = GetWorldSize();
    
    for (int i = 0; i < count; ++i) {
        stars.emplace_back(
            static_cast<double>(rand() % static_cast<int>(worldSize.x)),
            static_cast<double>(rand() % static_cast<int>(worldSize.y)),
            static_cast<double>(rand() % 1000) // Z depth for parallax
        );
    }
}

void SpaceEnvironment::Update(double deltaTime) {
    GameEnvironment::Update(deltaTime);
    UpdateStars(deltaTime);
}

void SpaceEnvironment::Render(sf::RenderWindow& window) {
    RenderStars(window);
    GameEnvironment::Render(window);
}

void SpaceEnvironment::UpdateStars(double deltaTime) {
    sf::Vector2f worldSize = GetWorldSize();
    
    for (auto& star : stars) {
        star.y += starSpeed * deltaTime * (1.0 + star.z / 1000.0); // Parallax effect
        
        if (star.y > worldSize.y) {
            star.y = -10;
            star.x = static_cast<double>(rand() % static_cast<int>(worldSize.x));
        }
    }
}

void SpaceEnvironment::RenderStars(sf::RenderWindow& window) {
    GetCamera().Apply(window);
    
    for (const auto& star : stars) {
        float brightness = static_cast<float>(star.z / 1000.0);
        sf::Uint8 alpha = static_cast<sf::Uint8>(255 * (0.3f + 0.7f * brightness));
        
        sf::CircleShape starShape(1.0f + brightness * 2.0f);
        starShape.setPosition(static_cast<float>(star.x), static_cast<float>(star.y));
        starShape.setFillColor(sf::Color(255, 255, 255, alpha));
        window.draw(starShape);
    }
}