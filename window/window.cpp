#include <window/window.h>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"

namespace ani {

Window::Window(uint32_t width, uint32_t height, const std::string& title)
    : sf::RenderWindow(sf::VideoMode(width, height), title.c_str()) {
    Reset();
    Clear();
}

bool Window::IsOpen() const {
    return sf::RenderWindow::isOpen();
}

bool Window::IsKeyPressed(sf::Keyboard::Key key) const {
    return sf::Keyboard::isKeyPressed(key);
}

bool Window::IsMouseButtonPressed(sf::Mouse::Button button) const {
    return sf::Mouse::isButtonPressed(button);
}

float Window::GetWidth() const {
    return static_cast<float>(sf::RenderWindow::getSize().x);
}

float Window::GetHeight() const {
    return static_cast<float>(sf::RenderWindow::getSize().y);
}

CursorState Window::GetCursorState() const {
    return cursor_state_;
}

void Window::SetSize(uint32_t width, uint32_t height) {
    sf::RenderWindow::setSize({width, height});
    Reset();
    Clear();
}

void Window::Display(const Image& image) {
    texture_.update(image.GetRawRGBA());
    sf::RenderWindow::draw(sprite_);
    sf::RenderWindow::display();
}

void Window::Clear(const RGB& color) {
    sf::RenderWindow::clear({color.r, color.g, color.b});
}

void Window::Close() {
    sf::RenderWindow::close();
}

void Window::PollEvents() {
    sf::Event event;

    while (sf::RenderWindow::pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            sf::RenderWindow::close();
        } else if (event.type == sf::Event::MouseWheelScrolled) {
            mouse_scrolled_callback_(event.mouseWheelScroll.delta);
        }
    }

    HandleCursorMoved();
}

void Window::SetCursorState(CursorState cursor_state) {
    if (cursor_state == cursor_state_) {
        return;
    }

    cursor_state_ = cursor_state;
    first_cursor_move_ = true;

    if (cursor_state == CursorState::NORMAL) {
        sf::RenderWindow::setMouseCursorGrabbed(false);
        sf::RenderWindow::setMouseCursorVisible(true);
    } else if (cursor_state == CursorState::LOCKED_INSIDE) {
        sf::RenderWindow::setMouseCursorGrabbed(true);
        sf::RenderWindow::setMouseCursorVisible(false);
    }
}

void Window::SetCursorMovedCallback(const std::function<void(int, int)>& callback) {
    cursor_moved_callback_ = callback;
}

void Window::SetMouseScrolledCallback(const std::function<void(float)>& callback) {
    mouse_scrolled_callback_ = callback;
}

void Window::Reset() {
    sf::RenderWindow::setView(
        sf::View({GetWidth() / 2.f, GetHeight() / 2.f}, {GetWidth(), GetHeight()}));

    if (!texture_.create(GetWidth(), GetHeight())) {
        throw std::runtime_error("Couldn't create sfml texture");
    }

    sprite_.setTexture(texture_, true);
}

void Window::HandleCursorMoved() {
    if (cursor_state_ == CursorState::LOCKED_INSIDE) {
        auto cursor_pos = sf::Mouse::getPosition();
        auto window_center =
            sf::RenderWindow::getPosition() + sf::Vector2i(GetWidth() / 2, GetHeight() / 2);

        if (first_cursor_move_) {
            first_cursor_move_ = false;
        } else {
            cursor_moved_callback_(cursor_pos.x - window_center.x, window_center.y - cursor_pos.y);
        }

        sf::Mouse::setPosition(window_center);
    } else if (cursor_state_ == CursorState::NORMAL) {
        auto cursor_pos = sf::Mouse::getPosition();
        static auto prev_cursor_pos = cursor_pos;

        cursor_moved_callback_(cursor_pos.x - prev_cursor_pos.x, prev_cursor_pos.y - cursor_pos.y);

        prev_cursor_pos = cursor_pos;
    }
}

void Window::onResize() {
    Reset();
    Clear();
}

}  // namespace ani
