@echo off
echo ╔══════════════════════════════════════════════════════════════╗
echo ║           The Dream Weaver's Heart - Build Script           ║
echo ╚══════════════════════════════════════════════════════════════╝
echo.

cd /d "F:\Epoch of Elria"

echo Checking for C++ compiler...
where g++ >nul 2>&1
if %errorlevel% == 0 (
    echo ✓ g++ found! Building with MinGW...
    goto :build_with_gcc
)

where cl >nul 2>&1
if %errorlevel% == 0 (
    echo ✓ Visual Studio compiler found! Building with MSVC...
    goto :build_with_msvc
)

echo ❌ No C++ compiler found!
echo.
echo Please install one of the following:
echo 1. MinGW-w64: https://www.mingw-w64.org/downloads/
echo 2. Visual Studio Community: https://visualstudio.microsoft.com/vs/community/
echo 3. Or use WSL: wsl --install
echo.
echo After installation, run this script again.
pause
exit /b 1

:build_with_gcc
echo.
echo Building The Dream Weaver's Heart with g++...
echo ═══════════════════════════════════════════════

echo Building complete game...
g++ -std=c++17 -Wall -Wextra -O2 main_dream_weaver_complete.cpp -o dream_weaver_complete.exe
if %errorlevel% neq 0 (
    echo ❌ Build failed! Trying with C++11...
    g++ -std=c++11 main_dream_weaver_complete.cpp -o dream_weaver_complete.exe
)

echo Building RPG characters demo...
g++ -std=c++17 main_rpg_characters.cpp -o rpg_characters.exe 2>nul
if %errorlevel% neq 0 (
    g++ -std=c++11 main_rpg_characters.cpp -o rpg_characters.exe
)

echo Building 3D world demo...
g++ -std=c++17 main_3d_openworld.cpp -o game_3d_openworld.exe 2>nul
if %errorlevel% neq 0 (
    g++ -std=c++11 main_3d_openworld.cpp -o game_3d_openworld.exe
)

goto :run_games

:build_with_msvc
echo.
echo Building The Dream Weaver's Heart with Visual Studio...
echo ═══════════════════════════════════════════════════════

echo Building complete game...
cl /EHsc /std:c++17 main_dream_weaver_complete.cpp /Fe:dream_weaver_complete.exe >nul 2>&1
if %errorlevel% neq 0 (
    echo ❌ C++17 build failed! Trying without version flag...
    cl /EHsc main_dream_weaver_complete.cpp /Fe:dream_weaver_complete.exe >nul 2>&1
)

echo Building RPG characters demo...
cl /EHsc main_rpg_characters.cpp /Fe:rpg_characters.exe >nul 2>&1

echo Building 3D world demo...
cl /EHsc main_3d_openworld.cpp /Fe:game_3d_openworld.exe >nul 2>&1

goto :run_games

:run_games
echo.
echo ✓ Build complete!
echo.
echo Available games:
echo ═══════════════
if exist dream_weaver_complete.exe (
    echo ✓ dream_weaver_complete.exe - Complete Dream Weaver's Heart experience
)
if exist rpg_characters.exe (
    echo ✓ rpg_characters.exe - Character system demo
)
if exist game_3d_openworld.exe (
    echo ✓ game_3d_openworld.exe - 3D world exploration
)

echo.
echo Choose which game to run:
echo 1. Complete Dream Weaver's Heart (recommended)
echo 2. RPG Characters Demo
echo 3. 3D World Exploration
echo 4. Exit
echo.
set /p choice="Enter your choice (1-4): "

if "%choice%"=="1" (
    if exist dream_weaver_complete.exe (
        echo.
        echo Starting The Dream Weaver's Heart...
        echo ═══════════════════════════════════════
        dream_weaver_complete.exe
    ) else (
        echo ❌ dream_weaver_complete.exe not found!
    )
)

if "%choice%"=="2" (
    if exist rpg_characters.exe (
        echo.
        echo Starting RPG Characters Demo...
        echo ═══════════════════════════════════
        rpg_characters.exe
    ) else (
        echo ❌ rpg_characters.exe not found!
    )
)

if "%choice%"=="3" (
    if exist game_3d_openworld.exe (
        echo.
        echo Starting 3D World Exploration...
        echo ═══════════════════════════════════
        game_3d_openworld.exe
    ) else (
        echo ❌ game_3d_openworld.exe not found!
    )
)

if "%choice%"=="4" (
    echo Goodbye!
    exit /b 0
)

echo.
echo Game finished. Press any key to exit...
pause >nul
