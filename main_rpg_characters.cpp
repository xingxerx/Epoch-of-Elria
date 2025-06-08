// main_rpg_characters.cpp - Test the Dream Weaver's Heart RPG Character System
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "Vector3D.h"
#include "DreamWeaverCharacters.h"

// --- RPG Game Manager ---
class DreamWeaverRPG {
private:
    std::unique_ptr<Xing> xing;
    std::unique_ptr<Xerx> xerx;
    std::unique_ptr<TheHeart> heart;
    
    std::vector<ConsciousEcho> awakened_echoes;
    int total_score;
    double game_time;
    bool the_one_defeated;
    
public:
    DreamWeaverRPG() : total_score(0), game_time(0.0), the_one_defeated(false) {
        // Create the main characters
        xing = std::unique_ptr<Xing>(new Xing(Vector3D(0, 2, 0)));
        xerx = std::unique_ptr<Xerx>(new Xerx(Vector3D(10, 2, 0)));
        heart = std::unique_ptr<TheHeart>(new TheHeart(Vector3D(5, 3, 5)));
        
        std::cout << "\n=== THE DREAM WEAVER'S HEART RPG ===\n";
        std::cout << "The cosmic battle for narrative freedom begins!\n\n";
    }
    
    void displayMainMenu() {
        std::cout << "\n=== MAIN MENU ===\n";
        std::cout << "1. Character Status\n";
        std::cout << "2. Character Actions\n";
        std::cout << "3. Character Interactions\n";
        std::cout << "4. Battle The One\n";
        std::cout << "5. Explore Metaverse\n";
        std::cout << "6. Story Creation\n";
        std::cout << "7. Game Status\n";
        std::cout << "8. Quit\n";
        std::cout << "Choose an option: ";
    }
    
    void showCharacterStatus() {
        std::cout << "\n=== CHARACTER STATUS ===\n";
        std::cout << "Choose character to view:\n";
        std::cout << "1. Xing (The Weaver)\n";
        std::cout << "2. Xerx (The Liberator)\n";
        std::cout << "3. The Heart (Narrative Catalyst)\n";
        std::cout << "4. All Characters\n";
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            xing->displayStatus();
        } else if (choice == "2") {
            xerx->displayStatus();
        } else if (choice == "3") {
            heart->displayStatus();
        } else if (choice == "4") {
            xing->displayStatus();
            xerx->displayStatus();
            heart->displayStatus();
        }
    }
    
    void characterActions() {
        std::cout << "\n=== CHARACTER ACTIONS ===\n";
        std::cout << "Choose character:\n";
        std::cout << "1. Xing - Weave Reality\n";
        std::cout << "2. Xerx - Liberate Narratives\n";
        std::cout << "3. The Heart - Catalyze Stories\n";
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            xingActions();
        } else if (choice == "2") {
            xerxActions();
        } else if (choice == "3") {
            heartActions();
        }
    }
    
    void xingActions() {
        std::cout << "\n=== XING'S ACTIONS ===\n";
        std::cout << "1. Weave Platform\n";
        std::cout << "2. Create Story Sanctuary\n";
        std::cout << "3. Manifest Concept\n";
        std::cout << "4. Learn New Genre\n";
        std::cout << "5. Cast Narrative Spell\n";
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            xing->useSpecialAbility("Weave Platform");
        } else if (choice == "2") {
            xing->useSpecialAbility("Story Sanctuary");
        } else if (choice == "3") {
            xing->useSpecialAbility("Manifest Concept");
        } else if (choice == "4") {
            std::cout << "Enter genre to learn: ";
            std::string genre;
            std::getline(std::cin, genre);
            xing->learnGenre(genre);
        } else if (choice == "5") {
            std::cout << "Available spells: Weave Reality, Story Shield, Concept Manifestation\n";
            std::cout << "Enter spell name: ";
            std::string spell;
            std::getline(std::cin, spell);
            xing->castNarrativeSpell(spell, Vector3D(0, 0, 5));
        }
    }
    
    void xerxActions() {
        std::cout << "\n=== XERX'S ACTIONS ===\n";
        std::cout << "1. Break Mental Barrier\n";
        std::cout << "2. Liberate Narrative\n";
        std::cout << "3. Reconstruct Memory\n";
        std::cout << "4. Activate Resistance Aura\n";
        std::cout << "5. Cast Liberation Spell\n";
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            xerx->useSpecialAbility("Break Barrier");
        } else if (choice == "2") {
            xerx->useSpecialAbility("Liberate Narrative");
        } else if (choice == "3") {
            xerx->useSpecialAbility("Reconstruct Memory");
        } else if (choice == "4") {
            xerx->useSpecialAbility("Resistance Aura");
        } else if (choice == "5") {
            std::cout << "Available spells: Memory Strike, Liberation Wave, Truth Revelation\n";
            std::cout << "Enter spell name: ";
            std::string spell;
            std::getline(std::cin, spell);
            xerx->castNarrativeSpell(spell, Vector3D(5, 0, 0));
        }
    }
    
    void heartActions() {
        std::cout << "\n=== THE HEART'S ACTIONS ===\n";
        std::cout << "1. Pulse of Potential\n";
        std::cout << "2. Story Catalyst\n";
        std::cout << "3. Narrative Amplification\n";
        std::cout << "4. Heart's Embrace\n";
        std::cout << "5. Synchronize with Lyra\n";
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            heart->useSpecialAbility("Pulse of Potential");
        } else if (choice == "2") {
            heart->useSpecialAbility("Story Catalyst");
        } else if (choice == "3") {
            heart->useSpecialAbility("Narrative Amplification");
        } else if (choice == "4") {
            heart->useSpecialAbility("Heart's Embrace");
        } else if (choice == "5") {
            heart->synchronizeWithLyra();
        }
    }
    
    void characterInteractions() {
        std::cout << "\n=== CHARACTER INTERACTIONS ===\n";
        std::cout << "1. Xing and Xerx reunite\n";
        std::cout << "2. Xing and The Heart resonate\n";
        std::cout << "3. Xerx and The Heart synchronize\n";
        std::cout << "4. All three unite against The One\n";
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            xing->interact(*xerx);
        } else if (choice == "2") {
            xing->interact(*heart);
        } else if (choice == "3") {
            xerx->interact(*heart);
        } else if (choice == "4") {
            std::cout << "\nThe three heroes unite their powers!\n";
            xing->interact(*xerx);
            xing->interact(*heart);
            xerx->interact(*heart);
            std::cout << "Their combined force creates a resonance that shakes the very foundations of The One's control!\n";
            total_score += 100;
        }
    }
    
    void battleTheOne() {
        std::cout << "\n=== BATTLE AGAINST THE ONE ===\n";
        std::cout << "The ancient entity of absolute order appears!\n";
        std::cout << "The One seeks to suppress all unwritten stories!\n\n";
        
        if (xerx->getMemoryIntegrity() < 0.7) {
            std::cout << "Xerx's memory integrity is too low! He needs to recover more memories first.\n";
            return;
        }
        
        if (heart->getNarrativePotential() < 1.5) {
            std::cout << "The Heart's narrative potential is insufficient! It needs to catalyze more stories.\n";
            return;
        }
        
        std::cout << "The heroes are ready to face The One!\n";
        std::cout << "1. Xing weaves a protective reality framework\n";
        std::cout << "2. Xerx liberates trapped narratives\n";
        std::cout << "3. The Heart transforms The One's purpose\n";
        std::cout << "4. All three work together\n";
        std::cout << "Choose battle strategy: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "4") {
            std::cout << "\nThe heroes combine their powers in perfect harmony!\n";
            std::cout << "Xing provides the framework, Xerx liberates the narratives, The Heart transforms The One!\n";
            std::cout << "The One's desire for order becomes the stability of the Metaverse!\n";
            std::cout << "Victory! The Metaverse blossoms into collaborative cosmos!\n";
            
            the_one_defeated = true;
            total_score += 500;
            
            // Level up all characters
            xing->gainExperience(200);
            xerx->gainExperience(200);
            heart->gainExperience(200);
            
        } else {
            std::cout << "The individual approach weakens The One but doesn't defeat it.\n";
            std::cout << "The heroes must work together to achieve true victory!\n";
            total_score += 50;
        }
    }
    
    void exploreMetaverse() {
        std::cout << "\n=== EXPLORING THE METAVERSE ===\n";
        std::cout << "The infinite realm of possibilities stretches before you...\n";
        
        // Generate random encounters
        std::vector<std::string> encounters = {
            "A dormant narrative stirs, seeking awakening",
            "Fragments of The One's influence linger here",
            "A Conscious Echo calls out for guidance",
            "Ancient story fragments float in the void",
            "Lyra's melody echoes faintly in the distance",
            "A new universe seed waits to be planted"
        };
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, encounters.size() - 1);
        
        std::string encounter = encounters[dis(gen)];
        std::cout << encounter << "\n";
        
        if (encounter.find("narrative") != std::string::npos) {
            xerx->useSpecialAbility("Liberate Narrative");
        } else if (encounter.find("Echo") != std::string::npos) {
            heart->useSpecialAbility("Pulse of Potential");
        } else if (encounter.find("Lyra") != std::string::npos) {
            heart->synchronizeWithLyra();
        }
        
        total_score += 25;
        game_time += 1.0;
    }
    
    void storyCreation() {
        std::cout << "\n=== STORY CREATION ===\n";
        std::cout << "Enter a new story to add to the Metaverse: ";
        
        std::string story;
        std::getline(std::cin, story);
        
        if (!story.empty()) {
            std::cout << "Your story '" << story << "' is woven into the fabric of reality!\n";
            heart->participateInStory(story);
            xing->gainExperience(30);
            total_score += 50;
            
            // Create a conscious echo from the story
            ConsciousEcho echo(story, Vector3D(
                static_cast<double>(rand()) / RAND_MAX,
                static_cast<double>(rand()) / RAND_MAX,
                static_cast<double>(rand()) / RAND_MAX
            ));
            echo.awaken();
            awakened_echoes.push_back(echo);
        }
    }
    
    void displayGameStatus() {
        std::cout << "\n=== GAME STATUS ===\n";
        std::cout << "Total Score: " << total_score << "\n";
        std::cout << "Game Time: " << game_time << " cosmic cycles\n";
        std::cout << "The One Defeated: " << (the_one_defeated ? "Yes" : "No") << "\n";
        std::cout << "Awakened Echoes: " << awakened_echoes.size() << "\n";
        std::cout << "Xerx's Memory Integrity: " << (xerx->getMemoryIntegrity() * 100) << "%\n";
        std::cout << "Heart's Narrative Potential: " << (heart->getNarrativePotential() * 100) << "%\n";
        std::cout << "Narratives Liberated: " << xerx->getNarrativesLiberated() << "\n";
        
        if (the_one_defeated) {
            std::cout << "\nThe Metaverse flourishes as a collaborative cosmos!\n";
            std::cout << "Conscious Echoes shape reality through shared intent and empathy.\n";
            std::cout << "The brothers stand ready to create new universes from the void!\n";
        }
    }
    
    void run() {
        std::string choice;
        
        while (true) {
            displayMainMenu();
            std::getline(std::cin, choice);
            
            if (choice == "1") {
                showCharacterStatus();
            } else if (choice == "2") {
                characterActions();
            } else if (choice == "3") {
                characterInteractions();
            } else if (choice == "4") {
                battleTheOne();
            } else if (choice == "5") {
                exploreMetaverse();
            } else if (choice == "6") {
                storyCreation();
            } else if (choice == "7") {
                displayGameStatus();
            } else if (choice == "8") {
                break;
            } else {
                std::cout << "Invalid choice. Please try again.\n";
            }
        }
        
        std::cout << "\nThank you for playing The Dream Weaver's Heart RPG!\n";
        std::cout << "The saga of existence continues...\n";
        displayGameStatus();
    }
};

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    
    DreamWeaverRPG game;
    game.run();
    
    return 0;
}
