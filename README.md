# Epoch of Elria - A Multifaceted Game Development Project
Epoch of Elria is a comprehensive game development project that encompasses several key components:
- **C++ Game Engine:** A high-performance engine featuring parallel processing, SVG graphics, and optimized texture management.
- **Rust Game Engine Port:** A port of the C++ engine to Rust, focusing on 3D capabilities using the `kiss3d` library.
- **Dream Weaver RPG:** A narrative-driven RPG with unique characters and gameplay mechanics, built within the Epoch of Elria ecosystem.
- **CGX AI Integration:** A TypeScript-based MCP (Model Context Protocol) server for integrating AI agent tools.

This README provides an overview of each component and how to get started.

## Epoch of Elria (C++ Game Engine)

This is the original Epoch of Elria game engine, written in C++.

### Core Engine Features
- **Parallel Processing**: Multi-threaded game object updates using `std::thread`
- **Thread-Safe Operations**: Mutex-protected collectible states
- **Optimized Performance**: Efficient collision detection and memory management
- **Cross-Platform**: Compatible with Windows (WSL), Linux, and macOS

### Graphics System Features
- **SVG Textures**: Scalable vector graphics for optimal performance
- **Multiple Renderers**: Console and HTML/SVG output support
- **Procedural Assets**: Runtime SVG generation for game objects
- **Texture Management**: Centralized asset loading and caching

### Game Environment
- **Camera System**: Smooth following and world-to-screen coordinate conversion
- **Particle Effects**: Dynamic particle systems for visual feedback
- **Platform Physics**: Collision detection and response
- **Background Layers**: Parallax scrolling support

- **Build/Run:**
    ```bash
    # Compile the original parallel demo
    g++ main.cpp -o game_engine_parallel_demo -std=c++11 -pthread

    # Compile the enhanced graphics version
    g++ main_simple_graphics.cpp Vector2D.cpp graphics.cpp -o game_engine_simple_graphics -std=c++11 -pthread

    # Or use the Makefile
    make all

    # Run the original parallel engine
    ./game_engine_parallel_demo

    # Run the enhanced graphics version
    ./game_engine_simple_graphics
    ```

## Epoch of Elria (Rust Game Engine)

This is a port of the C++ game engine to Rust, with a focus on 3D capabilities.
- **Core Features:**
    - **Modern Language:** Leverages Rust for memory safety and concurrent programming performance.
    - **3D Graphics Focus:** Utilizes the `kiss3d` library for 3D rendering and interaction.
- **Build/Run:**
    ```bash
    # Navigate to the Rust project directory (if not already there)
    # cd <path_to_rust_project_directory> # (Typically the root of the repository where Cargo.toml is located)

    # Build the project (for debug)
    cargo build

    # Run the project (for debug)
    cargo run

    # For a release build (optimized)
    cargo build --release
    cargo run --release
    ```
    Note: Ensure you have Rust and Cargo installed (see https://www.rust-lang.org/tools/install).
    The primary graphical demo uses the `winit` library for windowing. There are two `main` functions in `src/main.rs`; the `winit` based one is likely the target for `cargo run`.

## Dream Weaver RPG

A narrative-driven RPG set in the infinite Metaverse, where consciousness shapes reality. Four heroes unite against absolute order through collaborative storytelling and harmony.
- **Gameplay:**
    - **Dynamic Hero Switching:** Freely switch between four unique heroes: **Xing** (The Weaver), **Xerx** (The Liberator), **The Heart** (Emotional Catalyst), and **Lyra** (Pure Melody), each with distinct abilities.
    - **Immersive 3D Exploration:** Navigate the **Metaverse**, a vast 3D world that responds to character interactions.
    - **Deep RPG Mechanics:** Engage in character progression, learn new **abilities**, and build **relationships** that unlock synergies.
    - **Unique Narrative Goal:** Aim to transform **The One** (the antagonist) through **love and collaboration**, rather than traditional combat.
- **Build/Run:**
    ```bash
    # Build the complete game
    g++ -std=c++17 -Wall -Wextra -O2 main_dream_weaver_complete.cpp -o dream_weaver_complete

    # Run the game
    ./dream_weaver_complete

    # Or use the Makefile
    make dream_weaver_complete
    # or
    make game
    ```
    (For more details, see `DREAM_WEAVER_README.md`)

## CGX (AI Integration)

A TypeScript-based MCP (Model Context Protocol) server for integrating AI agent tools. This component allows AI agents to interact with the Epoch of Elria ecosystem or provide external functionalities.
- **Features:**
    - Implements tools that AI agents can use (e.g., the sample Weather Tool).
    - Connects to Agent Builder for testing and development.
    - Debuggable using the MCP Inspector for SSE and STDIO.
- **Get Started:**
    1. Ensure Node.js is installed (see https://nodejs.org/).
    2. Open VS Code Debug panel, select `Debug in Agent Builder` (or press F5).
    3. Use AI Toolkit Agent Builder to test the server.
    (For more details and other debugging modes, see `CGX/README.md`)

## 🏗️ Project Architecture

The Epoch of Elria project is composed of several interconnected components:

- **Core Game Engines (C++ and Rust):**
    - The **C++ Engine** serves as the foundational layer, providing robust 2D/SVG graphics, parallel processing, and core game mechanics.
    - The **Rust Engine** is an ongoing initiative to port and extend the engine's capabilities, with a current focus on 3D graphics using `kiss3d`. It aims to leverage Rust's strengths in safety and performance for modern game development.

- **Dream Weaver RPG:**
    - This narrative-driven RPG is primarily built using **C++** and likely utilizes or extends the **C++ Game Engine** for its world rendering, character systems, and gameplay logic.

- **CGX (AI Integration):**
    - The **CGX** component, written in TypeScript, functions as an MCP (Model Context Protocol) server. It's designed to allow AI agents to interact with the game world or provide auxiliary AI-powered tools and functionalities. This enables a flexible way to integrate intelligent behaviors or external data sources into the Epoch of Elria ecosystem.

The components are designed to be somewhat modular, allowing for independent development and testing, while also providing a framework for building rich and complex game experiences.

### 📁 Project Structure (C++ Component)
This describes the structure of the core C++ engine components. The overall project contains more directories related to the Rust engine, Dream Weaver RPG, and CGX integration, typically found in their respective subfolders or the root directory.

```
Epoch of Elria/
├── main.cpp                    # Original parallel game engine
├── main_simple_graphics.cpp    # Enhanced version with SVG graphics
├── Vector2D.h/.cpp            # 2D vector mathematics
├── graphics.h/.cpp            # Graphics and rendering system
├── GameEnvironment.h/.cpp     # Game world and environment
├── GameObject.h/.cpp          # Base game object class
├── assets/                    # SVG texture assets
│   ├── player.svg
│   ├── coin.svg
│   ├── gem.svg
│   └── background.svg
├── Makefile                   # Build configuration
└── README.md                  # This file
```

## 🎯 Game Features

### Player Character
- **SVG-based sprite**: Scalable character with detailed design
- **Smooth movement**: Physics-based motion with collision detection
- **Auto-wrapping**: Seamless screen boundary handling

### Collectibles
- **Multiple types**: Coins (10 points) and Gems (25 points)
- **Animated effects**: Bobbing motion and visual feedback
- **Thread-safe collection**: Parallel processing without race conditions

### Visual Output
- **Console rendering**: ASCII-based real-time display
- **HTML/SVG export**: High-quality vector graphics output
- **Frame capture**: Automatic screenshot generation

### Interactive Controls
- **Walkable gameplay**: Turn-based and real-time movement options
- **WASD/Arrow keys**: Standard movement controls
- **Command interface**: Text-based commands for turn-based play
- **SFML integration**: Real-time graphics with smooth movement

## 🔧 Technical Highlights

### Parallel Processing
- **Multi-threaded updates**: Distributes collectible processing across CPU cores
- **Thread safety**: Mutex-protected shared state
- **Load balancing**: Dynamic thread allocation based on object count

### SVG Graphics System
- **Procedural generation**: Runtime SVG creation for game assets
- **Texture management**: Efficient loading and caching
- **Multiple renderers**: Console and HTML output support

### Performance Optimizations
- **Efficient collision detection**: AABB (Axis-Aligned Bounding Box)
- **Memory management**: Smart pointers and RAII principles
- **Minimal dependencies**: Pure C++11 with standard library

## 🎨 SVG Asset Examples

### Player Character
```svg
<svg width="50" height="50" xmlns="http://www.w3.org/2000/svg">
  <rect x="10" y="15" width="30" height="25" fill="#4169E1" rx="5"/>
  <circle cx="25" cy="10" r="7.5" fill="#FFE4B5"/>
  <!-- Eyes, arms, legs... -->
</svg>
```

### Collectible Coin
```svg
<svg width="20" height="20" xmlns="http://www.w3.org/2000/svg">
  <circle cx="10" cy="10" r="8" fill="#FFD700" stroke="#FFA500" stroke-width="2"/>
  <text x="10" y="12" text-anchor="middle" font-size="8" fill="#B8860B">$</text>
</svg>
```

## 📊 Performance Metrics

- **Threading**: Utilizes all available CPU cores (typically 20+ threads)
- **Frame rate**: 60 FPS target with 16.67ms frame time
- **Memory**: Efficient asset management with shared pointers
- **Scalability**: Handles 1000+ game objects with parallel processing

## 🐛 Debugging

The engine includes comprehensive error handling and debugging features:
- Thread-safe logging
- Memory leak prevention
- Graceful error recovery
- Performance monitoring

## 🔮 Future Enhancements

- **Audio system**: Sound effects and music support
- **Input handling**: Keyboard and mouse controls
- **Networking**: Multiplayer capabilities
- **Advanced physics**: Gravity, friction, and forces
- **Level editor**: Visual game world creation tools

## 📝 License

This project is part of the "Epoch of Elria" game development series.

## 🤝 Contributing

Feel free to submit issues, feature requests, or pull requests to improve the engine!

---

**Built with ❤️ using modern C++ and SVG graphics**
