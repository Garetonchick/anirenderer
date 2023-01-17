#pragma once
#include <glm/vec4.hpp>
#include <iostream>

inline std::ostream& operator<<(std::ostream& o, const glm::vec4& v) {
    o << v.x << " " << v.y << " " << v.z << " " << v.w;

    return o;
}
