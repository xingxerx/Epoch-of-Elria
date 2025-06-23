@echo off
echo ╔══════════════════════════════════════════════════════════════╗
echo ║              EPOCH OF ELRIA GAME ENGINE                     ║
echo ║                   Build and Run Script                      ║
echo ╚══════════════════════════════════════════════════════════════╝
echo.

echo 🔨 Building standalone game engine...
g++ -std=c++17 -O2 standalone_game_engine.cpp -o standalone_game_engine.exe

if %errorlevel% neq 0 (
    echo ❌ Build failed! Make sure g++ is installed.
    echo.
    echo Trying to run existing games instead...
    echo.
    if exist simple_3d_game.exe (
        echo ✅ Running simple_3d_game.exe
        simple_3d_game.exe
    ) else if exist windowed_sandbox_game.exe (
        echo ✅ Running windowed_sandbox_game.exe  
        windowed_sandbox_game.exe
    ) else if exist dream_weaver_complete.exe (
        echo ✅ Running dream_weaver_complete.exe
        dream_weaver_complete.exe
    ) else (
        echo ❌ No executable games found!
        echo Please install MinGW-w64 or Visual Studio to compile.
    )
    pause
    exit /b 1
)

echo ✅ Build successful!
echo 🚀 Starting Epoch of Elria Game Engine...
echo.

standalone_game_engine.exe

echo.
echo 🌟 Thank you for using Epoch of Elria Game Engine!
pause
