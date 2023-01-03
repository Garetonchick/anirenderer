#pragma once
#include <SFML/Graphics.hpp>

#include <utility/image.h>

#include <stddef.h>
#include <cstdint>
#include <string_view>
#include <memory>
#include <functional>

namespace ani {

class Window {
public:
    Window(uint32_t width, uint32_t height, const std::string& title = "aniwindow");

    bool IsOpen() const;
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

    void SetSize(uint32_t width, uint32_t height);
    void Display(const Image& image);
    void Clear(const RGB& color = {0, 255, 0});

private:
    void CopyImageToScreenBuffer(const Image& image);
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
};

}  // namespace ani
