add_library(renderer STATIC 
    renderer.cpp
    model.cpp
)
target_link_libraries(renderer PRIVATE utility primitive_renderer)

# Tests
add_executable(test_model "tests/test_model.cpp")
target_link_libraries(test_model PRIVATE window renderer sfml-graphics)