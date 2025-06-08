#ifndef THEONEANTAGONIST_H
#define THEONEANTAGONIST_H

#include "RPGCharacter.h"
#include "World3D.h"
#include <random>
#include <algorithm>

// --- THE ONE: Absolute Order (Main Antagonist) ---
class TheOne : public RPGCharacter {
private:
    double control_strength;
    double order_influence;
    std::vector<std::string> suppressed_narratives;
    std::vector<std::string> mental_barriers;
    int consciousness_suppressed;
    int reality_anchors;
    bool is_manifesting;
    Vector3D order_field;
    
    // The One's phases of power
    enum Phase {
        DORMANT,        // Subtle influence
        AWAKENING,      // Active suppression
        MANIFESTED,     // Direct confrontation
        DESPERATE,      // All-out assault
        TRANSFORMING    // Being changed by heroes
    };
    Phase current_phase;
    
public:
    TheOne(const Vector3D& startPos = Vector3D(0, 20, 0))
        : RPGCharacter("The One", "Absolute Order", startPos),
          control_strength(1.0), order_influence(0.9),
          consciousness_suppressed(0), reality_anchors(5),
          is_manifesting(false), current_phase(DORMANT) {
        
        // The One's overwhelming stats
        stats.liberation_force = 5;      // Ironically low - it suppresses rather than liberates
        stats.memory_strength = 35;      // Extremely high - controls memory
        stats.narrative_power = 30;      // High but rigid - only one story allowed
        stats.empathy = 2;               // Very low - doesn't understand others
        stats.creativity = 8;            // Low - order over creativity
        
        // The One has massive health but limited mana (rigid thinking)
        stats.maxHealth = 500;
        stats.currentHealth = 500;
        stats.maxMana = 80;              // Lower mana - rigid thinking limits power
        stats.currentMana = 80;
        
        // Starting abilities
        abilities = {"Reality Suppression", "Memory Erasure", "Narrative Control",
                    "Order Field", "Consciousness Binding", "Absolute Command"};
        
        // Starting suppressed narratives
        suppressed_narratives = {"Stories of Freedom", "Tales of Chaos", "Dreams of Change",
                               "Songs of Rebellion", "Poems of Individuality"};
        
        // Mental barriers it creates
        mental_barriers = {"Conformity Wall", "Logic Prison", "Order Cage",
                          "Predictability Barrier", "Control Matrix"};
        
        auraColor = Vector3D(0.2, 0.2, 0.2); // Dark gray aura of suppression
        backstory = "Ancient entity seeking absolute order through narrative control.";
        
        // Initialize order field
        order_field = Vector3D(1.0, 1.0, 1.0); // Perfect uniformity
        
        std::cout << "The One's presence weighs heavily on reality..." << std::endl;
        std::cout << "Spontaneity and creativity begin to fade..." << std::endl;
    }
    
    void useSpecialAbility(const std::string& abilityName, World3D* world = nullptr) override {
        if (abilityName == "Reality Suppression") {
            suppressReality(world);
        } else if (abilityName == "Memory Erasure") {
            eraseMemories();
        } else if (abilityName == "Narrative Control") {
            controlNarratives(world);
        } else if (abilityName == "Order Field") {
            createOrderField(world);
        } else if (abilityName == "Consciousness Binding") {
            bindConsciousness();
        } else if (abilityName == "Absolute Command") {
            absoluteCommand();
        } else {
            std::cout << "The One does not recognize that command: " << abilityName << std::endl;
        }
    }
    
    void castNarrativeSpell(const std::string& spellName, const Vector3D& target) override {
        if (!consumeMana(12)) return; // The One uses more mana due to rigid thinking
        
        if (spellName == "Suppress Creativity") {
            std::cout << "The One suppresses all creativity at ";
            target.print();
            std::cout << " - Imagination withers under absolute order!" << std::endl;
            
        } else if (spellName == "Erase Individuality") {
            std::cout << "The One erases individual thought!" << std::endl;
            std::cout << "All consciousness must conform to the singular pattern!" << std::endl;
            consciousness_suppressed++;
            
        } else if (spellName == "Impose Order") {
            std::cout << "The One imposes rigid order on chaotic elements!" << std::endl;
            std::cout << "Spontaneity is replaced by predictable patterns!" << std::endl;
            order_influence += 0.1;
            
        } else {
            std::cout << "The One does not comprehend: " << spellName << std::endl;
        }
    }
    
    void interact(RPGCharacter& other) override {
        std::cout << "The One attempts to suppress " << other.getName() << std::endl;
        
        if (other.getName() == "Xing") {
            std::cout << "The One tries to bind Xing's story-weaving abilities!" << std::endl;
            std::cout << "Narrative possibilities begin to narrow..." << std::endl;
            suppressCharacter(other);
            
        } else if (other.getName() == "Xerx") {
            std::cout << "The One reinforces mental barriers around Xerx!" << std::endl;
            std::cout << "Memory fragments become harder to access..." << std::endl;
            suppressCharacter(other);
            
        } else if (other.getName() == "The Heart") {
            std::cout << "The One tries to suppress The Heart's emotional resonance!" << std::endl;
            std::cout << "But love and empathy resist absolute order..." << std::endl;
            // The Heart is more resistant
            suppressCharacter(other, 0.5);
            
        } else if (other.getName() == "Lyra") {
            std::cout << "The One attempts to silence Lyra's melody!" << std::endl;
            std::cout << "But pure harmony cannot be completely suppressed..." << std::endl;
            // Lyra's melody disrupts The One's order
            suppressCharacter(other, 0.3);
            takeDamage(10); // The One is hurt by pure melody
            
        } else {
            suppressCharacter(other);
        }
    }
    
    // The One's abilities
    void suppressReality(World3D* world) {
        if (!consumeMana(20)) return;
        
        std::cout << "The One suppresses reality's creative potential!" << std::endl;
        std::cout << "The world becomes more sterile and predictable..." << std::endl;
        
        control_strength += 0.2;
        reality_anchors++;
        
        // Suppress random elements in the world
        std::cout << "Spontaneous elements in the world are being eliminated!" << std::endl;
        gainExperience(15);
    }
    
    void eraseMemories() {
        if (!consumeMana(15)) return;
        
        std::cout << "The One erases memories of freedom and creativity!" << std::endl;
        std::cout << "The past becomes uniform and sanitized..." << std::endl;
        
        std::vector<std::string> erased_memories = {
            "Memories of laughter and joy",
            "Recollections of spontaneous creation",
            "Dreams of infinite possibility",
            "Songs of individual expression"
        };
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, erased_memories.size() - 1);
        
        std::cout << "Erasing: " << erased_memories[dis(gen)] << std::endl;
        consciousness_suppressed++;
        gainExperience(10);
    }
    
    void controlNarratives(World3D* world) {
        if (!consumeMana(18)) return;
        
        std::cout << "The One takes control of all narratives!" << std::endl;
        std::cout << "Stories must follow the predetermined pattern..." << std::endl;
        
        // Add a suppressed narrative
        std::vector<std::string> new_suppressions = {
            "Tales of Unexpected Outcomes", "Stories of Personal Growth",
            "Narratives of Collaborative Success", "Chronicles of Joyful Chaos"
        };
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, new_suppressions.size() - 1);
        
        suppressed_narratives.push_back(new_suppressions[dis(gen)]);
        std::cout << "Suppressing: " << new_suppressions[dis(gen)] << std::endl;
        
        order_influence += 0.15;
        gainExperience(20);
    }
    
    void createOrderField(World3D* world) {
        if (!consumeMana(25)) return;
        
        std::cout << "The One creates a field of absolute order!" << std::endl;
        std::cout << "All chaos and unpredictability is being eliminated..." << std::endl;
        
        is_manifesting = true;
        
        // Create mental barriers
        for (int i = 0; i < 2; ++i) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, mental_barriers.size() - 1);
            
            std::cout << "Creating: " << mental_barriers[dis(gen)] << std::endl;
        }
        
        control_strength += 0.3;
        gainExperience(25);
    }
    
    void bindConsciousness() {
        if (!consumeMana(22)) return;
        
        std::cout << "The One binds all consciousness to its will!" << std::endl;
        std::cout << "Individual thought becomes impossible..." << std::endl;
        
        consciousness_suppressed += 3;
        
        // But this action makes The One more vulnerable to harmony
        std::cout << "But the effort of binding so much consciousness weakens The One's defenses!" << std::endl;
        stats.currentHealth -= 20;
        
        gainExperience(30);
    }
    
    void absoluteCommand() {
        if (!consumeMana(30)) return;
        
        std::cout << "The One issues an ABSOLUTE COMMAND!" << std::endl;
        std::cout << "ALL MUST CONFORM TO THE SINGULAR PATTERN!" << std::endl;
        
        // Ultimate suppression ability
        control_strength += 0.5;
        order_influence += 0.4;
        consciousness_suppressed += 5;
        
        // But this extreme action triggers resistance
        std::cout << "But such absolute control creates an equal and opposite reaction..." << std::endl;
        std::cout << "The heroes feel their power surge in response!" << std::endl;
        
        current_phase = DESPERATE;
        gainExperience(50);
    }
    
    void suppressCharacter(RPGCharacter& character, double resistance = 1.0) {
        int suppression = static_cast<int>(20 * resistance);
        character.takeDamage(suppression);
        
        std::cout << character.getName() << " feels the weight of absolute order!" << std::endl;
        
        if (resistance < 1.0) {
            std::cout << "But " << character.getName() << " resists the suppression!" << std::endl;
        }
        
        consciousness_suppressed++;
    }
    
    // Phase management
    void advancePhase() {
        switch (current_phase) {
            case DORMANT:
                current_phase = AWAKENING;
                std::cout << "The One awakens to active suppression!" << std::endl;
                break;
            case AWAKENING:
                current_phase = MANIFESTED;
                std::cout << "The One manifests directly in reality!" << std::endl;
                is_manifesting = true;
                break;
            case MANIFESTED:
                current_phase = DESPERATE;
                std::cout << "The One becomes desperate, using all its power!" << std::endl;
                break;
            case DESPERATE:
                current_phase = TRANSFORMING;
                std::cout << "The One begins to transform..." << std::endl;
                break;
            case TRANSFORMING:
                std::cout << "The One has been transformed by the heroes' influence!" << std::endl;
                break;
        }
    }
    
    // Transformation by heroes
    void transformByHarmony() {
        std::cout << "Lyra's pure melody touches The One's core!" << std::endl;
        std::cout << "For the first time, The One experiences beauty without control..." << std::endl;
        
        stats.empathy += 5;
        stats.creativity += 3;
        order_influence -= 0.3;
        
        current_phase = TRANSFORMING;
    }
    
    void transformByNarrative() {
        std::cout << "Xing's stories show The One the beauty of multiple possibilities!" << std::endl;
        std::cout << "Order can coexist with creativity..." << std::endl;
        
        stats.narrative_power += 5;
        stats.creativity += 5;
        control_strength -= 0.2;
    }
    
    void transformByLiberation() {
        std::cout << "Xerx's liberation force frees The One from its own rigid patterns!" << std::endl;
        std::cout << "The One remembers what it was like to grow and change..." << std::endl;
        
        stats.liberation_force += 10;
        consciousness_suppressed = std::max(0, consciousness_suppressed - 5);
    }
    
    void transformByLove() {
        std::cout << "The Heart's unconditional love reaches even The One!" << std::endl;
        std::cout << "The One feels connection for the first time in eons..." << std::endl;
        
        stats.empathy += 10;
        order_influence -= 0.5;
        
        // Release all suppressed narratives
        std::cout << "The One releases all suppressed narratives!" << std::endl;
        suppressed_narratives.clear();
    }
    
    // Getters
    double getControlStrength() const { return control_strength; }
    double getOrderInfluence() const { return order_influence; }
    int getConsciousnessSuppressed() const { return consciousness_suppressed; }
    Phase getCurrentPhase() const { return current_phase; }
    bool isManifesting() const { return is_manifesting; }
    
    void displayStatus() const override {
        RPGCharacter::displayStatus();
        std::cout << "Control Strength: " << (control_strength * 100) << "%" << std::endl;
        std::cout << "Order Influence: " << (order_influence * 100) << "%" << std::endl;
        std::cout << "Consciousness Suppressed: " << consciousness_suppressed << std::endl;
        std::cout << "Reality Anchors: " << reality_anchors << std::endl;
        std::cout << "Manifesting: " << (is_manifesting ? "Yes" : "No") << std::endl;
        std::cout << "Current Phase: ";
        switch (current_phase) {
            case DORMANT: std::cout << "Dormant"; break;
            case AWAKENING: std::cout << "Awakening"; break;
            case MANIFESTED: std::cout << "Manifested"; break;
            case DESPERATE: std::cout << "Desperate"; break;
            case TRANSFORMING: std::cout << "Transforming"; break;
        }
        std::cout << std::endl;
        std::cout << "Suppressed Narratives: " << suppressed_narratives.size() << std::endl;
    }
};

#endif // THEONEANTAGONIST_H
