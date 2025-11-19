Write-Host "=== Windows Setup Script Starting ==="

# Determine script, dev, and project directories
$ScriptDir  = Split-Path -Parent $MyInvocation.MyCommand.Path        # /dev/scripts
$DevDir     = Split-Path -Parent $ScriptDir                          # /dev
$ProjectDir = Split-Path -Parent $DevDir                             # /project root

# Key folders
$VcpkgDir   = Join-Path $DevDir     "vcpkg"                          # /dev/vcpkg
$BackendDir = Join-Path $ProjectDir "backend"                        # /backend
$BuildDir   = Join-Path $BackendDir "build"                          # /backend/build

Write-Host "Repo root:   $RepoRoot"
Write-Host "vcpkg dir:   $VcpkgDir"
Write-Host "Backend dir: $BackendDir"
Write-Host "Build dir:   $BuildDir"

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
