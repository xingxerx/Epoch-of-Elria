# Epoch of Elria - Game Engine

Epoch of Elria is a game engine project with implementations in both C++ and Rust, designed to explore different approaches to game development, from 2D SVG graphics and parallel processing in C++ to 3D graphics in Rust.

## Epoch of Elria (C++ Version)

A high-performance C++ game engine featuring parallel processing, SVG graphics, and optimized texture management.

### 🎮 Features (C++)

#### Core Engine
- **Parallel Processing**: Multi-threaded game object updates using `std::thread`
- **Thread-Safe Operations**: Mutex-protected collectible states
- **Optimized Performance**: Efficient collision detection and memory management
- **Cross-Platform**: Compatible with Windows (WSL), Linux, and macOS

#### Graphics System
- **SVG Textures**: Scalable vector graphics for optimal performance
- **Multiple Renderers**: Console and HTML/SVG output support
- **Procedural Assets**: Runtime SVG generation for game objects
- **Texture Management**: Centralized asset loading and caching

#### Game Environment
- **Camera System**: Smooth following and world-to-screen coordinate conversion
- **Particle Effects**: Dynamic particle systems for visual feedback
- **Platform Physics**: Collision detection and response
- **Background Layers**: Parallax scrolling support

### 🚀 Quick Start (C++)

#### Prerequisites
- C++11 compatible compiler (g++, clang++)
- pthread support
- WSL (for Windows users)

#### Building (C++)

```bash
# Compile the original parallel demo
g++ main.cpp -o game_engine_parallel_demo -std=c++11 -pthread

# Compile the enhanced graphics version
g++ main_simple_graphics.cpp Vector2D.cpp graphics.cpp -o game_engine_simple_graphics -std=c++11 -pthread

# Or use the Makefile
make all
```

#### Running (C++)

```bash
# Run the original parallel engine
./game_engine_parallel_demo

# Run the enhanced graphics version
./game_engine_simple_graphics

# Run the walkable game (turn-based)
./game_walkable_simple

# Run the interactive SFML version (real-time)
./compile_interactive.sh  # Compile first
./game_interactive_simple

# Or use make targets
make run-original
make run-simple
```

### 📁 Project Structure (C++ Example)

(Refer to the C++ source files for detailed structure. Key components include `main.cpp`, `main_simple_graphics.cpp`, `Vector2D.h/cpp`, `graphics.h/cpp`, `GameEnvironment.h/cpp`, `GameObject.h/cpp`, and the `assets/` directory for SVG textures.)

### 🎯 Game Features (C++)

#### Player Character
- **SVG-based sprite**: Scalable character with detailed design
- **Smooth movement**: Physics-based motion with collision detection
- **Auto-wrapping**: Seamless screen boundary handling

#### Collectibles
- **Multiple types**: Coins (10 points) and Gems (25 points)
- **Animated effects**: Bobbing motion and visual feedback
- **Thread-safe collection**: Parallel processing without race conditions

#### Visual Output
- **Console rendering**: ASCII-based real-time display
- **HTML/SVG export**: High-quality vector graphics output
- **Frame capture**: Automatic screenshot generation

#### Interactive Controls
- **Walkable gameplay**: Turn-based and real-time movement options
- **WASD/Arrow keys**: Standard movement controls
- **Command interface**: Text-based commands for turn-based play
- **SFML integration**: Real-time graphics with smooth movement

### 🔧 Technical Highlights (C++)

#### Parallel Processing
- **Multi-threaded updates**: Distributes collectible processing across CPU cores
- **Thread safety**: Mutex-protected shared state
- **Load balancing**: Dynamic thread allocation based on object count

#### SVG Graphics System
- **Procedural generation**: Runtime SVG creation for game assets
- **Texture management**: Efficient loading and caching
- **Multiple renderers**: Console and HTML output support

#### Performance Optimizations
- **Efficient collision detection**: AABB (Axis-Aligned Bounding Box)
- **Memory management**: Smart pointers and RAII principles
- **Minimal dependencies**: Pure C++11 with standard library

### 🎨 SVG Asset Examples (C++)

Player Character:
```svg
<svg width="50" height="50" xmlns="http://www.w3.org/2000/svg">
  <rect x="10" y="15" width="30" height="25" fill="#4169E1" rx="5"/>
  <circle cx="25" cy="10" r="7.5" fill="#FFE4B5"/>
  <!-- Eyes, arms, legs... -->
</svg>
```

Collectible Coin:
```svg
<svg width="20" height="20" xmlns="http://www.w3.org/2000/svg">
  <circle cx="10" cy="10" r="8" fill="#FFD700" stroke="#FFA500" stroke-width="2"/>
  <text x="10" y="12" text-anchor="middle" font-size="8" fill="#B8860B">$</text>
</svg>
```

### 📊 Performance Metrics (C++)

- **Threading**: Utilizes all available CPU cores (typically 20+ threads)
- **Frame rate**: 60 FPS target with 16.67ms frame time
- **Memory**: Efficient asset management with shared pointers
- **Scalability**: Handles 1000+ game objects with parallel processing

### 🐛 Debugging (C++)

The C++ engine includes comprehensive error handling and debugging features:
- Thread-safe logging
- Memory leak prevention
- Graceful error recovery
- Performance monitoring

## Epoch of Elria (Rust Version)

The Rust version of Epoch of Elria is a port and re-imagination of the engine, focusing on 3D graphics rendering using the `kiss3d` library. It aims to explore modern game development practices in Rust.

### ✨ Features (Rust)

- **3D Graphics**: Utilizes the `kiss3d` library for 3D rendering.
- **Component-Based Architecture**: (Inferred from typical Rust game engine design, details in `src/lib.rs` and components).
- **Modern Tooling**: Built with Cargo, the Rust build system and package manager.
- **Safety Focus**: Leverages Rust's memory safety features.
- **Cross-Platform**: Builds on Rust's cross-platform capabilities.

### 🚀 Quick Start (Rust)

#### Prerequisites
- Rust toolchain (install via [rustup.rs](https://rustup.rs/))

#### Building (Rust)

```bash
# Build the engine and examples in debug mode
cargo build

# Build for release (optimized)
cargo build --release
```

#### Running (Rust)

The `src/main.rs` file currently serves as a test suite for engine components.
```bash
# Run the test suite
cargo run
```

To run the full graphics demo (as suggested in `src/main.rs` comments):
```bash
# Run the main graphics demo (if available as a separate example or main function)
cargo run --release
# (Note: The specific target for the graphics demo might be an example, e.g., cargo run --release --example your_demo_name)
# Based on `examples/complete_rpg_demo.rs`, you might run:
cargo run --release --example complete_rpg_demo
```
Check `Cargo.toml` and the `examples` directory for specific runnable targets.

## 🎮 Example Games

### The Dream Weaver's Heart
**The Dream Weaver's Heart** is an immersive RPG built with the C++ version of the Epoch of Elria engine. It is set in the infinite Metaverse, where consciousness shapes reality through stories, memories, and pure melody. Four unique heroes must transform The One - an ancient entity of absolute order - through collaboration rather than destruction.

For more details on this game, its features, and how to build and run it, please see its dedicated guide:
➡️ **[The Dream Weaver's Heart - Complete Game Guide](DREAM_WEAVER_README.md)**

## 🔮 Future Enhancements

This section applies to both engine versions or can be split as development progresses.
- **Audio system**: Sound effects and music support
- **Input handling**: Advanced keyboard, mouse, and controller support
- **Networking**: Multiplayer capabilities
- **Advanced physics**: More complex interactions, gravity, friction, and forces
- **Level editor**: Visual game world creation tools
- **Expanded Rust Features**: Further development of the Rust engine's 3D capabilities and game systems.

## 📝 License

This project is part of the "Epoch of Elria" game development series. The code is generally available under a permissive license like MIT (check individual `Cargo.toml` or source files for specifics).

## 🤝 Contributing

Feel free to submit issues, feature requests, or pull requests to improve the engine!

---

**Built with ❤️ using modern C++, Rust, and a passion for game development.**
