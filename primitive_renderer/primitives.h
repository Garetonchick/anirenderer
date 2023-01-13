#pragma once
#include <utility/utility.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cstdint>
#include <array>

namespace ani {

struct Point {
    glm::vec3 pos;
    glm::vec4 color;
};

struct Segment {
    Point points[2];
};

struct Triangle {
    Point points[3];
};

}  // namespace ani
