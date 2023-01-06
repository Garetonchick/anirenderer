#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <iostream>
#include <thread>
#include "SFML/System/Vector2.hpp"

void DrawImageUsingTexture(sf::RenderWindow& window, const sf::Image& image) {
    sf::Texture texture;
    texture.create(image.getSize().x, image.getSize().y);
    texture.update(image);
    sf::Sprite sprite;
    sprite.setTexture(texture, true);

    window.draw(sprite);
}

void DrawImageUsingPoints(sf::RenderWindow& window, const sf::Image& image) {
    static sf::VertexArray points(sf::Points, image.getSize().x * image.getSize().y);
    points.resize(image.getSize().x * image.getSize().y);

    for(size_t i = 0; i < image.getSize().x; ++i) {
        for(size_t j = 0; j < image.getSize().y; ++j) {
            points[i * image.getSize().y + j].position = sf::Vector2f{static_cast<float>(i), static_cast<float>(j)};
            points[i * image.getSize().y + j].color = image.getPixel(i, j);
        }
    }

    window.draw(points);
}

void DrawImageUsingPointsAlternative(sf::RenderWindow& window, const sf::Image& image) {
    static std::vector<sf::Vertex> points(image.getSize().x * image.getSize().y);
    points.resize(image.getSize().x * image.getSize().y);

    for(size_t i = 0; i < image.getSize().x; ++i) {
        for(size_t j = 0; j < image.getSize().y; ++j) {
            points[i * image.getSize().y + j].position = sf::Vector2f{static_cast<float>(i), static_cast<float>(j)};
            points[i * image.getSize().y + j].color = image.getPixel(i, j);
        }
    }

    window.draw(points.data(), points.size(), sf::Points);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Hello World");
    size_t frames = 0;

    sf::Clock clock;

    auto prev_wsize = sf::Vector2u{0, 0}; 

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

        static sf::Image image;

        if(window.getSize() != prev_wsize) {
            image.create(window.getSize().x, window.getSize().y, color1);
            prev_wsize = window.getSize();
        }

        for (size_t i = 0; i < image.getSize().x; ++i) {
            for (size_t j = 0; j < image.getSize().y; ++j) {
                if (((i / 8) & 1) + ((j / 8) & 1) == 1) {
                    image.setPixel(i, j, color2);
                }
            }
        }

        window.clear();
        DrawImageUsingTexture(window, image);
        window.display();
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
