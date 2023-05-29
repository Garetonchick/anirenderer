#include <window/window.h>
#include <primitive_renderer/primitive_renderer.h>

#include <iostream>
#include <random>
#include <chrono>
#include <utility/debug.h>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "primitive_renderer/texture.h"
#include "utility/image.h"

const float kRotationSpeed = 66.f;

int main() {
    ani::Window window(800, 800);

    sf::Clock clock;

    ani::Image image(512, 512);

    std::mt19937 rnd(4444);
    std::uniform_int_distribution<uint8_t> dist(0, 255);

    for(int ib = 0; ib < image.GetWidth() / 8; ++ib) {
        for(int jb = 0; jb < image.GetHeight() / 8; ++jb) {
            ani::RGB color = {dist(rnd), dist(rnd), dist(rnd)}; 
            for(int i = 0; i < 8; ++i) {
                for(int j = 0; j < 8; ++j) {
                    image.SetPixel(ib * 8 + i, jb * 8 + j, color);
                }
            }
        }
    }

    ani::Texture texture(image);

    ani::PrimitiveRenderer renderer(window.GetWidth(), window.GetHeight());

    // glm::vec4 p1 = {-0.5f, -0.5f, 0.f, 1.f};
    // glm::vec4 p2 = {0.f, 0.5f, 0.f, 1.f};
    // glm::vec4 p3 = {0.5f, -0.5f, 0.f, 1.f};
    glm::vec4 p1 = {-1.2f, -1.2f, 0.f, 1.f};
    glm::vec4 p2 = {0.f, 1.2f, 0.f, 1.f};
    glm::vec4 p3 = {1.2f, 0.0f, 0.f, 1.f};

    while (window.IsOpen()) {
        window.PollEvents();

        renderer.SetScreenSize(window.GetWidth(), window.GetHeight());

        renderer.Clear({50, 50, 50});

        ani::Triangle triangle(ani::Point{.pos = p1, .color = {1.f, 0.f, 0.f, 1.f}, .tex_coords = {0.2f, 0.2f}}, 
                               ani::Point{.pos = p2, .color = {0.f, 1.f, 0.f, 1.f}, .tex_coords = {0.5f, 0.8f}},
                               ani::Point{.pos = p3, .color = {0.f, 0.f, 1.f, 1.f}, .tex_coords = {0.8f, 0.2f}});

        glm::mat4 model(1.f);
        float rotation_angle = 40.f;
        model = glm::translate(model, {-4.f, 0.f, 0.f});
        model = glm::scale(model, glm::vec3{4.f});
        model = glm::rotate(model, glm::radians(clock.getElapsedTime().asSeconds() * kRotationSpeed), glm::vec3{0.f, 1.f, 0.f});
        // model = glm::translate(model, {0.f, -0.1f, 0.f});
        // model = glm::rotate(model, glm::radians(rotation_angle), glm::vec3{0.f, 1.f, 0.f});

        glm::mat4 view(1.f);
        view = glm::translate(view, glm::vec3{0.f, 0.f, -2.f});

        glm::mat4 projection(1.f);
        projection = glm::perspective(glm::radians(60.f), static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight()), 0.1f, 100.f);

        triangle.Transform(model);
        triangle.Transform(view);
        triangle.Transform(projection);

        renderer.Render(triangle, texture);

        window.Display(renderer.GetRendered());
    }

    return 0;
}
