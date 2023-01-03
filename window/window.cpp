#include <window/window.h>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <iostream>

namespace ani {

// Public members

Window::Window(uint32_t width, uint32_t height, const std::string& title)
    : sfml_window_(sf::VideoMode(width, height), title.c_str()) {
    sfml_window_.SetResizeCallback([this]() {
        std::cerr << "Callback was really called" << std::endl;
        this->Reset();
        this->Clear();
    });
    Reset();
    Clear();
}

bool Window::IsOpen() const {
    return sfml_window_.isOpen();
}

uint32_t Window::GetWidth() const {
    return sfml_window_.getSize().x;
}

uint32_t Window::GetHeight() const {
    return sfml_window_.getSize().y;
}

void Window::SetSize(uint32_t width, uint32_t height) {
    sfml_window_.setSize({width, height});
    Reset();
    Clear();
}

void Window::Display(const Image& image) {
    CopyImageToScreenBuffer(image);

    texture_.update(screen_buf_.get());
    sfml_window_.draw(sprite_);
    sfml_window_.display();
}

void Window::Clear(const RGB& color) {
    sfml_window_.clear({color.r, color.g, color.b});
}

// Private members

void Window::CopyImageToScreenBuffer(const Image& image) {
    auto rows_num = std::min<size_t>(image.pixels.size(), GetHeight());

    for (uint32_t i = 0; i < rows_num; ++i) {
        auto cols_num = std::min<size_t>(image.pixels[i].size(), GetWidth());

        for (uint32_t j = 0; j < cols_num; ++j) {
            auto offset = (i * GetWidth() + j) * kColorChannelsNum;

            screen_buf_.get()[offset] = image.pixels[i][j].r;
            screen_buf_.get()[offset + 1] = image.pixels[i][j].g;
            screen_buf_.get()[offset + 2] = image.pixels[i][j].b;
            screen_buf_.get()[offset + 3] = 255;
        }
    }
}

void Window::Reset() {
    if (!texture_.create(GetWidth(), GetHeight())) {
        throw std::runtime_error("Couldn't create sfml texture");
    }

    sprite_.setTexture(texture_, true);
    screen_buf_ = std::make_unique<uint8_t[]>(kColorChannelsNum * GetWidth() * GetHeight());
}

// AdaptedSFMLWindow

void Window::AdaptedSFMLWindow::SetResizeCallback(
    std::function<void()> on_resize) {
    on_resize_ = on_resize;
}

void Window::AdaptedSFMLWindow::onResize() {
    on_resize_();
}

}  // namespace ani
