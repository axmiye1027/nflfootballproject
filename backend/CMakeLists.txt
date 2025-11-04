# Minimum required version for CMake
cmake_minimum_required(VERSION 3.12)

# Include vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../dev/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Define your project name and the C++ standard you want to use
project(NFLFootballBackend VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define Windows target version (Windows 7 or later)
if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
endif()

# --- Dependencies ---
find_package(Crow CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(SQLite3 REQUIRED)

# --- Include directories ---
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# --- Source files ---
# Automatically gather all .cpp files in src/
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# --- Executable target ---
add_executable(
    ${PROJECT_NAME} 
    ${SRC_FILES}
)

# Link against Crow and its dependencies
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    Crow::Crow
    asio::asio
    SQLite::SQLite3
)

# On Windows, we need to link against Winsock
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()

# --- Optional: output directory ---
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)