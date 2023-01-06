#include <utility/image.h>

namespace ani { 
    Image::Image(uint32_t width, uint32_t height, const RGB& color) : width_(width), height_(height), 
        data_(width_ * height_ * kChannelsNum) {

        for(uint32_t i = 0; i < data_.size(); i += kChannelsNum) {
            data_[i] = color.r;
            data_[i + 1] = color.g;
            data_[i + 2] = color.b;
            data_[i + 3] = 255;
        }
    }

    uint32_t Image::GetWidth() const {
        return width_;
    }

    uint32_t Image::GetHeight() const {
        return height_;
    }

    RGB Image::GetPixel(uint32_t row, uint32_t col) const {
        uint32_t offset = (row * width_ + col) * kChannelsNum;
        return RGB{data_[offset], data_[offset + 1], data_[offset + 2]};
    }

    const uint8_t* Image::GetRawRGBA() const {
        return data_.data();
    }

    void Image::SetPixel(uint32_t row, uint32_t col, const RGB& color) {
        uint32_t offset = (row * width_ + col) * kChannelsNum;
        data_[offset] = color.r;
        data_[offset + 1] = color.g;
        data_[offset + 2] = color.b;
        data_[offset + 3] = 255;
    }

    void Image::SetSize(uint32_t width, uint32_t height) {
        width_ = width;
        height_ = height;
        data_.resize(width * height * kChannelsNum);
    }

}
