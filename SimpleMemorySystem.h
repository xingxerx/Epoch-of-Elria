#ifndef SIMPLEMEMORYSYSTEM_H
#define SIMPLEMEMORYSYSTEM_H

#include "Vector3D.h"
#include "RPGCharacter.h"
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <random>
#include <algorithm>

// Forward declarations
class RPGCharacter;

// --- Memory Types ---
enum class StoryMemoryType {
    PERSONAL_MEMORY,        // Individual character memories
    SHARED_EXPERIENCE,      // Memories shared between characters
    COLLECTIVE_UNCONSCIOUS, // Universal memories from The Heart
    LIBRARY_KNOWLEDGE,      // Infinite library information (Xing)
    LIBERATION_TRUTH,       // Memories of freedom and resistance (Xerx)
    EMOTIONAL_IMPRINT,      // Emotional memories (The Heart)
    NARRATIVE_FRAGMENT,     // Pieces of stories
    REALITY_ECHO,          // Memories of how reality used to be
    DREAM_MEMORY,          // Memories from dreams and visions
    THE_ONE_SUPPRESSION    // Memories suppressed by The One
};

// --- Story Memory Fragment (uses existing MemoryFragment as base) ---
class StoryMemoryFragment {
private:
    std::string id;
    std::string title;
    StoryMemoryType type;
    Vector3D origin_location;
    double emotional_intensity;
    double narrative_power;
    bool is_recovered;
    std::string original_owner;
    std::map<std::string, double> character_resonance;
    MemoryFragment base_fragment; // Use existing MemoryFragment
    
public:
    StoryMemoryFragment(const std::string& fragment_id, const std::string& mem_title, 
                       const std::string& mem_content, StoryMemoryType mem_type,
                       const Vector3D& origin = Vector3D::Zero())
        : id(fragment_id), title(mem_title), type(mem_type),
          origin_location(origin), emotional_intensity(0.5), narrative_power(1.0),
          is_recovered(false), original_owner("Unknown"),
          base_fragment(mem_content, origin, 0.3) {}
    
    // Getters
    const std::string& getId() const { return id; }
    const std::string& getTitle() const { return title; }
    const std::string& getContent() const { return base_fragment.getContent(); }
    StoryMemoryType getType() const { return type; }
    const Vector3D& getOriginLocation() const { return origin_location; }
    double getClarity() const { return base_fragment.getClarity(); }
    double getEmotionalIntensity() const { return emotional_intensity; }
    double getNarrativePower() const { return narrative_power; }
    bool isRecovered() const { return is_recovered; }
    const std::string& getOriginalOwner() const { return original_owner; }
    
    // Setters
    void setOriginalOwner(const std::string& owner) { original_owner = owner; }
    void setCharacterResonance(const std::string& character_name, double resonance) {
        character_resonance[character_name] = std::max(0.0, std::min(1.0, resonance));
    }
    
    double getCharacterResonance(const std::string& character_name) const {
        auto it = character_resonance.find(character_name);
        return (it != character_resonance.end()) ? it->second : 0.0;
    }
    
    // Memory manipulation
    void enhanceClarity(double amount) {
        base_fragment.enhanceClarity(amount);
        if (getClarity() >= 0.9 && !is_recovered) {
            is_recovered = true;
            std::cout << "Memory fragment '" << title << "' has been fully recovered!" << std::endl;
        }
    }
    
    void amplifyEmotion(double amount) {
        emotional_intensity = std::min(1.0, emotional_intensity + amount);
        narrative_power += amount * 0.5;
    }
    
    void reconstruct() {
        if (!is_recovered) {
            base_fragment.enhanceClarity(1.0 - getClarity()); // Enhance to full clarity
            base_fragment.recover(); // Mark as recovered
            is_recovered = true;
            narrative_power += 1.0;
            std::cout << "Memory '" << title << "' has been reconstructed!" << std::endl;
            std::cout << "Full memory: " << getContent() << std::endl;
        }
    }
    
    std::string getTypeString() const {
        switch (type) {
            case StoryMemoryType::PERSONAL_MEMORY: return "Personal Memory";
            case StoryMemoryType::SHARED_EXPERIENCE: return "Shared Experience";
            case StoryMemoryType::COLLECTIVE_UNCONSCIOUS: return "Collective Unconscious";
            case StoryMemoryType::LIBRARY_KNOWLEDGE: return "Library Knowledge";
            case StoryMemoryType::LIBERATION_TRUTH: return "Liberation Truth";
            case StoryMemoryType::EMOTIONAL_IMPRINT: return "Emotional Imprint";
            case StoryMemoryType::NARRATIVE_FRAGMENT: return "Narrative Fragment";
            case StoryMemoryType::REALITY_ECHO: return "Reality Echo";
            case StoryMemoryType::DREAM_MEMORY: return "Dream Memory";
            case StoryMemoryType::THE_ONE_SUPPRESSION: return "Suppressed by The One";
            default: return "Unknown";
        }
    }
    
    void display() const {
        std::cout << "\n=== Memory Fragment: " << title << " ===" << std::endl;
        std::cout << "ID: " << id << std::endl;
        std::cout << "Type: " << getTypeString() << std::endl;
        std::cout << "Clarity: " << (getClarity() * 100) << "%" << std::endl;
        std::cout << "Emotional Intensity: " << (emotional_intensity * 100) << "%" << std::endl;
        std::cout << "Narrative Power: " << narrative_power << std::endl;
        std::cout << "Recovered: " << (is_recovered ? "Yes" : "No") << std::endl;
        std::cout << "Original Owner: " << original_owner << std::endl;
        
        if (getClarity() > 0.5) {
            std::cout << "Content: " << getContent() << std::endl;
        } else {
            std::cout << "Content: [Too fragmented to read clearly]" << std::endl;
        }
        
        std::cout << "Origin Location: ";
        origin_location.print();
        std::cout << std::endl;
        std::cout << "================================" << std::endl;
    }
};

// --- Simple Memory Collection ---
class SimpleMemoryCollection {
private:
    std::string owner_name;
    std::vector<std::unique_ptr<StoryMemoryFragment>> fragments;
    double total_narrative_power;
    double memory_integrity;
    
public:
    SimpleMemoryCollection(const std::string& owner) 
        : owner_name(owner), total_narrative_power(0.0), memory_integrity(0.0) {}
    
    void addFragment(std::unique_ptr<StoryMemoryFragment> fragment) {
        fragment->setOriginalOwner(owner_name);
        total_narrative_power += fragment->getNarrativePower();
        updateMemoryIntegrity();
        
        std::cout << owner_name << " discovered memory: " << fragment->getTitle() << std::endl;
        fragments.push_back(std::move(fragment));
    }
    
    void updateMemoryIntegrity() {
        if (fragments.empty()) {
            memory_integrity = 0.0;
            return;
        }
        
        double total_clarity = 0.0;
        int recovered_count = 0;
        
        for (const auto& fragment : fragments) {
            total_clarity += fragment->getClarity();
            if (fragment->isRecovered()) {
                recovered_count++;
            }
        }
        
        double average_clarity = total_clarity / fragments.size();
        double recovery_ratio = static_cast<double>(recovered_count) / fragments.size();
        
        memory_integrity = (average_clarity * 0.6) + (recovery_ratio * 0.4);
    }
    
    void displayCollection() const {
        std::cout << "\n=== " << owner_name << "'s Memory Collection ===" << std::endl;
        std::cout << "Total Fragments: " << fragments.size() << std::endl;
        std::cout << "Memory Integrity: " << (memory_integrity * 100) << "%" << std::endl;
        std::cout << "Total Narrative Power: " << total_narrative_power << std::endl;
        
        if (!fragments.empty()) {
            std::cout << "\nRecent Fragments:" << std::endl;
            int count = 0;
            for (auto it = fragments.rbegin(); it != fragments.rend() && count < 3; ++it, ++count) {
                std::cout << "  • " << (*it)->getTitle() 
                          << " (Clarity: " << ((*it)->getClarity() * 100) << "%)" << std::endl;
            }
        }
        
        std::cout << "===========================================" << std::endl;
    }
    
    // Getters
    const std::string& getOwnerName() const { return owner_name; }
    size_t getFragmentCount() const { return fragments.size(); }
    double getMemoryIntegrity() const { return memory_integrity; }
    double getTotalNarrativePower() const { return total_narrative_power; }
    
    const std::vector<std::unique_ptr<StoryMemoryFragment>>& getFragments() const { return fragments; }
};

// --- Simple Memory System ---
class SimpleMemorySystem {
private:
    std::map<std::string, SimpleMemoryCollection> character_memories;
    std::vector<std::unique_ptr<StoryMemoryFragment>> world_fragments;
    
    // Global memory state
    double collective_memory_strength;
    double the_one_suppression_level;
    int total_fragments_discovered;
    int total_fragments_reconstructed;
    
public:
    SimpleMemorySystem() : collective_memory_strength(0.0), the_one_suppression_level(0.8),
                          total_fragments_discovered(0), total_fragments_reconstructed(0) {
        generateWorldFragments();
    }
    
    void generateWorldFragments() {
        // Create initial world fragments for discovery
        
        // Xerx's suppressed memories
        auto xerx_memory1 = std::unique_ptr<StoryMemoryFragment>(
            new StoryMemoryFragment("xerx_001", "Brother's Warmth", 
                                   "A memory of sitting with Xing in the infinite library, sharing stories and laughter.",
                                   StoryMemoryType::PERSONAL_MEMORY, Vector3D(0, 0, 0))
        );
        xerx_memory1->setCharacterResonance("Xerx", 0.9);
        xerx_memory1->setCharacterResonance("Xing", 0.8);
        world_fragments.push_back(std::move(xerx_memory1));
        
        auto xerx_memory2 = std::unique_ptr<StoryMemoryFragment>(
            new StoryMemoryFragment("xerx_002", "The Heart's Birth",
                                   "The moment when The Heart fell from a dying book, pulsing with pure narrative potential.",
                                   StoryMemoryType::SHARED_EXPERIENCE, Vector3D(5, 3, 5))
        );
        xerx_memory2->setCharacterResonance("Xerx", 0.7);
        xerx_memory2->setCharacterResonance("The Heart", 1.0);
        world_fragments.push_back(std::move(xerx_memory2));
        
        // Xing's library knowledge
        auto xing_memory1 = std::unique_ptr<StoryMemoryFragment>(
            new StoryMemoryFragment("xing_001", "First Story Weaving",
                                   "The first time Xing successfully wove a story into reality, creating a bridge of pure narrative.",
                                   StoryMemoryType::LIBRARY_KNOWLEDGE, Vector3D(-10, 5, 0))
        );
        xing_memory1->setCharacterResonance("Xing", 1.0);
        world_fragments.push_back(std::move(xing_memory1));
        
        // The Heart's collective memories
        auto heart_memory1 = std::unique_ptr<StoryMemoryFragment>(
            new StoryMemoryFragment("heart_001", "Lyra's Melody",
                                   "The pure melody of Lyra echoing through the Metaverse, awakening dormant narratives.",
                                   StoryMemoryType::COLLECTIVE_UNCONSCIOUS, Vector3D(0, 10, 0))
        );
        heart_memory1->setCharacterResonance("The Heart", 0.9);
        world_fragments.push_back(std::move(heart_memory1));
        
        // Suppressed memories
        auto suppressed1 = std::unique_ptr<StoryMemoryFragment>(
            new StoryMemoryFragment("suppressed_001", "Before The One",
                                   "A time when stories flowed freely, before The One imposed singular control.",
                                   StoryMemoryType::THE_ONE_SUPPRESSION, Vector3D(15, 0, 15))
        );
        suppressed1->enhanceClarity(-0.5); // Heavily suppressed
        world_fragments.push_back(std::move(suppressed1));
        
        total_fragments_discovered = world_fragments.size();
    }
    
    void addCharacter(const std::string& character_name) {
        if (character_memories.find(character_name) == character_memories.end()) {
            character_memories.insert(std::make_pair(character_name, SimpleMemoryCollection(character_name)));
        }
    }
    
    bool searchForMemory(const std::string& character_name, StoryMemoryType target_type,
                        const Vector3D& search_location = Vector3D::Zero()) {
        
        // Find fragments of the target type near the location
        std::vector<StoryMemoryFragment*> found_fragments;
        
        for (auto& fragment : world_fragments) {
            if (fragment->getType() == target_type) {
                double distance = search_location.distanceTo(fragment->getOriginLocation());
                if (distance <= 20.0) { // Search radius
                    found_fragments.push_back(fragment.get());
                }
            }
        }
        
        if (found_fragments.empty()) {
            std::cout << "No memory fragments of the target type found in this area." << std::endl;
            return false;
        }
        
        // Select the best fragment based on character resonance
        StoryMemoryFragment* best_fragment = nullptr;
        double best_resonance = 0.0;
        
        for (auto* fragment : found_fragments) {
            double resonance = fragment->getCharacterResonance(character_name);
            if (resonance > best_resonance) {
                best_resonance = resonance;
                best_fragment = fragment;
            }
        }
        
        if (best_fragment) {
            // Enhance the fragment
            best_fragment->enhanceClarity(0.3);
            
            // Move fragment to character's collection
            addCharacter(character_name);
            
            // Find and remove from world fragments
            auto it = std::find_if(world_fragments.begin(), world_fragments.end(),
                [best_fragment](const std::unique_ptr<StoryMemoryFragment>& f) {
                    return f.get() == best_fragment;
                });
            
            if (it != world_fragments.end()) {
                auto fragment_ptr = std::move(*it);
                world_fragments.erase(it);
                auto char_it = character_memories.find(character_name);
                if (char_it != character_memories.end()) {
                    char_it->second.addFragment(std::move(fragment_ptr));
                }

                updateGlobalMemoryState();
                return true;
            }
        }
        
        return false;
    }
    
    void updateGlobalMemoryState() {
        // Calculate collective memory strength
        double total_integrity = 0.0;
        int character_count = 0;
        
        for (const auto& pair : character_memories) {
            total_integrity += pair.second.getMemoryIntegrity();
            character_count++;
        }
        
        if (character_count > 0) {
            collective_memory_strength = total_integrity / character_count;
        }
        
        // Reduce The One's suppression as memories are recovered
        the_one_suppression_level = std::max(0.0, 0.8 - (collective_memory_strength * 0.5));
        
        // Check for memory milestones
        if (collective_memory_strength > 0.7 && the_one_suppression_level < 0.3) {
            std::cout << "\n🌟 MEMORY BREAKTHROUGH! 🌟" << std::endl;
            std::cout << "The collective memory strength weakens The One's control!" << std::endl;
            std::cout << "New suppressed memories become accessible!" << std::endl;
        }
    }
    
    void displayMemoryStatus() {
        std::cout << "\n=== GLOBAL MEMORY STATUS ===" << std::endl;
        std::cout << "Collective Memory Strength: " << (collective_memory_strength * 100) << "%" << std::endl;
        std::cout << "The One's Suppression Level: " << (the_one_suppression_level * 100) << "%" << std::endl;
        std::cout << "Total Fragments Discovered: " << total_fragments_discovered << std::endl;
        std::cout << "Total Fragments Reconstructed: " << total_fragments_reconstructed << std::endl;
        std::cout << "World Fragments Remaining: " << world_fragments.size() << std::endl;
        
        std::cout << "\n--- CHARACTER MEMORY COLLECTIONS ---" << std::endl;
        for (const auto& pair : character_memories) {
            pair.second.displayCollection();
        }
        
        if (!world_fragments.empty()) {
            std::cout << "\n--- DISCOVERABLE FRAGMENTS ---" << std::endl;
            for (const auto& fragment : world_fragments) {
                std::cout << "• " << fragment->getTitle() 
                          << " (" << fragment->getTypeString() << ")" << std::endl;
            }
        }
        
        std::cout << "============================" << std::endl;
    }
    
    // Getters
    double getCollectiveMemoryStrength() const { return collective_memory_strength; }
    double getTheOneSuppressionLevel() const { return the_one_suppression_level; }
    int getTotalFragmentsDiscovered() const { return total_fragments_discovered; }
    int getTotalFragmentsReconstructed() const { return total_fragments_reconstructed; }
    size_t getWorldFragmentCount() const { return world_fragments.size(); }
    
    SimpleMemoryCollection* getCharacterMemories(const std::string& character_name) {
        auto it = character_memories.find(character_name);
        return (it != character_memories.end()) ? &it->second : nullptr;
    }
};

#endif // SIMPLEMEMORYSYSTEM_H
