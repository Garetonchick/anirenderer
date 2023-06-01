#include <primitive_renderer/texture.h>
#include <cmath>
#include "utility/image.h"

namespace ani {

Texture::Texture(Image&& image) : image_(std::move(image)) { 
}

Texture::Texture(const Image& image) : image_(image) {
}

Texture::Texture(const std::string& path) : image_(LoadImage(path)) {
}

glm::vec4 Texture::Sample(glm::vec2 tex_coords) const {
    return Sample(tex_coords.x, tex_coords.y);
}

glm::vec4 Texture::Sample(float x, float y) const {
    if(!image_.GetHeight()) {
        return {1.f, 1.f, 1.f, 0.f};
    }

    RGB rgb = image_.GetPixel(ToPixelCoord(x, image_.GetWidth()), ToPixelCoord(y, image_.GetHeight()));
    return RGBToNormalizedColor(rgb);
}

int Texture::ToPixelCoord(float x, int32_t bound) const {
    int32_t coord = static_cast<int32_t>(std::round(x * static_cast<float>(bound - 1))) % bound; 
    return coord < 0 ? coord + bound : coord; 
}

}
