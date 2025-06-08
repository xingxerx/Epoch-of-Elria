#ifndef NARRATIVECOMBAT_H
#define NARRATIVECOMBAT_H

#include "RPGCharacter.h"
#include <vector>
#include <string>
#include <map>
#include <random>
#include <algorithm>

// Forward declarations
class RPGCharacter;
class Xing;
class Xerx;
class TheHeart;

// --- Narrative Attack Types ---
enum class NarrativeAttackType {
    STORY_WEAVING,      // Xing's reality manipulation
    MEMORY_STRIKE,      // Xerx's truth-based attacks
    HEART_RESONANCE,    // Heart's emotional attacks
    CONCEPT_BLAST,      // Abstract idea attacks
    LIBERATION_WAVE,    // Freedom-based attacks
    EMPATHY_SURGE,      // Emotional connection attacks
    TRUTH_REVELATION,   // Revealing hidden truths
    DREAM_MANIFESTATION // Bringing dreams to reality
};

// --- Story Weapon Class ---
class StoryWeapon {
private:
    std::string name;
    std::string description;
    NarrativeAttackType attack_type;
    int base_damage;
    int mana_cost;
    std::vector<std::string> special_effects;
    Vector3D narrative_color;
    
public:
    // Default constructor for map usage
    StoryWeapon() : name(""), attack_type(NarrativeAttackType::STORY_WEAVING),
                   base_damage(0), mana_cost(0), narrative_color(1, 1, 1) {}

    StoryWeapon(const std::string& n, NarrativeAttackType type, int damage, int cost,
                const Vector3D& color = Vector3D(1, 1, 1))
        : name(n), attack_type(type), base_damage(damage), mana_cost(cost), narrative_color(color) {}
    
    // Getters
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    NarrativeAttackType getAttackType() const { return attack_type; }
    int getBaseDamage() const { return base_damage; }
    int getManaCost() const { return mana_cost; }
    const Vector3D& getColor() const { return narrative_color; }
    
    // Setters
    void setDescription(const std::string& desc) { description = desc; }
    void addSpecialEffect(const std::string& effect) { special_effects.push_back(effect); }
    
    // Combat calculations
    int calculateDamage(const RPGCharacter& attacker, const RPGCharacter& defender) const {
        int damage = base_damage;
        
        // Modify damage based on character stats
        switch (attack_type) {
            case NarrativeAttackType::STORY_WEAVING:
                damage += attacker.getStats().narrative_power * 2;
                break;
            case NarrativeAttackType::MEMORY_STRIKE:
                damage += attacker.getStats().liberation_force * 2;
                break;
            case NarrativeAttackType::HEART_RESONANCE:
                damage += attacker.getStats().empathy * 2;
                break;
            case NarrativeAttackType::CONCEPT_BLAST:
                damage += attacker.getStats().creativity * 2;
                break;
            default:
                damage += attacker.getStats().narrative_power;
                break;
        }
        
        // Defender's resistance
        int resistance = defender.getStats().memory_strength;
        damage = std::max(1, damage - resistance);
        
        return damage;
    }
    
    void displayAttack() const {
        std::cout << "🌟 " << name << " 🌟" << std::endl;
        std::cout << description << std::endl;
        std::cout << "Narrative energy swirls with ";
        narrative_color.print();
        std::cout << " colors!" << std::endl;
        
        for (const auto& effect : special_effects) {
            std::cout << "✨ " << effect << std::endl;
        }
    }
};

// --- Narrative Enemy Class ---
class NarrativeEnemy {
private:
    std::string name;
    std::string description;
    int max_health;
    int current_health;
    int narrative_resistance;
    int attack_power;
    std::vector<std::string> attack_patterns;
    bool is_the_one;
    
public:
    NarrativeEnemy(const std::string& n, int health, int resistance, int power, bool the_one = false)
        : name(n), max_health(health), current_health(health), 
          narrative_resistance(resistance), attack_power(power), is_the_one(the_one) {}
    
    // Getters
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    int getCurrentHealth() const { return current_health; }
    int getMaxHealth() const { return max_health; }
    int getNarrativeResistance() const { return narrative_resistance; }
    int getAttackPower() const { return attack_power; }
    bool isTheOne() const { return is_the_one; }
    bool isAlive() const { return current_health > 0; }
    
    // Setters
    void setDescription(const std::string& desc) { description = desc; }
    void addAttackPattern(const std::string& pattern) { attack_patterns.push_back(pattern); }
    
    // Combat methods
    void takeDamage(int damage) {
        current_health = std::max(0, current_health - damage);
        if (current_health == 0) {
            std::cout << name << " has been defeated by the power of narrative!" << std::endl;
        }
    }
    
    void heal(int amount) {
        current_health = std::min(max_health, current_health + amount);
    }
    
    std::string performAttack() {
        if (attack_patterns.empty()) {
            return name + " attacks with oppressive force!";
        }
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, attack_patterns.size() - 1);
        
        return attack_patterns[dis(gen)];
    }
    
    void displayStatus() const {
        std::cout << "\n=== " << name << " ===" << std::endl;
        std::cout << "Health: " << current_health << "/" << max_health << std::endl;
        std::cout << "Narrative Resistance: " << narrative_resistance << std::endl;
        if (is_the_one) {
            std::cout << "⚠️  THE ANCIENT ENTITY OF ABSOLUTE ORDER ⚠️" << std::endl;
        }
        std::cout << description << std::endl;
    }
};

// --- Combat Action Class ---
class CombatAction {
public:
    enum ActionType {
        ATTACK,
        DEFEND,
        SPECIAL_ABILITY,
        USE_ITEM,
        COLLABORATE
    };
    
private:
    ActionType type;
    std::string action_name;
    std::string target_name;
    RPGCharacter* actor;
    
public:
    CombatAction(ActionType t, const std::string& name, RPGCharacter* character)
        : type(t), action_name(name), actor(character) {}
    
    ActionType getType() const { return type; }
    const std::string& getActionName() const { return action_name; }
    const std::string& getTargetName() const { return target_name; }
    RPGCharacter* getActor() const { return actor; }
    
    void setTarget(const std::string& target) { target_name = target; }
};

// --- Main Narrative Combat System ---
class NarrativeCombatSystem {
private:
    std::vector<RPGCharacter*> party;
    std::vector<NarrativeEnemy> enemies;
    std::map<std::string, StoryWeapon> weapon_library;
    bool combat_active;
    int turn_counter;
    
public:
    NarrativeCombatSystem() : combat_active(false), turn_counter(0) {
        initializeWeaponLibrary();
    }
    
    void initializeWeaponLibrary() {
        // Xing's Story Weapons
        StoryWeapon reality_weave("Reality Weave", NarrativeAttackType::STORY_WEAVING, 
                                 25, 15, Vector3D(0.8, 0.6, 1.0));
        reality_weave.setDescription("Xing weaves the fabric of reality itself into a weapon!");
        reality_weave.addSpecialEffect("Creates protective barriers");
        reality_weave.addSpecialEffect("May manifest helpful platforms");
        weapon_library["Reality Weave"] = reality_weave;
        
        StoryWeapon concept_storm("Concept Storm", NarrativeAttackType::CONCEPT_BLAST,
                                 30, 20, Vector3D(1.0, 0.8, 0.2));
        concept_storm.setDescription("A whirlwind of abstract concepts tears through the enemy!");
        concept_storm.addSpecialEffect("Confuses enemy with paradoxes");
        concept_storm.addSpecialEffect("May inspire allies");
        weapon_library["Concept Storm"] = concept_storm;
        
        // Xerx's Liberation Weapons
        StoryWeapon memory_blade("Memory Blade", NarrativeAttackType::MEMORY_STRIKE,
                                35, 12, Vector3D(1.0, 0.3, 0.3));
        memory_blade.setDescription("Xerx strikes with the sharp edge of recovered truth!");
        memory_blade.addSpecialEffect("Cuts through illusions");
        memory_blade.addSpecialEffect("Recovers lost memories");
        weapon_library["Memory Blade"] = memory_blade;
        
        StoryWeapon liberation_wave("Liberation Wave", NarrativeAttackType::LIBERATION_WAVE,
                                   40, 18, Vector3D(1.0, 0.5, 0.0));
        liberation_wave.setDescription("A wave of pure freedom crashes over the battlefield!");
        liberation_wave.addSpecialEffect("Frees trapped narratives");
        liberation_wave.addSpecialEffect("Breaks mental chains");
        weapon_library["Liberation Wave"] = liberation_wave;
        
        // Heart's Emotional Weapons
        StoryWeapon empathy_pulse("Empathy Pulse", NarrativeAttackType::HEART_RESONANCE,
                                 20, 10, Vector3D(1.0, 0.8, 0.9));
        empathy_pulse.setDescription("The Heart sends out waves of pure emotional connection!");
        empathy_pulse.addSpecialEffect("Heals allies");
        empathy_pulse.addSpecialEffect("May convert enemies");
        weapon_library["Empathy Pulse"] = empathy_pulse;
        
        StoryWeapon dream_manifestation("Dream Manifestation", NarrativeAttackType::DREAM_MANIFESTATION,
                                       45, 25, Vector3D(0.9, 0.9, 1.0));
        dream_manifestation.setDescription("The Heart manifests the collective dreams of all consciousness!");
        dream_manifestation.addSpecialEffect("Massive area effect");
        dream_manifestation.addSpecialEffect("Awakens dormant narratives");
        weapon_library["Dream Manifestation"] = dream_manifestation;
    }
    
    void addPartyMember(RPGCharacter* character) {
        party.push_back(character);
    }
    
    void addEnemy(const NarrativeEnemy& enemy) {
        enemies.push_back(enemy);
    }
    
    void createTheOneBattle() {
        NarrativeEnemy the_one("The One", 500, 50, 60, true);
        the_one.setDescription("The ancient entity of absolute order, seeking to suppress all unwritten stories.");
        the_one.addAttackPattern("The One attempts to erase your narrative existence!");
        the_one.addAttackPattern("Waves of oppressive order wash over the battlefield!");
        the_one.addAttackPattern("The One tries to reintegrate you into singular reality!");
        the_one.addAttackPattern("Absolute control seeks to silence your stories!");
        addEnemy(the_one);
    }
    
    void createMinorEnemies() {
        NarrativeEnemy suppressor("Narrative Suppressor", 80, 15, 25);
        suppressor.setDescription("A manifestation of creative blockage and censorship.");
        suppressor.addAttackPattern("The Suppressor tries to silence your voice!");
        suppressor.addAttackPattern("Waves of doubt and criticism assault your confidence!");
        addEnemy(suppressor);
        
        NarrativeEnemy void_echo("Void Echo", 60, 10, 20);
        void_echo.setDescription("An empty space where stories once lived, now hungry for narrative.");
        void_echo.addAttackPattern("The Void Echo drains your creative energy!");
        void_echo.addAttackPattern("Emptiness tries to consume your stories!");
        addEnemy(void_echo);
    }
    
    bool startCombat() {
        if (party.empty() || enemies.empty()) {
            std::cout << "Cannot start combat: missing party or enemies!" << std::endl;
            return false;
        }
        
        combat_active = true;
        turn_counter = 0;
        
        std::cout << "\n🌟 NARRATIVE COMBAT BEGINS! 🌟" << std::endl;
        std::cout << "The battlefield shimmers with story potential..." << std::endl;
        
        // Display initial status
        displayCombatStatus();
        
        return true;
    }
    
    void displayCombatStatus() {
        std::cout << "\n=== COMBAT STATUS ===" << std::endl;
        std::cout << "Turn: " << turn_counter << std::endl;
        
        std::cout << "\n--- PARTY ---" << std::endl;
        for (auto* character : party) {
            std::cout << character->getName() << ": " 
                      << character->getStats().currentHealth << "/" << character->getStats().maxHealth 
                      << " HP, " << character->getStats().currentMana << "/" << character->getStats().maxMana 
                      << " Mana" << std::endl;
        }
        
        std::cout << "\n--- ENEMIES ---" << std::endl;
        for (const auto& enemy : enemies) {
            if (enemy.isAlive()) {
                std::cout << enemy.getName() << ": " 
                          << enemy.getCurrentHealth() << "/" << enemy.getMaxHealth() << " HP" << std::endl;
            }
        }
        std::cout << "===================" << std::endl;
    }
    
    void performNarrativeAttack(RPGCharacter* attacker, const std::string& weapon_name, 
                               const std::string& target_name) {
        if (weapon_library.find(weapon_name) == weapon_library.end()) {
            std::cout << "Unknown weapon: " << weapon_name << std::endl;
            return;
        }
        
        StoryWeapon& weapon = weapon_library[weapon_name];
        
        // Check mana cost
        if (attacker->getStats().currentMana < weapon.getManaCost()) {
            std::cout << attacker->getName() << " doesn't have enough mana for " << weapon_name << "!" << std::endl;
            return;
        }
        
        // Find target enemy
        auto target_it = std::find_if(enemies.begin(), enemies.end(),
            [&target_name](const NarrativeEnemy& enemy) {
                return enemy.getName() == target_name && enemy.isAlive();
            });
        
        if (target_it == enemies.end()) {
            std::cout << "Target " << target_name << " not found or already defeated!" << std::endl;
            return;
        }
        
        // Perform attack
        std::cout << "\n" << attacker->getName() << " attacks with " << weapon_name << "!" << std::endl;
        weapon.displayAttack();
        
        int damage = weapon.calculateDamage(*attacker, *attacker); // Simplified for now
        target_it->takeDamage(damage);
        
        // Consume mana
        attacker->getStats().currentMana -= weapon.getManaCost();
        
        std::cout << target_name << " takes " << damage << " narrative damage!" << std::endl;
        
        // Special effects
        handleSpecialEffects(weapon, attacker, &(*target_it));
    }
    
    void handleSpecialEffects(const StoryWeapon& weapon, RPGCharacter* attacker, NarrativeEnemy* target) {
        switch (weapon.getAttackType()) {
            case NarrativeAttackType::STORY_WEAVING:
                std::cout << "Reality stabilizes around " << attacker->getName() << "!" << std::endl;
                attacker->heal(10);
                break;
                
            case NarrativeAttackType::MEMORY_STRIKE:
                std::cout << "Truth cuts through " << target->getName() << "'s defenses!" << std::endl;
                // Reduce enemy resistance temporarily
                break;
                
            case NarrativeAttackType::HEART_RESONANCE:
                std::cout << "Emotional energy flows through the party!" << std::endl;
                for (auto* member : party) {
                    member->heal(5);
                    member->restoreMana(5);
                }
                break;
                
            case NarrativeAttackType::LIBERATION_WAVE:
                std::cout << "Waves of freedom inspire all conscious beings!" << std::endl;
                for (auto* member : party) {
                    member->gainExperience(10);
                }
                break;
                
            default:
                break;
        }
    }
    
    void enemyTurn() {
        for (auto& enemy : enemies) {
            if (!enemy.isAlive()) continue;
            
            std::cout << "\n" << enemy.performAttack() << std::endl;
            
            // Simple damage to random party member
            if (!party.empty()) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, party.size() - 1);
                
                RPGCharacter* target = party[dis(gen)];
                int damage = enemy.getAttackPower() - target->getStats().memory_strength / 2;
                damage = std::max(1, damage);
                
                target->takeDamage(damage);
                std::cout << target->getName() << " takes " << damage << " damage!" << std::endl;
            }
        }
    }
    
    bool checkCombatEnd() {
        // Check if all enemies defeated
        bool all_enemies_dead = true;
        for (const auto& enemy : enemies) {
            if (enemy.isAlive()) {
                all_enemies_dead = false;
                break;
            }
        }
        
        if (all_enemies_dead) {
            std::cout << "\n🎉 VICTORY! All enemies have been defeated by the power of narrative! 🎉" << std::endl;
            combat_active = false;
            return true;
        }
        
        // Check if all party members defeated
        bool all_party_dead = true;
        for (auto* member : party) {
            if (member->getStats().currentHealth > 0) {
                all_party_dead = false;
                break;
            }
        }
        
        if (all_party_dead) {
            std::cout << "\n💀 DEFEAT! The party has fallen... but stories never truly die. 💀" << std::endl;
            combat_active = false;
            return true;
        }
        
        return false;
    }
    
    void nextTurn() {
        turn_counter++;
        
        // Regenerate some mana each turn
        for (auto* member : party) {
            member->restoreMana(5);
        }
    }
    
    bool isCombatActive() const { return combat_active; }
    
    std::vector<std::string> getAvailableWeapons(const std::string& character_class) {
        std::vector<std::string> available;
        
        for (const auto& pair : weapon_library) {
            const StoryWeapon& weapon = pair.second;
            
            // Filter weapons by character type
            if (character_class == "The Weaver" && 
                (weapon.getAttackType() == NarrativeAttackType::STORY_WEAVING ||
                 weapon.getAttackType() == NarrativeAttackType::CONCEPT_BLAST)) {
                available.push_back(pair.first);
            } else if (character_class == "The Liberator" &&
                      (weapon.getAttackType() == NarrativeAttackType::MEMORY_STRIKE ||
                       weapon.getAttackType() == NarrativeAttackType::LIBERATION_WAVE)) {
                available.push_back(pair.first);
            } else if (character_class == "Narrative Catalyst" &&
                      (weapon.getAttackType() == NarrativeAttackType::HEART_RESONANCE ||
                       weapon.getAttackType() == NarrativeAttackType::DREAM_MANIFESTATION)) {
                available.push_back(pair.first);
            }
        }
        
        return available;
    }
    
    std::vector<std::string> getAliveEnemyNames() {
        std::vector<std::string> names;
        for (const auto& enemy : enemies) {
            if (enemy.isAlive()) {
                names.push_back(enemy.getName());
            }
        }
        return names;
    }
};

#endif // NARRATIVECOMBAT_H
