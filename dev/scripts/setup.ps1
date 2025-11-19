Write-Host "=== Windows Setup Script Starting ==="

# Paths
$RepoRoot = Split-Path $PSScriptRoot -Parent
$VcpkgDir = Join-Path $RepoRoot "dev\vcpkg"
$BackendDir = Join-Path $RepoRoot "backend"
$BuildDir = Join-Path $BackendDir "build"

# Clean build directory if it exists
if (Test-Path $BuildDir) {
    Write-Host "Cleaning old build directory..."
    Remove-Item -Recurse -Force $BuildDir
}

# 1. Install vcpkg if missing
if (!(Test-Path $VcpkgDir)) {
    Write-Host "Cloning vcpkg..."
    git clone https://github.com/microsoft/vcpkg $VcpkgDir
}

# 2. Bootstrap vcpkg
Write-Host "Bootstrapping vcpkg..."
& "$VcpkgDir\bootstrap-vcpkg.bat"

# 3. Install required libraries
Write-Host "Installing vcpkg packages..."
& "$VcpkgDir\vcpkg.exe" install crow asio sqlite3 --triplet x64-windows

# 4. Configure CMake
if (!(Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

Write-Host "Running CMake configure..."
cmake -B "$BuildDir" -S "$BackendDir" `
    -G "Ninja" `
    -DCMAKE_TOOLCHAIN_FILE="$VcpkgDir/scripts/buildsystems/vcpkg.cmake" `
    -DCMAKE_BUILD_TYPE=Release

# 5. Build
Write-Host "Building project..."
cmake --build "$BuildDir" --config Release

# 6. Run executable
$Exe = Join-Path $BuildDir "Release\NFLFootballBackend.exe"
if (Test-Path $Exe) {
    Write-Host "Running backend..."
    & $Exe
} else {
    Write-Host "Executable not found."
}

Write-Host "=== Setup Complete ==="
