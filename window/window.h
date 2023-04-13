#pragma once
#include <SFML/Graphics.hpp>

#include <utility/image.h>

#include <stddef.h>
#include <cstdint>
#include <string_view>
#include <memory>
#include <functional>
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Keyboard.hpp"

namespace ani {

enum class CursorState { NORMAL, LOCKED_INSIDE };

class Window : private sf::RenderWindow {
public:
    Window(uint32_t width = 800, uint32_t height = 600, const std::string& title = "aniwindow");

    bool IsOpen() const;
    bool IsKeyPressed(sf::Keyboard::Key key) const;
    bool IsMouseButtonPressed(sf::Mouse::Button button) const;
    float GetWidth() const;
    float GetHeight() const;
    CursorState GetCursorState() const;

    void Display(const Image& image);
    void Clear(const RGB& color = {0, 255, 0});
    void Close();
    void PollEvents();
    void SetSize(uint32_t width, uint32_t height);
    void SetCursorState(CursorState cursor_state);
    void SetCursorMovedCallback(const std::function<void(int, int)>& callback);
    void SetMouseScrolledCallback(const std::function<void(float)>& callback);
    void SetWindowResizedCallback(const std::function<void()>& callback);

private:
    void Reset();
    void HandleCursorMoved();

    void onResize() override; // NOLINT

private:
    static const uint32_t kColorChannelsNum = 4;

    sf::Texture texture_;
    sf::Sprite sprite_;

    CursorState cursor_state_ = CursorState::NORMAL;
    bool first_cursor_move_ = true;

    std::function<void(int, int)> cursor_moved_callback_ = [](int, int){};
    std::function<void(float)> mouse_scrolled_callback_ = [](float){};
    std::function<void()> window_resized_callback_ = [](){};
};

}  // namespace ani
