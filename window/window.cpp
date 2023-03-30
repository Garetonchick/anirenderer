#include <window/window.h>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"

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

bool Window::IsKeyPressed(sf::Keyboard::Key key) const {
    return sf::Keyboard::isKeyPressed(key);
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
        } else if(event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::Escape) {
                sfml_window_.close();
            }
        }
    }

    static bool first_time = true;

    if(is_cursor_grabbed_) {
        auto cursor_pos = sf::Mouse::getPosition();
        auto window_center = sfml_window_.getPosition() + sf::Vector2i(GetWidth() / 2, GetHeight() / 2);

        if(first_time) {
            first_time = false;
        } else {
            mouse_moved_callback_(cursor_pos.x - window_center.x, cursor_pos.y - window_center.y);
        }

            sf::Mouse::setPosition(window_center);
    } else {
        first_time = true;
    }
}

void Window::SetGrabCursor(bool grab) {
    sfml_window_.setMouseCursorGrabbed(grab);
    is_cursor_grabbed_ = grab;
}

void Window::SetCursorVisible(bool visible) {
    sfml_window_.setMouseCursorVisible(visible);
}

void Window::SetMouseMovedCallback(const std::function<void(int, int)>& callback) {
    mouse_moved_callback_ = callback;
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
