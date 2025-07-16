#!/bin/bash

echo "╔══════════════════════════════════════════════════════════════╗"
echo "║              BUILDING WINDOWED SANDBOX GAMES                ║"
echo "╚══════════════════════════════════════════════════════════════╝"
echo

# Build console-based windowed sandbox
echo "Building console-based windowed sandbox..."
g++ -std=c++17 -O2 windowed_sandbox_game.cpp -o windowed_sandbox_game
if [ $? -eq 0 ]; then
    echo "✓ Console sandbox built successfully!"
else
    echo "❌ Console sandbox build failed!"
fi

# Check for SDL2
echo
echo "Checking for SDL2 libraries..."
if pkg-config --exists sdl2 sdl2_ttf; then
    echo "✓ SDL2 found! Building graphical version..."
    
    # Build SDL2 version
    g++ -std=c++17 -O2 -DUSE_SDL2 sdl_sandbox_game.cpp -o sdl_sandbox_game \
        $(pkg-config --cflags --libs sdl2 sdl2_ttf)
    
    if [ $? -eq 0 ]; then
        echo "✓ SDL2 sandbox built successfully!"
        SDL2_AVAILABLE=true
    else
        echo "❌ SDL2 sandbox build failed!"
        SDL2_AVAILABLE=false
    fi
else
    echo "⚠ SDL2 not found. Building fallback version..."
    g++ -std=c++17 -O2 sdl_sandbox_game.cpp -o sdl_sandbox_game_fallback
    SDL2_AVAILABLE=false
fi

echo
echo "╔══════════════════════════════════════════════════════════════╗"
echo "║                    BUILD COMPLETE                           ║"
echo "╚══════════════════════════════════════════════════════════════╝"
echo

echo "Available games:"
if [ -f "windowed_sandbox_game" ]; then
    echo "✓ ./windowed_sandbox_game - Console-based windowed sandbox with FPS counter"
fi

if [ "$SDL2_AVAILABLE" = true ] && [ -f "sdl_sandbox_game" ]; then
    echo "✓ ./sdl_sandbox_game - Real windowed game with SDL2 graphics"
fi

if [ -f "sdl_sandbox_game_fallback" ]; then
    echo "✓ ./sdl_sandbox_game_fallback - Fallback version (shows SDL2 install instructions)"
fi

echo
echo "To install SDL2 on your system:"
echo "• Arch Linux: sudo pacman -S sdl2 sdl2_ttf"
echo "• Ubuntu/Debian: sudo apt install libsdl2-dev libsdl2-ttf-dev"
echo "• Windows: Download from https://libsdl.org"
echo
echo "Run any game to start playing!"
