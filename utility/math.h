#pragma once
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace ani {

template<class T>
T Lerp(const T& val1, const T& val2, float lerp_amount) {
    return val1 + (val2 - val1) * lerp_amount;
}

inline glm::vec4 PerspectiveDivide(const glm::vec4& v) {
    return glm::vec4{v.x / v.w, v.y / v.w, v.z / v.w, 1.0}; 
}

inline glm::mat4x4 ScreenSpaceTransformMatrix(int32_t width, int32_t height) {
    float half_w = static_cast<float>(width - 1) * 0.5f;
    float half_h = static_cast<float>(height - 1) * 0.5f;

    return glm::mat4x4{{half_w, 0.f, 0.f, 0.f},
                       {0.f, -half_h, 0.f, 0.f},
                       {0.f, 0.f, 1.f, 0.f},
                       {half_w, half_h, 0.f, 1.f}};
}

}
