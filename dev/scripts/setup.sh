#!/usr/bin/env bash
set -e

echo "=== Linux/macOS Setup Script Starting ==="

REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
VCPKG_DIR="$REPO_ROOT/dev/vcpkg"
BACKEND_DIR="$REPO_ROOT/backend"
BUILD_DIR="$BACKEND_DIR/build"

# 1. Install vcpkg if missing
if [ ! -d "$VCPKG_DIR" ]; then
    echo "Cloning vcpkg..."
    git clone https://github.com/microsoft/vcpkg "$VCPKG_DIR"
fi

# 2. Bootstrap vcpkg
echo "Bootstrapping vcpkg..."
"$VCPKG_DIR/bootstrap-vcpkg.sh"

# 3. Install packages
echo "Installing vcpkg packages..."
"$VCPKG_DIR/vcpkg" install crow asio sqlite3

# 4. Configure CMake
mkdir -p "$BUILD_DIR"
echo "Running CMake configure..."
cmake -B "$BUILD_DIR" -S "$BACKEND_DIR" -DCMAKE_TOOLCHAIN_FILE="$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake"

# 5. Build
echo "Building project..."
cmake --build "$BUILD_DIR" --config Release

# 6. Run executable
"$BUILD_DIR/NFLFootballBackend"

echo "=== Setup Complete ==="
