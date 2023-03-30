#pragma once
#include <SFML/Graphics.hpp>

#include <utility/image.h>

#include <stddef.h>
#include <cstdint>
#include <string_view>
#include <memory>
#include <functional>
#include "SFML/Window/Keyboard.hpp"

namespace ani {

class Window {
public:
    Window(uint32_t width, uint32_t height, const std::string& title = "aniwindow");

    bool IsOpen() const;
    bool IsKeyPressed(sf::Keyboard::Key key) const;
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

    void SetSize(uint32_t width, uint32_t height);
    void Display(const Image& image);
    void Clear(const RGB& color = {0, 255, 0});
    void PollEvents();
    void SetGrabCursor(bool grab);
    void SetCursorVisible(bool visible);

    void SetMouseMovedCallback(const std::function<void(int, int)>& callback);

private:
    void Reset();

private:
    class AdaptedSFMLWindow : public sf::RenderWindow {
        using sf::RenderWindow::RenderWindow;

    public:
        void SetResizeCallback(std::function<void()> on_resize);

    protected:
        void onResize() override;

    private:
        std::function<void()> on_resize_;
    };

private:
    static const uint32_t kColorChannelsNum = 4;

    AdaptedSFMLWindow sfml_window_;
    sf::Texture texture_;
    sf::Sprite sprite_;
    std::unique_ptr<uint8_t[]> screen_buf_;
    bool is_cursor_grabbed_;

    // Callbacks
    std::function<void(int, int)> mouse_moved_callback_ = [](int, int){};
};

}  // namespace ani
