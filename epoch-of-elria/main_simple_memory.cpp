// main_simple_memory.cpp - Test the Simple Memory System
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "Vector3D.h"
#include "DreamWeaverCharacters.h"
#include "SimpleMemorySystem.h"

// --- Simple Memory System Demo ---
class SimpleMemoryDemo {
private:
    std::unique_ptr<Xing> xing;
    std::unique_ptr<Xerx> xerx;
    std::unique_ptr<TheHeart> heart;
    SimpleMemorySystem memory_system;
    
    Vector3D player_position;
    int total_searches;
    double demo_time;
    
public:
    SimpleMemoryDemo() : player_position(0, 0, 0), total_searches(0), demo_time(0.0) {
        // Create the main characters
        xing = std::unique_ptr<Xing>(new Xing(Vector3D(0, 2, 0)));
        xerx = std::unique_ptr<Xerx>(new Xerx(Vector3D(10, 2, 0)));
        heart = std::unique_ptr<TheHeart>(new TheHeart(Vector3D(5, 3, 5)));
        
        std::cout << "\n🧠 THE DREAM WEAVER'S HEART - SIMPLE MEMORY SYSTEM 🧠\n";
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
        std::cout << "\n=== SIMPLE MEMORY SYSTEM MENU ===\n";
        std::cout << "1. Xerx's Memory Recovery\n";
        std::cout << "2. Xing's Library Access\n";
        std::cout << "3. The Heart's Emotional Memories\n";
        std::cout << "4. Search for Memory Fragments\n";
        std::cout << "5. View Memory Status\n";
        std::cout << "6. Memory System Tutorial\n";
        std::cout << "7. Auto-Demo (Watch AI Recover Memories)\n";
        std::cout << "8. Quit\n";
        std::cout << "Choose an option: ";
    }
    
    void xerxMemoryRecovery() {
        std::cout << "\n⚔️ XERX'S MEMORY RECOVERY ⚔️\n";
        std::cout << "The liberator seeks to recover his suppressed memories...\n\n";
        
        std::cout << "Choose memory type to search for:\n";
        std::cout << "1. Personal Memory - Memories of his past\n";
        std::cout << "2. Liberation Truth - Memories of resistance\n";
        std::cout << "3. Suppressed Memory - Memories hidden by The One\n";
        std::cout << "4. Back to main menu\n";
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        StoryMemoryType target_type;
        if (choice == "1") {
            target_type = StoryMemoryType::PERSONAL_MEMORY;
        } else if (choice == "2") {
            target_type = StoryMemoryType::LIBERATION_TRUTH;
        } else if (choice == "3") {
            target_type = StoryMemoryType::THE_ONE_SUPPRESSION;
        } else {
            return;
        }
        
        Vector3D search_pos = getSearchLocation();
        
        if (memory_system.searchForMemory("Xerx", target_type, search_pos)) {
            total_searches++;
            xerx->gainExperience(30);
            std::cout << "\nXerx's determination pierces through The One's suppression!" << std::endl;
            std::cout << "Flashes of memory return..." << std::endl;
            std::cout << "A warm library... a brother's laughter... stories shared in secret..." << std::endl;
            std::cout << "The truth becomes clearer with each recovered fragment!" << std::endl;
        }
    }
    
    void xingLibraryAccess() {
        std::cout << "\n🔮 XING'S LIBRARY ACCESS 🔮\n";
        std::cout << "The weaver reaches into the infinite library's depths...\n\n";
        
        std::cout << "Choose knowledge type to access:\n";
        std::cout << "1. Library Knowledge - Ancient wisdom and stories\n";
        std::cout << "2. Narrative Fragment - Pieces of unfinished stories\n";
        std::cout << "3. Reality Echo - Memories of how reality used to be\n";
        std::cout << "4. Back to main menu\n";
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        StoryMemoryType target_type;
        if (choice == "1") {
            target_type = StoryMemoryType::LIBRARY_KNOWLEDGE;
        } else if (choice == "2") {
            target_type = StoryMemoryType::NARRATIVE_FRAGMENT;
        } else if (choice == "3") {
            target_type = StoryMemoryType::REALITY_ECHO;
        } else {
            return;
        }
        
        Vector3D search_pos = getSearchLocation();
        
        if (memory_system.searchForMemory("Xing", target_type, search_pos)) {
            total_searches++;
            xing->gainExperience(30);
            std::cout << "\nXing's narrative threads reach across infinite possibilities!" << std::endl;
            std::cout << "The infinite library responds..." << std::endl;
            std::cout << "Countless stories whisper their secrets..." << std::endl;
            std::cout << "Knowledge flows like rivers of light through consciousness!" << std::endl;
        }
    }
    
    void heartEmotionalMemories() {
        std::cout << "\n💖 THE HEART'S EMOTIONAL MEMORIES 💖\n";
        std::cout << "The narrative catalyst connects with the deepest feelings...\n\n";
        
        std::cout << "Choose emotional memory type:\n";
        std::cout << "1. Emotional Imprint - Feelings left behind by others\n";
        std::cout << "2. Collective Unconscious - Universal memories\n";
        std::cout << "3. Dream Memory - Memories from dreams and visions\n";
        std::cout << "4. Back to main menu\n";
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        StoryMemoryType target_type;
        if (choice == "1") {
            target_type = StoryMemoryType::EMOTIONAL_IMPRINT;
        } else if (choice == "2") {
            target_type = StoryMemoryType::COLLECTIVE_UNCONSCIOUS;
        } else if (choice == "3") {
            target_type = StoryMemoryType::DREAM_MEMORY;
        } else {
            return;
        }
        
        Vector3D search_pos = getSearchLocation();
        
        if (memory_system.searchForMemory("The Heart", target_type, search_pos)) {
            total_searches++;
            heart->gainExperience(30);
            std::cout << "\nThe Heart's emotional resonance awakens dormant memories!" << std::endl;
            std::cout << "The collective unconscious stirs..." << std::endl;
            std::cout << "Every story ever told echoes through the Metaverse..." << std::endl;
            std::cout << "The Heart becomes a conduit for universal memory!" << std::endl;
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
        std::cout << "3. Random exploration\n";
        std::cout << "4. Back to main menu\n";
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            performLocationSearch(player_position);
        } else if (choice == "2") {
            player_position = getSearchLocation();
            performLocationSearch(player_position);
        } else if (choice == "3") {
            performRandomExploration();
        }
    }
    
    void performLocationSearch(const Vector3D& location) {
        std::cout << "\nSearching at location: ";
        location.print();
        std::cout << std::endl;
        
        // Try searching with each character for different memory types
        std::vector<std::string> characters = {"Xerx", "Xing", "The Heart"};
        std::vector<StoryMemoryType> types = {
            StoryMemoryType::PERSONAL_MEMORY,
            StoryMemoryType::LIBRARY_KNOWLEDGE,
            StoryMemoryType::COLLECTIVE_UNCONSCIOUS
        };
        
        bool found_something = false;
        for (size_t i = 0; i < characters.size(); ++i) {
            if (memory_system.searchForMemory(characters[i], types[i], location)) {
                found_something = true;
                total_searches++;
                break;
            }
        }
        
        if (!found_something) {
            std::cout << "No memory fragments found at this location." << std::endl;
            std::cout << "The echoes of the past remain silent here..." << std::endl;
        }
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
        std::cout << "\n=== SIMPLE MEMORY SYSTEM TUTORIAL ===\n";
        std::cout << "🧠 Welcome to the Memory System - the key to understanding! 🧠\n\n";
        
        std::cout << "MEMORY SYSTEM BASICS:\n";
        std::cout << "• Memory fragments are scattered throughout the Metaverse\n";
        std::cout << "• Each fragment contains pieces of suppressed or lost memories\n";
        std::cout << "• Characters can search for different types of memories\n";
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
        std::cout << "   - Best at finding Personal Memories and Liberation Truth\n";
        std::cout << "   - Can break through The One's suppression\n\n";
        
        std::cout << "🔮 XING (The Weaver):\n";
        std::cout << "   - Excels at accessing Library Knowledge\n";
        std::cout << "   - Can find Narrative Fragments and Reality Echoes\n\n";
        
        std::cout << "💖 THE HEART (Narrative Catalyst):\n";
        std::cout << "   - Connects with Emotional Imprints\n";
        std::cout << "   - Accesses Collective Unconscious and Dream Memories\n\n";
        
        std::cout << "SEARCH MECHANICS:\n";
        std::cout << "• Characters have different resonance with memory types\n";
        std::cout << "• Search location affects what memories can be found\n";
        std::cout << "• Memory integrity improves as more fragments are recovered\n";
        std::cout << "• High collective memory strength weakens The One's control\n\n";
        
        std::cout << "THE ULTIMATE GOAL:\n";
        std::cout << "Recover enough memories to understand:\n";
        std::cout << "• The relationship between Xing and Xerx\n";
        std::cout << "• The Heart's purpose in the Metaverse\n";
        std::cout << "• The One's weaknesses and fears\n";
        std::cout << "• The path to liberation and collective consciousness\n\n";
    }
    
    void autoDemo() {
        std::cout << "\n🤖 AUTO-DEMO: WATCH AI RECOVER MEMORIES 🤖\n";
        std::cout << "Observe as the AI demonstrates the memory recovery process!\n\n";
        
        std::vector<std::string> demo_sequence = {
            "Xerx searches for personal memories",
            "Xing accesses library knowledge",
            "The Heart connects with collective unconscious",
            "Xerx finds suppressed memories",
            "Xing discovers narrative fragments",
            "The Heart recovers emotional imprints",
            "Memory breakthrough achieved!"
        };
        
        std::vector<StoryMemoryType> demo_types = {
            StoryMemoryType::PERSONAL_MEMORY,
            StoryMemoryType::LIBRARY_KNOWLEDGE,
            StoryMemoryType::COLLECTIVE_UNCONSCIOUS,
            StoryMemoryType::THE_ONE_SUPPRESSION,
            StoryMemoryType::NARRATIVE_FRAGMENT,
            StoryMemoryType::EMOTIONAL_IMPRINT,
            StoryMemoryType::SHARED_EXPERIENCE
        };
        
        std::vector<std::string> demo_characters = {
            "Xerx", "Xing", "The Heart", "Xerx", "Xing", "The Heart", "Xerx"
        };
        
        for (size_t i = 0; i < demo_sequence.size() && i < demo_types.size(); ++i) {
            std::cout << "\n--- Step " << (i + 1) << ": " << demo_sequence[i] << " ---\n";
            
            Vector3D demo_pos(i * 8 - 24, 2, i * 6 - 18);
            
            if (i < 6) {
                memory_system.searchForMemory(demo_characters[i], demo_types[i], demo_pos);
            } else {
                std::cout << "Collective memory strength reaches critical threshold!" << std::endl;
                std::cout << "The One's suppression begins to crumble!" << std::endl;
                std::cout << "New memories become accessible across the Metaverse!" << std::endl;
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
    }
    
    void displayGameStatus() {
        std::cout << "\n=== DEMO STATUS ===\n";
        std::cout << "Total Memory Searches: " << total_searches << "\n";
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
                searchForMemoryFragments();
            } else if (choice == "5") {
                memory_system.displayMemoryStatus();
            } else if (choice == "6") {
                showMemoryTutorial();
            } else if (choice == "7") {
                autoDemo();
            } else if (choice == "8") {
                break;
            } else {
                std::cout << "Invalid choice. Please try again.\n";
            }
            
            // Update system
            update(1.0);
        }
        
        std::cout << "\nThank you for exploring the Simple Memory System!\n";
        std::cout << "The recovered memories will guide the heroes toward liberation! 🌟\n";
        displayGameStatus();
    }
};

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    
    SimpleMemoryDemo demo;
    demo.run();
    
    return 0;
}
