# Minimum required version for CMake
cmake_minimum_required(VERSION 3.12)

# Include vcpkg toolchain file
# set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../dev/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Define your project name and the C++ standard you want to use
project(NFL_PROJECT_NAME VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define Windows target version (Windows 7 or later)
add_definitions(-D_WIN32_WINNT=0x0601)

include (cmake/CPM.cmake)

CPMAddPackage("gh:CrowCpp/Crow@1.3.0")

CPMAddPackage("gh:nlohmann/json@3.10.5")

find_package(Threads REQUIRED)

CPMAddPackage("gh:chriskohlhoff/asio#asio-1-18-1@1.18.1")

# ASIO doesn't use CMake, we have to configure it manually. Extra notes for using on Windows:
#
# 1) If _WIN32_WINNT is not set, ASIO assumes _WIN32_WINNT=0x0501, i.e. Windows XP target, which is
# definitely not the platform which most users target.
#
# 2) WIN32_LEAN_AND_MEAN is defined to make Winsock2 work.
if(asio_ADDED)
  add_library(asio INTERFACE)

  target_include_directories(asio SYSTEM INTERFACE ${asio_SOURCE_DIR}/asio/include)

  target_compile_definitions(asio INTERFACE ASIO_STANDALONE ASIO_NO_DEPRECATED)

  target_link_libraries(asio INTERFACE Threads::Threads)

  if(WIN32)
    # macro see @ https://stackoverflow.com/a/40217291/1746503
    macro(get_win32_winnt version)
      if(CMAKE_SYSTEM_VERSION)
        set(ver ${CMAKE_SYSTEM_VERSION})
        string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
        string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
        # Check for Windows 10, b/c we'll need to convert to hex 'A'.
        if("${verMajor}" MATCHES "10")
          set(verMajor "A")
          string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
        endif("${verMajor}" MATCHES "10")
        # Remove all remaining '.' characters.
        string(REPLACE "." "" ver ${ver})
        # Prepend each digit with a zero.
        string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
        set(${version} "0x${ver}")
      endif()
    endmacro()

    if(NOT DEFINED _WIN32_WINNT)
      get_win32_winnt(ver)
      set(_WIN32_WINNT ${ver})
    endif()

    message(STATUS "Set _WIN32_WINNET=${_WIN32_WINNT}")

    target_compile_definitions(asio INTERFACE _WIN32_WINNT=${_WIN32_WINNT} WIN32_LEAN_AND_MEAN)
  endif()
endif()

# Find required packages
# find_package(asio CONFIG REQUIRED)
# find_package(Crow CONFIG REQUIRED)

# Add your executable target
add_executable(
    ${PROJECT_NAME} 
    main.cpp
    stadium.cpp
    backendManager.cpp
)

# Add include directories
# target_include_directories(${PROJECT_NAME} PRIVATE
#     "${CMAKE_CURRENT_SOURCE_DIR}/../dev/vcpkg/installed/x64-windows/include"
# )

# Link against Crow and its dependencies
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    Crow::Crow
    asio
    nlohmann_json::nlohmann_json
)

# On Windows, we need to link against Winsock
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()