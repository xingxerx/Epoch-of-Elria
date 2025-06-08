#ifndef RPGCHARACTER_H
#define RPGCHARACTER_H

#include "Vector3D.h"
#include "GameObject3D.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

// Forward declarations
class World3D;
class StoryElement;
class MemoryFragment;
class ConsciousEcho;

// --- Base RPG Character Stats ---
struct CharacterStats {
    int level;
    int experience;
    int maxHealth;
    int currentHealth;
    int maxMana;
    int currentMana;
    
    // RPG Attributes
    int narrative_power;    // Ability to manipulate stories
    int memory_strength;    // Resistance to mental attacks
    int empathy;           // Connection to other consciousness
    int creativity;        // Ability to create new realities
    int liberation_force;  // Power to free trapped narratives
    
    CharacterStats() : level(1), experience(0), maxHealth(100), currentHealth(100),
                      maxMana(50), currentMana(50), narrative_power(10), 
                      memory_strength(10), empathy(10), creativity(10), liberation_force(10) {}
};

// --- Story Elements (RPG Items/Spells) ---
class StoryElement {
public:
    enum ElementType {
        CONCEPT,        // Abstract ideas
        MEMORY,         // Memory fragments
        EMOTION,        // Emotional resonance
        NARRATIVE,      // Story threads
        DREAM_FRAGMENT  // Pieces of dreams
    };
    
private:
    std::string name;
    std::string description;
    ElementType type;
    int power;
    Vector3D manifestation_color;
    
public:
    StoryElement(const std::string& n, ElementType t, int p, const Vector3D& color = Vector3D(1,1,1))
        : name(n), type(t), power(p), manifestation_color(color) {}
    
    const std::string& getName() const { return name; }
    ElementType getType() const { return type; }
    int getPower() const { return power; }
    const Vector3D& getColor() const { return manifestation_color; }
    void setDescription(const std::string& desc) { description = desc; }
    const std::string& getDescription() const { return description; }
};

// --- Base RPG Character Class ---
class RPGCharacter : public GameObject3D {
protected:
    CharacterStats stats;
    std::vector<std::unique_ptr<StoryElement>> inventory;
    std::vector<std::string> abilities;
    std::string characterClass;
    std::string backstory;
    
    // Character state
    bool isAwakened;
    double awakenessLevel; // 0.0 to 1.0
    Vector3D auraColor;
    
public:
    RPGCharacter(const std::string& name, const std::string& charClass, 
                 const Vector3D& startPos = Vector3D::Zero())
        : GameObject3D(name, startPos, Vector3D(1, 2, 1)), characterClass(charClass),
          isAwakened(false), awakenessLevel(0.0), auraColor(1, 1, 1) {}
    
    virtual ~RPGCharacter() = default;
    
    // Getters
    const CharacterStats& getStats() const { return stats; }
    const std::string& getCharacterClass() const { return characterClass; }
    bool getIsAwakened() const { return isAwakened; }
    double getAwakenessLevel() const { return awakenessLevel; }
    const Vector3D& getAuraColor() const { return auraColor; }
    
    // Character progression
    virtual void gainExperience(int exp) {
        stats.experience += exp;
        checkLevelUp();
    }
    
    virtual void levelUp() {
        stats.level++;
        stats.maxHealth += 20;
        stats.maxMana += 10;
        stats.currentHealth = stats.maxHealth;
        stats.currentMana = stats.maxMana;
        
        // Increase random stats
        stats.narrative_power += 2;
        stats.memory_strength += 2;
        stats.empathy += 1;
        stats.creativity += 1;
        stats.liberation_force += 1;
        
        std::cout << name << " reached level " << stats.level << "!" << std::endl;
    }
    
    virtual void awaken(double level = 1.0) {
        isAwakened = true;
        awakenessLevel = std::min(1.0, level);
        std::cout << name << " awakens to consciousness level " << awakenessLevel << "!" << std::endl;
    }
    
    // Inventory management
    void addStoryElement(std::unique_ptr<StoryElement> element) {
        std::cout << name << " acquired story element: " << element->getName() << std::endl;
        inventory.push_back(std::move(element));
    }
    
    // Virtual abilities (to be overridden by specific characters)
    virtual void useSpecialAbility(const std::string& abilityName, World3D* world = nullptr) = 0;
    virtual void castNarrativeSpell(const std::string& spellName, const Vector3D& target) = 0;
    virtual void interact(RPGCharacter& other) = 0;
    
    // Combat and health
    virtual void takeDamage(int damage) {
        stats.currentHealth = std::max(0, stats.currentHealth - damage);
        if (stats.currentHealth == 0) {
            std::cout << name << " has fallen unconscious!" << std::endl;
        }
    }
    
    virtual void heal(int amount) {
        stats.currentHealth = std::min(stats.maxHealth, stats.currentHealth + amount);
    }
    
    virtual void restoreMana(int amount) {
        stats.currentMana = std::min(stats.maxMana, stats.currentMana + amount);
    }
    
    // Display character info
    virtual void displayStatus() const {
        std::cout << "\n=== " << name << " (" << characterClass << ") ===" << std::endl;
        std::cout << "Level: " << stats.level << " | XP: " << stats.experience << std::endl;
        std::cout << "Health: " << stats.currentHealth << "/" << stats.maxHealth << std::endl;
        std::cout << "Mana: " << stats.currentMana << "/" << stats.maxMana << std::endl;
        std::cout << "Position: ";
        transform.position.print();
        std::cout << std::endl;
        std::cout << "Awakened: " << (isAwakened ? "Yes" : "No");
        if (isAwakened) std::cout << " (" << (awakenessLevel * 100) << "%)";
        std::cout << std::endl;
        std::cout << "Narrative Power: " << stats.narrative_power << std::endl;
        std::cout << "Memory Strength: " << stats.memory_strength << std::endl;
        std::cout << "Empathy: " << stats.empathy << std::endl;
        std::cout << "Creativity: " << stats.creativity << std::endl;
        std::cout << "Liberation Force: " << stats.liberation_force << std::endl;
        std::cout << "Story Elements: " << inventory.size() << std::endl;
        std::cout << "================================" << std::endl;
    }
    
protected:
    void checkLevelUp() {
        int expNeeded = stats.level * 100; // Simple progression
        if (stats.experience >= expNeeded) {
            levelUp();
        }
    }
    
    bool consumeMana(int cost) {
        if (stats.currentMana >= cost) {
            stats.currentMana -= cost;
            return true;
        }
        std::cout << name << " doesn't have enough mana!" << std::endl;
        return false;
    }
};

// --- Memory Fragment Class ---
class MemoryFragment {
private:
    std::string content;
    std::string emotional_context;
    Vector3D origin_location;
    bool is_recovered;
    double clarity; // 0.0 to 1.0
    
public:
    MemoryFragment(const std::string& memory, const Vector3D& location, double clearness = 0.5)
        : content(memory), origin_location(location), is_recovered(false), clarity(clearness) {}
    
    const std::string& getContent() const { return content; }
    const Vector3D& getOriginLocation() const { return origin_location; }
    bool isRecovered() const { return is_recovered; }
    double getClarity() const { return clarity; }
    
    void recover() { is_recovered = true; }
    void enhanceClarity(double amount) { 
        clarity = std::min(1.0, clarity + amount); 
    }
    
    void setEmotionalContext(const std::string& context) { 
        emotional_context = context; 
    }
    
    const std::string& getEmotionalContext() const { return emotional_context; }
};

// --- Conscious Echo Class ---
class ConsciousEcho {
private:
    std::string narrative_essence;
    Vector3D resonance_frequency; // Represented as 3D vector
    bool is_awakened;
    double consciousness_level;
    std::vector<std::string> contributed_stories;
    
public:
    ConsciousEcho(const std::string& essence, const Vector3D& frequency)
        : narrative_essence(essence), resonance_frequency(frequency), 
          is_awakened(false), consciousness_level(0.0) {}
    
    void awaken() { 
        is_awakened = true; 
        consciousness_level = 0.1;
        std::cout << "Conscious Echo '" << narrative_essence << "' awakens!" << std::endl;
    }
    
    void contributeStory(const std::string& story) {
        contributed_stories.push_back(story);
        consciousness_level += 0.1;
        std::cout << "Echo contributes: " << story << std::endl;
    }
    
    bool isAwakened() const { return is_awakened; }
    double getConsciousnessLevel() const { return consciousness_level; }
    const std::string& getEssence() const { return narrative_essence; }
    const Vector3D& getFrequency() const { return resonance_frequency; }
};

#endif // RPGCHARACTER_H
