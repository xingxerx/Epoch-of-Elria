# Epoch of Elria - Enhanced C++ Game Engine

A high-performance C++ game engine featuring parallel processing, SVG graphics, and optimized texture management.

## 🎮 Features

### Core Engine
- **Parallel Processing**: Multi-threaded game object updates using `std::thread`
- **Thread-Safe Operations**: Mutex-protected collectible states
- **Optimized Performance**: Efficient collision detection and memory management
- **Cross-Platform**: Compatible with Windows (WSL), Linux, and macOS

### Graphics System
- **SVG Textures**: Scalable vector graphics for optimal performance
- **Multiple Renderers**: Console and HTML/SVG output support
- **Procedural Assets**: Runtime SVG generation for game objects
- **Texture Management**: Centralized asset loading and caching

### Game Environment
- **Camera System**: Smooth following and world-to-screen coordinate conversion
- **Particle Effects**: Dynamic particle systems for visual feedback
- **Platform Physics**: Collision detection and response
- **Background Layers**: Parallax scrolling support

## 🚀 Quick Start

### Prerequisites
- C++11 compatible compiler (g++, clang++)
- pthread support
- WSL (for Windows users)

### Building

```bash
# Compile the original parallel demo
g++ main.cpp -o game_engine_parallel_demo -std=c++11 -pthread

# Compile the enhanced graphics version
g++ main_simple_graphics.cpp Vector2D.cpp graphics.cpp -o game_engine_simple_graphics -std=c++11 -pthread

# Or use the Makefile
make all
```

### Running

```bash
# Run the original parallel engine
./game_engine_parallel_demo

# Run the enhanced graphics version
./game_engine_simple_graphics

# Or use make targets
make run-original
make run-simple
```

## 📁 Project Structure

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
