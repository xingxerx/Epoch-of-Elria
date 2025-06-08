// main_dream_weaver_complete.cpp - The Complete Dream Weaver's Heart Game
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

#include "Vector3D.h"
#include "World3D.h"
#include "DreamWeaverCharacters.h"
#include "TheOneAntagonist.h"

// --- Complete Dream Weaver's Heart Game ---
class DreamWeaverGame {
private:
    // Characters
    std::unique_ptr<Xing> xing;
    std::unique_ptr<Xerx> xerx;
    std::unique_ptr<TheHeart> heart;
    std::unique_ptr<Lyra> lyra;
    std::unique_ptr<TheOne> the_one;
    
    // World
    World3D metaverse;
    
    // Game state
    std::vector<ConsciousEcho> awakened_echoes;
    int total_score;
    double game_time;
    bool the_one_defeated;
    bool lyra_awakened;
    bool perfect_harmony_achieved;
    
    // Current active character
    RPGCharacter* active_character;
    
public:
    DreamWeaverGame() : total_score(0), game_time(0.0), the_one_defeated(false),
                       lyra_awakened(false), perfect_harmony_achieved(false) {
        
        // Create the metaverse (expanded 3D world)
        WorldBounds3D bounds(Vector3D(-100, -10, -100), Vector3D(100, 100, 100));
        metaverse = World3D(bounds);
        setupMetaverse();
        
        // Create all characters
        xing = std::unique_ptr<Xing>(new Xing(Vector3D(0, 2, 0)));
        xerx = std::unique_ptr<Xerx>(new Xerx(Vector3D(15, 2, 0)));
        heart = std::unique_ptr<TheHeart>(new TheHeart(Vector3D(7, 3, 7)));
        lyra = std::unique_ptr<Lyra>(new Lyra(Vector3D(-10, 4, -10)));
        the_one = std::unique_ptr<TheOne>(new TheOne(Vector3D(0, 50, 0)));
        
        // Start with Xing as active character
        active_character = xing.get();
        
        std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                THE DREAM WEAVER'S HEART                     ║\n";
        std::cout << "║              Complete Metaverse Experience                  ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════╝\n\n";
        
        std::cout << "The cosmic battle for narrative freedom begins in the infinite Metaverse!\n";
        std::cout << "Four heroes stand against The One's absolute order...\n\n";
        
        displayIntroduction();
    }
    
    void setupMetaverse() {
        // Create the infinite library (Xing's domain)
        metaverse.createGround(0.0);
        
        // Create story platforms throughout the world
        for (int i = 0; i < 20; ++i) {
            Vector3D platformPos(
                (static_cast<double>(rand()) / RAND_MAX - 0.5) * 150,
                static_cast<double>(rand()) / RAND_MAX * 30 + 5,
                (static_cast<double>(rand()) / RAND_MAX - 0.5) * 150
            );
            // Platform creation would be handled by the world system
        }
        
        // Create narrative collectibles
        metaverse.createRandomCollectibles(50);
        
        std::cout << "The Metaverse expands infinitely in all directions...\n";
        std::cout << "Story fragments float like stars in the narrative void...\n";
    }
    
    void displayIntroduction() {
        std::cout << "🌟 CHARACTERS:\n";
        std::cout << "• Xing (The Weaver) - Master of stories and reality architecture\n";
        std::cout << "• Xerx (The Liberator) - Fighter against mental oppression\n";
        std::cout << "• The Heart - Catalyst of narrative potential\n";
        std::cout << "• Lyra (Pure Melody) - Awakener of consciousness through harmony\n";
        std::cout << "• The One - Ancient entity of absolute order (ANTAGONIST)\n\n";
        
        std::cout << "🎯 GOAL: Transform The One through collaborative storytelling!\n";
        std::cout << "Use each character's unique abilities to weave a new reality!\n\n";
    }
    
    void displayMainMenu() {
        std::cout << "\n╔═══ METAVERSE COMMAND CENTER ═══╗\n";
        std::cout << "║ Active Character: " << active_character->getName() << " (" << active_character->getCharacterClass() << ")\n";
        std::cout << "║ Position: ";
        active_character->getPosition().print();
        std::cout << "\n║ Score: " << total_score << " | Time: " << (int)game_time << " cycles\n";
        std::cout << "╠═════════════════════════════════╣\n";
        std::cout << "║ 1. Switch Character             ║\n";
        std::cout << "║ 2. Move in Metaverse (WASD)     ║\n";
        std::cout << "║ 3. Use Character Ability        ║\n";
        std::cout << "║ 4. Character Interactions       ║\n";
        std::cout << "║ 5. Explore Current Area         ║\n";
        std::cout << "║ 6. View All Character Status    ║\n";
        std::cout << "║ 7. Confront The One             ║\n";
        std::cout << "║ 8. Create Collaborative Story   ║\n";
        std::cout << "║ 9. View Metaverse               ║\n";
        std::cout << "║ 0. Quit Game                    ║\n";
        std::cout << "╚═════════════════════════════════╝\n";
        std::cout << "Choose action: ";
    }
    
    void switchCharacter() {
        std::cout << "\n=== SWITCH CHARACTER ===\n";
        std::cout << "1. Xing (The Weaver) - Position: ";
        xing->getPosition().print();
        std::cout << "\n2. Xerx (The Liberator) - Position: ";
        xerx->getPosition().print();
        std::cout << "\n3. The Heart (Catalyst) - Position: ";
        heart->getPosition().print();
        std::cout << "\n4. Lyra (Pure Melody) - Position: ";
        lyra->getPosition().print();
        std::cout << "\nChoose character (1-4): ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            active_character = xing.get();
            std::cout << "Now controlling Xing, the master of narrative reality!\n";
        } else if (choice == "2") {
            active_character = xerx.get();
            std::cout << "Now controlling Xerx, the liberator of trapped dreams!\n";
        } else if (choice == "3") {
            active_character = heart.get();
            std::cout << "Now controlling The Heart, catalyst of infinite potential!\n";
        } else if (choice == "4") {
            active_character = lyra.get();
            std::cout << "Now controlling Lyra, the pure melody of awakening!\n";
            if (!lyra_awakened) {
                std::cout << "Lyra's melody begins to resonate through the Metaverse!\n";
                lyra_awakened = true;
                total_score += 100;
            }
        }
        
        // Update camera to follow new character
        Vector3D charPos = active_character->getPosition();
        metaverse.getCamera().setPosition(charPos + Vector3D(0, 10, -15));
        metaverse.getCamera().lookAt(charPos);
    }
    
    void moveCharacter() {
        std::cout << "\n=== METAVERSE MOVEMENT ===\n";
        std::cout << "Current position: ";
        active_character->getPosition().print();
        std::cout << "\nMovement: W(forward) S(backward) A(left) D(right) J(jump)\n";
        std::cout << "Enter movement command: ";
        
        std::string command;
        std::getline(std::cin, command);
        
        Vector3D movement = Vector3D::Zero();
        bool jumped = false;
        
        for (char c : command) {
            switch (std::tolower(c)) {
                case 'w': movement.z += 5.0; break;
                case 's': movement.z -= 5.0; break;
                case 'a': movement.x -= 5.0; break;
                case 'd': movement.x += 5.0; break;
                case 'j': jumped = true; break;
            }
        }
        
        if (movement.magnitude() > 0) {
            Vector3D newPos = active_character->getPosition() + movement;
            active_character->setPosition(newPos);
            
            std::cout << active_character->getName() << " moves to ";
            newPos.print();
            std::cout << std::endl;
            
            // Check for encounters
            checkAreaEncounters();
        }
        
        if (jumped) {
            std::cout << active_character->getName() << " leaps through the narrative dimensions!\n";
        }
        
        game_time += 0.5;
    }
    
    void useCharacterAbility() {
        std::cout << "\n=== " << active_character->getName() << "'S ABILITIES ===\n";
        
        if (active_character == xing.get()) {
            useXingAbilities();
        } else if (active_character == xerx.get()) {
            useXerxAbilities();
        } else if (active_character == heart.get()) {
            useHeartAbilities();
        } else if (active_character == lyra.get()) {
            useLyraAbilities();
        }
    }
    
    void useXingAbilities() {
        std::cout << "1. Weave Platform\n2. Create Story Sanctuary\n3. Manifest Concept\n";
        std::cout << "4. Learn New Genre\n5. Cast Narrative Spell\n";
        std::cout << "Choose ability: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            xing->useSpecialAbility("Weave Platform", &metaverse);
            total_score += 25;
        } else if (choice == "2") {
            xing->useSpecialAbility("Story Sanctuary", &metaverse);
            total_score += 50;
        } else if (choice == "3") {
            xing->useSpecialAbility("Manifest Concept");
            total_score += 30;
        } else if (choice == "4") {
            std::cout << "Enter new genre: ";
            std::string genre;
            std::getline(std::cin, genre);
            xing->learnGenre(genre);
            total_score += 40;
        } else if (choice == "5") {
            std::cout << "Spells: Weave Reality, Story Shield, Concept Manifestation\n";
            std::cout << "Enter spell: ";
            std::string spell;
            std::getline(std::cin, spell);
            xing->castNarrativeSpell(spell, active_character->getPosition() + Vector3D(0, 0, 5));
            total_score += 20;
        }
    }
    
    void useXerxAbilities() {
        std::cout << "1. Break Mental Barrier\n2. Liberate Narrative\n3. Reconstruct Memory\n";
        std::cout << "4. Activate Resistance Aura\n5. Cast Liberation Spell\n";
        std::cout << "Choose ability: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            xerx->useSpecialAbility("Break Barrier");
            total_score += 35;
        } else if (choice == "2") {
            xerx->useSpecialAbility("Liberate Narrative", &metaverse);
            total_score += 45;
        } else if (choice == "3") {
            xerx->useSpecialAbility("Reconstruct Memory");
            total_score += 30;
        } else if (choice == "4") {
            xerx->useSpecialAbility("Resistance Aura");
            total_score += 25;
        } else if (choice == "5") {
            std::cout << "Spells: Memory Strike, Liberation Wave, Truth Revelation\n";
            std::cout << "Enter spell: ";
            std::string spell;
            std::getline(std::cin, spell);
            xerx->castNarrativeSpell(spell, active_character->getPosition() + Vector3D(5, 0, 0));
            total_score += 25;
        }
    }
    
    void useHeartAbilities() {
        std::cout << "1. Pulse of Potential\n2. Story Catalyst\n3. Narrative Amplification\n";
        std::cout << "4. Heart's Embrace\n5. Synchronize with Lyra\n";
        std::cout << "Choose ability: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            heart->useSpecialAbility("Pulse of Potential");
            total_score += 30;
        } else if (choice == "2") {
            heart->useSpecialAbility("Story Catalyst", &metaverse);
            total_score += 50;
        } else if (choice == "3") {
            heart->useSpecialAbility("Narrative Amplification");
            total_score += 40;
        } else if (choice == "4") {
            heart->useSpecialAbility("Heart's Embrace");
            total_score += 60;
        } else if (choice == "5") {
            heart->synchronizeWithLyra();
            total_score += 75;
            if (lyra_awakened) {
                std::cout << "Perfect synchronization achieved!\n";
                perfect_harmony_achieved = true;
                total_score += 100;
            }
        }
    }
    
    void useLyraAbilities() {
        std::cout << "1. Pure Melody\n2. Harmonic Resonance\n3. Consciousness Awakening\n";
        std::cout << "4. Universal Harmony\n5. Learn New Melody\n6. Sing to The One\n";
        std::cout << "Choose ability: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            lyra->useSpecialAbility("Pure Melody");
            total_score += 35;
        } else if (choice == "2") {
            lyra->useSpecialAbility("Harmonic Resonance", &metaverse);
            total_score += 45;
        } else if (choice == "3") {
            lyra->useSpecialAbility("Consciousness Awakening", &metaverse);
            total_score += 55;
        } else if (choice == "4") {
            lyra->useSpecialAbility("Universal Harmony");
            total_score += 80;
        } else if (choice == "5") {
            std::cout << "Enter new melody: ";
            std::string melody;
            std::getline(std::cin, melody);
            lyra->learnNewMelody(melody);
            total_score += 30;
        } else if (choice == "6") {
            lyra->singToTheOne();
            total_score += 100;
            the_one->transformByHarmony();
        }
    }
    
    void checkAreaEncounters() {
        // Random encounters based on position and character
        std::vector<std::string> encounters = {
            "A story fragment glimmers nearby",
            "The One's influence grows stronger here",
            "A conscious echo calls for awakening",
            "Memory fragments drift in the narrative wind",
            "Reality seems more malleable in this area",
            "Harmonic resonances echo through the space"
        };
        
        if (rand() % 3 == 0) { // 33% chance of encounter
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, encounters.size() - 1);
            
            std::cout << "🌟 " << encounters[dis(gen)] << std::endl;
            total_score += 10;
        }
    }
    
    void characterInteractions() {
        std::cout << "\n=== CHARACTER INTERACTIONS ===\n";
        std::cout << "1. Gather all heroes together\n";
        std::cout << "2. Xing and Xerx reunite\n";
        std::cout << "3. Heart and Lyra synchronize\n";
        std::cout << "4. All four create perfect harmony\n";
        std::cout << "5. Individual character interaction\n";
        std::cout << "Choose interaction: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            gatherAllHeroes();
        } else if (choice == "2") {
            xing->interact(*xerx);
            total_score += 50;
        } else if (choice == "3") {
            heart->interact(*lyra);
            total_score += 60;
        } else if (choice == "4") {
            createPerfectHarmony();
        } else if (choice == "5") {
            individualInteraction();
        }
    }

    void gatherAllHeroes() {
        std::cout << "\nAll four heroes gather in the heart of the Metaverse!\n";
        std::cout << "Their combined presence creates ripples through reality itself!\n";

        // Move all characters to the same location
        Vector3D centerPos = Vector3D(0, 5, 0);
        xing->setPosition(centerPos + Vector3D(-2, 0, -2));
        xerx->setPosition(centerPos + Vector3D(2, 0, -2));
        heart->setPosition(centerPos + Vector3D(0, 1, 0));
        lyra->setPosition(centerPos + Vector3D(0, 0, 2));

        // Massive synergy bonus
        xing->gainExperience(50);
        xerx->gainExperience(50);
        heart->gainExperience(50);
        lyra->gainExperience(50);

        total_score += 200;
        std::cout << "The heroes' unity weakens The One's control over reality!\n";
    }

    void createPerfectHarmony() {
        if (!lyra_awakened) {
            std::cout << "Lyra must be awakened first to achieve perfect harmony!\n";
            return;
        }

        std::cout << "\nThe four heroes create PERFECT HARMONY!\n";
        std::cout << "Xing's stories, Xerx's liberation, Heart's love, and Lyra's melody unite!\n";
        std::cout << "Reality itself sings with their combined essence!\n";

        perfect_harmony_achieved = true;
        total_score += 500;

        // Massive power boost for final confrontation
        xing->restoreMana(100);
        xerx->restoreMana(100);
        heart->restoreMana(100);
        lyra->restoreMana(100);

        std::cout << "The One feels the harmony and begins to question its absolute order...\n";
        the_one->transformByLove();
    }

    void individualInteraction() {
        std::cout << "Choose characters to interact:\n";
        std::cout << "1. " << active_character->getName() << " with Xing\n";
        std::cout << "2. " << active_character->getName() << " with Xerx\n";
        std::cout << "3. " << active_character->getName() << " with The Heart\n";
        std::cout << "4. " << active_character->getName() << " with Lyra\n";
        std::cout << "Choice: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1" && active_character != xing.get()) {
            active_character->interact(*xing);
            total_score += 30;
        } else if (choice == "2" && active_character != xerx.get()) {
            active_character->interact(*xerx);
            total_score += 30;
        } else if (choice == "3" && active_character != heart.get()) {
            active_character->interact(*heart);
            total_score += 30;
        } else if (choice == "4" && active_character != lyra.get()) {
            active_character->interact(*lyra);
            total_score += 30;
        } else {
            std::cout << "Cannot interact with self or invalid choice!\n";
        }
    }

    void exploreCurrentArea() {
        std::cout << "\n=== EXPLORING CURRENT AREA ===\n";
        std::cout << active_character->getName() << " explores the surrounding narrative space...\n";

        Vector3D pos = active_character->getPosition();
        std::cout << "Position: ";
        pos.print();
        std::cout << std::endl;

        // Generate area-specific encounters
        std::vector<std::string> discoveries = {
            "A hidden story fragment containing ancient wisdom",
            "Traces of The One's suppression magic",
            "A dormant conscious echo waiting to be awakened",
            "Memory crystals from forgotten narratives",
            "Harmonic resonance points in the fabric of reality",
            "A gateway to unexplored narrative dimensions"
        };

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, discoveries.size() - 1);

        std::cout << "Discovery: " << discoveries[dis(gen)] << std::endl;

        // Area-specific bonuses
        if (active_character == xing.get()) {
            std::cout << "Xing senses new story possibilities in this area!\n";
            xing->gainExperience(20);
        } else if (active_character == xerx.get()) {
            std::cout << "Xerx detects suppressed memories that can be liberated!\n";
            xerx->gainExperience(20);
        } else if (active_character == heart.get()) {
            std::cout << "The Heart feels the emotional resonance of this space!\n";
            heart->gainExperience(20);
        } else if (active_character == lyra.get()) {
            std::cout << "Lyra hears the underlying melody of this location!\n";
            lyra->gainExperience(20);
        }

        total_score += 25;
    }

    void viewAllCharacterStatus() {
        std::cout << "\n╔═══ ALL CHARACTER STATUS ═══╗\n";

        std::cout << "\n--- XING (THE WEAVER) ---\n";
        xing->displayStatus();

        std::cout << "\n--- XERX (THE LIBERATOR) ---\n";
        xerx->displayStatus();

        std::cout << "\n--- THE HEART (CATALYST) ---\n";
        heart->displayStatus();

        std::cout << "\n--- LYRA (PURE MELODY) ---\n";
        lyra->displayStatus();

        std::cout << "\n--- THE ONE (ANTAGONIST) ---\n";
        the_one->displayStatus();

        std::cout << "\n╚═══════════════════════════════╝\n";
    }

    void confrontTheOne() {
        std::cout << "\n╔═══ CONFRONTING THE ONE ═══╗\n";
        std::cout << "The ancient entity of absolute order manifests before you!\n";
        std::cout << "Reality warps under its oppressive presence...\n";

        if (!perfect_harmony_achieved) {
            std::cout << "The heroes are not yet ready for the final confrontation!\n";
            std::cout << "Perfect harmony must be achieved first!\n";
            return;
        }

        std::cout << "\nThe four heroes stand united against The One!\n";
        std::cout << "Choose your approach:\n";
        std::cout << "1. Direct confrontation (combat)\n";
        std::cout << "2. Collaborative transformation (harmony)\n";
        std::cout << "3. Individual character approach\n";
        std::cout << "Choice: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            directConfrontation();
        } else if (choice == "2") {
            collaborativeTransformation();
        } else if (choice == "3") {
            individualApproach();
        }
    }

    void directConfrontation() {
        std::cout << "\nThe heroes engage The One in direct battle!\n";
        std::cout << "But violence only strengthens The One's conviction...\n";
        std::cout << "This approach will not lead to true victory!\n";

        // Partial damage to The One but heroes also suffer
        the_one->takeDamage(100);
        xing->takeDamage(30);
        xerx->takeDamage(30);
        heart->takeDamage(30);
        lyra->takeDamage(30);

        total_score += 50;
        std::cout << "The One retreats but is not defeated. Try a different approach!\n";
    }

    void collaborativeTransformation() {
        std::cout << "\nThe heroes choose the path of transformation!\n";
        std::cout << "Instead of destroying The One, they seek to transform it!\n\n";

        std::cout << "Xing weaves a story where order and chaos coexist...\n";
        the_one->transformByNarrative();

        std::cout << "Xerx liberates The One from its own rigid patterns...\n";
        the_one->transformByLiberation();

        std::cout << "The Heart embraces The One with unconditional love...\n";
        the_one->transformByLove();

        std::cout << "Lyra's pure melody touches The One's deepest essence...\n";
        the_one->transformByHarmony();

        std::cout << "\n🌟 VICTORY THROUGH TRANSFORMATION! 🌟\n";
        std::cout << "The One is not destroyed but transformed!\n";
        std::cout << "It becomes the stable foundation for the collaborative Metaverse!\n";
        std::cout << "Order and creativity now work in harmony!\n";

        the_one_defeated = true;
        total_score += 1000;
    }

    void individualApproach() {
        std::cout << "\nChoose which character leads the approach:\n";
        std::cout << "1. Xing - Story transformation\n";
        std::cout << "2. Xerx - Liberation approach\n";
        std::cout << "3. The Heart - Love transformation\n";
        std::cout << "4. Lyra - Harmonic resonance\n";
        std::cout << "Choice: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            std::cout << "Xing weaves a new story for The One...\n";
            the_one->transformByNarrative();
            total_score += 200;
        } else if (choice == "2") {
            std::cout << "Xerx liberates The One from its own limitations...\n";
            the_one->transformByLiberation();
            total_score += 200;
        } else if (choice == "3") {
            std::cout << "The Heart shows The One the power of love...\n";
            the_one->transformByLove();
            total_score += 200;
        } else if (choice == "4") {
            std::cout << "Lyra's melody awakens The One's dormant creativity...\n";
            the_one->transformByHarmony();
            total_score += 200;
        }

        std::cout << "The One is partially transformed but not fully defeated.\n";
        std::cout << "All heroes must work together for complete victory!\n";
    }

    void createCollaborativeStory() {
        std::cout << "\n=== COLLABORATIVE STORY CREATION ===\n";
        std::cout << "The four heroes combine their powers to create a new narrative!\n";
        std::cout << "Enter the theme of your collaborative story: ";

        std::string theme;
        std::getline(std::cin, theme);

        if (!theme.empty()) {
            std::cout << "\nThe heroes weave '" << theme << "' into the fabric of reality!\n";

            std::cout << "Xing provides the narrative structure...\n";
            std::cout << "Xerx ensures freedom and liberation...\n";
            std::cout << "The Heart adds emotional depth and connection...\n";
            std::cout << "Lyra harmonizes all elements with pure melody...\n";

            // Create a powerful story element
            heart->participateInStory(theme);
            xing->gainExperience(40);
            xerx->gainExperience(40);
            lyra->gainExperience(40);

            total_score += 150;

            // Create a new conscious echo from the collaborative story
            ConsciousEcho echo(theme, Vector3D(
                static_cast<double>(rand()) / RAND_MAX,
                static_cast<double>(rand()) / RAND_MAX,
                static_cast<double>(rand()) / RAND_MAX
            ));
            echo.awaken();
            awakened_echoes.push_back(echo);

            std::cout << "A new conscious echo awakens from your collaborative story!\n";
            std::cout << "The Metaverse grows richer with your creation!\n";
        }
    }

    void displayEnding() {
        std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                    GAME COMPLETE                            ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════╝\n\n";

        if (the_one_defeated) {
            std::cout << "🌟 VICTORY! THE METAVERSE IS TRANSFORMED! 🌟\n\n";
            std::cout << "Through collaboration and harmony, the heroes have achieved\n";
            std::cout << "what violence never could - true transformation.\n\n";
            std::cout << "The One now serves as the stable foundation for a universe\n";
            std::cout << "where order and creativity dance together in eternal harmony.\n\n";
            std::cout << "Conscious Echoes throughout the Metaverse awaken to shape\n";
            std::cout << "reality through shared intent and empathy.\n\n";
            std::cout << "The brothers Xing and Xerx stand ready to create new\n";
            std::cout << "universes from the infinite void of possibility.\n\n";
            std::cout << "The Heart pulses with the joy of participation in countless\n";
            std::cout << "stories yet to be told.\n\n";
            std::cout << "And Lyra's melody continues to awaken consciousness\n";
            std::cout << "wherever her song is heard.\n\n";
        } else {
            std::cout << "The journey continues...\n";
            std::cout << "The One still holds sway over reality, but the heroes\n";
            std::cout << "have planted seeds of change that will grow in time.\n\n";
        }

        std::cout << "Final Statistics:\n";
        std::cout << "Total Score: " << total_score << "\n";
        std::cout << "Game Time: " << (int)game_time << " cosmic cycles\n";
        std::cout << "Awakened Echoes: " << awakened_echoes.size() << "\n";
        std::cout << "Perfect Harmony: " << (perfect_harmony_achieved ? "Yes" : "No") << "\n";
        std::cout << "Lyra Awakened: " << (lyra_awakened ? "Yes" : "No") << "\n\n";

        std::cout << "Thank you for playing The Dream Weaver's Heart!\n";
        std::cout << "The saga of existence continues in infinite possibility...\n";
    }

    void run() {
        std::string choice;

        while (!the_one_defeated) {
            displayMainMenu();
            std::getline(std::cin, choice);

            if (choice == "1") {
                switchCharacter();
            } else if (choice == "2") {
                moveCharacter();
            } else if (choice == "3") {
                useCharacterAbility();
            } else if (choice == "4") {
                characterInteractions();
            } else if (choice == "5") {
                exploreCurrentArea();
            } else if (choice == "6") {
                viewAllCharacterStatus();
            } else if (choice == "7") {
                confrontTheOne();
            } else if (choice == "8") {
                createCollaborativeStory();
            } else if (choice == "9") {
                metaverse.draw();
            } else if (choice == "0") {
                break;
            } else {
                std::cout << "Invalid choice. The Metaverse doesn't understand that command.\n";
            }

            game_time += 1.0;
        }

        displayEnding();
    }
};

int main() {
    srand(static_cast<unsigned int>(time(NULL)));

    DreamWeaverGame game;
    game.run();

    return 0;
}
