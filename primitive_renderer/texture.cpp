#include <primitive_renderer/texture.h>
#include <cmath>
#include "utility/image.h"

namespace ani {

Texture::Texture(const Image& image) : image_(image) {
}

glm::vec4 Texture::Sample(glm::vec2 tex_coords) const {
    return Sample(tex_coords.x, tex_coords.y);
}

glm::vec4 Texture::Sample(float x, float y) const {
    if(!image_.GetHeight()) {
        return {1.f, 1.f, 1.f, 1.f};
    }

    auto to_pixel_coord = [](float x, int32_t pixels_cnt) {
        int32_t coord = static_cast<int32_t>(std::round(x * static_cast<float>(pixels_cnt - 1))) % pixels_cnt; 
        return coord < 0 ? coord + pixels_cnt : coord; 
    };

    RGB rgb = image_.GetPixel(to_pixel_coord(x, image_.GetWidth()), to_pixel_coord(y, image_.GetHeight()));
    return RGBToNormalizedColor(rgb);
}

}