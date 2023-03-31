add_library(camera STATIC 
    camera.cpp
)

# Tests
add_executable(test_camera "tests/test_camera.cpp")
target_link_libraries(test_camera PRIVATE window renderer sfml-graphics camera)