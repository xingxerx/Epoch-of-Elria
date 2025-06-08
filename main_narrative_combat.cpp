// main_narrative_combat.cpp - Test the Narrative Combat System
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "Vector3D.h"
#include "DreamWeaverCharacters.h"
#include "NarrativeCombat.h"

// --- Combat Demo Manager ---
class NarrativeCombatDemo {
private:
    std::unique_ptr<Xing> xing;
    std::unique_ptr<Xerx> xerx;
    std::unique_ptr<TheHeart> heart;
    NarrativeCombatSystem combat_system;
    
public:
    NarrativeCombatDemo() {
        // Create the main characters
        xing = std::unique_ptr<Xing>(new Xing(Vector3D(0, 2, 0)));
        xerx = std::unique_ptr<Xerx>(new Xerx(Vector3D(10, 2, 0)));
        heart = std::unique_ptr<TheHeart>(new TheHeart(Vector3D(5, 3, 5)));
        
        // Add to combat system
        combat_system.addPartyMember(xing.get());
        combat_system.addPartyMember(xerx.get());
        combat_system.addPartyMember(heart.get());
        
        std::cout << "\n🌟 THE DREAM WEAVER'S HEART - NARRATIVE COMBAT SYSTEM 🌟\n";
        std::cout << "Where battles are fought with stories, not swords!\n\n";
    }
    
    void displayMainMenu() {
        std::cout << "\n=== NARRATIVE COMBAT MENU ===\n";
        std::cout << "1. Tutorial Combat (vs Narrative Suppressor)\n";
        std::cout << "2. Minor Battle (vs Multiple Enemies)\n";
        std::cout << "3. Epic Battle (vs The One)\n";
        std::cout << "4. Custom Combat Setup\n";
        std::cout << "5. View Character Status\n";
        std::cout << "6. View Combat Weapons\n";
        std::cout << "7. Combat Tutorial\n";
        std::cout << "8. Quit\n";
        std::cout << "Choose an option: ";
    }
    
    void tutorialCombat() {
        std::cout << "\n🎓 TUTORIAL: NARRATIVE COMBAT BASICS 🎓\n";
        std::cout << "You will face a Narrative Suppressor - a manifestation of creative blockage.\n";
        std::cout << "Learn to use story weapons to defeat enemies through narrative power!\n\n";
        
        // Reset characters for tutorial
        resetCharacters();
        
        // Create tutorial enemy
        NarrativeEnemy suppressor("Narrative Suppressor", 60, 10, 15);
        suppressor.setDescription("A dark force that tries to silence creative expression.");
        suppressor.addAttackPattern("The Suppressor whispers doubts about your stories!");
        suppressor.addAttackPattern("Waves of criticism try to break your confidence!");
        combat_system.addEnemy(suppressor);
        
        runCombat();
    }
    
    void minorBattle() {
        std::cout << "\n⚔️ MINOR BATTLE: MULTIPLE NARRATIVE THREATS ⚔️\n";
        std::cout << "Face multiple enemies that threaten the freedom of stories!\n\n";
        
        resetCharacters();
        combat_system.createMinorEnemies();
        runCombat();
    }
    
    void epicBattle() {
        std::cout << "\n🌟 EPIC BATTLE: THE ONE - ABSOLUTE ORDER 🌟\n";
        std::cout << "The ultimate confrontation with the ancient entity of singular control!\n";
        std::cout << "Only by working together can the heroes hope to transform The One's purpose!\n\n";
        
        // Check if characters are ready
        if (xerx->getMemoryIntegrity() < 0.7) {
            std::cout << "⚠️ WARNING: Xerx's memory integrity is too low for this battle!\n";
            std::cout << "Consider having him recover more memories first.\n";
            std::cout << "Proceeding anyway for demo purposes...\n\n";
        }
        
        if (heart->getNarrativePotential() < 1.5) {
            std::cout << "⚠️ WARNING: The Heart's narrative potential may be insufficient!\n";
            std::cout << "Consider having it catalyze more stories first.\n";
            std::cout << "Proceeding anyway for demo purposes...\n\n";
        }
        
        resetCharacters();
        // Boost characters for epic battle
        xing->gainExperience(100);
        xerx->gainExperience(100);
        heart->gainExperience(100);
        
        combat_system.createTheOneBattle();
        runCombat();
    }
    
    void resetCharacters() {
        // Reset health and mana
        xing->heal(1000);
        xing->restoreMana(1000);
        xerx->heal(1000);
        xerx->restoreMana(1000);
        heart->heal(1000);
        heart->restoreMana(1000);
        
        // Clear previous enemies
        // Note: In a full implementation, we'd have a method to clear enemies
    }
    
    void runCombat() {
        if (!combat_system.startCombat()) {
            std::cout << "Failed to start combat!\n";
            return;
        }
        
        while (combat_system.isCombatActive()) {
            // Player turn
            playerTurn();
            
            if (!combat_system.isCombatActive()) break;
            
            // Enemy turn
            std::cout << "\n--- ENEMY TURN ---\n";
            combat_system.enemyTurn();
            
            // Check combat end
            if (combat_system.checkCombatEnd()) break;
            
            // Next turn
            combat_system.nextTurn();
            combat_system.displayCombatStatus();
            
            // Pause for readability
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore();
            std::cin.get();
        }
        
        std::cout << "\nCombat ended! Returning to main menu...\n";
    }
    
    void playerTurn() {
        std::cout << "\n--- PLAYER TURN ---\n";
        
        for (int i = 0; i < 3; ++i) { // Each character gets an action
            RPGCharacter* current_char = nullptr;
            std::string char_name;
            
            switch (i) {
                case 0: current_char = xing.get(); char_name = "Xing"; break;
                case 1: current_char = xerx.get(); char_name = "Xerx"; break;
                case 2: current_char = heart.get(); char_name = "The Heart"; break;
            }
            
            if (current_char->getStats().currentHealth <= 0) {
                std::cout << char_name << " is unconscious and cannot act!\n";
                continue;
            }
            
            std::cout << "\n" << char_name << "'s turn:\n";
            std::cout << "1. Attack with Story Weapon\n";
            std::cout << "2. Use Special Ability\n";
            std::cout << "3. Defend (restore mana)\n";
            std::cout << "4. Skip turn\n";
            std::cout << "Choice: ";
            
            std::string choice;
            std::getline(std::cin, choice);
            
            if (choice == "1") {
                performAttack(current_char);
            } else if (choice == "2") {
                useSpecialAbility(current_char);
            } else if (choice == "3") {
                std::cout << char_name << " focuses on defense and narrative meditation.\n";
                current_char->restoreMana(20);
                current_char->heal(10);
            } else {
                std::cout << char_name << " skips their turn.\n";
            }
        }
    }
    
    void performAttack(RPGCharacter* character) {
        // Get available weapons for this character
        auto weapons = combat_system.getAvailableWeapons(character->getCharacterClass());
        
        if (weapons.empty()) {
            std::cout << "No weapons available for " << character->getName() << "!\n";
            return;
        }
        
        std::cout << "\nAvailable Story Weapons:\n";
        for (size_t i = 0; i < weapons.size(); ++i) {
            std::cout << (i + 1) << ". " << weapons[i] << "\n";
        }
        std::cout << "Choose weapon (1-" << weapons.size() << "): ";
        
        std::string weapon_choice;
        std::getline(std::cin, weapon_choice);
        
        int weapon_index = std::stoi(weapon_choice) - 1;
        if (weapon_index < 0 || weapon_index >= static_cast<int>(weapons.size())) {
            std::cout << "Invalid weapon choice!\n";
            return;
        }
        
        // Get available targets
        auto targets = combat_system.getAliveEnemyNames();
        if (targets.empty()) {
            std::cout << "No enemies to target!\n";
            return;
        }
        
        std::cout << "\nAvailable Targets:\n";
        for (size_t i = 0; i < targets.size(); ++i) {
            std::cout << (i + 1) << ". " << targets[i] << "\n";
        }
        std::cout << "Choose target (1-" << targets.size() << "): ";
        
        std::string target_choice;
        std::getline(std::cin, target_choice);
        
        int target_index = std::stoi(target_choice) - 1;
        if (target_index < 0 || target_index >= static_cast<int>(targets.size())) {
            std::cout << "Invalid target choice!\n";
            return;
        }
        
        // Perform the attack
        combat_system.performNarrativeAttack(character, weapons[weapon_index], targets[target_index]);
    }
    
    void useSpecialAbility(RPGCharacter* character) {
        std::cout << "\n" << character->getName() << " uses a special ability!\n";
        
        if (character->getName() == "Xing") {
            std::cout << "1. Weave Platform\n2. Story Sanctuary\n3. Reality Anchor\n";
            std::cout << "Choose ability: ";
            std::string choice;
            std::getline(std::cin, choice);
            
            if (choice == "1") character->useSpecialAbility("Weave Platform");
            else if (choice == "2") character->useSpecialAbility("Story Sanctuary");
            else if (choice == "3") character->useSpecialAbility("Reality Anchor");
            
        } else if (character->getName() == "Xerx") {
            std::cout << "1. Break Barrier\n2. Liberate Narrative\n3. Reconstruct Memory\n";
            std::cout << "Choose ability: ";
            std::string choice;
            std::getline(std::cin, choice);
            
            if (choice == "1") character->useSpecialAbility("Break Barrier");
            else if (choice == "2") character->useSpecialAbility("Liberate Narrative");
            else if (choice == "3") character->useSpecialAbility("Reconstruct Memory");
            
        } else if (character->getName() == "The Heart") {
            std::cout << "1. Pulse of Potential\n2. Story Catalyst\n3. Heart's Embrace\n";
            std::cout << "Choose ability: ";
            std::string choice;
            std::getline(std::cin, choice);
            
            if (choice == "1") character->useSpecialAbility("Pulse of Potential");
            else if (choice == "2") character->useSpecialAbility("Story Catalyst");
            else if (choice == "3") character->useSpecialAbility("Heart's Embrace");
        }
    }
    
    void viewCharacterStatus() {
        std::cout << "\n=== PARTY STATUS ===\n";
        xing->displayStatus();
        xerx->displayStatus();
        heart->displayStatus();
    }
    
    void viewCombatWeapons() {
        std::cout << "\n=== STORY WEAPONS LIBRARY ===\n";
        
        std::cout << "\n--- XING'S WEAPONS ---\n";
        auto xing_weapons = combat_system.getAvailableWeapons("The Weaver");
        for (const auto& weapon : xing_weapons) {
            std::cout << "• " << weapon << "\n";
        }
        
        std::cout << "\n--- XERX'S WEAPONS ---\n";
        auto xerx_weapons = combat_system.getAvailableWeapons("The Liberator");
        for (const auto& weapon : xerx_weapons) {
            std::cout << "• " << weapon << "\n";
        }
        
        std::cout << "\n--- THE HEART'S WEAPONS ---\n";
        auto heart_weapons = combat_system.getAvailableWeapons("Narrative Catalyst");
        for (const auto& weapon : heart_weapons) {
            std::cout << "• " << weapon << "\n";
        }
    }
    
    void showCombatTutorial() {
        std::cout << "\n=== NARRATIVE COMBAT TUTORIAL ===\n";
        std::cout << "🌟 Welcome to revolutionary story-based combat! 🌟\n\n";
        
        std::cout << "COMBAT BASICS:\n";
        std::cout << "• Battles are fought with stories, concepts, and narrative power\n";
        std::cout << "• Each character has unique story weapons based on their role\n";
        std::cout << "• Mana represents narrative energy - use it wisely!\n";
        std::cout << "• Special abilities can turn the tide of battle\n\n";
        
        std::cout << "CHARACTER ROLES:\n";
        std::cout << "🔮 XING (The Weaver):\n";
        std::cout << "   - Reality manipulation and protective abilities\n";
        std::cout << "   - High narrative power and creativity\n";
        std::cout << "   - Weapons: Reality Weave, Concept Storm\n\n";
        
        std::cout << "⚔️ XERX (The Liberator):\n";
        std::cout << "   - Direct combat and barrier-breaking\n";
        std::cout << "   - High liberation force and memory strength\n";
        std::cout << "   - Weapons: Memory Blade, Liberation Wave\n\n";
        
        std::cout << "💖 THE HEART (Narrative Catalyst):\n";
        std::cout << "   - Support and emotional resonance\n";
        std::cout << "   - Highest empathy and healing abilities\n";
        std::cout << "   - Weapons: Empathy Pulse, Dream Manifestation\n\n";
        
        std::cout << "STRATEGY TIPS:\n";
        std::cout << "• Use Xing to create defensive structures\n";
        std::cout << "• Use Xerx to break enemy defenses\n";
        std::cout << "• Use The Heart to heal and empower the party\n";
        std::cout << "• Combine abilities for devastating effects!\n\n";
        
        std::cout << "SPECIAL MECHANICS:\n";
        std::cout << "• Story weapons have unique special effects\n";
        std::cout << "• Character interactions create powerful synergies\n";
        std::cout << "• Defeating enemies may awaken new narratives\n";
        std::cout << "• The ultimate goal is transformation, not destruction\n\n";
    }
    
    void run() {
        std::string choice;
        
        while (true) {
            displayMainMenu();
            std::getline(std::cin, choice);
            
            if (choice == "1") {
                tutorialCombat();
            } else if (choice == "2") {
                minorBattle();
            } else if (choice == "3") {
                epicBattle();
            } else if (choice == "4") {
                std::cout << "Custom combat setup not yet implemented.\n";
            } else if (choice == "5") {
                viewCharacterStatus();
            } else if (choice == "6") {
                viewCombatWeapons();
            } else if (choice == "7") {
                showCombatTutorial();
            } else if (choice == "8") {
                break;
            } else {
                std::cout << "Invalid choice. Please try again.\n";
            }
        }
        
        std::cout << "\nThank you for experiencing Narrative Combat!\n";
        std::cout << "The power of stories will always triumph over oppression! 🌟\n";
    }
};

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    
    NarrativeCombatDemo demo;
    demo.run();
    
    return 0;
}
