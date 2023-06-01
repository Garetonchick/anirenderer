#pragma once
#include <glm/vec4.hpp>

#include <cstdint>
#include <vector>
#include <string>

namespace ani {
struct RGB {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};

RGB NormalizedColorToRGB(const glm::vec4& color);
glm::vec4 RGBToNormalizedColor(RGB rgb);

glm::vec4 RandomColor();

class Image {
public:
    explicit Image(uint32_t width = 0, uint32_t height = 0, RGB color = {});
    Image(const unsigned char* data, uint32_t width, uint32_t height);

    Image(const Image&) = default;
    Image& operator=(const Image&) = default;
    Image(Image&&) = default;
    Image& operator=(Image&&) = default;
    ~Image() = default;

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    RGB GetPixel(uint32_t x, uint32_t y) const;
    const uint8_t* GetRawRGBA() const;

    void SetPixel(uint32_t x, uint32_t y, RGB color);
    void SetSize(uint32_t width, uint32_t height, RGB color = {});
    void Resize(uint32_t width, uint32_t height);
    void Fill(RGB color);

private:
    static constexpr uint32_t kChannelsNum = 4;

    uint32_t width_;
    uint32_t height_;
    std::vector<uint8_t> data_;
};

Image LoadImage(const std::string& path);

}  // namespace ani
