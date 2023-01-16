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

    while (window.IsOpen()) {
        window.PollEvents();

        renderer.SetScreenSize(window.GetWidth(), window.GetHeight());

        renderer.Clear({0, 0, 255});

        ani::Triangle triangle(ani::Point{.pos = {100, 100, 0, 1}, .color = {1.f, 0, 0, 1.f}}, 
                                           ani::Point{.pos = {600, 100, 0, 1}, .color = {1.f, 0, 0, 1.f}},
                                           ani::Point{.pos = {300, 500, 0, 1}, .color = {1.f, 0, 0, 1.f}});

        renderer.Render(triangle);

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
