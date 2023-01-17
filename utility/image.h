#pragma once
#include <glm/vec4.hpp>

#include <cstdint>
#include <vector>

namespace ani {
struct RGB {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};

RGB NormalizedColorToRGB(const glm::vec4& color);

class Image {
public:
    explicit Image(uint32_t width = 0, uint32_t height = 0, const RGB& color = {});

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    RGB GetPixel(uint32_t x, uint32_t y) const;
    const uint8_t* GetRawRGBA() const;

    void SetPixel(uint32_t x, uint32_t y, const RGB& color);
    void SetSize(uint32_t width, uint32_t height, const RGB& color = {});
    void Fill(const RGB& color);
    
private:
    static const uint32_t kChannelsNum = 4;

    uint32_t width_;
    uint32_t height_;
    std::vector<uint8_t> data_;
};
}  // namespace ani
