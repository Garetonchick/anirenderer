#include <window/window.h>
#include <primitive_renderer/primitive_renderer.h>

#include <iostream>
#include <random>
#include <utility/debug.h>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

const float kRotationSpeed = 66.f;

int main() {
    std::mt19937 rnd(4444);
    std::uniform_real_distribution<float> dist(0.f, 1.f);
    ani::Window window(800, 800);
    std::vector<float> cube_poses = {
        // Back face
        -0.5f, -0.5f, -0.5f,   // Bottom-left
         0.5f,  0.5f, -0.5f,   // top-right
         0.5f, -0.5f, -0.5f,   // bottom-right         
         0.5f,  0.5f, -0.5f,   // top-right
        -0.5f, -0.5f, -0.5f,   // bottom-left
        -0.5f,  0.5f, -0.5f,   // top-left
        // Front face
        -0.5f, -0.5f,  0.5f,   // bottom-left
         0.5f, -0.5f,  0.5f,   // bottom-right
         0.5f,  0.5f,  0.5f,   // top-right
         0.5f,  0.5f,  0.5f,   // top-right
        -0.5f,  0.5f,  0.5f,   // top-left
        -0.5f, -0.5f,  0.5f,   // bottom-left
        // Left face
        -0.5f,  0.5f,  0.5f,   // top-right
        -0.5f,  0.5f, -0.5f,   // top-left
        -0.5f, -0.5f, -0.5f,   // bottom-left
        -0.5f, -0.5f, -0.5f,   // bottom-left
        -0.5f, -0.5f,  0.5f,   // bottom-right
        -0.5f,  0.5f,  0.5f,   // top-right
        // Right face
         0.5f,  0.5f,  0.5f,   // top-left
         0.5f, -0.5f, -0.5f,   // bottom-right
         0.5f,  0.5f, -0.5f,   // top-right         
         0.5f, -0.5f, -0.5f,   // bottom-right
         0.5f,  0.5f,  0.5f,   // top-left
         0.5f, -0.5f,  0.5f,   // bottom-left     
        // Bottom face
        -0.5f, -0.5f, -0.5f,   // top-right
         0.5f, -0.5f, -0.5f,   // top-left
         0.5f, -0.5f,  0.5f,   // bottom-left
         0.5f, -0.5f,  0.5f,   // bottom-left
        -0.5f, -0.5f,  0.5f,   // bottom-right
        -0.5f, -0.5f, -0.5f,   // top-right
        // Top face
        -0.5f,  0.5f, -0.5f,   // top-left
         0.5f,  0.5f,  0.5f,   // bottom-right
         0.5f,  0.5f, -0.5f,   // top-right     
         0.5f,  0.5f,  0.5f,   // bottom-right
        -0.5f,  0.5f, -0.5f,   // top-left
        -0.5f,  0.5f,  0.5f,   // bottom-left        
    };


    sf::Clock clock;

    ani::Image image;
    ani::PrimitiveRenderer renderer(window.GetWidth(), window.GetHeight());

    std::vector<ani::Triangle> triangles;

    for(int i = 0; i < cube_poses.size(); i += 6 * 3) {
        glm::vec4 v1 = {cube_poses[i], cube_poses[i + 1], cube_poses[i + 2], 1.f};
        glm::vec4 v2 = {cube_poses[i + 3], cube_poses[i + 3 + 1], cube_poses[i + 3 + 2], 1.f};
        glm::vec4 v3 = {cube_poses[i + 6], cube_poses[i + 6 + 1], cube_poses[i + 6 + 2], 1.f};
        glm::vec4 v4 = {cube_poses[i + 9], cube_poses[i + 9 + 1], cube_poses[i + 9 + 2], 1.f};
        glm::vec4 v5 = {cube_poses[i + 12], cube_poses[i + 12 + 1], cube_poses[i + 12 + 2], 1.f};
        glm::vec4 v6 = {cube_poses[i + 15], cube_poses[i + 15 + 1], cube_poses[i + 15 + 2], 1.f};
        glm::vec4 c1 = {dist(rnd), dist(rnd), dist(rnd), 1.f};
        glm::vec4 c2 = {dist(rnd), dist(rnd), dist(rnd), 1.f};
        glm::vec4 c3 = {dist(rnd), dist(rnd), dist(rnd), 1.f};
        glm::vec4 c4 = {dist(rnd), dist(rnd), dist(rnd), 1.f};
        glm::vec4 c5 = {dist(rnd), dist(rnd), dist(rnd), 1.f};
        glm::vec4 c6 = {dist(rnd), dist(rnd), dist(rnd), 1.f};
        triangles.emplace_back(ani::Point{.pos = v1, .color = c1}, ani::Point{.pos = v2, .color = c2}, ani::Point{.pos = v3, .color = c3});
        triangles.emplace_back(ani::Point{.pos = v4, .color = c4}, ani::Point{.pos = v5, .color = c5}, ani::Point{.pos = v6, .color = c6});
    }

    while (window.IsOpen()) {
        window.PollEvents();

        renderer.SetScreenSize(window.GetWidth(), window.GetHeight());

        renderer.Clear({50, 50, 50});

        glm::mat4 model(1.f);
        float rotation_angle = 91.f;
        model = glm::rotate(model, glm::radians(clock.getElapsedTime().asSeconds() * kRotationSpeed), glm::vec3{0.f, 1.f, 0.f});

        glm::mat4 view(1.f);
        view = glm::translate(view, glm::vec3{0.f, -0.4f, -3.f});
        view = glm::rotate(view, glm::radians(20.f), glm::vec3{1.f, 0.f, 0.f});

        glm::mat4 projection(1.f);
        projection = glm::perspective(glm::radians(60.f), static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight()), 0.1f, 100.f);

        auto trans = projection * view * model;

        for(ani::Triangle triangle : triangles) {
            triangle.Transform(trans);
            renderer.Render(triangle);
        }

        window.Display(renderer.GetRendered());
    }

    return 0;
}
