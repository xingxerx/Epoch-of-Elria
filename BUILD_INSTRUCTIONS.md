# Building The Dream Weaver's Heart Game Engine

## 🔧 Setup Options

### Option 1: Install MinGW-w64 (Recommended)

1. **Download MinGW-w64**:
   - Go to: https://www.mingw-w64.org/downloads/
   - Download the installer for Windows
   - Or use: https://github.com/niXman/mingw-builds-binaries/releases

2. **Install and Add to PATH**:
   ```cmd
   # Add to your PATH environment variable:
   C:\mingw64\bin
   ```

3. **Verify Installation**:
   ```cmd
   gcc --version
   g++ --version
   ```

4. **Build the Game**:
   ```cmd
   cd "F:\Epoch of Elria"
   g++ -std=c++17 -Wall -Wextra -O2 main_dream_weaver_complete.cpp -o dream_weaver_complete.exe
   ```

### Option 2: Visual Studio Community (Free)

1. **Download Visual Studio Community**:
   - Go to: https://visualstudio.microsoft.com/vs/community/
   - Install with "Desktop development with C++" workload

2. **Open Developer Command Prompt**:
   ```cmd
   # Search for "Developer Command Prompt" in Start Menu
   ```

3. **Build with MSVC**:
   ```cmd
   cd "F:\Epoch of Elria"
   cl /EHsc /std:c++17 main_dream_weaver_complete.cpp /Fe:dream_weaver_complete.exe
   ```

### Option 3: Use Online Compiler (Quick Test)

1. **Copy code to online compiler**:
   - Go to: https://replit.com/languages/cpp
   - Or: https://www.onlinegdb.com/online_c++_compiler

2. **Paste the main file content and run**

### Option 4: WSL (Windows Subsystem for Linux)

1. **Install WSL**:
   ```cmd
   wsl --install
   ```

2. **Install build tools in WSL**:
   ```bash
   sudo apt update
   sudo apt install build-essential
   ```

3. **Build in WSL**:
   ```bash
   cd /mnt/f/Epoch\ of\ Elria/
   g++ -std=c++17 -Wall -Wextra -O2 main_dream_weaver_complete.cpp -o dream_weaver_complete
   ./dream_weaver_complete
   ```

## 🎮 Quick Start Commands

### If you have g++ available:
```bash
# Navigate to project directory
cd "F:\Epoch of Elria"

# Build the complete Dream Weaver's Heart game
g++ -std=c++17 -Wall -Wextra -O2 main_dream_weaver_complete.cpp -o dream_weaver_complete

# Run the game
./dream_weaver_complete

# Build other game variants
g++ -std=c++17 main_rpg_characters.cpp -o rpg_characters
g++ -std=c++17 main_3d_openworld.cpp -o game_3d_openworld
g++ -std=c++17 main_memory_system.cpp -o memory_system
```

### Using the Makefile (if make is available):
```bash
# Build all games
make all

# Build specific games
make dream_weaver_complete
make rpg_characters
make game_3d_openworld

# Quick build and run
make game
```

## 🔍 Troubleshooting

### Common Issues:

1. **"g++ not found"**:
   - Install MinGW-w64 or Visual Studio
   - Add compiler to PATH environment variable

2. **"C++17 not supported"**:
   - Use older standard: `-std=c++11` or `-std=c++14`
   - Update your compiler

3. **Missing headers**:
   - All required headers are standard C++ library
   - No external dependencies needed

4. **Linking errors**:
   - Add `-pthread` flag if needed: `g++ -std=c++17 -pthread main_dream_weaver_complete.cpp -o game`

### Alternative Build Commands:

```bash
# Minimal build (C++11 compatible)
g++ -std=c++11 main_dream_weaver_complete.cpp -o game

# Debug build
g++ -std=c++17 -g -Wall -Wextra main_dream_weaver_complete.cpp -o game_debug

# Optimized release build
g++ -std=c++17 -O3 -DNDEBUG main_dream_weaver_complete.cpp -o game_release
```

## 🎯 Running the Games

### The Complete Dream Weaver's Heart Experience:
```bash
./dream_weaver_complete
```
- Full integrated game with all 4 characters + The One
- 3D world exploration with WASD movement
- Character switching and ability system
- Perfect harmony and transformation victory

### Individual Game Components:
```bash
# Character system demo
./rpg_characters

# 3D world exploration
./game_3d_openworld

# Memory system demo
./memory_system

# Original parallel engine
./game_engine_parallel_demo
```

## 📁 Project Structure for Building

```
F:\Epoch of Elria\
├── main_dream_weaver_complete.cpp  # Main game file
├── DreamWeaverCharacters.h        # Character definitions
├── TheOneAntagonist.h             # Antagonist class
├── RPGCharacter.h                 # Base RPG system
├── Vector3D.h                     # 3D math
├── World3D.h                      # 3D world system
├── MemorySystem.h                 # Memory mechanics
└── assets/                        # SVG graphics
    └── metaverse_portal.svg
```

## 🌟 Development Workflow

1. **Edit source files** with any text editor
2. **Compile** with g++ or Visual Studio
3. **Run** the executable
4. **Test** different game features
5. **Iterate** and rebuild

### Example Development Session:
```bash
# 1. Navigate to project
cd "F:\Epoch of Elria"

# 2. Edit files (use any editor)
notepad main_dream_weaver_complete.cpp

# 3. Build
g++ -std=c++17 main_dream_weaver_complete.cpp -o game

# 4. Run and test
./game

# 5. Repeat as needed
```

## 🚀 Next Steps

Once you have a compiler set up:

1. **Build the complete game** using the commands above
2. **Run and play** through the full experience
3. **Modify the source** to add your own features
4. **Experiment** with different character abilities
5. **Create new stories** and see them integrated into the game world

The game is fully functional and ready to play once compiled!
