#pragma once
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace ani {

template <typename T>
T Bpow(T val, int32_t power) {
    if (!power) {
        return T(1);
    }
    T tmp = Bpow(val, power >> 1);
    if (power & 1) {
        return tmp * tmp * val;
    }
    return tmp * tmp;
}

template <class T>
T Lerp(const T& val1, const T& val2, float lerp_amount) {
    return val1 + (val2 - val1) * lerp_amount;
}

inline glm::vec4 PerspectiveDivide(const glm::vec4& v) {
    return glm::vec4{v.x / v.w, v.y / v.w, v.z / v.w, 1.0};
}

inline glm::vec4 Mix(const glm::vec4& c1, const glm::vec4& c2) {
    return {c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a};
}

}  // namespace ani
