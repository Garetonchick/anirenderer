add_library(utility STATIC image.cpp)
target_compile_options(utility PRIVATE ${ANIRENDERER_DEBUG_FLAGS})
