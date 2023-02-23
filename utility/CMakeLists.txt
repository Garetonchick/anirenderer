add_library(utility STATIC image.cpp random.cpp)
target_compile_options(utility PRIVATE ${ANIRENDERER_DEBUG_FLAGS})
