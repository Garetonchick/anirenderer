#include <stddef.h>

#include <SFML/Graphics.hpp>
#include <string_view>

namespace ani {

class Window {
public:
    Window(size_t width, size_t height, const std::string& title = "aniwindow");
    bool IsOpen();

    size_t GetWidth();
    size_t GetHeight();

    void SetSize(size_t width, size_t height);

private:
    sf::RenderWindow sfml_window_;
};

}  // namespace ani
