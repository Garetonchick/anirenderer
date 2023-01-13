cmake_minimum_required(VERSION 3.16)
project(Anirenderer LANGUAGES CXX)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Flags for debug
list(APPEND ANIRENDERER_DEBUG_FLAGS
    -O0
    -Wall
    -Wextra
    -Werror
    -std=c++20
)

# Add roots of includes
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/dependencies)

# Load SFML targets
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(SFML)

# Interface library to propogate sfml include directories for other targets
add_library(sfml-includes INTERFACE)
target_link_libraries(sfml-includes INTERFACE sfml-graphics)

# Create main executable
add_executable(anirenderer main.cpp)
target_compile_options(anirenderer PRIVATE ${ANIRENDERER_DEBUG_FLAGS})
message(STATUS "Anirenderer debug flags are: ${ANIRENDERER_DEBUG_FLAGS}")

# Link external dependencies to executable
target_link_libraries(anirenderer PRIVATE sfml-graphics)

# Setup clang-tidy 
set_target_properties(
    anirenderer
    PROPERTIES
        CXX_CLANG_TIDY "clang-tidy"
)

add_subdirectory(window)
add_subdirectory(utility)
add_subdirectory(primitive_renderer)
add_subdirectory(examples)
