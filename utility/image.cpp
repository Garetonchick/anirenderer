#include "image.h"
#include <memory>
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

    glm::vec4 RGBToNormalizedColor(RGB rgb) {
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

    Image::Image(uint32_t width, uint32_t height, RGB color) : width_(width), height_(height), 
        data_(width_ * height_ * kChannelsNum) {
        Fill(color);
    }

    Image::Image(const unsigned char* data, uint32_t width, uint32_t height) {
        width_ = width;
        height_ = height;
        data_.resize(width_ * height_ * kChannelsNum);
        memcpy(&data_[0], data, data_.size());
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

    void Image::SetPixel(uint32_t x, uint32_t y, RGB color) {
        uint32_t offset = (y * width_ + x) * kChannelsNum;
        data_[offset] = color.r;
        data_[offset + 1] = color.g;
        data_[offset + 2] = color.b;
    }

    void Image::SetSize(uint32_t width, uint32_t height, RGB color) {
        width_ = width;
        height_ = height;
        data_.resize(width * height * kChannelsNum);
        Fill(color);
    }

    void Image::Resize(uint32_t width, uint32_t height) {
        std::vector<uint8_t> new_data; 

        for(int i = 0; i < width; ++i) {
            for(int j = 0; j < height; ++j) {
                int off = 4 * (j * width + i);
                int x = i * width_ / width;
                int y = j * height_ / height;
                RGB color = GetPixel(x, y);
                memcpy(new_data.data() + off, data_.data() + 4 * (y * width_ + x), 4);
            }
        }

        std::swap(data_, new_data);
    }

    void Image::Fill(RGB color) {    
        for(uint32_t i = 0; i < data_.size(); i += kChannelsNum) {
            data_[i] = color.r;
            data_[i + 1] = color.g;
            data_[i + 2] = color.b;
            data_[i + 3] = 255;
        }
    }

    Image LoadImage(const std::string& path) {
        int width;
        int height;
        int channels_num;
        constexpr int kChannelsNum = 4;
        unsigned char *data = nullptr;
        Image image;

        try {
            data = stbi_load(path.c_str(), &width, &height, &channels_num, kChannelsNum);

            if(!data) {
                throw std::runtime_error("Couldn't load image: " + path + "\n" + stbi_failure_reason());
            }

            image = Image(data, width, height);
        } catch(...) {
            stbi_image_free(data);
        }

        return image;
    }

}
