@echo off
echo ╔══════════════════════════════════════════════════════════════╗
echo ║              EPOCH OF ELRIA GAME ENGINE                     ║
echo ║                   Windowed Application                      ║
echo ╚══════════════════════════════════════════════════════════════╝
echo.
echo 🌍 Starting Epoch of Elria 3D Game Engine...

REM Check if windowed app engine exists, if not build it
if not exist "EpochOfElriaEngine.exe" (
    echo 🔨 Building windowed 3D application...
    g++ -std=c++17 -O2 -mwindows windowed_app_engine.cpp -o EpochOfElriaEngine.exe -lopengl32 -lglu32 -lgdi32 -luser32

    if %errorlevel% neq 0 (
        echo ❌ OpenGL build failed! Trying fallback windowed version...
        g++ -std=c++17 -O2 windowed_game_engine.cpp -o windowed_game_engine.exe

        if %errorlevel% neq 0 (
            echo ❌ Windowed build failed! Trying standalone version...
            if not exist "standalone_game_engine.exe" (
                g++ -std=c++17 -O2 standalone_game_engine.cpp -o standalone_game_engine.exe
            )

            if exist "standalone_game_engine.exe" (
                echo 🚀 Starting standalone engine...
                standalone_game_engine.exe
            ) else (
                echo ❌ Could not build engine. Trying existing games...
                if exist "simple_3d_game.exe" (
                    simple_3d_game.exe
                ) else if exist "dream_weaver_complete.exe" (
                    dream_weaver_complete.exe
                ) else (
                    echo ❌ No games available. Please install MinGW-w64 or Visual Studio.
                    echo.
                    echo Required: g++ compiler with OpenGL support
                    echo Download: https://www.mingw-w64.org/downloads/
                    pause
                )
            )
            exit /b 1
        ) else (
            echo ✅ Fallback windowed version built successfully!
            echo 🚀 Launching windowed game engine...
            windowed_game_engine.exe
            exit /b 0
        )
    ) else (
        echo ✅ 3D windowed application built successfully!
    )
)

echo 🚀 Launching Epoch of Elria 3D Game Engine...
echo 🎮 Opening application window...
start "" "EpochOfElriaEngine.exe"
