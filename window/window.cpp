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
    texture_.update(image.GetRawRGBA());
    sfml_window_.draw(sprite_);
    sfml_window_.display();
}

void Window::Clear(const RGB& color) {
    sfml_window_.clear({color.r, color.g, color.b});
}

void Window::PollEvents() {
    sf::Event event;

    while (sfml_window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            sfml_window_.close();
        }

        // TODO: Call subscribed callbacks for different types of events
    }
}

// Private members

void Window::Reset() {
    sfml_window_.setView(
        sf::View({static_cast<float>(GetWidth()) / 2.f, static_cast<float>(GetHeight()) / 2.f},
                 {static_cast<float>(GetWidth()), static_cast<float>(GetHeight())}));

    if (!texture_.create(GetWidth(), GetHeight())) {
        throw std::runtime_error("Couldn't create sfml texture");
    }

    sprite_.setTexture(texture_, true);

    screen_buf_ = std::make_unique<uint8_t[]>(kColorChannelsNum * GetWidth() * GetHeight());
}

// AdaptedSFMLWindow

void Window::AdaptedSFMLWindow::SetResizeCallback(std::function<void()> on_resize) {
    on_resize_ = on_resize;
}

void Window::AdaptedSFMLWindow::onResize() {
    on_resize_();
}

}  // namespace ani
