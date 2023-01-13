#include <utility/image.h>

namespace ani { 
    Image::Image(uint32_t width, uint32_t height, const RGB& color) : width_(width), height_(height), 
        data_(width_ * height_ * kChannelsNum) {
        Fill(color);
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