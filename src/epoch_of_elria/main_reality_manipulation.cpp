// main_reality_manipulation.cpp - Test the Reality Manipulation System
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "Vector3D.h"
#include "DreamWeaverCharacters.h"
#include "RealityManipulation.h"

// --- Reality Manipulation Demo Manager ---
class RealityManipulationDemo {
private:
    std::unique_ptr<Xing> xing;
    std::unique_ptr<Xerx> xerx;
    std::unique_ptr<TheHeart> heart;
    RealityManipulationSystem reality_system;
    
    Vector3D player_position;
    int total_manipulations;
    double demo_time;
    
public:
    RealityManipulationDemo() : player_position(0, 0, 0), total_manipulations(0), demo_time(0.0) {
        // Create the main characters
        xing = std::unique_ptr<Xing>(new Xing(Vector3D(0, 2, 0)));
        xerx = std::unique_ptr<Xerx>(new Xerx(Vector3D(10, 2, 0)));
        heart = std::unique_ptr<TheHeart>(new TheHeart(Vector3D(5, 3, 5)));
        
        std::cout << "\n🌍 THE DREAM WEAVER'S HEART - REALITY MANIPULATION SYSTEM 🌍\n";
        std::cout << "Where consciousness shapes the very fabric of existence!\n\n";
        
        // Give characters some extra mana for demonstration
        xing->restoreMana(100);
        xerx->restoreMana(100);
        heart->restoreMana(100);
    }
    
    void displayMainMenu() {
        std::cout << "\n=== REALITY MANIPULATION MENU ===\n";
        std::cout << "1. Xing's Reality Weaving\n";
        std::cout << "2. Xerx's Reality Liberation\n";
        std::cout << "3. The Heart's Emotional Reality\n";
        std::cout << "4. Collaborative Reality Shaping\n";
        std::cout << "5. View Reality Status\n";
        std::cout << "6. Explore Reality Fragments\n";
        std::cout << "7. Reality Manipulation Tutorial\n";
        std::cout << "8. Auto-Demo (Watch AI Create Reality)\n";
        std::cout << "9. Quit\n";
        std::cout << "Choose an option: ";
    }
    
    void xingRealityWeaving() {
        std::cout << "\n🔮 XING'S REALITY WEAVING 🔮\n";
        std::cout << "The master of narrative threads shapes existence itself...\n\n";
        
        auto abilities = reality_system.getAvailableAbilities("The Weaver");
        
        std::cout << "Available Reality Weaving Abilities:\n";
        for (size_t i = 0; i < abilities.size(); ++i) {
            std::cout << (i + 1) << ". " << abilities[i] << "\n";
        }
        std::cout << (abilities.size() + 1) << ". Back to main menu\n";
        std::cout << "Choose ability: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        int ability_index = std::stoi(choice) - 1;
        if (ability_index >= 0 && ability_index < static_cast<int>(abilities.size())) {
            Vector3D target_pos = getTargetPosition();
            Vector3D target_size = getTargetSize(abilities[ability_index]);
            
            if (reality_system.executeRealityManipulation(xing.get(), abilities[ability_index], 
                                                         target_pos, target_size)) {
                total_manipulations++;
                xing->gainExperience(25);
                std::cout << "\nXing's narrative power flows through reality!" << std::endl;
            }
        }
    }
    
    void xerxRealityLiberation() {
        std::cout << "\n⚔️ XERX'S REALITY LIBERATION ⚔️\n";
        std::cout << "The liberator breaks the chains that bind existence...\n\n";
        
        auto abilities = reality_system.getAvailableAbilities("The Liberator");
        
        std::cout << "Available Reality Liberation Abilities:\n";
        for (size_t i = 0; i < abilities.size(); ++i) {
            std::cout << (i + 1) << ". " << abilities[i] << "\n";
        }
        std::cout << (abilities.size() + 1) << ". Back to main menu\n";
        std::cout << "Choose ability: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        int ability_index = std::stoi(choice) - 1;
        if (ability_index >= 0 && ability_index < static_cast<int>(abilities.size())) {
            Vector3D target_pos = getTargetPosition();
            Vector3D target_size = getTargetSize(abilities[ability_index]);
            
            if (reality_system.executeRealityManipulation(xerx.get(), abilities[ability_index], 
                                                         target_pos, target_size)) {
                total_manipulations++;
                xerx->gainExperience(25);
                std::cout << "\nXerx's liberation force reshapes the world!" << std::endl;
            }
        }
    }
    
    void heartEmotionalReality() {
        std::cout << "\n💖 THE HEART'S EMOTIONAL REALITY 💖\n";
        std::cout << "Pure emotion transforms the very nature of existence...\n\n";
        
        auto abilities = reality_system.getAvailableAbilities("Narrative Catalyst");
        
        std::cout << "Available Emotional Reality Abilities:\n";
        for (size_t i = 0; i < abilities.size(); ++i) {
            std::cout << (i + 1) << ". " << abilities[i] << "\n";
        }
        std::cout << (abilities.size() + 1) << ". Back to main menu\n";
        std::cout << "Choose ability: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        int ability_index = std::stoi(choice) - 1;
        if (ability_index >= 0 && ability_index < static_cast<int>(abilities.size())) {
            Vector3D target_pos = getTargetPosition();
            Vector3D target_size = getTargetSize(abilities[ability_index]);
            
            if (reality_system.executeRealityManipulation(heart.get(), abilities[ability_index], 
                                                         target_pos, target_size)) {
                total_manipulations++;
                heart->gainExperience(25);
                std::cout << "\nThe Heart's emotional resonance permeates reality!" << std::endl;
            }
        }
    }
    
    void collaborativeRealityShaping() {
        std::cout << "\n🌟 COLLABORATIVE REALITY SHAPING 🌟\n";
        std::cout << "The three heroes combine their powers to reshape existence!\n\n";
        
        std::cout << "Choose collaborative manipulation:\n";
        std::cout << "1. Create Narrative Sanctuary (All three work together)\n";
        std::cout << "2. Open Metaverse Portal (Xing + Heart)\n";
        std::cout << "3. Liberate and Rebuild (Xerx + Xing)\n";
        std::cout << "4. Emotional Memory Palace (Heart + Xerx)\n";
        std::cout << "5. Back to main menu\n";
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        Vector3D center_pos = getTargetPosition();
        
        if (choice == "1") {
            createNarrativeSanctuary(center_pos);
        } else if (choice == "2") {
            openMetaversePortal(center_pos);
        } else if (choice == "3") {
            liberateAndRebuild(center_pos);
        } else if (choice == "4") {
            emotionalMemoryPalace(center_pos);
        }
    }
    
    void createNarrativeSanctuary(const Vector3D& center) {
        std::cout << "\n🏛️ CREATING NARRATIVE SANCTUARY 🏛️\n";
        std::cout << "All three heroes unite their powers!\n\n";
        
        // Xing creates the foundation
        reality_system.executeRealityManipulation(xing.get(), "Anchor Reality", 
                                                 center, Vector3D(10, 2, 10));
        
        // Xerx liberates the space
        reality_system.executeRealityManipulation(xerx.get(), "Liberate Narrative", 
                                                 center + Vector3D(0, 3, 0), Vector3D(8, 4, 8));
        
        // Heart fills it with emotional resonance
        reality_system.executeRealityManipulation(heart.get(), "Emotional Resonance", 
                                                 center + Vector3D(0, 1, 0), Vector3D(12, 6, 12));
        
        std::cout << "\n🌟 NARRATIVE SANCTUARY CREATED! 🌟\n";
        std::cout << "A sacred space where all stories are welcome and protected!\n";
        std::cout << "The combined power of the three heroes creates something greater than the sum of its parts!\n";
        
        total_manipulations += 3;
        xing->gainExperience(50);
        xerx->gainExperience(50);
        heart->gainExperience(50);
    }
    
    void openMetaversePortal(const Vector3D& position) {
        std::cout << "\n🌀 OPENING METAVERSE PORTAL 🌀\n";
        std::cout << "Xing and The Heart create a gateway to infinite possibilities!\n\n";
        
        // Xing opens the portal
        reality_system.executeRealityManipulation(xing.get(), "Open Portal", 
                                                 position, Vector3D(3, 6, 3));
        
        // Heart stabilizes it with emotional energy
        reality_system.executeRealityManipulation(heart.get(), "Transform Space", 
                                                 position, Vector3D(5, 8, 5));
        
        std::cout << "\n🌟 METAVERSE PORTAL OPENED! 🌟\n";
        std::cout << "A shimmering gateway appears, leading to infinite narrative possibilities!\n";
        std::cout << "Through it, you can see glimpses of other stories, other realities!\n";
        
        total_manipulations += 2;
        xing->gainExperience(40);
        heart->gainExperience(40);
    }
    
    void liberateAndRebuild(const Vector3D& position) {
        std::cout << "\n🔨 LIBERATE AND REBUILD 🔨\n";
        std::cout << "Xerx and Xing work together to free and reshape reality!\n\n";
        
        // Xerx breaks down old limitations
        reality_system.executeRealityManipulation(xerx.get(), "Break Barrier", 
                                                 position, Vector3D(6, 4, 6));
        
        // Xerx liberates trapped narratives
        reality_system.executeRealityManipulation(xerx.get(), "Liberate Narrative", 
                                                 position + Vector3D(3, 0, 3), Vector3D(4, 3, 4));
        
        // Xing rebuilds with new possibilities
        reality_system.executeRealityManipulation(xing.get(), "Manifest Concept", 
                                                 position, Vector3D(8, 5, 8));
        
        std::cout << "\n🌟 LIBERATION AND RECONSTRUCTION COMPLETE! 🌟\n";
        std::cout << "The old limitations are shattered and replaced with infinite potential!\n";
        std::cout << "New narratives flow freely through the reconstructed space!\n";
        
        total_manipulations += 3;
        xerx->gainExperience(45);
        xing->gainExperience(35);
    }
    
    void emotionalMemoryPalace(const Vector3D& position) {
        std::cout << "\n🏰 EMOTIONAL MEMORY PALACE 🏰\n";
        std::cout << "The Heart and Xerx create a space where memories and emotions intertwine!\n\n";
        
        // Xerx reconstructs from memories
        reality_system.executeRealityManipulation(xerx.get(), "Memory Reconstruction", 
                                                 position, Vector3D(7, 4, 7));
        
        // Heart infuses with emotional resonance
        reality_system.executeRealityManipulation(heart.get(), "Emotional Resonance", 
                                                 position + Vector3D(0, 2, 0), Vector3D(9, 6, 9));
        
        // Heart materializes collective dreams
        reality_system.executeRealityManipulation(heart.get(), "Dream Materialization", 
                                                 position + Vector3D(0, 4, 0), Vector3D(5, 3, 5));
        
        std::cout << "\n🌟 EMOTIONAL MEMORY PALACE MANIFESTED! 🌟\n";
        std::cout << "A magnificent structure rises, built from recovered memories and emotional truth!\n";
        std::cout << "Within its halls, past and present, memory and dream, become one!\n";
        
        total_manipulations += 3;
        heart->gainExperience(50);
        xerx->gainExperience(40);
    }
    
    Vector3D getTargetPosition() {
        std::cout << "\nEnter target position (x y z) or press Enter for random: ";
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) {
            // Generate random position
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(-20.0, 20.0);
            return Vector3D(dis(gen), dis(gen) * 0.5 + 2.0, dis(gen));
        } else {
            // Parse input (simplified)
            return Vector3D(0, 2, 5); // Default if parsing fails
        }
    }
    
    Vector3D getTargetSize(const std::string& ability_name) {
        if (ability_name.find("Portal") != std::string::npos) {
            return Vector3D(3, 6, 3);
        } else if (ability_name.find("Platform") != std::string::npos) {
            return Vector3D(5, 1, 5);
        } else if (ability_name.find("Concept") != std::string::npos) {
            return Vector3D(4, 4, 4);
        } else if (ability_name.find("Barrier") != std::string::npos) {
            return Vector3D(6, 3, 6);
        } else if (ability_name.find("Dream") != std::string::npos) {
            return Vector3D(8, 5, 8);
        } else {
            return Vector3D(4, 3, 4); // Default size
        }
    }
    
    void exploreRealityFragments() {
        std::cout << "\n🔍 EXPLORING REALITY FRAGMENTS 🔍\n";
        std::cout << "Moving through the manipulated reality...\n\n";
        
        std::cout << "Current position: ";
        player_position.print();
        std::cout << std::endl;
        
        // Check for fragments at current position
        reality_system.checkFragmentAt(player_position);
        
        std::cout << "\nMovement options:\n";
        std::cout << "1. Move North (z+5)\n";
        std::cout << "2. Move South (z-5)\n";
        std::cout << "3. Move East (x+5)\n";
        std::cout << "4. Move West (x-5)\n";
        std::cout << "5. Move Up (y+3)\n";
        std::cout << "6. Move Down (y-3)\n";
        std::cout << "7. Teleport to random location\n";
        std::cout << "8. Back to main menu\n";
        std::cout << "Choose movement: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") player_position.z += 5;
        else if (choice == "2") player_position.z -= 5;
        else if (choice == "3") player_position.x += 5;
        else if (choice == "4") player_position.x -= 5;
        else if (choice == "5") player_position.y += 3;
        else if (choice == "6") player_position.y -= 3;
        else if (choice == "7") {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(-25.0, 25.0);
            player_position = Vector3D(dis(gen), dis(gen) * 0.3 + 2.0, dis(gen));
        }
        
        if (choice != "8") {
            std::cout << "\nMoved to position: ";
            player_position.print();
            std::cout << std::endl;
            
            // Check new position
            reality_system.checkFragmentAt(player_position);
        }
    }
    
    void showRealityTutorial() {
        std::cout << "\n=== REALITY MANIPULATION TUTORIAL ===\n";
        std::cout << "🌍 Welcome to the ultimate power - reshaping existence itself! 🌍\n\n";
        
        std::cout << "REALITY MANIPULATION BASICS:\n";
        std::cout << "• Characters can literally reshape the world through narrative power\n";
        std::cout << "• Each manipulation creates 'Reality Fragments' that persist in the world\n";
        std::cout << "• Fragments have stability - some fade over time, others become permanent\n";
        std::cout << "• Global reality stability affects how well manipulations work\n\n";
        
        std::cout << "CHARACTER SPECIALIZATIONS:\n";
        std::cout << "🔮 XING (The Weaver):\n";
        std::cout << "   - Weave Platform: Creates solid surfaces to walk on\n";
        std::cout << "   - Manifest Concept: Brings abstract ideas into physical reality\n";
        std::cout << "   - Anchor Reality: Creates stable points that resist distortion\n";
        std::cout << "   - Open Portal: Creates passages between different spaces\n\n";
        
        std::cout << "⚔️ XERX (The Liberator):\n";
        std::cout << "   - Break Barrier: Shatters obstacles and limitations\n";
        std::cout << "   - Liberate Narrative: Frees trapped stories to reshape reality\n";
        std::cout << "   - Memory Reconstruction: Rebuilds areas from recovered memories\n\n";
        
        std::cout << "💖 THE HEART (Narrative Catalyst):\n";
        std::cout << "   - Emotional Resonance: Infuses areas with emotional energy\n";
        std::cout << "   - Dream Materialization: Makes collective dreams physically real\n";
        std::cout << "   - Transform Space: Fundamentally changes the nature of reality\n\n";
        
        std::cout << "COLLABORATIVE POWER:\n";
        std::cout << "• Characters can work together for more powerful effects\n";
        std::cout << "• Combined manipulations create unique, stronger reality fragments\n";
        std::cout << "• The three heroes together can reshape entire regions\n\n";
        
        std::cout << "ADVANCED CONCEPTS:\n";
        std::cout << "• Narrative Saturation: High levels unlock new possibilities\n";
        std::cout << "• Fragment Resonance: Similar fragments strengthen each other\n";
        std::cout << "• Reality Anchors: Permanent stable points in chaotic areas\n";
        std::cout << "• Metaverse Portals: Gateways to infinite narrative possibilities\n\n";
    }
    
    void autoDemo() {
        std::cout << "\n🤖 AUTO-DEMO: WATCH AI CREATE REALITY 🤖\n";
        std::cout << "Sit back and watch as the AI demonstrates reality manipulation!\n\n";
        
        std::vector<std::string> demo_sequence = {
            "Xing creates a foundation with Anchor Reality",
            "Xerx breaks through limitations with Break Barrier", 
            "Heart infuses the space with Emotional Resonance",
            "Xing manifests a concept into reality",
            "Xerx liberates trapped narratives",
            "Heart materializes collective dreams",
            "All three create a Narrative Sanctuary together"
        };
        
        for (size_t i = 0; i < demo_sequence.size(); ++i) {
            std::cout << "\n--- Step " << (i + 1) << ": " << demo_sequence[i] << " ---\n";
            
            Vector3D demo_pos(i * 5 - 15, 2, i * 3 - 9);
            
            if (i == 0) {
                reality_system.executeRealityManipulation(xing.get(), "Anchor Reality", demo_pos);
            } else if (i == 1) {
                reality_system.executeRealityManipulation(xerx.get(), "Break Barrier", demo_pos);
            } else if (i == 2) {
                reality_system.executeRealityManipulation(heart.get(), "Emotional Resonance", demo_pos);
            } else if (i == 3) {
                reality_system.executeRealityManipulation(xing.get(), "Manifest Concept", demo_pos);
            } else if (i == 4) {
                reality_system.executeRealityManipulation(xerx.get(), "Liberate Narrative", demo_pos);
            } else if (i == 5) {
                reality_system.executeRealityManipulation(heart.get(), "Dream Materialization", demo_pos);
            } else if (i == 6) {
                createNarrativeSanctuary(Vector3D(0, 5, 0));
            }
            
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }
        
        std::cout << "\n🌟 AUTO-DEMO COMPLETE! 🌟\n";
        std::cout << "Reality has been thoroughly reshaped by the power of narrative!\n";
        reality_system.displayRealityStatus();
    }
    
    void update(double deltaTime) {
        demo_time += deltaTime;
        reality_system.update(deltaTime);
    }
    
    void displayGameStatus() {
        std::cout << "\n=== DEMO STATUS ===\n";
        std::cout << "Total Reality Manipulations: " << total_manipulations << "\n";
        std::cout << "Demo Time: " << (int)demo_time << " seconds\n";
        std::cout << "Player Position: ";
        player_position.print();
        std::cout << "\n";
        
        xing->displayStatus();
        xerx->displayStatus();
        heart->displayStatus();
        
        reality_system.displayRealityStatus();
    }
    
    void run() {
        std::string choice;
        
        while (true) {
            displayMainMenu();
            std::getline(std::cin, choice);
            
            if (choice == "1") {
                xingRealityWeaving();
            } else if (choice == "2") {
                xerxRealityLiberation();
            } else if (choice == "3") {
                heartEmotionalReality();
            } else if (choice == "4") {
                collaborativeRealityShaping();
            } else if (choice == "5") {
                reality_system.displayRealityStatus();
            } else if (choice == "6") {
                exploreRealityFragments();
            } else if (choice == "7") {
                showRealityTutorial();
            } else if (choice == "8") {
                autoDemo();
            } else if (choice == "9") {
                break;
            } else {
                std::cout << "Invalid choice. Please try again.\n";
            }
            
            // Update reality system
            update(1.0);
        }
        
        std::cout << "\nThank you for experiencing Reality Manipulation!\n";
        std::cout << "The power to reshape existence through narrative will always be yours! 🌟\n";
        displayGameStatus();
    }
};

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    
    RealityManipulationDemo demo;
    demo.run();
    
    return 0;
}
