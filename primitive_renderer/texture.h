#pragma once
#include <utility/image.h>
#include <glm/vec2.hpp>

namespace ani {

class Texture {
public:
    Texture() = default;
    Texture(Image&& image);
    Texture(const std::string& path);
    Texture(const Image& image);

    glm::vec4 Sample(glm::vec2 tex_coords) const;
    glm::vec4 Sample(float x, float y) const;

private:
    int ToPixelCoord(float x, int32_t bound) const;

    Image image_;
};
}  // namespace ani
