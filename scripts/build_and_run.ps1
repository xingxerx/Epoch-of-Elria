# The Dream Weaver's Heart - PowerShell Build Script

Write-Host "╔══════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║           The Dream Weaver's Heart - Build Script           ║" -ForegroundColor Cyan  
Write-Host "╚══════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan
Write-Host ""

Set-Location "F:\Epoch of Elria"

Write-Host "Checking for C++ compiler..." -ForegroundColor Yellow

# Check for g++
$gccFound = $false
$msvcFound = $false

try {
    $null = Get-Command g++ -ErrorAction Stop
    $gccFound = $true
    Write-Host "✓ g++ found! Building with MinGW..." -ForegroundColor Green
} catch {
    try {
        $null = Get-Command cl -ErrorAction Stop
        $msvcFound = $true
        Write-Host "✓ Visual Studio compiler found! Building with MSVC..." -ForegroundColor Green
    } catch {
        Write-Host "❌ No C++ compiler found!" -ForegroundColor Red
        Write-Host ""
        Write-Host "Please install one of the following:" -ForegroundColor Yellow
        Write-Host "1. MinGW-w64: https://www.mingw-w64.org/downloads/" -ForegroundColor White
        Write-Host "2. Visual Studio Community: https://visualstudio.microsoft.com/vs/community/" -ForegroundColor White
        Write-Host "3. Or use WSL: wsl --install" -ForegroundColor White
        Write-Host ""
        Write-Host "After installation, run this script again." -ForegroundColor Yellow
        Read-Host "Press Enter to exit"
        exit 1
    }
}

Write-Host ""
Write-Host "Building The Dream Weaver's Heart..." -ForegroundColor Cyan
Write-Host "═══════════════════════════════════════════════" -ForegroundColor Cyan

function Build-WithGCC {
    Write-Host "Building complete game..." -ForegroundColor Yellow
    $result = Start-Process -FilePath "g++" -ArgumentList "-std=c++17", "-Wall", "-Wextra", "-O2", "main_dream_weaver_complete.cpp", "-o", "dream_weaver_complete.exe" -Wait -PassThru -NoNewWindow
    
    if ($result.ExitCode -ne 0) {
        Write-Host "❌ C++17 build failed! Trying with C++11..." -ForegroundColor Red
        $result = Start-Process -FilePath "g++" -ArgumentList "-std=c++11", "main_dream_weaver_complete.cpp", "-o", "dream_weaver_complete.exe" -Wait -PassThru -NoNewWindow
    }
    
    Write-Host "Building RPG characters demo..." -ForegroundColor Yellow
    Start-Process -FilePath "g++" -ArgumentList "-std=c++17", "main_rpg_characters.cpp", "-o", "rpg_characters.exe" -Wait -PassThru -NoNewWindow | Out-Null
    
    Write-Host "Building 3D world demo..." -ForegroundColor Yellow
    Start-Process -FilePath "g++" -ArgumentList "-std=c++17", "main_3d_openworld.cpp", "-o", "game_3d_openworld.exe" -Wait -PassThru -NoNewWindow | Out-Null
}

function Build-WithMSVC {
    Write-Host "Building complete game..." -ForegroundColor Yellow
    $result = Start-Process -FilePath "cl" -ArgumentList "/EHsc", "/std:c++17", "main_dream_weaver_complete.cpp", "/Fe:dream_weaver_complete.exe" -Wait -PassThru -NoNewWindow
    
    if ($result.ExitCode -ne 0) {
        Write-Host "❌ C++17 build failed! Trying without version flag..." -ForegroundColor Red
        Start-Process -FilePath "cl" -ArgumentList "/EHsc", "main_dream_weaver_complete.cpp", "/Fe:dream_weaver_complete.exe" -Wait -PassThru -NoNewWindow | Out-Null
    }
    
    Write-Host "Building RPG characters demo..." -ForegroundColor Yellow
    Start-Process -FilePath "cl" -ArgumentList "/EHsc", "main_rpg_characters.cpp", "/Fe:rpg_characters.exe" -Wait -PassThru -NoNewWindow | Out-Null
    
    Write-Host "Building 3D world demo..." -ForegroundColor Yellow
    Start-Process -FilePath "cl" -ArgumentList "/EHsc", "main_3d_openworld.cpp", "/Fe:game_3d_openworld.exe" -Wait -PassThru -NoNewWindow | Out-Null
}

if ($gccFound) {
    Build-WithGCC
} elseif ($msvcFound) {
    Build-WithMSVC
}

Write-Host ""
Write-Host "✓ Build complete!" -ForegroundColor Green
Write-Host ""
Write-Host "Available games:" -ForegroundColor Cyan
Write-Host "═══════════════" -ForegroundColor Cyan

$games = @()
if (Test-Path "dream_weaver_complete.exe") {
    Write-Host "✓ dream_weaver_complete.exe - Complete Dream Weaver's Heart experience" -ForegroundColor Green
    $games += @{Name="Complete Dream Weaver's Heart (recommended)"; File="dream_weaver_complete.exe"}
}
if (Test-Path "rpg_characters.exe") {
    Write-Host "✓ rpg_characters.exe - Character system demo" -ForegroundColor Green
    $games += @{Name="RPG Characters Demo"; File="rpg_characters.exe"}
}
if (Test-Path "game_3d_openworld.exe") {
    Write-Host "✓ game_3d_openworld.exe - 3D world exploration" -ForegroundColor Green
    $games += @{Name="3D World Exploration"; File="game_3d_openworld.exe"}
}

if ($games.Count -eq 0) {
    Write-Host "❌ No games were built successfully!" -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Host ""
Write-Host "Choose which game to run:" -ForegroundColor Yellow
for ($i = 0; $i -lt $games.Count; $i++) {
    Write-Host "$($i + 1). $($games[$i].Name)" -ForegroundColor White
}
Write-Host "$($games.Count + 1). Exit" -ForegroundColor White

do {
    $choice = Read-Host "`nEnter your choice (1-$($games.Count + 1))"
    $choiceNum = [int]$choice
} while ($choiceNum -lt 1 -or $choiceNum -gt ($games.Count + 1))

if ($choiceNum -le $games.Count) {
    $selectedGame = $games[$choiceNum - 1]
    Write-Host ""
    Write-Host "Starting $($selectedGame.Name)..." -ForegroundColor Cyan
    Write-Host "═══════════════════════════════════════" -ForegroundColor Cyan
    
    try {
        & ".\$($selectedGame.File)"
    } catch {
        Write-Host "❌ Failed to run $($selectedGame.File): $($_.Exception.Message)" -ForegroundColor Red
    }
} else {
    Write-Host "Goodbye!" -ForegroundColor Green
    exit 0
}

Write-Host ""
Write-Host "Game finished. Press Enter to exit..." -ForegroundColor Yellow
Read-Host
