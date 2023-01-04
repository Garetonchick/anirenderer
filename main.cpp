#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <iostream>
#include <thread>

int main() {
    sf::RenderWindow window(sf::VideoMode(100, 100), "Hello World");
    size_t frames = 0;

    window.setSize({800, 800});

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.setView(sf::View(
            {static_cast<float>(window.getSize().x) / 2.f,
             static_cast<float>(window.getSize().y) / 2.f},
            {static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)}));
        sf::Color color1 = ((frames / 8) & 1) ? sf::Color{100, 50, 32} : sf::Color{0, 0, 0};
        sf::Color color2 =
            sf::Color{static_cast<uint8_t>(255 - color1.r), static_cast<uint8_t>(255 - color1.g),
                      static_cast<uint8_t>(255 - color1.b)};

        sf::Image image;
        image.create(window.getSize().x, window.getSize().y, color1);

        for (size_t i = 0; i < image.getSize().x; ++i) {
            for (size_t j = 0; j < image.getSize().y; ++j) {
                if (((i / 8) & 1) + ((j / 8) & 1) == 1) {
                    image.setPixel(i, j, color2);
                }
            }
        }

        sf::Texture texture;
        texture.create(image.getSize().x, image.getSize().y);
        texture.update(image);
        sf::Sprite sprite;
        sprite.setTexture(texture, true);

        window.clear();
        window.draw(sprite);
        window.display();
        ++frames;
    }
    return 0;
}
