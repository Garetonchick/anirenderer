#pragma once
#include <random>
#include <glm/vec4.hpp>

namespace ani {

extern std::mt19937 rnd;

template<typename T>
T RandomInt(T l, T r) {
    return std::uniform_int_distribution<T>(l, r)(rnd);
}

template<typename T>
T RandomReal(T l, T r) {
    return std::uniform_real_distribution<T>(l, r)(rnd);
}

glm::vec4 RandomVec4(float l, float r);

}