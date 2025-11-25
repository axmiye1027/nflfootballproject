#!/usr/bin/env bash
set -e

echo "=== Linux/macOS Setup Script Starting ==="

# Detect repository root (parent of dev/scripts/)
REPO_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"

# Paths
VCPKG_DIR="$REPO_ROOT/dev/vcpkg"
BACKEND_DIR="$REPO_ROOT/Backend"
BUILD_DIR="$BACKEND_DIR/build"

echo "Repo root: $REPO_ROOT"
echo "Using vcpkg directory: $VCPKG_DIR"
echo "Backend directory: $BACKEND_DIR"
echo "Build directory: $BUILD_DIR"

# 1. Ensure pkg-config (macOS)
if ! command -v pkg-config >/dev/null 2>&1; then
    echo "Installing pkg-config (required by asio)..."
    brew install pkg-config
fi

# 2. Install vcpkg if missing
if [ ! -d "$VCPKG_DIR" ]; then
    echo "Cloning vcpkg into dev/vcpkg..."
    git clone https://github.com/microsoft/vcpkg "$VCPKG_DIR"
fi

# 3. Bootstrap vcpkg
echo "Bootstrapping vcpkg..."
"$VCPKG_DIR/bootstrap-vcpkg.sh"

# 4. Install libraries
echo "Installing vcpkg packages..."
"$VCPKG_DIR/vcpkg" install crow asio sqlite3 --clean-after-build

# 5. Configure build directory
mkdir -p "$BUILD_DIR"
echo "Running CMake configure..."
cmake -B "$BUILD_DIR" \
      -S "$BACKEND_DIR" \
      -DCMAKE_TOOLCHAIN_FILE="$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake"

# 6. Build backend
echo "Building project..."
cmake --build "$BUILD_DIR" --config Release

# 7. Run executable
EXEC="$BUILD_DIR/NFLFootballBackend"
if [ -f "$EXEC" ]; then
    echo "Running backend..."
    "$EXEC"
else
    echo "Executable not found at: $EXEC"
fi

echo "=== Setup Complete ==="
