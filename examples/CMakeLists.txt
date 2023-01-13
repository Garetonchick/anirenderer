add_executable(starfield "starfield.cpp")
target_link_libraries(starfield PRIVATE window primitive_renderer sfml-graphics)
