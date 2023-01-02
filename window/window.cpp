#include <window/window.h>

namespace ani {

Window::Window(size_t width, size_t height, const std::string& title)
    : sfml_window_(sf::VideoMode(width, height), title.c_str()) {
}

bool Window::IsOpen() {
    return sfml_window_.isOpen();
}

}  // namespace ani
