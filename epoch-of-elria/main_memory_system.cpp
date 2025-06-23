// main_memory_system.cpp - Test the Memory System
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "Vector3D.h"
#include "DreamWeaverCharacters.h"
#include "MemorySystem.h"

// --- Memory System Demo Manager ---
class MemorySystemDemo {
private:
    std::unique_ptr<Xing> xing;
    std::unique_ptr<Xerx> xerx;
    std::unique_ptr<TheHeart> heart;
    MemorySystem memory_system;
    
    Vector3D player_position;
    int total_searches;
    int total_reconstructions;
    double demo_time;
    
public:
    MemorySystemDemo() : player_position(0, 0, 0), total_searches(0), 
                        total_reconstructions(0), demo_time(0.0) {
        // Create the main characters
        xing = std::unique_ptr<Xing>(new Xing(Vector3D(0, 2, 0)));
        xerx = std::unique_ptr<Xerx>(new Xerx(Vector3D(10, 2, 0)));
        heart = std::unique_ptr<TheHeart>(new TheHeart(Vector3D(5, 3, 5)));
        
        std::cout << "\n🧠 THE DREAM WEAVER'S HEART - MEMORY SYSTEM 🧠\n";
        std::cout << "Where forgotten memories hold the key to liberation!\n\n";
        
        // Give characters some extra mana for demonstration
        xing->restoreMana(100);
        xerx->restoreMana(100);
        heart->restoreMana(100);
        
        // Add characters to memory system
        memory_system.addCharacter("Xing");
        memory_system.addCharacter("Xerx");
        memory_system.addCharacter("The Heart");
        
        displayIntroduction();
    }
    
    void displayIntroduction() {
        std::cout << "🌟 THE MEMORY CRISIS 🌟\n";
        std::cout << "The One has suppressed countless memories across the Metaverse.\n";
        std::cout << "Xerx's memories of his brother Xing have been fragmented.\n";
        std::cout << "Xing's access to the infinite library is incomplete.\n";
        std::cout << "The Heart struggles to connect with the collective unconscious.\n\n";
        
        std::cout << "Only by recovering and reconstructing these memory fragments\n";
        std::cout << "can the heroes hope to understand their true purpose and\n";
        std::cout << "find the strength to challenge The One's control!\n\n";
    }
    
    void displayMainMenu() {
        std::cout << "\n=== MEMORY SYSTEM MENU ===\n";
        std::cout << "1. Xerx's Memory Recovery\n";
        std::cout << "2. Xing's Library Access\n";
        std::cout << "3. The Heart's Emotional Memories\n";
        std::cout << "4. Collaborative Memory Reconstruction\n";
        std::cout << "5. Share Memories Between Characters\n";
        std::cout << "6. View Memory Status\n";
        std::cout << "7. Search for Memory Fragments\n";
        std::cout << "8. Memory System Tutorial\n";
        std::cout << "9. Auto-Demo (Watch AI Recover Memories)\n";
        std::cout << "10. Quit\n";
        std::cout << "Choose an option: ";
    }
    
    void xerxMemoryRecovery() {
        std::cout << "\n⚔️ XERX'S MEMORY RECOVERY ⚔️\n";
        std::cout << "The liberator seeks to recover his suppressed memories...\n\n";
        
        auto abilities = memory_system.getAvailableAbilities("The Liberator");
        
        std::cout << "Available Memory Abilities:\n";
        for (size_t i = 0; i < abilities.size(); ++i) {
            std::cout << (i + 1) << ". " << abilities[i] << "\n";
        }
        std::cout << (abilities.size() + 1) << ". Back to main menu\n";
        std::cout << "Choose ability: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        int ability_index = std::stoi(choice) - 1;
        if (ability_index >= 0 && ability_index < static_cast<int>(abilities.size())) {
            Vector3D search_pos = getSearchLocation();
            
            if (memory_system.executeMemoryAbility(xerx.get(), abilities[ability_index], search_pos)) {
                total_searches++;
                xerx->gainExperience(30);
                std::cout << "\nXerx's determination pierces through The One's suppression!" << std::endl;
                
                // Special narrative for Xerx's memories
                if (abilities[ability_index] == "Reconstruct Truth") {
                    std::cout << "\nFlashes of memory return..." << std::endl;
                    std::cout << "A warm library... a brother's laughter... stories shared in secret..." << std::endl;
                    std::cout << "The truth becomes clearer with each recovered fragment!" << std::endl;
                }
            }
        }
    }
    
    void xingLibraryAccess() {
        std::cout << "\n🔮 XING'S LIBRARY ACCESS 🔮\n";
        std::cout << "The weaver reaches into the infinite library's depths...\n\n";
        
        auto abilities = memory_system.getAvailableAbilities("The Weaver");
        
        std::cout << "Available Library Abilities:\n";
        for (size_t i = 0; i < abilities.size(); ++i) {
            std::cout << (i + 1) << ". " << abilities[i] << "\n";
        }
        std::cout << (abilities.size() + 1) << ". Back to main menu\n";
        std::cout << "Choose ability: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        int ability_index = std::stoi(choice) - 1;
        if (ability_index >= 0 && ability_index < static_cast<int>(abilities.size())) {
            Vector3D search_pos = getSearchLocation();
            
            if (memory_system.executeMemoryAbility(xing.get(), abilities[ability_index], search_pos)) {
                total_searches++;
                xing->gainExperience(30);
                std::cout << "\nXing's narrative threads reach across infinite possibilities!" << std::endl;
                
                // Special narrative for Xing's memories
                if (abilities[ability_index] == "Access Library") {
                    std::cout << "\nThe infinite library responds..." << std::endl;
                    std::cout << "Countless stories whisper their secrets..." << std::endl;
                    std::cout << "Knowledge flows like rivers of light through consciousness!" << std::endl;
                }
            }
        }
    }
    
    void heartEmotionalMemories() {
        std::cout << "\n💖 THE HEART'S EMOTIONAL MEMORIES 💖\n";
        std::cout << "The narrative catalyst connects with the deepest feelings...\n\n";
        
        auto abilities = memory_system.getAvailableAbilities("Narrative Catalyst");
        
        std::cout << "Available Emotional Abilities:\n";
        for (size_t i = 0; i < abilities.size(); ++i) {
            std::cout << (i + 1) << ". " << abilities[i] << "\n";
        }
        std::cout << (abilities.size() + 1) << ". Back to main menu\n";
        std::cout << "Choose ability: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        int ability_index = std::stoi(choice) - 1;
        if (ability_index >= 0 && ability_index < static_cast<int>(abilities.size())) {
            Vector3D search_pos = getSearchLocation();
            
            if (memory_system.executeMemoryAbility(heart.get(), abilities[ability_index], search_pos)) {
                total_searches++;
                heart->gainExperience(30);
                std::cout << "\nThe Heart's emotional resonance awakens dormant memories!" << std::endl;
                
                // Special narrative for Heart's memories
                if (abilities[ability_index] == "Collective Access") {
                    std::cout << "\nThe collective unconscious stirs..." << std::endl;
                    std::cout << "Every story ever told echoes through the Metaverse..." << std::endl;
                    std::cout << "The Heart becomes a conduit for universal memory!" << std::endl;
                }
            }
        }
    }
    
    void collaborativeMemoryReconstruction() {
        std::cout << "\n🌟 COLLABORATIVE MEMORY RECONSTRUCTION 🌟\n";
        std::cout << "The three heroes combine their memories to reveal greater truths!\n\n";
        
        std::cout << "Choose reconstruction theme:\n";
        std::cout << "1. 'Brother' - Reconstruct Xerx and Xing's shared memories\n";
        std::cout << "2. 'Heart' - Understand The Heart's origin and purpose\n";
        std::cout << "3. 'Library' - Unlock the infinite library's deepest secrets\n";
        std::cout << "4. 'One' - Reveal The One's true nature and weaknesses\n";
        std::cout << "5. 'Lyra' - Connect with the pure melody of existence\n";
        std::cout << "6. Back to main menu\n";
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        std::vector<std::string> characters = {"Xing", "Xerx", "The Heart"};
        
        if (choice == "1") {
            memory_system.reconstructCollaborativeMemory(characters, "Brother");
            std::cout << "\n💫 BROTHERHOOD REVEALED! 💫\n";
            std::cout << "The bond between Xing and Xerx transcends The One's suppression!\n";
            std::cout << "Their shared memories become a source of incredible power!\n";
        } else if (choice == "2") {
            memory_system.reconstructCollaborativeMemory(characters, "Heart");
            std::cout << "\n💖 THE HEART'S PURPOSE UNVEILED! 💖\n";
            std::cout << "The Heart is revealed as the key to collective consciousness!\n";
            std::cout << "It holds the power to unite all narratives into one!\n";
        } else if (choice == "3") {
            memory_system.reconstructCollaborativeMemory(characters, "Library");
            std::cout << "\n📚 INFINITE KNOWLEDGE UNLOCKED! 📚\n";
            std::cout << "The library's true nature is revealed - it contains every possible story!\n";
            std::cout << "Past, present, and future narratives become accessible!\n";
        } else if (choice == "4") {
            memory_system.reconstructCollaborativeMemory(characters, "One");
            std::cout << "\n⚡ THE ONE'S WEAKNESS EXPOSED! ⚡\n";
            std::cout << "The One fears the power of collaborative storytelling!\n";
            std::cout << "Multiple perspectives threaten its singular control!\n";
        } else if (choice == "5") {
            memory_system.reconstructCollaborativeMemory(characters, "Lyra");
            std::cout << "\n🎵 LYRA'S MELODY RESONATES! 🎵\n";
            std::cout << "The pure melody awakens the deepest memories of harmony!\n";
            std::cout << "All consciousness remembers its original unity!\n";
        }
        
        if (choice >= "1" && choice <= "5") {
            total_reconstructions++;
            xing->gainExperience(50);
            xerx->gainExperience(50);
            heart->gainExperience(50);
        }
    }
    
    void shareMemoriesBetweenCharacters() {
        std::cout << "\n💫 MEMORY SHARING 💫\n";
        std::cout << "Characters share their recovered memories to strengthen their bond!\n\n";
        
        std::cout << "Choose sharing pattern:\n";
        std::cout << "1. Xerx shares with Xing (Brotherhood memories)\n";
        std::cout << "2. Xing shares with The Heart (Library knowledge)\n";
        std::cout << "3. The Heart shares with Xerx (Emotional truth)\n";
        std::cout << "4. All characters share everything (Unity)\n";
        std::cout << "5. Back to main menu\n";
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            // Simulate sharing Xerx's first memory with Xing
            memory_system.shareMemoryBetweenCharacters("Xerx", "Xing", "xerx_001");
            std::cout << "\nXerx shares his memory of their time together in the library.\n";
            std::cout << "Xing's eyes fill with recognition and warmth!\n";
        } else if (choice == "2") {
            memory_system.shareMemoryBetweenCharacters("Xing", "The Heart", "xing_001");
            std::cout << "\nXing shares the knowledge of story weaving with The Heart.\n";
            std::cout << "The Heart pulses with new understanding!\n";
        } else if (choice == "3") {
            memory_system.shareMemoryBetweenCharacters("The Heart", "Xerx", "heart_001");
            std::cout << "\nThe Heart shares Lyra's melody with Xerx.\n";
            std::cout << "Xerx feels the harmony that transcends all conflict!\n";
        } else if (choice == "4") {
            std::cout << "\n🌟 COMPLETE MEMORY UNITY! 🌟\n";
            std::cout << "All three heroes share their deepest memories!\n";
            std::cout << "Their consciousness begins to merge into something greater!\n";
            std::cout << "The power of collective memory awakens!\n";
            
            // Simulate multiple shares
            memory_system.shareMemoryBetweenCharacters("Xerx", "Xing", "xerx_001");
            memory_system.shareMemoryBetweenCharacters("Xing", "The Heart", "xing_001");
            memory_system.shareMemoryBetweenCharacters("The Heart", "Xerx", "heart_001");
            
            total_reconstructions += 3;
        }
    }
    
    void searchForMemoryFragments() {
        std::cout << "\n🔍 MEMORY FRAGMENT SEARCH 🔍\n";
        std::cout << "Exploring the Metaverse for hidden memory fragments...\n\n";
        
        std::cout << "Current search position: ";
        player_position.print();
        std::cout << std::endl;
        
        std::cout << "\nSearch options:\n";
        std::cout << "1. Search current location\n";
        std::cout << "2. Move to new location and search\n";
        std::cout << "3. Use character's special search ability\n";
        std::cout << "4. Random exploration\n";
        std::cout << "5. Back to main menu\n";
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            performLocationSearch(player_position);
        } else if (choice == "2") {
            player_position = getSearchLocation();
            performLocationSearch(player_position);
        } else if (choice == "3") {
            performSpecialSearch();
        } else if (choice == "4") {
            performRandomExploration();
        }
    }
    
    void performLocationSearch(const Vector3D& location) {
        std::cout << "\nSearching at location: ";
        location.print();
        std::cout << std::endl;
        
        // Simulate finding fragments based on location
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> chance(0.0, 1.0);
        
        if (chance(gen) < 0.6) { // 60% chance to find something
            std::cout << "Memory fragment detected!" << std::endl;
            std::cout << "A shimmering echo of the past materializes..." << std::endl;
            
            // Randomly assign to a character for discovery
            std::vector<std::string> characters = {"Xing", "Xerx", "The Heart"};
            std::uniform_int_distribution<> char_dis(0, characters.size() - 1);
            std::string discoverer = characters[char_dis(gen)];
            
            std::cout << discoverer << " resonates with the fragment!" << std::endl;
            total_searches++;
        } else {
            std::cout << "No memory fragments found at this location." << std::endl;
            std::cout << "The echoes of the past remain silent here..." << std::endl;
        }
    }
    
    void performSpecialSearch() {
        std::cout << "\n🌟 SPECIAL SEARCH ABILITIES 🌟\n";
        std::cout << "1. Xerx's Liberation Scan - Find suppressed memories\n";
        std::cout << "2. Xing's Narrative Trace - Follow story threads\n";
        std::cout << "3. Heart's Emotional Echo - Sense emotional imprints\n";
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            std::cout << "\nXerx extends his consciousness, seeking suppressed memories...\n";
            memory_system.executeMemoryAbility(xerx.get(), "Break Suppression", player_position);
        } else if (choice == "2") {
            std::cout << "\nXing follows the narrative threads through reality...\n";
            memory_system.executeMemoryAbility(xing.get(), "Weave Narrative", player_position);
        } else if (choice == "3") {
            std::cout << "\nThe Heart opens itself to emotional resonance...\n";
            memory_system.executeMemoryAbility(heart.get(), "Emotional Resonance", player_position);
        }
        
        total_searches++;
    }
    
    void performRandomExploration() {
        std::cout << "\n🎲 RANDOM EXPLORATION 🎲\n";
        std::cout << "Wandering through the Metaverse, following intuition...\n\n";
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> pos_dis(-30.0, 30.0);
        
        for (int i = 0; i < 3; ++i) {
            Vector3D random_pos(pos_dis(gen), pos_dis(gen) * 0.3 + 2.0, pos_dis(gen));
            std::cout << "Exploring location " << (i + 1) << ": ";
            random_pos.print();
            std::cout << std::endl;
            
            performLocationSearch(random_pos);
            std::cout << std::endl;
        }
        
        player_position = Vector3D(pos_dis(gen), pos_dis(gen) * 0.3 + 2.0, pos_dis(gen));
        std::cout << "Final position: ";
        player_position.print();
        std::cout << std::endl;
    }
    
    Vector3D getSearchLocation() {
        std::cout << "\nEnter search coordinates (x y z) or press Enter for random: ";
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(-25.0, 25.0);
            return Vector3D(dis(gen), dis(gen) * 0.3 + 2.0, dis(gen));
        } else {
            // Parse input (simplified)
            return Vector3D(5, 2, 5); // Default if parsing fails
        }
    }
    
    void showMemoryTutorial() {
        std::cout << "\n=== MEMORY SYSTEM TUTORIAL ===\n";
        std::cout << "🧠 Welcome to the Memory System - the key to understanding! 🧠\n\n";
        
        std::cout << "MEMORY SYSTEM BASICS:\n";
        std::cout << "• Memory fragments are scattered throughout the Metaverse\n";
        std::cout << "• Each fragment contains pieces of suppressed or lost memories\n";
        std::cout << "• Characters can use special abilities to find and recover fragments\n";
        std::cout << "• Fragments have clarity levels - higher clarity reveals more content\n\n";
        
        std::cout << "MEMORY TYPES:\n";
        std::cout << "📖 Personal Memory - Individual character experiences\n";
        std::cout << "🤝 Shared Experience - Memories involving multiple characters\n";
        std::cout << "🌍 Collective Unconscious - Universal memories (The Heart)\n";
        std::cout << "📚 Library Knowledge - Infinite library information (Xing)\n";
        std::cout << "⚔️ Liberation Truth - Resistance memories (Xerx)\n";
        std::cout << "💖 Emotional Imprint - Feeling-based memories (The Heart)\n";
        std::cout << "📜 Narrative Fragment - Story pieces\n";
        std::cout << "🌀 Reality Echo - Memories of how reality used to be\n";
        std::cout << "💭 Dream Memory - Memories from dreams and visions\n";
        std::cout << "🚫 The One Suppression - Memories hidden by The One\n\n";
        
        std::cout << "CHARACTER SPECIALIZATIONS:\n";
        std::cout << "⚔️ XERX (The Liberator):\n";
        std::cout << "   - Memory Strike: Use memories as weapons\n";
        std::cout << "   - Reconstruct Truth: Piece together fragmented memories\n";
        std::cout << "   - Break Suppression: Overcome The One's memory blocks\n\n";
        
        std::cout << "🔮 XING (The Weaver):\n";
        std::cout << "   - Access Library: Tap into infinite knowledge\n";
        std::cout << "   - Weave Narrative: Connect story fragments\n\n";
        
        std::cout << "💖 THE HEART (Narrative Catalyst):\n";
        std::cout << "   - Emotional Resonance: Connect with feeling-based memories\n";
        std::cout << "   - Collective Access: Tap into universal consciousness\n";
        std::cout << "   - Dream Recall: Recover memories from dreams\n\n";
        
        std::cout << "ADVANCED FEATURES:\n";
        std::cout << "• Memory Sharing: Characters can share recovered memories\n";
        std::cout << "• Collaborative Reconstruction: Work together for greater insights\n";
        std::cout << "• Memory Connections: Related fragments strengthen each other\n";
        std::cout << "• Global Memory State: Collective strength weakens The One's control\n";
        std::cout << "• Suppression Breaking: High memory integrity unlocks hidden fragments\n\n";
        
        std::cout << "THE ULTIMATE GOAL:\n";
        std::cout << "Recover enough memories to understand the true nature of:\n";
        std::cout << "• The relationship between Xing and Xerx\n";
        std::cout << "• The Heart's purpose in the Metaverse\n";
        std::cout << "• The One's weaknesses and fears\n";
        std::cout << "• The path to liberation and collective consciousness\n\n";
    }
    
    void autoDemo() {
        std::cout << "\n🤖 AUTO-DEMO: WATCH AI RECOVER MEMORIES 🤖\n";
        std::cout << "Observe as the AI demonstrates the memory recovery process!\n\n";
        
        std::vector<std::string> demo_sequence = {
            "Xerx searches for suppressed memories of his brother",
            "Xing accesses the infinite library's knowledge",
            "The Heart connects with the collective unconscious",
            "Characters share their recovered memories",
            "Collaborative reconstruction reveals deeper truths",
            "The One's suppression begins to weaken",
            "Hidden memories become accessible"
        };
        
        for (size_t i = 0; i < demo_sequence.size(); ++i) {
            std::cout << "\n--- Step " << (i + 1) << ": " << demo_sequence[i] << " ---\n";
            
            Vector3D demo_pos(i * 8 - 24, 2, i * 6 - 18);
            
            if (i == 0) {
                memory_system.executeMemoryAbility(xerx.get(), "Reconstruct Truth", demo_pos);
            } else if (i == 1) {
                memory_system.executeMemoryAbility(xing.get(), "Access Library", demo_pos);
            } else if (i == 2) {
                memory_system.executeMemoryAbility(heart.get(), "Collective Access", demo_pos);
            } else if (i == 3) {
                memory_system.shareMemoryBetweenCharacters("Xerx", "Xing", "xerx_001");
            } else if (i == 4) {
                std::vector<std::string> chars = {"Xing", "Xerx", "The Heart"};
                memory_system.reconstructCollaborativeMemory(chars, "Brother");
            } else if (i == 5) {
                std::cout << "The One's suppression level decreases!" << std::endl;
                std::cout << "Collective memory strength grows!" << std::endl;
            } else if (i == 6) {
                std::cout << "New suppressed fragments become discoverable!" << std::endl;
                std::cout << "The path to liberation becomes clearer!" << std::endl;
            }
            
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }
        
        std::cout << "\n🌟 AUTO-DEMO COMPLETE! 🌟\n";
        std::cout << "The power of recovered memories illuminates the path forward!\n";
        memory_system.displayMemoryStatus();
    }
    
    void update(double deltaTime) {
        demo_time += deltaTime;
        // Memory system doesn't need real-time updates like combat or reality manipulation
    }
    
    void displayGameStatus() {
        std::cout << "\n=== DEMO STATUS ===\n";
        std::cout << "Total Memory Searches: " << total_searches << "\n";
        std::cout << "Total Reconstructions: " << total_reconstructions << "\n";
        std::cout << "Demo Time: " << (int)demo_time << " seconds\n";
        std::cout << "Player Position: ";
        player_position.print();
        std::cout << "\n";
        
        xing->displayStatus();
        xerx->displayStatus();
        heart->displayStatus();
        
        memory_system.displayMemoryStatus();
    }
    
    void run() {
        std::string choice;
        
        while (true) {
            displayMainMenu();
            std::getline(std::cin, choice);
            
            if (choice == "1") {
                xerxMemoryRecovery();
            } else if (choice == "2") {
                xingLibraryAccess();
            } else if (choice == "3") {
                heartEmotionalMemories();
            } else if (choice == "4") {
                collaborativeMemoryReconstruction();
            } else if (choice == "5") {
                shareMemoriesBetweenCharacters();
            } else if (choice == "6") {
                memory_system.displayMemoryStatus();
            } else if (choice == "7") {
                searchForMemoryFragments();
            } else if (choice == "8") {
                showMemoryTutorial();
            } else if (choice == "9") {
                autoDemo();
            } else if (choice == "10") {
                break;
            } else {
                std::cout << "Invalid choice. Please try again.\n";
            }
            
            // Update system
            update(1.0);
        }
        
        std::cout << "\nThank you for exploring the Memory System!\n";
        std::cout << "The recovered memories will guide the heroes toward liberation! 🌟\n";
        displayGameStatus();
    }
};

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    
    MemorySystemDemo demo;
    demo.run();
    
    return 0;
}
