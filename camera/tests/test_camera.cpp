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
#include "glm/geometric.hpp"

const float kRotationSpeed = 0.f;
const float kMovementSpeed = 0.01f;

sf::Clock coc;

glm::vec3 camera_pos = glm::vec3{0.f, 0.4f, 3.f};
float camera_pitch = 0.0f;
float camera_yaw = 0.0f;
float sensitivity = 0.1f;
bool is_mouse_locked = true;

glm::vec3 GetDirUnitVec(float pitch, float yaw) {
    pitch = glm::radians(pitch);
    yaw = glm::radians(yaw);
    glm::vec3 dir = {sin(yaw) * cos(pitch) , -sin(pitch), -cos(yaw) * cos(pitch)};

    return glm::normalize(dir);
}

glm::vec3 GetRightUnitVec(float pitch, float yaw) {
    return glm::normalize(glm::cross(GetDirUnitVec(pitch, yaw), {0.f, 1.f, 0.f})); 
}

glm::mat4 GetCameraRotationMat(float pitch, float yaw) {
    return glm::lookAt({0.f, 0.f, 0.f}, GetDirUnitVec(pitch, yaw), {0.f, 1.f, 0.f});
}

void MoveMouseCallback(int dx, int dy) {
    if(!is_mouse_locked) {
        return;
    }

    camera_pitch = std::clamp(camera_pitch + 1.f * dy * sensitivity, -89.f, 89.f);
    camera_yaw = camera_yaw + 1.f * dx * sensitivity;

    if(camera_yaw <= 0.f) {
        camera_yaw += 360.f;
    } else if(camera_yaw >= 360.f) {
        camera_yaw -= 360.f;
    }
} 

void DrawScene(ani::Window* window, ani::Renderer* renderer, ani::Model* teapot) {
    renderer->SetScreenSize(window->GetWidth(), window->GetHeight());
    renderer->Clear({50, 50, 50});

    glm::mat4 model(1.f);
    model = glm::rotate(model, glm::radians(coc.getElapsedTime().asSeconds() * kRotationSpeed), glm::vec3{0.f, 1.f, 0.f});

    glm::mat4 view(1.f);
    view *= GetCameraRotationMat(camera_pitch, camera_yaw);
    view = glm::translate(view, -camera_pos);

    glm::mat4 projection(1.f);
    projection = glm::perspective(glm::radians(60.f), static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight()), 0.1f, 100.f);

    auto trans = projection * view * model;

    renderer->Render(*teapot, trans);

    window->Display(renderer->GetRendered());
}

void Update(ani::Window* window, float dt) {
    if(window->IsKeyPressed(sf::Keyboard::W)) {
        camera_pos += kMovementSpeed * dt * GetDirUnitVec(camera_pitch, camera_yaw);
        // camera_pos.z -= kMovementSpeed * dt; 
    } 
    if(window->IsKeyPressed(sf::Keyboard::S)) {
        camera_pos -= kMovementSpeed * dt * GetDirUnitVec(camera_pitch, camera_yaw);
        // camera_pos.z += kMovementSpeed * dt; 
    } 
    if(window->IsKeyPressed(sf::Keyboard::D)) {
        camera_pos += kMovementSpeed * dt * GetRightUnitVec(camera_pitch, camera_yaw);
        // camera_pos.x += kMovementSpeed * dt; 
    } 
    if(window->IsKeyPressed(sf::Keyboard::A)) {
        camera_pos -= kMovementSpeed * dt * GetRightUnitVec(camera_pitch, camera_yaw);
        // camera_pos.x -= kMovementSpeed * dt; 
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
