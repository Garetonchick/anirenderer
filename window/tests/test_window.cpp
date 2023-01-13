#include <window/window.h>

#include <limits>
#include <thread>
#include <iostream>
#include <SFML/System/Clock.hpp>

int main() {
    ani::Window window(800, 800);

    size_t frames = 0;

    sf::Clock clock;

    ani::Image image;
    size_t prev_wwidth = 0;
    size_t prev_wheight = 0;

    while (window.IsOpen()) {
        window.PollEvents();

        ani::RGB color1 = ((frames / 8) & 1) ? ani::RGB{100, 50, 32} : ani::RGB{0, 0, 0};
        ani::RGB color2 =
            ani::RGB{static_cast<uint8_t>(255 - color1.r), static_cast<uint8_t>(255 - color1.g),
                     static_cast<uint8_t>(255 - color1.b)};


        if(prev_wwidth != window.GetWidth() || prev_wheight != window.GetHeight()) {
            prev_wwidth = window.GetWidth();
            prev_wheight = window.GetHeight();

            image = ani::Image(window.GetWidth(), window.GetHeight(), color1);
        }

        for (size_t x = 0; x < window.GetWidth(); ++x) {
            for (size_t y = 0; y < window.GetHeight(); ++y) {
                if (((x / 8) & 1) + ((y / 8) & 1) == 1) {
                    image.SetPixel(x, y, color2);
                } 
            }
        }

        window.Display(image);
        ++frames;

        if (frames & 8) {
            std::cerr << "FPS: "
                      << static_cast<size_t>(static_cast<float>(frames) /
                                             clock.getElapsedTime().asSeconds())
                      << std::endl;
            
            if(frames > 100) {
                frames = 0;
                clock.restart();
            }
        }
    }

    return 0;
}
