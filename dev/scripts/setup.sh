#!/bin/bash
set -e

# -------------------------
# Detect OS
# -------------------------
OS="$(uname -s)"
REPO_ROOT="$( cd "$(dirname "$0")/.." ; pwd )"
BACKEND="$REPO_ROOT/backend"
VCPKG="$REPO_ROOT/dev/vcpkg"

echo "Detected OS: $OS"

# -------------------------
# Windows-specific commands
# -------------------------
if [[ "$OS" == *"MINGW"* ]] || [[ "$OS" == *"CYGWIN"* ]] || [[ "$OS" == *"MSYS"* ]]; then
    echo "Running Windows setup..."

    if [ ! -d "$VCPKG" ]; then
        echo "Cloning vcpkg..."
        git clone https://github.com/microsoft/vcpkg.git "$VCPKG"
    fi

    cd "$VCPKG"
    if [ ! -f "vcpkg.exe" ]; then
        echo "Bootstrapping vcpkg..."
        ./bootstrap-vcpkg.bat
    fi

    cd "$BACKEND"
    "$VCPKG/vcpkg.exe" install crow asio sqlite3

    cd "$BACKEND"
    if [ -d "build" ]; then rm -rf build; fi
    mkdir build
    cd build
    cmake .. -DCMAKE_TOOLCHAIN_FILE="$VCPKG/scripts/buildsystems/vcpkg.cmake"
    cmake --build .

    echo "Running NFLFootballBackend..."
    NFLFootballBackend.exe

# -------------------------
# Linux/macOS commands
# -------------------------
else
    echo "Running Linux/macOS setup..."

    if [ ! -d "$VCPKG" ]; then
        echo "Cloning vcpkg..."
        git clone https://github.com/microsoft/vcpkg.git "$VCPKG"
    fi

    cd "$VCPKG"
    if [ ! -f "vcpkg" ]; then
        echo "Bootstrapping vcpkg..."
        ./bootstrap-vcpkg.sh
    fi

    cd "$BACKEND"
    "$VCPKG/vcpkg" install crow asio sqlite3

    cd "$BACKEND"
    if [ -d "build" ]; then rm -rf build; fi
    mkdir build
    cd build
    cmake .. -DCMAKE_TOOLCHAIN_FILE="$VCPKG/scripts/buildsystems/vcpkg.cmake"
    cmake --build .

    echo "Running NFLFootballBackend..."
    ./NFLFootballBackend
fi

# LINUX/MACOS -- RUN THIS TO MAKE IT EXECUTABLE
# chmod +x dev/scripts/setup