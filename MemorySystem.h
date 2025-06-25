#ifndef MEMORYSYSTEM_H
#define MEMORYSYSTEM_H

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
class Xing;
class Xerx;
class TheHeart;

// --- Memory Fragment Types ---
enum class MemoryType {
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

// --- Enhanced Memory Fragment Class (extends existing MemoryFragment) ---
class EnhancedMemoryFragment : public MemoryFragment {
private:
    std::string id;
    std::string title;
    MemoryType type;
    Vector3D origin_location;
    Vector3D discovery_location;

    double emotional_intensity; // 0.0 to 1.0 - emotional strength
    double narrative_power;   // How much story energy it contains
    bool is_recovered;        // Has been fully reconstructed
    bool is_shared;          // Can be shared with other characters

    std::string original_owner;
    std::vector<std::string> connected_fragments; // IDs of related fragments
    std::map<std::string, double> character_resonance; // How much each character connects

public:
    EnhancedMemoryFragment(const std::string& fragment_id, const std::string& mem_title,
                          const std::string& mem_content, MemoryType mem_type,
                          const Vector3D& origin = Vector3D::Zero())
        : MemoryFragment(mem_content, origin, 0.3), // Call parent constructor
          id(fragment_id), title(mem_title), type(mem_type),
          origin_location(origin), discovery_location(origin),
          emotional_intensity(0.5), narrative_power(1.0), is_recovered(false),
          is_shared(false), original_owner("Unknown") {}

    // Getters
    const std::string& getId() const { return id; }
    const std::string& getTitle() const { return title; }
    const std::string& getContent() const { return getMemoryContent(); } // Use parent method
    MemoryType getType() const { return type; }
    const Vector3D& getOriginLocation() const { return origin_location; }
    const Vector3D& getDiscoveryLocation() const { return discovery_location; }
    double getClarity() const { return getClearness(); } // Use parent method
    double getEmotionalIntensity() const { return emotional_intensity; }
    double getNarrativePower() const { return narrative_power; }
    bool isRecovered() const { return is_recovered; }
    bool isShared() const { return is_shared; }
    const std::string& getOriginalOwner() const { return original_owner; }

    // Setters
    void setDiscoveryLocation(const Vector3D& location) { discovery_location = location; }
    void setOriginalOwner(const std::string& owner) { original_owner = owner; }
    void setShared(bool shared) { is_shared = shared; }

    // Memory manipulation
    void enhanceClarity(double amount) {
        double new_clarity = std::min(1.0, getClearness() + amount);
        setClearness(new_clarity); // Use parent method
        if (new_clarity >= 0.9 && !is_recovered) {
            is_recovered = true;
            std::cout << "Memory fragment '" << title << "' has been fully recovered!" << std::endl;
        }
    }

    void amplifyEmotion(double amount) {
        emotional_intensity = std::min(1.0, emotional_intensity + amount);
        narrative_power += amount * 0.5;
    }

    void addConnection(const std::string& fragment_id) {
        if (std::find(connected_fragments.begin(), connected_fragments.end(), fragment_id)
            == connected_fragments.end()) {
            connected_fragments.push_back(fragment_id);
        }
    }

    void setCharacterResonance(const std::string& character_name, double resonance) {
        character_resonance[character_name] = std::max(0.0, std::min(1.0, resonance));
    }

    double getCharacterResonance(const std::string& character_name) const {
        auto it = character_resonance.find(character_name);
        return (it != character_resonance.end()) ? it->second : 0.0;
    }

    // Memory reconstruction
    bool canReconstructWith(const MemoryFragment& other) const {
        // Check if fragments are connected
        if (std::find(connected_fragments.begin(), connected_fragments.end(), other.id)
            != connected_fragments.end()) {
            return true;
        }

        // Check if they're the same type and from same owner
        if (type == other.type && original_owner == other.original_owner) {
            return true;
        }

        // Check if they're emotionally compatible
        if (std::abs(emotional_intensity - other.emotional_intensity) < 0.3) {
            return true;
        }

        return false;
    }

    void reconstruct() {
        if (!is_recovered) {
            setClearness(1.0); // Use parent method
            is_recovered = true;
            narrative_power += 1.0;
            std::cout << "Memory '" << title << "' has been reconstructed!" << std::endl;
            std::cout << "Full memory: " << getContent() << std::endl;
        }
    }

    void display() const {
        std::cout << "\n=== Memory Fragment: " << title << " ===" << std::endl;
        std::cout << "ID: " << id << std::endl;
        std::cout << "Type: ";
        switch (type) {
            case MemoryType::PERSONAL_MEMORY: std::cout << "Personal Memory"; break;
            case MemoryType::SHARED_EXPERIENCE: std::cout << "Shared Experience"; break;
            case MemoryType::COLLECTIVE_UNCONSCIOUS: std::cout << "Collective Unconscious"; break;
            case MemoryType::LIBRARY_KNOWLEDGE: std::cout << "Library Knowledge"; break;
            case MemoryType::LIBERATION_TRUTH: std::cout << "Liberation Truth"; break;
            case MemoryType::EMOTIONAL_IMPRINT: std::cout << "Emotional Imprint"; break;
            case MemoryType::NARRATIVE_FRAGMENT: std::cout << "Narrative Fragment"; break;
            case MemoryType::REALITY_ECHO: std::cout << "Reality Echo"; break;
            case MemoryType::DREAM_MEMORY: std::cout << "Dream Memory"; break;
            case MemoryType::THE_ONE_SUPPRESSION: std::cout << "Suppressed by The One"; break;
        }
        std::cout << std::endl;

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

        if (!connected_fragments.empty()) {
            std::cout << "Connected to " << connected_fragments.size() << " other fragments" << std::endl;
        }

        std::cout << "Origin Location: ";
        origin_location.print();
        std::cout << std::endl;
        std::cout << "================================" << std::endl;
    }

    std::string getTypeString() const {
        switch (type) {
            case MemoryType::PERSONAL_MEMORY: return "Personal Memory";
            case MemoryType::SHARED_EXPERIENCE: return "Shared Experience";
            case MemoryType::COLLECTIVE_UNCONSCIOUS: return "Collective Unconscious";
            case MemoryType::LIBRARY_KNOWLEDGE: return "Library Knowledge";
            case MemoryType::LIBERATION_TRUTH: return "Liberation Truth";
            case MemoryType::EMOTIONAL_IMPRINT: return "Emotional Imprint";
            case MemoryType::NARRATIVE_FRAGMENT: return "Narrative Fragment";
            case MemoryType::REALITY_ECHO: return "Reality Echo";
            case MemoryType::DREAM_MEMORY: return "Dream Memory";
            case MemoryType::THE_ONE_SUPPRESSION: return "Suppressed by The One";
            default: return "Unknown";
        }
    }
};

// --- Memory Collection Class ---
class MemoryCollection {
private:
    std::string owner_name;
    std::vector<std::unique_ptr<EnhancedMemoryFragment>> fragments;
    std::map<MemoryType, int> type_counts;
    double total_narrative_power;
    double memory_integrity;

public:
    MemoryCollection(const std::string& owner)
        : owner_name(owner), total_narrative_power(0.0), memory_integrity(0.0) {}

    void addFragment(std::unique_ptr<EnhancedMemoryFragment> fragment) {
        fragment->setOriginalOwner(owner_name);
        type_counts[fragment->getType()]++;
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

    std::vector<EnhancedMemoryFragment*> findConnectedFragments(const std::string& fragment_id) {
        std::vector<EnhancedMemoryFragment*> connected;

        for (auto& fragment : fragments) {
            if (fragment->getId() != fragment_id) {
                // Find the target fragment first
                auto target_it = std::find_if(fragments.begin(), fragments.end(),
                    [&fragment_id](const std::unique_ptr<EnhancedMemoryFragment>& f) {
                        return f->getId() == fragment_id;
                    });

                if (target_it != fragments.end() &&
                    fragment->canReconstructWith(**target_it)) {
                    connected.push_back(fragment.get());
                }
            }
        }

        return connected;
    }

    bool reconstructMemory(const std::string& fragment_id) {
        auto it = std::find_if(fragments.begin(), fragments.end(),
            [&fragment_id](const std::unique_ptr<EnhancedMemoryFragment>& f) {
                return f->getId() == fragment_id;
            });

        if (it != fragments.end()) {
            auto connected = findConnectedFragments(fragment_id);

            if (!connected.empty()) {
                // Enhance clarity using connected fragments
                double clarity_boost = connected.size() * 0.2;
                (*it)->enhanceClarity(clarity_boost);

                // If enough connected fragments, fully reconstruct
                if (connected.size() >= 2) {
                    (*it)->reconstruct();
                    updateMemoryIntegrity();
                    return true;
                }
            }
        }

        return false;
    }

    void shareMemory(const std::string& fragment_id, MemoryCollection& other_collection) {
        auto it = std::find_if(fragments.begin(), fragments.end(),
            [&fragment_id](const std::unique_ptr<EnhancedMemoryFragment>& f) {
                return f->getId() == fragment_id;
            });

        if (it != fragments.end() && (*it)->isRecovered()) {
            // Create a copy for the other collection
            auto shared_fragment = std::unique_ptr<EnhancedMemoryFragment>(
                new EnhancedMemoryFragment((*it)->getId() + "_shared", (*it)->getTitle(),
                                         (*it)->getContent(), (*it)->getType(),
                                         (*it)->getOriginLocation())
            );

            shared_fragment->enhanceClarity((*it)->getClarity());
            shared_fragment->setShared(true);

            std::cout << owner_name << " shares memory '" << (*it)->getTitle()
                      << "' with " << other_collection.owner_name << std::endl;

            other_collection.addFragment(std::move(shared_fragment));
            (*it)->setShared(true);
        }
    }

    std::vector<EnhancedMemoryFragment*> getFragmentsByType(MemoryType type) {
        std::vector<EnhancedMemoryFragment*> result;
        for (auto& fragment : fragments) {
            if (fragment->getType() == type) {
                result.push_back(fragment.get());
            }
        }
        return result;
    }

    void displayCollection() const {
        std::cout << "\n=== " << owner_name << "'s Memory Collection ===" << std::endl;
        std::cout << "Total Fragments: " << fragments.size() << std::endl;
        std::cout << "Memory Integrity: " << (memory_integrity * 100) << "%" << std::endl;
        std::cout << "Total Narrative Power: " << total_narrative_power << std::endl;

        std::cout << "\nFragments by Type:" << std::endl;
        for (const auto& pair : type_counts) {
            std::cout << "  " << getTypeString(pair.first)
                      << ": " << pair.second << std::endl;
        }

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

    const std::vector<std::unique_ptr<EnhancedMemoryFragment>>& getFragments() const { return fragments; }

private:
    std::string getTypeString(MemoryType type) const {
        switch (type) {
            case MemoryType::PERSONAL_MEMORY: return "Personal Memory";
            case MemoryType::SHARED_EXPERIENCE: return "Shared Experience";
            case MemoryType::COLLECTIVE_UNCONSCIOUS: return "Collective Unconscious";
            case MemoryType::LIBRARY_KNOWLEDGE: return "Library Knowledge";
            case MemoryType::LIBERATION_TRUTH: return "Liberation Truth";
            case MemoryType::EMOTIONAL_IMPRINT: return "Emotional Imprint";
            case MemoryType::NARRATIVE_FRAGMENT: return "Narrative Fragment";
            case MemoryType::REALITY_ECHO: return "Reality Echo";
            case MemoryType::DREAM_MEMORY: return "Dream Memory";
            case MemoryType::THE_ONE_SUPPRESSION: return "Suppressed by The One";
            default: return "Unknown";
        }
    }
};

// --- Memory Reconstruction Ability ---
class MemoryAbility {
private:
    std::string name;
    std::string description;
    int mana_cost;
    MemoryType target_type;
    double effectiveness;
    std::string character_restriction;

public:
    MemoryAbility(const std::string& n, MemoryType type, int cost, double effect,
                  const std::string& restriction = "")
        : name(n), target_type(type), mana_cost(cost), effectiveness(effect),
          character_restriction(restriction) {}

    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    int getManaCost() const { return mana_cost; }
    MemoryType getTargetType() const { return target_type; }
    double getEffectiveness() const { return effectiveness; }
    const std::string& getCharacterRestriction() const { return character_restriction; }

    void setDescription(const std::string& desc) { description = desc; }

    bool canUse(const RPGCharacter& character) const {
        if (!character_restriction.empty() &&
            character.getCharacterClass() != character_restriction) {
            return false;
        }
        return character.getStats().currentMana >= mana_cost;
    }
};

// --- Main Memory System ---
class MemorySystem {
private:
    std::map<std::string, MemoryCollection> character_memories;
    std::vector<std::unique_ptr<EnhancedMemoryFragment>> world_fragments; // Unclaimed fragments
    std::map<std::string, MemoryAbility> ability_library;

    // Global memory state
    double collective_memory_strength;
    double the_one_suppression_level;
    int total_fragments_discovered;
    int total_fragments_reconstructed;

public:
    MemorySystem() : collective_memory_strength(0.0), the_one_suppression_level(0.8),
                    total_fragments_discovered(0), total_fragments_reconstructed(0) {
        initializeAbilityLibrary();
        generateWorldFragments();
    }

    void initializeAbilityLibrary() {
        // Xerx's Memory Abilities
        MemoryAbility memory_strike("Memory Strike", MemoryType::LIBERATION_TRUTH,
                                   12, 1.5, "The Liberator");
        memory_strike.setDescription("Xerx uses recovered memories as weapons against oppression.");
        ability_library["Memory Strike"] = memory_strike;

        MemoryAbility reconstruct_truth("Reconstruct Truth", MemoryType::PERSONAL_MEMORY,
                                       18, 2.0, "The Liberator");
        reconstruct_truth.setDescription("Xerx pieces together fragmented memories to reveal truth.");
        ability_library["Reconstruct Truth"] = reconstruct_truth;

        MemoryAbility break_suppression("Break Suppression", MemoryType::THE_ONE_SUPPRESSION,
                                       25, 2.5, "The Liberator");
        break_suppression.setDescription("Xerx breaks through The One's memory suppression.");
        ability_library["Break Suppression"] = break_suppression;

        // Xing's Library Abilities
        MemoryAbility access_library("Access Library", MemoryType::LIBRARY_KNOWLEDGE,
                                    15, 1.8, "The Weaver");
        access_library.setDescription("Xing accesses the infinite library's vast knowledge.");
        ability_library["Access Library"] = access_library;

        MemoryAbility weave_narrative("Weave Narrative", MemoryType::NARRATIVE_FRAGMENT,
                                     20, 2.2, "The Weaver");
        weave_narrative.setDescription("Xing weaves story fragments into coherent narratives.");
        ability_library["Weave Narrative"] = weave_narrative;

        // The Heart's Emotional Abilities
        MemoryAbility emotional_resonance("Emotional Resonance", MemoryType::EMOTIONAL_IMPRINT,
                                         10, 1.3, "Narrative Catalyst");
        emotional_resonance.setDescription("The Heart connects with emotional memories.");
        ability_library["Emotional Resonance"] = emotional_resonance;

        MemoryAbility collective_access("Collective Access", MemoryType::COLLECTIVE_UNCONSCIOUS,
                                       30, 3.0, "Narrative Catalyst");
        collective_access.setDescription("The Heart taps into the collective unconscious.");
        ability_library["Collective Access"] = collective_access;

        MemoryAbility dream_recall("Dream Recall", MemoryType::DREAM_MEMORY,
                                  22, 2.0, "Narrative Catalyst");
        dream_recall.setDescription("The Heart recovers memories from dreams and visions.");
        ability_library["Dream Recall"] = dream_recall;
    }

    void generateWorldFragments() {
        // Create initial world fragments for discovery

        // Xerx's suppressed memories
        auto xerx_memory1 = std::unique_ptr<EnhancedMemoryFragment>(
            new EnhancedMemoryFragment("xerx_001", "Brother's Warmth",
                                     "A memory of sitting with Xing in the infinite library, sharing stories and laughter.",
                                     MemoryType::PERSONAL_MEMORY, Vector3D(0, 0, 0))
        );
        xerx_memory1->setCharacterResonance("Xerx", 0.9);
        xerx_memory1->setCharacterResonance("Xing", 0.8);
        world_fragments.push_back(std::move(xerx_memory1));

        auto xerx_memory2 = std::unique_ptr<EnhancedMemoryFragment>(
            new EnhancedMemoryFragment("xerx_002", "The Heart's Birth",
                                     "The moment when The Heart fell from a dying book, pulsing with pure narrative potential.",
                                     MemoryType::SHARED_EXPERIENCE, Vector3D(5, 3, 5))
        );
        xerx_memory2->setCharacterResonance("Xerx", 0.7);
        xerx_memory2->setCharacterResonance("The Heart", 1.0);
        world_fragments.push_back(std::move(xerx_memory2));

        // Xing's library knowledge
        auto xing_memory1 = std::unique_ptr<EnhancedMemoryFragment>(
            new EnhancedMemoryFragment("xing_001", "First Story Weaving",
                                     "The first time Xing successfully wove a story into reality, creating a bridge of pure narrative.",
                                     MemoryType::LIBRARY_KNOWLEDGE, Vector3D(-10, 5, 0))
        );
        xing_memory1->setCharacterResonance("Xing", 1.0);
        world_fragments.push_back(std::move(xing_memory1));

        // The Heart's collective memories
        auto heart_memory1 = std::unique_ptr<EnhancedMemoryFragment>(
            new EnhancedMemoryFragment("heart_001", "Lyra's Melody",
                                     "The pure melody of Lyra echoing through the Metaverse, awakening dormant narratives.",
                                     MemoryType::COLLECTIVE_UNCONSCIOUS, Vector3D(0, 10, 0))
        );
        heart_memory1->setCharacterResonance("The Heart", 0.9);
        world_fragments.push_back(std::move(heart_memory1));

        // Suppressed memories
        auto suppressed1 = std::unique_ptr<EnhancedMemoryFragment>(
            new EnhancedMemoryFragment("suppressed_001", "Before The One",
                                     "A time when stories flowed freely, before The One imposed singular control.",
                                     MemoryType::THE_ONE_SUPPRESSION, Vector3D(15, 0, 15))
        );
        suppressed1->enhanceClarity(-0.5); // Heavily suppressed
        world_fragments.push_back(std::move(suppressed1));

        total_fragments_discovered = world_fragments.size();
    }

    void addCharacter(const std::string& character_name) {
        if (character_memories.find(character_name) == character_memories.end()) {
            character_memories.emplace(character_name, MemoryCollection(character_name));
        }
    }

    bool executeMemoryAbility(RPGCharacter* character, const std::string& ability_name,
                             const Vector3D& search_location = Vector3D::Zero()) {

        if (ability_library.find(ability_name) == ability_library.end()) {
            std::cout << "Unknown memory ability: " << ability_name << std::endl;
            return false;
        }

        MemoryAbility& ability = ability_library[ability_name];

        if (!ability.canUse(*character)) {
            std::cout << character->getName() << " cannot use " << ability_name << "!" << std::endl;
            return false;
        }

        // Consume mana
        character->getStats().currentMana -= ability.getManaCost();

        std::cout << "\n🧠 MEMORY ABILITY ACTIVATED! 🧠" << std::endl;
        std::cout << character->getName() << " uses " << ability_name << "!" << std::endl;
        std::cout << ability.getDescription() << std::endl;

        // Execute the ability
        return performMemorySearch(character->getName(), ability, search_location);
    }

    bool performMemorySearch(const std::string& character_name, const MemoryAbility& ability,
                            const Vector3D& location) {

        // Find fragments of the target type near the location
        std::vector<EnhancedMemoryFragment*> found_fragments;

        for (auto& fragment : world_fragments) {
            if (fragment->getType() == ability.getTargetType()) {
                double distance = location.distanceTo(fragment->getOriginLocation());
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
        EnhancedMemoryFragment* best_fragment = nullptr;
        double best_resonance = 0.0;

        for (auto* fragment : found_fragments) {
            double resonance = fragment->getCharacterResonance(character_name);
            if (resonance > best_resonance) {
                best_resonance = resonance;
                best_fragment = fragment;
            }
        }

        if (best_fragment) {
            // Enhance the fragment based on ability effectiveness
            best_fragment->enhanceClarity(ability.getEffectiveness() * 0.3);

            // Move fragment to character's collection
            addCharacter(character_name);

            // Find and remove from world fragments
            auto it = std::find_if(world_fragments.begin(), world_fragments.end(),
                [best_fragment](const std::unique_ptr<EnhancedMemoryFragment>& f) {
                    return f.get() == best_fragment;
                });

            if (it != world_fragments.end()) {
                auto fragment_ptr = std::move(*it);
                world_fragments.erase(it);
                character_memories[character_name].addFragment(std::move(fragment_ptr));

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

            // Unlock more suppressed fragments
            generateSuppressedFragments();
        }
    }

    void generateSuppressedFragments() {
        // Generate additional suppressed memories when breakthrough occurs
        std::vector<std::string> suppressed_titles = {
            "The Original Harmony", "Stories Without Chains", "The First Rebellion",
            "Collective Creation", "The Time Before Order", "Infinite Possibilities"
        };

        std::vector<std::string> suppressed_contents = {
            "A time when all consciousness worked together to create reality.",
            "Stories that flowed freely without The One's interference.",
            "The first attempt to resist The One's singular control.",
            "When multiple minds could shape reality simultaneously.",
            "Before The One imposed absolute order on existence.",
            "A realm where every possibility could coexist."
        };

        for (size_t i = 0; i < std::min(suppressed_titles.size(), suppressed_contents.size()); ++i) {
            auto fragment = std::unique_ptr<EnhancedMemoryFragment>(
                new EnhancedMemoryFragment("suppressed_" + std::to_string(i + 2),
                                         suppressed_titles[i], suppressed_contents[i],
                                         MemoryType::THE_ONE_SUPPRESSION,
                                         Vector3D((i-2)*10, 0, (i-2)*8))
            );
            fragment->enhanceClarity(collective_memory_strength * 0.5);
            world_fragments.push_back(std::move(fragment));
        }

        total_fragments_discovered += suppressed_titles.size();
    }

    void shareMemoryBetweenCharacters(const std::string& from_character,
                                     const std::string& to_character,
                                     const std::string& fragment_id) {

        auto from_it = character_memories.find(from_character);
        auto to_it = character_memories.find(to_character);

        if (from_it != character_memories.end() && to_it != character_memories.end()) {
            from_it->second.shareMemory(fragment_id, to_it->second);

            std::cout << "\n💫 MEMORY SHARED! 💫" << std::endl;
            std::cout << "The connection between " << from_character << " and "
                      << to_character << " strengthens!" << std::endl;

            // Boost collective memory strength
            collective_memory_strength += 0.1;
            updateGlobalMemoryState();
        }
    }

    void reconstructCollaborativeMemory(const std::vector<std::string>& character_names,
                                       const std::string& memory_theme) {

        std::cout << "\n🌟 COLLABORATIVE MEMORY RECONSTRUCTION! 🌟" << std::endl;
        std::cout << "Characters work together to reconstruct: " << memory_theme << std::endl;

        // Find related fragments across all characters
        std::vector<EnhancedMemoryFragment*> related_fragments;

        for (const auto& char_name : character_names) {
            auto it = character_memories.find(char_name);
            if (it != character_memories.end()) {
                for (const auto& fragment : it->second.getFragments()) {
                    if (fragment->getTitle().find(memory_theme) != std::string::npos ||
                        fragment->getContent().find(memory_theme) != std::string::npos) {
                        related_fragments.push_back(fragment.get());
                    }
                }
            }
        }

        if (related_fragments.size() >= 2) {
            // Enhance all related fragments
            for (auto* fragment : related_fragments) {
                fragment->enhanceClarity(0.4);
                fragment->amplifyEmotion(0.3);
            }

            // Create connections between fragments
            for (size_t i = 0; i < related_fragments.size(); ++i) {
                for (size_t j = i + 1; j < related_fragments.size(); ++j) {
                    related_fragments[i]->addConnection(related_fragments[j]->getId());
                    related_fragments[j]->addConnection(related_fragments[i]->getId());
                }
            }

            std::cout << "Collaborative reconstruction successful!" << std::endl;
            std::cout << related_fragments.size() << " fragments enhanced and connected!" << std::endl;

            total_fragments_reconstructed += related_fragments.size();
            collective_memory_strength += 0.2;
            updateGlobalMemoryState();
        } else {
            std::cout << "Not enough related fragments found for reconstruction." << std::endl;
        }
    }

    std::vector<std::string> getAvailableAbilities(const std::string& character_class) {
        std::vector<std::string> available;

        for (const auto& pair : ability_library) {
            const MemoryAbility& ability = pair.second;
            if (ability.getCharacterRestriction().empty() ||
                ability.getCharacterRestriction() == character_class) {
                available.push_back(pair.first);
            }
        }

        return available;
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

    MemoryCollection* getCharacterMemories(const std::string& character_name) {
        auto it = character_memories.find(character_name);
        return (it != character_memories.end()) ? &it->second : nullptr;
    }
};

#endif // MEMORYSYSTEM_H