cmake_minimum_required(VERSION 3.16)
project(Anirenderer LANGUAGES CXX)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND ANIRENDERER_FLAGS)

# Flags 
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    list(APPEND ANIRENDERER_FLAGS
        -O3
        -std=c++20
        -Wall
        -Wextra
        -Werror
    )
endif()

# Load SFML targets
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(SFML)

add_compile_definitions(ANIRENDERER_PATH="${CMAKE_CURRENT_LIST_DIR}")

# Interface library to propogate sfml include directories for other targets
add_library(sfml-includes INTERFACE)
target_link_libraries(sfml-includes INTERFACE sfml-graphics)

# Create main executable
add_executable(anirenderer main.cpp)
target_compile_options(anirenderer PRIVATE ${ANIRENDERER_FLAGS})
message(STATUS "Anirenderer flags are: ${ANIRENDERER_FLAGS}")

# Link external dependencies to executable
target_link_libraries(anirenderer PRIVATE application sfml-includes)

# Add roots of includes
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/dependencies)
include_directories(${CMAKE_CURRENT_LIST_DIR}/glm)

#Setup clang-tidy 
set_target_properties(
    anirenderer
    PROPERTIES
        CXX_CLANG_TIDY "clang-tidy"
)

add_subdirectory(application)
add_subdirectory(window)
add_subdirectory(utility)
add_subdirectory(primitive_renderer)
add_subdirectory(renderer)
add_subdirectory(camera)
