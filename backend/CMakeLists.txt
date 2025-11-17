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
# Vcpkg integration
# -------------------------
# If you haven't installed vcpkg yet, this won't break
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../dev/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../dev/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

# link libraries
# find_package(crow CONFIG REQUIRED)      # For Crow
find_package(asio CONFIG REQUIRED)      # For standalone Asio
find_package(SQLite3 REQUIRED)   # For SQLite3
target_link_libraries(
  ${PROJECT_NAME} 
  PRIVATE  
  asio 
  SQLite::SQLite3
)
