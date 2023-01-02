#include <SFML/Graphics.hpp>
#include <stddef.h>
#include <string_view>

namespace ani {

class Window {
    public:
        Window(size_t width, size_t height, const std::string& title = "aniwindow");
        bool IsOpen();
        
    private:
        sf::RenderWindow sfml_window_; 
};

}
