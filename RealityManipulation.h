#ifndef REALITYMANIPULATION_H
#define REALITYMANIPULATION_H

#include "Vector3D.h"
#include "RPGCharacter.h"
#include "World3D.h"
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <random>

// Forward declarations
class RPGCharacter;
class Xing;
class Xerx;
class TheHeart;

// --- Reality Manipulation Types ---
enum class RealityManipulationType {
    WEAVE_PLATFORM,         // Create solid surfaces
    OPEN_PORTAL,           // Create passages between spaces
    MANIFEST_CONCEPT,      // Bring abstract ideas into reality
    BREAK_BARRIER,         // Destroy obstacles and limitations
    TRANSFORM_SPACE,       // Change the nature of an area
    ANCHOR_REALITY,        // Stabilize unstable areas
    LIBERATE_NARRATIVE,    // Free trapped stories
    EMOTIONAL_RESONANCE,   // Change atmosphere through emotion
    MEMORY_RECONSTRUCTION, // Rebuild lost areas from memories
    DREAM_MATERIALIZATION  // Make dreams physically real
};

// --- Reality Fragment Class ---
class RealityFragment {
private:
    std::string name;
    std::string description;
    Vector3D position;
    Vector3D size;
    RealityManipulationType type;
    double stability;        // 0.0 to 1.0
    double narrative_power;  // How much story energy it contains
    Vector3D color_signature;
    bool is_permanent;
    std::string creator_name;
    
public:
    RealityFragment(const std::string& n, const Vector3D& pos, const Vector3D& sz, 
                   RealityManipulationType t, const std::string& creator)
        : name(n), position(pos), size(sz), type(t), stability(0.8), 
          narrative_power(1.0), color_signature(1, 1, 1), is_permanent(false), creator_name(creator) {}
    
    // Getters
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    const Vector3D& getPosition() const { return position; }
    const Vector3D& getSize() const { return size; }
    RealityManipulationType getType() const { return type; }
    double getStability() const { return stability; }
    double getNarrativePower() const { return narrative_power; }
    const Vector3D& getColorSignature() const { return color_signature; }
    bool isPermanent() const { return is_permanent; }
    const std::string& getCreator() const { return creator_name; }
    
    // Setters
    void setDescription(const std::string& desc) { description = desc; }
    void setStability(double stab) { stability = std::max(0.0, std::min(1.0, stab)); }
    void setNarrativePower(double power) { narrative_power = std::max(0.0, power); }
    void setColorSignature(const Vector3D& color) { color_signature = color; }
    void setPermanent(bool perm) { is_permanent = perm; }
    
    // Reality fragment behavior
    void update(double deltaTime) {
        if (!is_permanent) {
            // Unstable fragments decay over time
            stability -= deltaTime * 0.1;
            if (stability <= 0.0) {
                std::cout << "Reality fragment '" << name << "' fades from existence..." << std::endl;
            }
        }
    }
    
    void stabilize(double amount) {
        stability = std::min(1.0, stability + amount);
        if (stability >= 1.0 && !is_permanent) {
            is_permanent = true;
            std::cout << "Reality fragment '" << name << "' becomes permanently woven into existence!" << std::endl;
        }
    }
    
    void resonate(const RealityFragment& other) {
        if (creator_name == other.creator_name) {
            // Fragments from same creator strengthen each other
            stabilize(0.1);
            narrative_power += 0.1;
        }
    }
    
    bool contains(const Vector3D& point) const {
        Vector3D min_bounds = position - size * 0.5;
        Vector3D max_bounds = position + size * 0.5;
        
        return (point.x >= min_bounds.x && point.x <= max_bounds.x &&
                point.y >= min_bounds.y && point.y <= max_bounds.y &&
                point.z >= min_bounds.z && point.z <= max_bounds.z);
    }
    
    void display() const {
        std::cout << "Reality Fragment: " << name << std::endl;
        std::cout << "  Type: ";
        switch (type) {
            case RealityManipulationType::WEAVE_PLATFORM: std::cout << "Woven Platform"; break;
            case RealityManipulationType::OPEN_PORTAL: std::cout << "Narrative Portal"; break;
            case RealityManipulationType::MANIFEST_CONCEPT: std::cout << "Manifested Concept"; break;
            case RealityManipulationType::BREAK_BARRIER: std::cout << "Broken Barrier"; break;
            case RealityManipulationType::TRANSFORM_SPACE: std::cout << "Transformed Space"; break;
            case RealityManipulationType::ANCHOR_REALITY: std::cout << "Reality Anchor"; break;
            case RealityManipulationType::LIBERATE_NARRATIVE: std::cout << "Liberated Narrative"; break;
            case RealityManipulationType::EMOTIONAL_RESONANCE: std::cout << "Emotional Field"; break;
            case RealityManipulationType::MEMORY_RECONSTRUCTION: std::cout << "Reconstructed Memory"; break;
            case RealityManipulationType::DREAM_MATERIALIZATION: std::cout << "Materialized Dream"; break;
        }
        std::cout << std::endl;
        std::cout << "  Position: ";
        position.print();
        std::cout << std::endl;
        std::cout << "  Size: ";
        size.print();
        std::cout << std::endl;
        std::cout << "  Stability: " << (stability * 100) << "%" << std::endl;
        std::cout << "  Narrative Power: " << narrative_power << std::endl;
        std::cout << "  Creator: " << creator_name << std::endl;
        std::cout << "  Permanent: " << (is_permanent ? "Yes" : "No") << std::endl;
        if (!description.empty()) {
            std::cout << "  Description: " << description << std::endl;
        }
    }
};

// --- Reality Manipulation Ability Class ---
class RealityAbility {
private:
    std::string name;
    std::string description;
    RealityManipulationType type;
    int mana_cost;
    double power_level;
    std::vector<std::string> requirements;
    std::string character_class_restriction;
    
public:
    RealityAbility(const std::string& n, RealityManipulationType t, int cost, double power,
                   const std::string& class_restriction = "")
        : name(n), type(t), mana_cost(cost), power_level(power), 
          character_class_restriction(class_restriction) {}
    
    // Getters
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    RealityManipulationType getType() const { return type; }
    int getManaCost() const { return mana_cost; }
    double getPowerLevel() const { return power_level; }
    const std::string& getClassRestriction() const { return character_class_restriction; }
    
    // Setters
    void setDescription(const std::string& desc) { description = desc; }
    void addRequirement(const std::string& req) { requirements.push_back(req); }
    
    // Check if character can use this ability
    bool canUse(const RPGCharacter& character) const {
        // Check class restriction
        if (!character_class_restriction.empty() && 
            character.getCharacterClass() != character_class_restriction) {
            return false;
        }
        
        // Check mana cost
        if (character.getStats().currentMana < mana_cost) {
            return false;
        }
        
        // Check requirements (simplified for now)
        return true;
    }
    
    void display() const {
        std::cout << "Reality Ability: " << name << std::endl;
        std::cout << "  Mana Cost: " << mana_cost << std::endl;
        std::cout << "  Power Level: " << power_level << std::endl;
        if (!character_class_restriction.empty()) {
            std::cout << "  Restricted to: " << character_class_restriction << std::endl;
        }
        if (!description.empty()) {
            std::cout << "  Description: " << description << std::endl;
        }
    }
};

// --- Main Reality Manipulation System ---
class RealityManipulationSystem {
private:
    std::vector<std::unique_ptr<RealityFragment>> active_fragments;
    std::map<std::string, RealityAbility> ability_library;
    std::vector<std::string> reality_log;
    double global_stability;
    double narrative_saturation;
    
public:
    RealityManipulationSystem() : global_stability(1.0), narrative_saturation(0.0) {
        initializeAbilityLibrary();
    }
    
    void initializeAbilityLibrary() {
        // Xing's Reality Weaving Abilities
        RealityAbility weave_platform("Weave Platform", RealityManipulationType::WEAVE_PLATFORM, 
                                     15, 1.0, "The Weaver");
        weave_platform.setDescription("Xing weaves narrative threads into solid platforms that can be walked on.");
        ability_library["Weave Platform"] = weave_platform;
        
        RealityAbility manifest_concept("Manifest Concept", RealityManipulationType::MANIFEST_CONCEPT,
                                       20, 1.5, "The Weaver");
        manifest_concept.setDescription("Xing brings abstract concepts into physical reality.");
        ability_library["Manifest Concept"] = manifest_concept;
        
        RealityAbility anchor_reality("Anchor Reality", RealityManipulationType::ANCHOR_REALITY,
                                     25, 2.0, "The Weaver");
        anchor_reality.setDescription("Xing creates stable points that resist reality distortion.");
        ability_library["Anchor Reality"] = anchor_reality;
        
        RealityAbility open_portal("Open Portal", RealityManipulationType::OPEN_PORTAL,
                                  30, 2.5, "The Weaver");
        open_portal.setDescription("Xing opens passages between different narrative spaces.");
        ability_library["Open Portal"] = open_portal;
        
        // Xerx's Reality Liberation Abilities
        RealityAbility break_barrier("Break Barrier", RealityManipulationType::BREAK_BARRIER,
                                    12, 1.2, "The Liberator");
        break_barrier.setDescription("Xerx shatters obstacles and limitations imposed by The One.");
        ability_library["Break Barrier"] = break_barrier;
        
        RealityAbility liberate_narrative("Liberate Narrative", RealityManipulationType::LIBERATE_NARRATIVE,
                                         18, 1.8, "The Liberator");
        liberate_narrative.setDescription("Xerx frees trapped stories, allowing them to reshape reality.");
        ability_library["Liberate Narrative"] = liberate_narrative;
        
        RealityAbility memory_reconstruction("Memory Reconstruction", RealityManipulationType::MEMORY_RECONSTRUCTION,
                                           22, 2.2, "The Liberator");
        memory_reconstruction.setDescription("Xerx rebuilds lost areas using recovered memories.");
        ability_library["Memory Reconstruction"] = memory_reconstruction;
        
        // The Heart's Emotional Reality Abilities
        RealityAbility emotional_resonance("Emotional Resonance", RealityManipulationType::EMOTIONAL_RESONANCE,
                                          10, 1.0, "Narrative Catalyst");
        emotional_resonance.setDescription("The Heart infuses areas with emotional energy, changing their nature.");
        ability_library["Emotional Resonance"] = emotional_resonance;
        
        RealityAbility dream_materialization("Dream Materialization", RealityManipulationType::DREAM_MATERIALIZATION,
                                           35, 3.0, "Narrative Catalyst");
        dream_materialization.setDescription("The Heart makes collective dreams physically manifest.");
        ability_library["Dream Materialization"] = dream_materialization;
        
        RealityAbility transform_space("Transform Space", RealityManipulationType::TRANSFORM_SPACE,
                                      28, 2.5, "Narrative Catalyst");
        transform_space.setDescription("The Heart fundamentally changes the nature of reality in an area.");
        ability_library["Transform Space"] = transform_space;
    }
    
    bool executeRealityManipulation(RPGCharacter* character, const std::string& ability_name,
                                   const Vector3D& target_position, const Vector3D& target_size = Vector3D(3, 1, 3)) {
        
        if (ability_library.find(ability_name) == ability_library.end()) {
            std::cout << "Unknown reality manipulation ability: " << ability_name << std::endl;
            return false;
        }
        
        RealityAbility& ability = ability_library[ability_name];
        
        if (!ability.canUse(*character)) {
            std::cout << character->getName() << " cannot use " << ability_name << "!" << std::endl;
            return false;
        }
        
        // Consume mana
        character->getStats().currentMana -= ability.getManaCost();
        
        // Create reality fragment
        std::unique_ptr<RealityFragment> fragment = createRealityFragment(
            ability, character->getName(), target_position, target_size
        );
        
        if (fragment) {
            std::cout << "\n🌟 REALITY MANIPULATION ACTIVATED! 🌟" << std::endl;
            std::cout << character->getName() << " uses " << ability_name << "!" << std::endl;
            fragment->display();
            
            // Add to active fragments
            active_fragments.push_back(std::move(fragment));
            
            // Update global reality state
            updateGlobalReality(ability.getType(), ability.getPowerLevel());
            
            // Log the manipulation
            std::string log_entry = character->getName() + " used " + ability_name + " at ";
            reality_log.push_back(log_entry);
            
            return true;
        }
        
        return false;
    }
    
    std::unique_ptr<RealityFragment> createRealityFragment(const RealityAbility& ability, 
                                                          const std::string& creator,
                                                          const Vector3D& position, 
                                                          const Vector3D& size) {
        
        std::string fragment_name = generateFragmentName(ability.getType(), creator);
        
        auto fragment = std::unique_ptr<RealityFragment>(
            new RealityFragment(fragment_name, position, size, ability.getType(), creator)
        );
        
        // Set properties based on ability type
        switch (ability.getType()) {
            case RealityManipulationType::WEAVE_PLATFORM:
                fragment->setDescription("A solid platform woven from narrative threads");
                fragment->setColorSignature(Vector3D(0.8, 0.6, 1.0)); // Purple-blue
                fragment->setStability(0.9);
                break;
                
            case RealityManipulationType::MANIFEST_CONCEPT:
                fragment->setDescription("An abstract concept given physical form");
                fragment->setColorSignature(Vector3D(1.0, 0.8, 0.2)); // Golden
                fragment->setStability(0.7);
                fragment->setNarrativePower(2.0);
                break;
                
            case RealityManipulationType::BREAK_BARRIER:
                fragment->setDescription("A shattered barrier, opening new possibilities");
                fragment->setColorSignature(Vector3D(1.0, 0.3, 0.3)); // Red
                fragment->setStability(0.6);
                break;
                
            case RealityManipulationType::LIBERATE_NARRATIVE:
                fragment->setDescription("A freed story reshaping local reality");
                fragment->setColorSignature(Vector3D(1.0, 0.5, 0.0)); // Orange
                fragment->setStability(0.8);
                fragment->setNarrativePower(1.5);
                break;
                
            case RealityManipulationType::EMOTIONAL_RESONANCE:
                fragment->setDescription("An area suffused with emotional energy");
                fragment->setColorSignature(Vector3D(1.0, 0.8, 0.9)); // Pink
                fragment->setStability(0.5);
                break;
                
            case RealityManipulationType::DREAM_MATERIALIZATION:
                fragment->setDescription("A collective dream made manifest");
                fragment->setColorSignature(Vector3D(0.9, 0.9, 1.0)); // Pale blue
                fragment->setStability(0.4);
                fragment->setNarrativePower(3.0);
                break;
                
            case RealityManipulationType::ANCHOR_REALITY:
                fragment->setDescription("A stable anchor point in reality");
                fragment->setColorSignature(Vector3D(0.5, 0.8, 0.5)); // Green
                fragment->setStability(1.0);
                fragment->setPermanent(true);
                break;
                
            case RealityManipulationType::MEMORY_RECONSTRUCTION:
                fragment->setDescription("A space rebuilt from recovered memories");
                fragment->setColorSignature(Vector3D(0.7, 0.7, 0.9)); // Lavender
                fragment->setStability(0.8);
                break;
                
            case RealityManipulationType::OPEN_PORTAL:
                fragment->setDescription("A passage between narrative spaces");
                fragment->setColorSignature(Vector3D(0.2, 1.0, 0.8)); // Cyan
                fragment->setStability(0.6);
                break;
                
            case RealityManipulationType::TRANSFORM_SPACE:
                fragment->setDescription("A fundamentally altered region of reality");
                fragment->setColorSignature(Vector3D(1.0, 0.6, 1.0)); // Magenta
                fragment->setStability(0.7);
                fragment->setNarrativePower(2.5);
                break;
        }
        
        return fragment;
    }
    
    std::string generateFragmentName(RealityManipulationType type, const std::string& creator) {
        std::vector<std::string> prefixes;
        std::vector<std::string> suffixes;
        
        switch (type) {
            case RealityManipulationType::WEAVE_PLATFORM:
                prefixes = {"Woven", "Narrative", "Story", "Thread"};
                suffixes = {"Platform", "Bridge", "Foundation", "Path"};
                break;
            case RealityManipulationType::MANIFEST_CONCEPT:
                prefixes = {"Manifested", "Crystallized", "Embodied", "Realized"};
                suffixes = {"Concept", "Idea", "Thought", "Vision"};
                break;
            case RealityManipulationType::BREAK_BARRIER:
                prefixes = {"Shattered", "Broken", "Liberated", "Freed"};
                suffixes = {"Barrier", "Wall", "Limitation", "Boundary"};
                break;
            case RealityManipulationType::LIBERATE_NARRATIVE:
                prefixes = {"Liberated", "Awakened", "Freed", "Unbound"};
                suffixes = {"Story", "Tale", "Narrative", "Chronicle"};
                break;
            case RealityManipulationType::EMOTIONAL_RESONANCE:
                prefixes = {"Resonant", "Emotional", "Empathic", "Feeling"};
                suffixes = {"Field", "Aura", "Zone", "Space"};
                break;
            default:
                prefixes = {"Reality", "Narrative", "Dream", "Story"};
                suffixes = {"Fragment", "Manifestation", "Creation", "Weaving"};
                break;
        }
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> prefix_dis(0, prefixes.size() - 1);
        std::uniform_int_distribution<> suffix_dis(0, suffixes.size() - 1);
        
        return prefixes[prefix_dis(gen)] + " " + suffixes[suffix_dis(gen)];
    }
    
    void updateGlobalReality(RealityManipulationType type, double power) {
        narrative_saturation += power * 0.1;
        
        switch (type) {
            case RealityManipulationType::ANCHOR_REALITY:
                global_stability += power * 0.1;
                break;
            case RealityManipulationType::BREAK_BARRIER:
            case RealityManipulationType::LIBERATE_NARRATIVE:
                global_stability -= power * 0.05; // Liberation can destabilize
                break;
            case RealityManipulationType::DREAM_MATERIALIZATION:
                global_stability -= power * 0.08; // Dreams are inherently unstable
                break;
            default:
                // Most manipulations slightly affect stability
                global_stability += (power - 1.0) * 0.02;
                break;
        }
        
        global_stability = std::max(0.0, std::min(2.0, global_stability));
        narrative_saturation = std::max(0.0, narrative_saturation);
        
        if (narrative_saturation > 10.0) {
            std::cout << "\n🌟 REALITY SATURATION REACHED! 🌟" << std::endl;
            std::cout << "The Metaverse pulses with narrative energy!" << std::endl;
            std::cout << "New possibilities emerge from the collective consciousness!" << std::endl;
        }
    }
    
    void update(double deltaTime) {
        // Update all active fragments
        for (auto it = active_fragments.begin(); it != active_fragments.end();) {
            (*it)->update(deltaTime);
            
            if ((*it)->getStability() <= 0.0 && !(*it)->isPermanent()) {
                it = active_fragments.erase(it);
            } else {
                ++it;
            }
        }
        
        // Handle fragment interactions
        for (size_t i = 0; i < active_fragments.size(); ++i) {
            for (size_t j = i + 1; j < active_fragments.size(); ++j) {
                active_fragments[i]->resonate(*active_fragments[j]);
            }
        }
        
        // Slowly restore global stability
        if (global_stability < 1.0) {
            global_stability += deltaTime * 0.05;
        }
    }
    
    std::vector<std::string> getAvailableAbilities(const std::string& character_class) {
        std::vector<std::string> available;
        
        for (const auto& pair : ability_library) {
            const RealityAbility& ability = pair.second;
            if (ability.getClassRestriction().empty() || 
                ability.getClassRestriction() == character_class) {
                available.push_back(pair.first);
            }
        }
        
        return available;
    }
    
    void displayRealityStatus() {
        std::cout << "\n=== REALITY STATUS ===" << std::endl;
        std::cout << "Global Stability: " << (global_stability * 100) << "%" << std::endl;
        std::cout << "Narrative Saturation: " << narrative_saturation << std::endl;
        std::cout << "Active Fragments: " << active_fragments.size() << std::endl;
        
        if (!active_fragments.empty()) {
            std::cout << "\n--- ACTIVE REALITY FRAGMENTS ---" << std::endl;
            for (const auto& fragment : active_fragments) {
                fragment->display();
                std::cout << std::endl;
            }
        }
        
        if (!reality_log.empty() && reality_log.size() <= 5) {
            std::cout << "\n--- RECENT REALITY MANIPULATIONS ---" << std::endl;
            for (const auto& entry : reality_log) {
                std::cout << "• " << entry << std::endl;
            }
        }
        
        std::cout << "======================" << std::endl;
    }
    
    bool checkFragmentAt(const Vector3D& position) {
        for (const auto& fragment : active_fragments) {
            if (fragment->contains(position)) {
                std::cout << "Reality fragment detected: " << fragment->getName() << std::endl;
                return true;
            }
        }
        return false;
    }
    
    double getGlobalStability() const { return global_stability; }
    double getNarrativeSaturation() const { return narrative_saturation; }
    size_t getActiveFragmentCount() const { return active_fragments.size(); }
};

#endif // REALITYMANIPULATION_H
