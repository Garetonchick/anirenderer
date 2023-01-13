#include <window/window.h>
#include <primitive_renderer/primitive_renderer.h>

#include <iostream>
#include "primitive_renderer/primitives.h"

int main() {
    ani::Window window(800, 800);

    size_t frames = 0;

    sf::Clock clock;

    ani::Image image;
    ani::PrimitiveRenderer renderer(window.GetWidth(), window.GetHeight());
    size_t prev_wwidth = 0;
    size_t prev_wheight = 0;

    while (window.IsOpen()) {
        window.PollEvents();

        ani::RGB color1 = ((frames / 8) & 1) ? ani::RGB{100, 50, 32} : ani::RGB{0, 0, 0};
        ani::RGB color2 =
            ani::RGB{static_cast<uint8_t>(255 - color1.r), static_cast<uint8_t>(255 - color1.g),
                     static_cast<uint8_t>(255 - color1.b)};


        renderer.SetScreenSize(window.GetWidth(), window.GetHeight());

        renderer.Clear({0, 0, 255});

        ani::Segment segment = {.points = {ani::Point{.pos = {-1.f, -1.f, 0}, .color = {1.f, 0, 0, 1.f}}, 
                                           ani::Point{.pos = {1.f, -0.8f, 0}, .color = {0.f, 1.f, 0, 1.f}}}};

        renderer.Render(segment);

        window.Display(renderer.GetRendered());
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
