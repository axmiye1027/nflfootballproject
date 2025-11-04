cmake_minimum_required(VERSION 3.14)
project(nfl_cpp_api VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch Crow and nlohmann/json
FetchContent_Declare(
  crow
  GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
  GIT_TAG        v0.3          # pick a release tag; adjust if needed
)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.2
)

FetchContent_MakeAvailable(crow nlohmann_json)

add_executable(nfl_cpp_api main.cpp)

# Crow may provide an imported target; try to link it if present,
# otherwise add the Crow include dir as a fallback:
if (TARGET Crow::Crow)
  target_link_libraries(nfl_cpp_api PRIVATE Crow::Crow)
else()
  target_include_directories(nfl_cpp_api PRIVATE ${crow_SOURCE_DIR}/include)
endif()

# link JSON lib
if (TARGET nlohmann_json::nlohmann_json)
  target_link_libraries(nfl_cpp_api PRIVATE nlohmann_json::nlohmann_json)
endif()

# link sqlite3 (system-installed)
find_package(PkgConfig REQUIRED)
pkg_search_module(SQLITE3 REQUIRED sqlite3)
target_include_directories(nfl_cpp_api PRIVATE ${SQLITE3_INCLUDE_DIRS})
target_link_libraries(nfl_cpp_api PRIVATE ${SQLITE3_LIBRARIES})