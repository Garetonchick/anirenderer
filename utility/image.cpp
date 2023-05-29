#include "image.h"
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <utility/random.h>

namespace ani { 

    RGB NormalizedColorToRGB(const glm::vec4& color) {
        return RGB{static_cast<uint8_t>(color.r * 255.0f), 
                   static_cast<uint8_t>(color.g * 255.0f), 
                   static_cast<uint8_t>(color.b * 255.0f)};
    }

    glm::vec4 RGBToNormalizedColor(const ani::RGB& rgb) {
        return glm::vec4{static_cast<float>(rgb.r) / 255.f, 
                         static_cast<float>(rgb.g) / 255.f, 
                         static_cast<float>(rgb.b) / 255.f, 1.f};
    }

    glm::vec4 RandomColor() {
        return glm::vec4{ RandomReal(0.f, 1.f),
                          RandomReal(0.f, 1.f),
                          RandomReal(0.f, 1.f),
                          1.f }; 
    }

    Image::Image(uint32_t width, uint32_t height, const RGB& color) : width_(width), height_(height), 
        data_(width_ * height_ * kChannelsNum) {
        Fill(color);
    }

    Image::Image(const std::string& path) {
        int width;
        int height;
        int channels_num;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels_num, kChannelsNum);

        if(!data) {
            throw std::runtime_error("Couldn't load image: " + path + "\n" + stbi_failure_reason());
        }

        width_ = width;
        height_ = height;
        data_.resize(width_ * height_ * kChannelsNum);
        memcpy(&data_[0], data, data_.size());

        stbi_image_free(data);
    }

    uint32_t Image::GetWidth() const {
        return width_;
    }

    uint32_t Image::GetHeight() const {
        return height_;
    }

    RGB Image::GetPixel(uint32_t x, uint32_t y) const {
        uint32_t offset = (y * width_ + x) * kChannelsNum;
        return RGB{data_[offset], data_[offset + 1], data_[offset + 2]};
    }

    const uint8_t* Image::GetRawRGBA() const {
        return data_.data();
    }

    void Image::SetPixel(uint32_t x, uint32_t y, const RGB& color) {
        uint32_t offset = (y * width_ + x) * kChannelsNum;
        data_[offset] = color.r;
        data_[offset + 1] = color.g;
        data_[offset + 2] = color.b;
    }

    void Image::SetSize(uint32_t width, uint32_t height, const RGB& color) {
        width_ = width;
        height_ = height;
        data_.resize(width * height * kChannelsNum);
        Fill(color);
    }

    void Image::Fill(const RGB& color) {    
        for(uint32_t i = 0; i < data_.size(); i += kChannelsNum) {
            data_[i] = color.r;
            data_[i + 1] = color.g;
            data_[i + 2] = color.b;
            data_[i + 3] = 255;
        }
    }

}
