#include <window/window.h>
#include <primitive_renderer/primitive_renderer.h>

#include <filesystem>
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

    ani::Texture texture(std::filesystem::path(ANIRENDERER_PATH) / "assets/teapot/dirt.jpg");
    ani::PrimitiveRenderer renderer(window.GetWidth(), window.GetHeight());

    glm::vec4 p1 = {-0.5f, -0.5f, 0.f, 1.f};
    glm::vec4 p2 = {0.f, 0.5f, 0.f, 1.f};
    glm::vec4 p3 = {0.5f, -0.5f, 0.f, 1.f};
    // glm::vec4 p1 = {-0.5f, -0.5f, 0.f, 1.f};
    // glm::vec4 p2 = {0.f, 0.5f, 0.f, 1.f};
    // glm::vec4 p3 = {0.5f, 0.0f, 0.f, 1.f};

    while (window.IsOpen()) {
        window.PollEvents();

        renderer.SetScreenSize(window.GetWidth(), window.GetHeight());

        renderer.Clear({50, 50, 50});

        ani::Triangle triangle(ani::Point{.pos = p1, .color = {1.f, 0.f, 0.f, 1.f}, .tex_coords = {0.2f, 0.2f}}, 
                               ani::Point{.pos = p2, .color = {0.f, 1.f, 0.f, 1.f}, .tex_coords = {0.5f, 0.8f}},
                               ani::Point{.pos = p3, .color = {0.f, 0.f, 1.f, 1.f}, .tex_coords = {0.8f, 0.2f}});

        glm::mat4 model(1.f);
        float rotation_angle = 40.f;
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
