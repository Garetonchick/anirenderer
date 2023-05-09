#include <utility/random.h>

namespace ani {

std::mt19937 rnd;

glm::vec4 RandomVec4(float l, float r) {
    return glm::vec4{ RandomReal(l, r), 
                      RandomReal(l, r),
                      RandomReal(l, r),
                      RandomReal(l, r) };
}

}
