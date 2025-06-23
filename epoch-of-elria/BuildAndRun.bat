@echo off
title Epoch of Elria - Game Engine Builder

echo ╔══════════════════════════════════════════════════════════════╗
echo ║              EPOCH OF ELRIA GAME ENGINE                     ║
echo ║                Building Windowed Application                ║
echo ╚══════════════════════════════════════════════════════════════╝
echo.

echo 🔨 Building windowed .exe application...
echo.

REM Build the windowed OpenGL application
g++ -std=c++17 -O2 -mwindows windowed_app_engine.cpp -o EpochOfElriaEngine.exe -lopengl32 -lglu32 -lgdi32 -luser32

if %errorlevel% neq 0 (
    echo ❌ OpenGL build failed! Trying simpler windowed version...
    echo.
    
    REM Try building without OpenGL
    g++ -std=c++17 -O2 windowed_game_engine.cpp -o WindowedEngine.exe
    
    if %errorlevel% neq 0 (
        echo ❌ Windowed build failed! Using console version...
        echo.
        
        REM Fallback to console version
        g++ -std=c++17 -O2 standalone_game_engine.cpp -o ConsoleEngine.exe
        
        if %errorlevel% neq 0 (
            echo ❌ All builds failed! Please install MinGW-w64 or Visual Studio.
            echo.
            echo Download MinGW-w64: https://www.mingw-w64.org/downloads/
            echo.
            pause
            exit /b 1
        ) else (
            echo ✅ Console version built successfully!
            echo 🚀 Starting console game engine...
            echo.
            ConsoleEngine.exe
        )
    ) else (
        echo ✅ Windowed version built successfully!
        echo 🚀 Starting windowed game engine...
        echo.
        start "" WindowedEngine.exe
    )
) else (
    echo ✅ OpenGL windowed application built successfully!
    echo 🚀 Opening 3D game engine window...
    echo.
    echo The application window should open now!
    echo If no window appears, check Windows Defender or antivirus settings.
    echo.
    start "" EpochOfElriaEngine.exe
)

echo.
echo 🌟 Epoch of Elria Game Engine launched!
echo 🎮 Check for the application window on your screen.
echo.
pause
