#pragma once
#include <iostream>

#include <glm/vec4.hpp>
#include <glm/detail/qualifier.hpp>

template <int length>
inline std::ostream& operator<<(std::ostream& o, const glm::vec<length, float, glm::defaultp>& v) {
    for (int i = 0; i < length; ++i) {
        o << v[i] << " ";
    }

    return o;
}
