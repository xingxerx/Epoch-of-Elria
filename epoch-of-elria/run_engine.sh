#!/bin/bash

echo "╔══════════════════════════════════════════════════════════════╗"
echo "║              EPOCH OF ELRIA GAME ENGINE                     ║"
echo "║                   Linux Build Script                        ║"
echo "╚══════════════════════════════════════════════════════════════╝"
echo ""

echo "🔨 Building standalone game engine..."
g++ -std=c++17 -O2 standalone_game_engine.cpp -o standalone_game_engine

if [ $? -ne 0 ]; then
    echo "❌ Build failed! Trying existing games..."
    echo ""
    
    if [ -f "simple_3d_game" ]; then
        echo "✅ Running simple_3d_game"
        ./simple_3d_game
    elif [ -f "windowed_sandbox_game" ]; then
        echo "✅ Running windowed_sandbox_game"
        ./windowed_sandbox_game
    elif [ -f "game_svg" ]; then
        echo "✅ Running SVG game"
        ./game_svg
    else
        echo "❌ No executable games found!"
        echo "Please install g++ to compile: sudo apt install g++"
    fi
    exit 1
fi

echo "✅ Build successful!"
echo "🚀 Starting Epoch of Elria Game Engine..."
echo ""

./standalone_game_engine

echo ""
echo "🌟 Thank you for using Epoch of Elria Game Engine!"
