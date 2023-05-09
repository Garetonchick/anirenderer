#pragma once
#include <cstdint>
#include <memory>

template <typename T>
class Array2D {
public:
    Array2D(uint32_t width, uint32_t height, const T& fill_val = {})
        : arr_(std::make_unique<T[]>(width * height)), width_(width), height_(height) {
        Fill(fill_val);
    }

    void Resize(uint32_t width, uint32_t height) {
        width_ = width;
        height_ = height;
        arr_ = std::make_unique<T[]>(width * height);
    }

    T& operator()(uint32_t i, uint32_t j) {
        return arr_[i * width_ + j];
    }

    const T& operator()(uint32_t i, uint32_t j) const {
        return arr_[i * width_ + j];
    }

    void Set(uint32_t i, uint32_t j, T val) {
        arr_[i * width_ + j] = val;
    }

    const T& Get(uint32_t i, uint32_t j) const {
        return arr_[i * width_ + j];
    }

    uint32_t GetWidth() const {
        return width_;
    }

    uint32_t GetHeight() const {
        return height_;
    }

    void Fill(const T& val) {
        for (uint32_t i = 0; i < width_ * height_; ++i) {
            arr_[i] = val;
        }
    }

private:
    std::unique_ptr<T[]> arr_;
    uint32_t width_;
    uint32_t height_;
};
