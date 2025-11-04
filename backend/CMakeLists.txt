# Minimum required version for CMake
cmake_minimum_required(VERSION 3.12)

# Include vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../dev/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Define your project name and the C++ standard you want to use
project(NFL_PROJECT_NAME VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define Windows target version (Windows 7 or later)
add_definitions(-D_WIN32_WINNT=0x0601)

# Find required packages
find_package(asio CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)

# Add your executable target
add_executable(
    ${PROJECT_NAME} 
    main.cpp
    stadium.cpp
    backendManager.cpp
)

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../dev/vcpkg/installed/x64-windows/include"
)

# Link against Crow and its dependencies
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    Crow::Crow
    asio::asio
)

# On Windows, we need to link against Winsock
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()
