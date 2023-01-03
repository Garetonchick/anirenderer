#pragma once
#include <cstdint>
#include <vector>

namespace ani {
struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct Image {
    std::vector<std::vector<RGB>> pixels;
};
}  // namespace ani
