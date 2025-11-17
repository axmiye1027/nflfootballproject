cmake_minimum_required(VERSION 3.12)
project(NFLFootballBackend)

# -------------------------
# C++ standard
# -------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------
# Include headers
# -------------------------
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# -------------------------
# Find all source files automatically
# -------------------------
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# -------------------------
# Create executable
# -------------------------
add_executable(${PROJECT_NAME} ${SOURCES})

# -------------------------
# Vcpkg libraries (link via targets)
# -------------------------
find_package(asio CONFIG REQUIRED)                # standalone Asio
find_package(Crow CONFIG REQUIRED)                # Crow (optional if you use it)
find_package(unofficial-sqlite3 CONFIG REQUIRED)  # SQLite3 via vcpkg

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    asio::asio
    Crow::Crow
    unofficial::sqlite3::sqlite3
)
