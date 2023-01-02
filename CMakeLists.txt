cmake_minimum_required(VERSION 3.16)
project(Anirenderer LANGUAGES CXX)

# Load SFML targets
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(SFML)

# Create main executable
add_executable(anirenderer main.cpp)
target_compile_options(anirenderer PRIVATE "-O0;-Wall;-Wextra;-Werror;-std=c++20;")

# Link external dependencies to executable
target_link_libraries(anirenderer PRIVATE sfml-graphics)
