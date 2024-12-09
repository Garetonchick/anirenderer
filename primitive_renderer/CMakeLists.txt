add_library(primitive_renderer STATIC 
    primitive_renderer.cpp 
    primitives.cpp 
    edge_walk.cpp 
    gradients.cpp 
    texture.cpp
    lights.cpp
)
target_link_libraries(primitive_renderer PRIVATE utility)
target_compile_options(primitive_renderer PRIVATE ${ANIRENDERER_DEBUG_FLAGS})

# Tests
add_executable(test_primitive_renderer_points "tests/test_primitive_renderer_points.cpp")
target_link_libraries(test_primitive_renderer_points PRIVATE window primitive_renderer sfml-graphics)

add_executable(test_primitive_renderer_segments "tests/test_primitive_renderer_segments.cpp")
target_link_libraries(test_primitive_renderer_segments PRIVATE window primitive_renderer sfml-graphics)

add_executable(test_primitive_renderer_triangles "tests/test_primitive_renderer_triangles.cpp")
target_link_libraries(test_primitive_renderer_triangles PRIVATE window primitive_renderer sfml-graphics)

add_executable(test_primitive_renderer_transformations "tests/test_primitive_renderer_transformations.cpp")
target_link_libraries(test_primitive_renderer_transformations PRIVATE window primitive_renderer sfml-graphics)

add_executable(test_primitive_renderer_cube "tests/test_primitive_renderer_cube.cpp")
target_link_libraries(test_primitive_renderer_cube PRIVATE window primitive_renderer sfml-graphics)

add_executable(test_textures "tests/test_textures.cpp")
target_link_libraries(test_textures PRIVATE window primitive_renderer sfml-graphics)

add_executable(test_clipping "tests/test_clipping.cpp")
target_link_libraries(test_clipping PRIVATE window primitive_renderer sfml-graphics)
