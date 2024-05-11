add_library(window STATIC window.cpp)
target_link_libraries(window PRIVATE sfml-includes utility sfml-graphics)
target_compile_options(window PRIVATE ${ANIRENDERER_DEBUG_FLAGS})

# Test program
add_executable(test_window "tests/test_window.cpp")
target_link_libraries(test_window PRIVATE window sfml-graphics)
