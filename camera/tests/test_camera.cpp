#include <window/window.h>
#include <renderer/renderer.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <utility/debug.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "SFML/Window/Keyboard.hpp"
#include "camera/camera.h"
#include "glm/geometric.hpp"

const float kRotationSpeed = 0.f;
const float kMovementSpeed = 0.01f;

sf::Clock coc;
ani::Camera camera({0.f, 0.f, 10.f});

float sensitivity = 0.1f;
bool is_mouse_locked = true;

void MoveMouseCallback(int dx, int dy) {
    if(!is_mouse_locked) {
        return;
    }

    camera.Rotate(1.f * dy * sensitivity, 1.f * dx * sensitivity);
} 

void DrawScene(ani::Window* window, ani::Renderer* renderer, ani::Model* teapot) {
    renderer->SetScreenSize(window->GetWidth(), window->GetHeight());
    renderer->Clear({50, 50, 50});

    glm::mat4 model(1.f);
    model = glm::rotate(model, glm::radians(coc.getElapsedTime().asSeconds() * kRotationSpeed), glm::vec3{0.f, 1.f, 0.f});

    glm::mat4 view = camera.GetViewMatrix();

    glm::mat4 projection(1.f);
    projection = glm::perspective(glm::radians(60.f), window->GetWidth() / window->GetHeight(), 0.1f, 100.f);

    auto trans = projection * view * model;

    renderer->Render(*teapot, trans);

    window->Display(renderer->GetRendered());
}

void Update(ani::Window* window, float dt) {
    if(window->IsKeyPressed(sf::Keyboard::W)) {
        camera.Move(kMovementSpeed * dt * camera.GetDirectionVec());
    } 
    if(window->IsKeyPressed(sf::Keyboard::S)) {
        camera.Move(-kMovementSpeed * dt * camera.GetDirectionVec());
    } 
    if(window->IsKeyPressed(sf::Keyboard::D)) {
        camera.Move(kMovementSpeed * dt * camera.GetRightVec());
    } 
    if(window->IsKeyPressed(sf::Keyboard::A)) {
        camera.Move(-kMovementSpeed * dt * camera.GetRightVec());
    } 
    if(window->IsKeyPressed(sf::Keyboard::Space)) {
        camera.Move(kMovementSpeed * dt * camera.GetUpVec());
    } 
    if(window->IsKeyPressed(sf::Keyboard::LShift)) {
        camera.Move(-kMovementSpeed * dt * camera.GetUpVec());
    } 

    static bool released_c = true;

    if(window->IsKeyPressed(sf::Keyboard::C)) {
        if(released_c) {
            is_mouse_locked = !is_mouse_locked;
            
            if(window->GetCursorState() == ani::CursorState::LOCKED_INSIDE) {
                window->SetCursorState(ani::CursorState::NORMAL);
            } else {
                window->SetCursorState(ani::CursorState::LOCKED_INSIDE);
            }
        }

        released_c = false; 
    } else {
        released_c = true;
    } 

    if(window->IsKeyPressed(sf::Keyboard::Escape)) {
        window->Close();
    }
}

int main() {
    ani::Window window(600, 600);

    is_mouse_locked = true;
    window.SetCursorState(ani::CursorState::LOCKED_INSIDE);
    window.SetCursorMovedCallback(MoveMouseCallback);

    ani::Renderer renderer(window.GetWidth(), window.GetHeight());
    ani::Model teapot("/home/gareton/repos/anirenderer/assets/teapot/teapot.obj");

    float dt = 0;

    while (window.IsOpen()) {
        sf::Time frame_start_time = coc.getElapsedTime();

        window.PollEvents();
        Update(&window, dt);
        DrawScene(&window, &renderer, &teapot);
        dt = static_cast<float>((coc.getElapsedTime() - frame_start_time).asMilliseconds());
    }

    return 0;
}
