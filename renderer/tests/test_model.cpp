#include <window/window.h>
#include <renderer/renderer.h>

#include <iostream>
#include <random>
#include <utility/debug.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

const float kRotationSpeed = 66.f;

int main() {
    std::mt19937 rnd(4444);
    ani::Window window(200, 200);
    sf::Clock clock;

    ani::Image image;
    ani::Renderer renderer(window.GetWidth(), window.GetHeight());
    ani::Model teapot("/home/gareton/repos/anirenderer/assets/teapot/teapot.obj");
    std::cout << "Model loaded" << std::endl;
    std::cout.flush();

    while (window.IsOpen()) {
        window.PollEvents();

        renderer.SetScreenSize(window.GetWidth(), window.GetHeight());

        renderer.Clear({50, 50, 50});

        glm::mat4 model(1.f);
        float rotation_angle = 91.f;
        // model = glm::scale(model, glm::vec3{0.4f, 0.4f, 0.4f});
        model = glm::rotate(model, glm::radians(clock.getElapsedTime().asSeconds() * kRotationSpeed), glm::vec3{0.f, 1.f, 0.f});

        glm::mat4 view(1.f);
        view = glm::translate(view, glm::vec3{0.f, -0.4f, -3.f});
        view = glm::rotate(view, glm::radians(20.f), glm::vec3{1.f, 0.f, 0.f});

        glm::mat4 projection(1.f);
        projection = glm::perspective(glm::radians(60.f), static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight()), 0.1f, 100.f);

        auto trans = projection * view * model;

        // std::cout << "Before render" << std::endl;
        // std::cout.flush();
        renderer.Render(teapot, trans);
        // std::cout << "After render" << std::endl;
        // std::cout.flush();

        window.Display(renderer.GetRendered());
    }

    return 0;
}
