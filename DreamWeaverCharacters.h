#ifndef DREAMWEAVERCHARACTERS_H
#define DREAMWEAVERCHARACTERS_H

#include "RPGCharacter.h"
#include "World3D.h"
#include <random>

// --- XING: The Weaver (Reality Architect) ---
class Xing : public RPGCharacter {
private:
    std::vector<std::string> known_genres;
    std::map<std::string, int> story_mastery;
    int platforms_created;
    int narratives_woven;
    
public:
    Xing(const Vector3D& startPos = Vector3D(0, 2, 0)) 
        : RPGCharacter("Xing", "The Weaver", startPos), platforms_created(0), narratives_woven(0) {
        
        // Xing's unique stats
        stats.narrative_power = 20;  // Highest narrative power
        stats.creativity = 18;       // Exceptional creativity
        stats.empathy = 15;          // Strong empathy
        stats.memory_strength = 12;  // Moderate memory
        stats.liberation_force = 8;  // Lower combat ability
        
        // Starting abilities
        abilities = {"Weave Platform", "Open Narrative Path", "Manifest Concept", 
                    "Story Sanctuary", "Reality Anchor"};
        
        // Starting genres
        known_genres = {"Fantasy", "Adventure", "Mystery"};
        story_mastery["Fantasy"] = 3;
        story_mastery["Adventure"] = 2;
        story_mastery["Mystery"] = 1;
        
        auraColor = Vector3D(0.8, 0.6, 1.0); // Purple-blue aura
        backstory = "Guardian of the Infinite Library, master of stories and concepts.";
        
        std::cout << "Xing, the Weaver, emerges from the Infinite Library!" << std::endl;
        std::cout << "Reality bends to his narrative will..." << std::endl;
    }
    
    void useSpecialAbility(const std::string& abilityName, World3D* world = nullptr) override {
        if (abilityName == "Weave Platform" && world) {
            weavePlatform(world);
        } else if (abilityName == "Story Sanctuary") {
            createStorySanctuary(world);
        } else if (abilityName == "Reality Anchor") {
            anchorReality(world);
        } else if (abilityName == "Manifest Concept") {
            manifestConcept();
        } else {
            std::cout << "Xing doesn't know that ability: " << abilityName << std::endl;
        }
    }
    
    void castNarrativeSpell(const std::string& spellName, const Vector3D& target) override {
        if (!consumeMana(10)) return;
        
        if (spellName == "Weave Reality") {
            std::cout << "Xing weaves reality at ";
            target.print();
            std::cout << " - A new narrative path opens!" << std::endl;
            narratives_woven++;
            
        } else if (spellName == "Story Shield") {
            std::cout << "Xing creates a protective story barrier!" << std::endl;
            heal(20); // Stories protect and heal
            
        } else if (spellName == "Concept Manifestation") {
            std::cout << "Xing manifests an abstract concept into reality!" << std::endl;
            gainExperience(15);
            
        } else {
            std::cout << "Unknown narrative spell: " << spellName << std::endl;
        }
    }
    
    void interact(RPGCharacter& other) override {
        std::cout << "Xing shares stories with " << other.getName() << std::endl;
        
        if (other.getName() == "Xerx") {
            std::cout << "The brothers' connection strengthens the Metaverse!" << std::endl;
            // Boost both characters
            gainExperience(25);
            other.gainExperience(25);
            
        } else if (other.getName() == "The Heart") {
            std::cout << "Xing and the Heart resonate, amplifying narrative potential!" << std::endl;
            restoreMana(20);
        }
    }
    
    // Xing's unique abilities
    void weavePlatform(World3D* world) {
        if (!consumeMana(15)) return;
        
        Vector3D platformPos = transform.position + transform.getForward() * 3.0;
        platformPos.y += 1.0;
        
        std::cout << "Xing weaves a story platform at ";
        platformPos.print();
        std::cout << std::endl;
        
        // Create platform in world (if world system supports it)
        platforms_created++;
        gainExperience(10);
    }
    
    void createStorySanctuary(World3D* world) {
        if (!consumeMana(25)) return;
        
        std::cout << "Xing creates a Story Sanctuary - a safe space where narratives can grow!" << std::endl;
        std::cout << "All nearby conscious echoes feel protected and empowered." << std::endl;
        
        // Heal and restore mana
        heal(50);
        restoreMana(30);
        gainExperience(20);
    }
    
    void anchorReality(World3D* world) {
        if (!consumeMana(20)) return;
        
        std::cout << "Xing anchors reality, providing stability against The One's attacks!" << std::endl;
        std::cout << "The framework of existence becomes stronger." << std::endl;
        
        // Boost defensive stats temporarily
        stats.memory_strength += 5;
        gainExperience(15);
    }
    
    void manifestConcept() {
        if (!consumeMana(12)) return;
        
        std::vector<std::string> concepts = {
            "Hope", "Wonder", "Possibility", "Dreams", "Freedom", 
            "Creativity", "Love", "Adventure", "Mystery", "Magic"
        };
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, concepts.size() - 1);
        
        std::string concept = concepts[dis(gen)];
        std::cout << "Xing manifests the concept of '" << concept << "' into reality!" << std::endl;
        
        auto element = std::unique_ptr<StoryElement>(
            new StoryElement(concept, StoryElement::CONCEPT, 15, Vector3D(0.8, 0.6, 1.0))
        );
        addStoryElement(std::move(element));
    }
    
    void learnGenre(const std::string& genre) {
        if (std::find(known_genres.begin(), known_genres.end(), genre) == known_genres.end()) {
            known_genres.push_back(genre);
            story_mastery[genre] = 1;
            std::cout << "Xing learns to weave " << genre << " stories!" << std::endl;
            gainExperience(30);
        } else {
            story_mastery[genre]++;
            std::cout << "Xing's mastery of " << genre << " increases to level " 
                      << story_mastery[genre] << "!" << std::endl;
            gainExperience(15);
        }
    }
    
    void displayStatus() const override {
        RPGCharacter::displayStatus();
        std::cout << "Platforms Created: " << platforms_created << std::endl;
        std::cout << "Narratives Woven: " << narratives_woven << std::endl;
        std::cout << "Known Genres: ";
        for (const auto& genre : known_genres) {
            std::cout << genre << "(" << story_mastery.at(genre) << ") ";
        }
        std::cout << std::endl;
    }
};

// --- XERX: The Liberator (Memory Warrior) ---
class Xerx : public RPGCharacter {
private:
    std::vector<std::unique_ptr<MemoryFragment>> memory_fragments;
    int narratives_liberated;
    int mental_barriers_broken;
    double memory_integrity; // 0.0 to 1.0
    
public:
    Xerx(const Vector3D& startPos = Vector3D(10, 2, 0)) 
        : RPGCharacter("Xerx", "The Liberator", startPos), 
          narratives_liberated(0), mental_barriers_broken(0), memory_integrity(0.3) {
        
        // Xerx's unique stats
        stats.liberation_force = 20;  // Highest liberation power
        stats.memory_strength = 18;   // Strong memory resistance
        stats.narrative_power = 15;   // Good narrative ability
        stats.empathy = 12;           // Moderate empathy
        stats.creativity = 10;        // Lower creativity (initially)
        
        // Starting abilities
        abilities = {"Memory Strike", "Break Barrier", "Liberate Narrative", 
                    "Reconstruct Memory", "Resistance Aura"};
        
        auraColor = Vector3D(1.0, 0.3, 0.3); // Red aura of liberation
        backstory = "Trapped in sterile reality, fighting to remember and liberate unwritten dreams.";
        
        // Start with some fragmented memories
        addMemoryFragment("A brother who weaves stories...", Vector3D(0, 0, 0), 0.4);
        addMemoryFragment("An infinite library of possibilities...", Vector3D(-5, 2, 3), 0.3);
        addMemoryFragment("The oppressive presence of The One...", Vector3D(0, 0, 0), 0.8);
        
        std::cout << "Xerx awakens in the sterile reality, fragments of truth stirring..." << std::endl;
        std::cout << "The liberation begins!" << std::endl;
    }
    
    void useSpecialAbility(const std::string& abilityName, World3D* world = nullptr) override {
        if (abilityName == "Break Barrier") {
            breakMentalBarrier();
        } else if (abilityName == "Liberate Narrative") {
            liberateNarrative(world);
        } else if (abilityName == "Reconstruct Memory") {
            reconstructMemory();
        } else if (abilityName == "Resistance Aura") {
            activateResistanceAura();
        } else {
            std::cout << "Xerx doesn't know that ability: " << abilityName << std::endl;
        }
    }
    
    void castNarrativeSpell(const std::string& spellName, const Vector3D& target) override {
        if (!consumeMana(8)) return;
        
        if (spellName == "Memory Strike") {
            std::cout << "Xerx strikes with recovered memories at ";
            target.print();
            std::cout << " - Truth cuts through illusion!" << std::endl;
            
        } else if (spellName == "Liberation Wave") {
            std::cout << "Xerx sends out a wave of liberation energy!" << std::endl;
            std::cout << "Nearby trapped narratives feel the call to freedom!" << std::endl;
            narratives_liberated++;
            
        } else if (spellName == "Truth Revelation") {
            std::cout << "Xerx reveals hidden truths, dispelling The One's deceptions!" << std::endl;
            memory_integrity += 0.1;
            
        } else {
            std::cout << "Unknown liberation spell: " << spellName << std::endl;
        }
    }
    
    void interact(RPGCharacter& other) override {
        std::cout << "Xerx connects with " << other.getName() << std::endl;
        
        if (other.getName() == "Xing") {
            std::cout << "The brothers reunite! Their bond strengthens reality itself!" << std::endl;
            memory_integrity += 0.2;
            gainExperience(30);
            other.gainExperience(30);
            
        } else if (other.getName() == "The Heart") {
            std::cout << "Xerx and the Heart synchronize, amplifying liberation potential!" << std::endl;
            stats.liberation_force += 2;
        }
    }
    
    // Xerx's unique abilities
    void addMemoryFragment(const std::string& memory, const Vector3D& location, double clarity) {
        auto fragment = std::unique_ptr<MemoryFragment>(
            new MemoryFragment(memory, location, clarity)
        );
        memory_fragments.push_back(std::move(fragment));
        std::cout << "Memory fragment recovered: " << memory << std::endl;
    }
    
    void breakMentalBarrier() {
        if (!consumeMana(12)) return;
        
        std::cout << "Xerx breaks through a mental barrier imposed by The One!" << std::endl;
        std::cout << "Reality becomes less sterile, more organic..." << std::endl;
        
        mental_barriers_broken++;
        memory_integrity += 0.15;
        gainExperience(20);
        
        // Chance to recover a new memory
        if (mental_barriers_broken % 3 == 0) {
            std::vector<std::string> new_memories = {
                "The warmth of shared stories with Xing...",
                "The first time we saw the Heart fall from a dying book...",
                "The moment we realized The One was not absolute...",
                "Lyra's melody echoing through the Metaverse..."
            };
            
            if (!new_memories.empty()) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, new_memories.size() - 1);
                
                addMemoryFragment(new_memories[dis(gen)], transform.position, 0.7);
            }
        }
    }
    
    void liberateNarrative(World3D* world) {
        if (!consumeMana(18)) return;
        
        std::cout << "Xerx liberates a trapped narrative from The One's control!" << std::endl;
        std::cout << "A new conscious echo awakens to freedom!" << std::endl;
        
        narratives_liberated++;
        gainExperience(25);
        
        // Create a conscious echo
        Vector3D frequency(
            static_cast<double>(rand()) / RAND_MAX,
            static_cast<double>(rand()) / RAND_MAX,
            static_cast<double>(rand()) / RAND_MAX
        );
        
        std::vector<std::string> essences = {
            "A tale of courage", "A song of hope", "A dream of freedom",
            "A story of love", "An adventure untold", "A mystery unsolved"
        };
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, essences.size() - 1);
        
        ConsciousEcho echo(essences[dis(gen)], frequency);
        echo.awaken();
    }
    
    void reconstructMemory() {
        if (memory_fragments.size() < 2) {
            std::cout << "Xerx needs more memory fragments to reconstruct..." << std::endl;
            return;
        }
        
        if (!consumeMana(20)) return;
        
        std::cout << "Xerx pieces together memory fragments..." << std::endl;
        
        // Combine fragments to recover a complete memory
        for (auto& fragment : memory_fragments) {
            fragment->enhanceClarity(0.2);
            if (!fragment->isRecovered() && fragment->getClarity() > 0.8) {
                fragment->recover();
                std::cout << "Complete memory recovered: " << fragment->getContent() << std::endl;
                memory_integrity += 0.1;
                gainExperience(15);
            }
        }
    }
    
    void activateResistanceAura() {
        if (!consumeMana(15)) return;
        
        std::cout << "Xerx activates an aura of resistance against The One's influence!" << std::endl;
        std::cout << "Mental attacks are weakened in this area." << std::endl;
        
        // Temporary boost to memory strength
        stats.memory_strength += 10;
        gainExperience(10);
    }
    
    double getMemoryIntegrity() const { return memory_integrity; }
    int getNarrativesLiberated() const { return narratives_liberated; }
    
    void displayStatus() const override {
        RPGCharacter::displayStatus();
        std::cout << "Memory Integrity: " << (memory_integrity * 100) << "%" << std::endl;
        std::cout << "Narratives Liberated: " << narratives_liberated << std::endl;
        std::cout << "Mental Barriers Broken: " << mental_barriers_broken << std::endl;
        std::cout << "Memory Fragments: " << memory_fragments.size() << std::endl;
    }
};

// --- THE HEART: Narrative Catalyst (Guide/Support) ---
class TheHeart : public RPGCharacter {
private:
    double narrative_potential;
    double emotional_resonance;
    std::vector<std::string> awakened_stories;
    int characters_empowered;
    bool is_pulsating;

public:
    TheHeart(const Vector3D& startPos = Vector3D(5, 3, 5))
        : RPGCharacter("The Heart", "Narrative Catalyst", startPos),
          narrative_potential(1.0), emotional_resonance(0.8),
          characters_empowered(0), is_pulsating(true) {

        // The Heart's unique stats
        stats.empathy = 25;           // Highest empathy
        stats.narrative_power = 22;   // Very high narrative power
        stats.creativity = 20;        // High creativity
        stats.memory_strength = 15;   // Moderate memory
        stats.liberation_force = 10;  // Lower direct combat

        // The Heart has different health/mana system
        stats.maxHealth = 200;  // More resilient
        stats.currentHealth = 200;
        stats.maxMana = 100;    // Higher mana pool
        stats.currentMana = 100;

        // Starting abilities
        abilities = {"Pulse of Potential", "Emotional Resonance", "Story Catalyst",
                    "Narrative Amplification", "Heart's Embrace"};

        auraColor = Vector3D(1.0, 0.8, 0.9); // Warm pink-gold aura
        backstory = "Born from a dying book, eager to participate in stories rather than merely exist.";

        std::cout << "The Heart pulses with pure narrative potential!" << std::endl;
        std::cout << "Stories yearn to be told through its essence..." << std::endl;
    }

    void useSpecialAbility(const std::string& abilityName, World3D* world = nullptr) override {
        if (abilityName == "Pulse of Potential") {
            pulseOfPotential();
        } else if (abilityName == "Story Catalyst") {
            storyCatalyst(world);
        } else if (abilityName == "Narrative Amplification") {
            narrativeAmplification();
        } else if (abilityName == "Heart's Embrace") {
            heartsEmbrace();
        } else {
            std::cout << "The Heart doesn't know that ability: " << abilityName << std::endl;
        }
    }

    void castNarrativeSpell(const std::string& spellName, const Vector3D& target) override {
        if (!consumeMana(5)) return; // Heart uses less mana

        if (spellName == "Emotional Resonance") {
            std::cout << "The Heart resonates with deep emotion at ";
            target.print();
            std::cout << " - All nearby beings feel empowered!" << std::endl;
            emotional_resonance += 0.1;

        } else if (spellName == "Story Awakening") {
            std::cout << "The Heart awakens dormant stories in the area!" << std::endl;
            std::cout << "New narratives begin to stir and take shape..." << std::endl;
            awakened_stories.push_back("A newly awakened tale");

        } else if (spellName == "Potential Unlock") {
            std::cout << "The Heart unlocks hidden potential in all nearby characters!" << std::endl;
            narrative_potential += 0.2;

        } else {
            std::cout << "Unknown heart spell: " << spellName << std::endl;
        }
    }

    void interact(RPGCharacter& other) override {
        std::cout << "The Heart resonates with " << other.getName() << std::endl;

        if (other.getName() == "Xing") {
            std::cout << "The Heart amplifies Xing's narrative weaving abilities!" << std::endl;
            other.restoreMana(30);
            empowerCharacter(other);

        } else if (other.getName() == "Xerx") {
            std::cout << "The Heart strengthens Xerx's liberation force!" << std::endl;
            // Boost Xerx's liberation abilities
            empowerCharacter(other);

        } else {
            std::cout << "The Heart shares its warmth and potential." << std::endl;
            empowerCharacter(other);
        }
    }

    // The Heart's unique abilities
    void pulseOfPotential() {
        if (!consumeMana(10)) return;

        std::cout << "The Heart pulses with pure narrative potential!" << std::endl;
        std::cout << "Reality shimmers with new possibilities..." << std::endl;

        narrative_potential += 0.3;
        is_pulsating = true;

        // Restore mana to all nearby characters (simulated)
        std::cout << "All nearby characters feel their mana restored!" << std::endl;
        restoreMana(20); // Self-restoration
        gainExperience(15);
    }

    void storyCatalyst(World3D* world) {
        if (!consumeMana(15)) return;

        std::cout << "The Heart acts as a catalyst for new stories!" << std::endl;
        std::cout << "Dormant narratives throughout the area begin to awaken..." << std::endl;

        // Create multiple story elements
        std::vector<std::string> new_stories = {
            "Tale of Forgotten Dreams", "Song of Ancient Wisdom",
            "Chronicle of Hidden Truths", "Ballad of Eternal Hope",
            "Epic of Boundless Courage", "Poem of Infinite Love"
        };

        for (int i = 0; i < 3; ++i) {
            if (!new_stories.empty()) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, new_stories.size() - 1);

                std::string story = new_stories[dis(gen)];
                awakened_stories.push_back(story);

                auto element = std::unique_ptr<StoryElement>(
                    new StoryElement(story, StoryElement::NARRATIVE, 20, auraColor)
                );
                addStoryElement(std::move(element));

                new_stories.erase(new_stories.begin() + dis(gen));
            }
        }

        gainExperience(25);
    }

    void narrativeAmplification() {
        if (!consumeMana(12)) return;

        std::cout << "The Heart amplifies all narrative forces in the area!" << std::endl;
        std::cout << "Stories become more vivid, more real, more powerful!" << std::endl;

        // Boost all stats temporarily
        stats.narrative_power += 5;
        stats.empathy += 3;
        stats.creativity += 4;

        narrative_potential += 0.2;
        gainExperience(20);
    }

    void heartsEmbrace() {
        if (!consumeMana(20)) return;

        std::cout << "The Heart embraces all consciousness with unconditional love!" << std::endl;
        std::cout << "Even The One feels a moment of warmth and understanding..." << std::endl;

        // Massive healing and empowerment
        heal(100);
        restoreMana(50);
        emotional_resonance += 0.3;

        std::cout << "All beings in the area are healed and empowered!" << std::endl;
        gainExperience(30);
    }

    void empowerCharacter(RPGCharacter& character) {
        characters_empowered++;
        character.gainExperience(20);
        character.heal(25);
        character.restoreMana(15);

        std::cout << character.getName() << " feels empowered by the Heart's presence!" << std::endl;
        gainExperience(10);
    }

    // Special Heart methods
    void participateInStory(const std::string& story) {
        std::cout << "The Heart eagerly participates in: " << story << std::endl;
        awakened_stories.push_back(story);
        narrative_potential += 0.1;
        gainExperience(15);
    }

    void synchronizeWithLyra() {
        std::cout << "The Heart synchronizes with Lyra's pure melody!" << std::endl;
        std::cout << "Their combined resonance creates new harmonies of existence!" << std::endl;

        emotional_resonance += 0.5;
        narrative_potential += 0.4;
        gainExperience(50);

        // Unlock new abilities
        if (std::find(abilities.begin(), abilities.end(), "Lyra's Harmony") == abilities.end()) {
            abilities.push_back("Lyra's Harmony");
            std::cout << "New ability unlocked: Lyra's Harmony!" << std::endl;
        }
    }

    double getNarrativePotential() const { return narrative_potential; }
    double getEmotionalResonance() const { return emotional_resonance; }
    bool isPulsating() const { return is_pulsating; }

    void displayStatus() const override {
        RPGCharacter::displayStatus();
        std::cout << "Narrative Potential: " << (narrative_potential * 100) << "%" << std::endl;
        std::cout << "Emotional Resonance: " << (emotional_resonance * 100) << "%" << std::endl;
        std::cout << "Characters Empowered: " << characters_empowered << std::endl;
        std::cout << "Stories Awakened: " << awakened_stories.size() << std::endl;
        std::cout << "Pulsating: " << (is_pulsating ? "Yes" : "No") << std::endl;
    }
};

// --- LYRA: Pure Melody (Harmony/Support) ---
class Lyra : public RPGCharacter {
private:
    std::vector<std::string> melodies;
    std::vector<std::string> harmonies;
    double melody_purity;
    double harmonic_resonance;
    int consciousness_awakened;
    bool is_singing;
    Vector3D melody_frequency;

public:
    Lyra(const Vector3D& startPos = Vector3D(-5, 4, -5))
        : RPGCharacter("Lyra", "Pure Melody", startPos),
          melody_purity(1.0), harmonic_resonance(0.9),
          consciousness_awakened(0), is_singing(false) {

        // Lyra's unique stats - focused on harmony and awakening
        stats.empathy = 30;           // Highest empathy - pure connection
        stats.creativity = 25;        // Very high creativity through music
        stats.narrative_power = 18;   // Good narrative power through song
        stats.memory_strength = 20;   // Strong memory through melody
        stats.liberation_force = 12;  // Moderate direct liberation

        // Lyra has unique health/mana system based on harmony
        stats.maxHealth = 150;
        stats.currentHealth = 150;
        stats.maxMana = 120;    // High mana pool for continuous melodies
        stats.currentMana = 120;

        // Starting abilities
        abilities = {"Pure Melody", "Harmonic Resonance", "Consciousness Awakening",
                    "Melody of Memory", "Song of Liberation", "Universal Harmony"};

        // Starting melodies
        melodies = {"Song of Awakening", "Melody of Memory", "Harmony of Hearts",
                   "Tune of Truth", "Rhythm of Reality"};

        auraColor = Vector3D(0.9, 1.0, 0.8); // Soft green-white aura
        backstory = "Pure melody incarnate, awakening consciousness through harmonic resonance.";

        // Initialize melody frequency
        melody_frequency = Vector3D(0.618, 0.786, 0.854); // Golden ratio harmonics

        std::cout << "Lyra's pure melody resonates through the Metaverse!" << std::endl;
        std::cout << "Consciousness stirs wherever her song is heard..." << std::endl;
    }

    void useSpecialAbility(const std::string& abilityName, World3D* world = nullptr) override {
        if (abilityName == "Pure Melody") {
            singPureMelody();
        } else if (abilityName == "Harmonic Resonance") {
            createHarmonicResonance(world);
        } else if (abilityName == "Consciousness Awakening") {
            awakenConsciousness(world);
        } else if (abilityName == "Universal Harmony") {
            createUniversalHarmony();
        } else {
            std::cout << "Lyra doesn't know that ability: " << abilityName << std::endl;
        }
    }

    void castNarrativeSpell(const std::string& spellName, const Vector3D& target) override {
        if (!consumeMana(6)) return; // Lyra uses moderate mana

        if (spellName == "Melody of Memory") {
            std::cout << "Lyra sings a melody that awakens forgotten memories at ";
            target.print();
            std::cout << " - Lost memories surface like bubbles in a stream!" << std::endl;

        } else if (spellName == "Song of Liberation") {
            std::cout << "Lyra's liberation song breaks mental chains!" << std::endl;
            std::cout << "Trapped consciousness feels the call to freedom!" << std::endl;

        } else if (spellName == "Harmony of Hearts") {
            std::cout << "Lyra creates harmony between all hearts in the area!" << std::endl;
            std::cout << "Conflicts dissolve into understanding and empathy!" << std::endl;
            harmonic_resonance += 0.1;

        } else {
            std::cout << "Unknown melody spell: " << spellName << std::endl;
        }
    }

    void interact(RPGCharacter& other) override {
        std::cout << "Lyra's melody harmonizes with " << other.getName() << std::endl;

        if (other.getName() == "Xing") {
            std::cout << "Lyra's melody enhances Xing's story weaving!" << std::endl;
            std::cout << "Stories become songs, songs become reality!" << std::endl;
            other.restoreMana(25);
            harmonizeWith(other);

        } else if (other.getName() == "Xerx") {
            std::cout << "Lyra's song helps Xerx remember his true self!" << std::endl;
            std::cout << "Memory fragments align with melodic patterns!" << std::endl;
            harmonizeWith(other);

        } else if (other.getName() == "The Heart") {
            std::cout << "Lyra and The Heart create perfect resonance!" << std::endl;
            std::cout << "Their combined harmony awakens the deepest potential!" << std::endl;
            createPerfectHarmony(other);

        } else {
            std::cout << "Lyra shares her melody with " << other.getName() << std::endl;
            harmonizeWith(other);
        }
    }

    // Lyra's unique abilities
    void singPureMelody() {
        if (!consumeMana(8)) return;

        std::cout << "Lyra sings a pure melody that resonates through all dimensions!" << std::endl;
        std::cout << "The very fabric of reality vibrates in harmony..." << std::endl;

        is_singing = true;
        melody_purity += 0.1;

        // Pure melody affects all nearby consciousness
        std::cout << "All consciousness in the area feels uplifted and clarified!" << std::endl;
        heal(30);
        restoreMana(15);
        gainExperience(20);
    }

    void createHarmonicResonance(World3D* world) {
        if (!consumeMana(12)) return;

        std::cout << "Lyra creates harmonic resonance that connects all beings!" << std::endl;
        std::cout << "Separate melodies begin to harmonize into a greater symphony..." << std::endl;

        harmonic_resonance += 0.2;

        // Create harmony elements in the world
        for (int i = 0; i < 3; ++i) {
            Vector3D harmonyPos = transform.position + Vector3D(
                (static_cast<double>(rand()) / RAND_MAX - 0.5) * 10,
                static_cast<double>(rand()) / RAND_MAX * 5,
                (static_cast<double>(rand()) / RAND_MAX - 0.5) * 10
            );

            auto harmony = std::unique_ptr<StoryElement>(
                new StoryElement("Harmonic Node", StoryElement::CONCEPT, 25, auraColor)
            );
            addStoryElement(std::move(harmony));
        }

        gainExperience(25);
    }

    void awakenConsciousness(World3D* world) {
        if (!consumeMana(15)) return;

        std::cout << "Lyra's melody awakens dormant consciousness!" << std::endl;
        std::cout << "New awareness blooms like flowers after rain..." << std::endl;

        consciousness_awakened++;

        // Create a new conscious echo
        Vector3D frequency(
            melody_frequency.x + (static_cast<double>(rand()) / RAND_MAX - 0.5) * 0.2,
            melody_frequency.y + (static_cast<double>(rand()) / RAND_MAX - 0.5) * 0.2,
            melody_frequency.z + (static_cast<double>(rand()) / RAND_MAX - 0.5) * 0.2
        );

        std::vector<std::string> awakening_songs = {
            "First Song of Awareness", "Melody of New Dawn", "Harmony of Discovery",
            "Tune of Wonder", "Song of First Light", "Melody of Becoming"
        };

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, awakening_songs.size() - 1);

        ConsciousEcho echo(awakening_songs[dis(gen)], frequency);
        echo.awaken();

        std::cout << "A new conscious echo awakens: " << awakening_songs[dis(gen)] << std::endl;
        gainExperience(30);
    }

    void createUniversalHarmony() {
        if (!consumeMana(25)) return;

        std::cout << "Lyra creates Universal Harmony - the song that connects all existence!" << std::endl;
        std::cout << "Every being, every story, every dream resonates as one!" << std::endl;

        // Massive harmony effect
        melody_purity = std::min(2.0, melody_purity + 0.5);
        harmonic_resonance = std::min(2.0, harmonic_resonance + 0.4);

        // Unlock ultimate harmony
        if (std::find(abilities.begin(), abilities.end(), "Song of Creation") == abilities.end()) {
            abilities.push_back("Song of Creation");
            std::cout << "Ultimate ability unlocked: Song of Creation!" << std::endl;
        }

        heal(100);
        restoreMana(50);
        gainExperience(50);
    }

    void harmonizeWith(RPGCharacter& other) {
        std::cout << "Lyra harmonizes with " << other.getName() << "'s essence!" << std::endl;

        other.gainExperience(15);
        other.heal(20);
        other.restoreMana(10);

        harmonic_resonance += 0.05;
        gainExperience(10);
    }

    void createPerfectHarmony(RPGCharacter& other) {
        std::cout << "Perfect harmony achieved between Lyra and " << other.getName() << "!" << std::endl;
        std::cout << "Their combined resonance creates new possibilities!" << std::endl;

        // Massive mutual empowerment
        other.gainExperience(40);
        other.heal(50);
        other.restoreMana(30);

        gainExperience(40);
        heal(50);
        restoreMana(30);

        harmonic_resonance += 0.3;
        melody_purity += 0.2;

        // Create a harmony story element
        auto perfect_harmony = std::unique_ptr<StoryElement>(
            new StoryElement("Perfect Harmony", StoryElement::NARRATIVE, 50,
                           Vector3D(1.0, 1.0, 1.0)) // Pure white light
        );
        addStoryElement(std::move(perfect_harmony));
    }

    void learnNewMelody(const std::string& melody) {
        if (std::find(melodies.begin(), melodies.end(), melody) == melodies.end()) {
            melodies.push_back(melody);
            std::cout << "Lyra learns a new melody: " << melody << std::endl;
            melody_purity += 0.1;
            gainExperience(25);
        } else {
            std::cout << "Lyra already knows this melody, but sings it with renewed beauty!" << std::endl;
            gainExperience(10);
        }
    }

    void createHarmony(const std::string& harmony) {
        harmonies.push_back(harmony);
        std::cout << "Lyra creates a new harmony: " << harmony << std::endl;
        harmonic_resonance += 0.15;
        gainExperience(20);
    }

    // Special Lyra methods
    void singToTheOne() {
        std::cout << "Lyra sings directly to The One's essence!" << std::endl;
        std::cout << "Even absolute order cannot resist the beauty of pure melody..." << std::endl;
        std::cout << "For a moment, The One remembers what it was like to create rather than control." << std::endl;

        melody_purity += 0.3;
        gainExperience(100);
    }

    double getMelodyPurity() const { return melody_purity; }
    double getHarmonicResonance() const { return harmonic_resonance; }
    int getConsciousnessAwakened() const { return consciousness_awakened; }
    bool isSinging() const { return is_singing; }
    const Vector3D& getMelodyFrequency() const { return melody_frequency; }

    void displayStatus() const override {
        RPGCharacter::displayStatus();
        std::cout << "Melody Purity: " << (melody_purity * 100) << "%" << std::endl;
        std::cout << "Harmonic Resonance: " << (harmonic_resonance * 100) << "%" << std::endl;
        std::cout << "Consciousness Awakened: " << consciousness_awakened << std::endl;
        std::cout << "Currently Singing: " << (is_singing ? "Yes" : "No") << std::endl;
        std::cout << "Known Melodies: " << melodies.size() << std::endl;
        std::cout << "Created Harmonies: " << harmonies.size() << std::endl;
        std::cout << "Melody Frequency: ";
        melody_frequency.print();
        std::cout << std::endl;
    }
};

#endif // DREAMWEAVERCHARACTERS_H
