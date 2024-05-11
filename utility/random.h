#pragma once
#include <random>
#include <glm/vec4.hpp>

namespace ani {

std::mt19937& GetRandGen();

template <typename T>
T RandomInt(T l, T r) {
    return std::uniform_int_distribution<T>(l, r)(GetRandGen());
}

template <typename T>
T RandomReal(T l, T r) {
    return std::uniform_real_distribution<T>(l, r)(GetRandGen());
}

glm::vec4 RandomVec4(float l, float r);

}  // namespace ani
