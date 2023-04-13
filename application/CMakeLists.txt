add_library(application STATIC 
    application.cpp
)
target_link_libraries(application PRIVATE renderer window camera utility sfml-includes)