#include <window/window.h>
#include <primitive_renderer/primitive_renderer.h>

#include <iostream>
#include <utility/debug.h>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext/matrix_clip_space.hpp"

const float kRotationSpeed = 66.f;

int main() {
    ani::Window window(800, 800);

    sf::Clock clock;

    ani::Image image;
    ani::PrimitiveRenderer renderer(window.GetWidth(), window.GetHeight());

    glm::vec4 p1 = {-0.5f, -0.5f, 0.f, 1.f};
    glm::vec4 p2 = {0.f, 0.5f, 0.f, 1.f};
    glm::vec4 p3 = {0.5f, -0.5f, 0.f, 1.f};

    while (window.IsOpen()) {
        window.PollEvents();

        renderer.SetScreenSize(window.GetWidth(), window.GetHeight());

        renderer.Clear({50, 50, 50});

        ani::Triangle triangle(ani::Point{.pos = p1, .color = {1.f, 0.f, 0.f, 1.f}}, 
                               ani::Point{.pos = p2, .color = {0.f, 1.f, 0.f, 1.f}},
                               ani::Point{.pos = p3, .color = {0.f, 0.f, 1.f, 1.f}});

        glm::mat4 model(1.f);
        float rotation_angle = 91.f;
        model = glm::rotate(model, glm::radians(clock.getElapsedTime().asSeconds() * kRotationSpeed), glm::vec3{0.f, 1.f, 0.f});

        glm::mat4 view(1.f);
        view = glm::translate(view, glm::vec3{0.f, 0.f, -2.f});

        glm::mat4 projection(1.f);
        projection = glm::perspective(glm::radians(60.f), static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight()), 0.1f, 100.f);

        triangle.Transform(model);
        triangle.Transform(view);
        triangle.Transform(projection);

        renderer.Render(triangle);

        window.Display(renderer.GetRendered());
    }

    return 0;
}
