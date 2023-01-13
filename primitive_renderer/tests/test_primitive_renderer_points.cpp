#include <window/window.h>
#include <primitive_renderer/primitive_renderer.h>

#include <iostream>

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

        for(float x = -1.0; x < 1.0; x += 0.025) {
            for(float y = -1.0; y < 1.0; y += 0.025) {
                ani::Point point = {.pos = {x, y, 0.f}, .color = {1.f, 0.f, 0.f, 1.f}};
                renderer.Render(point);
            }
        }

        const auto& rendered = renderer.GetRendered();

        if(rendered.GetWidth() != window.GetWidth() || rendered.GetHeight() != window.GetHeight()) {
            assert(false && "Why?");
        }

        std::cerr << "Before display" << std::endl;
        window.Display(rendered);
        std::cerr << "After display" << std::endl;
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
