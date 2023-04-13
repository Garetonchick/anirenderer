#include "application.h"

#include <window/window.h>

#include <filesystem>

namespace ani {

const std::string Application::kAppPath = ANIRENDERER_PATH; 

Application::Application() : window_(800, 600), renderer_(800, 600) {
    window_.SetCursorState(ani::CursorState::LOCKED_INSIDE);
    window_.SetCursorMovedCallback([this](int dx, int dy){
        this->CursorMovedCallback(dx, dy);
    });
    window_.SetWindowResizedCallback([this](){
        this->WindowResizedCallback();
    });

    models_.emplace_back(Model(FullPath("assets/teapot/teapot.obj")), glm::translate(glm::mat4(1.f), {0.f, -1.6f, -6.2f}));
    models_.emplace_back(Model(FullPath("assets/teapot/teapot.obj")), glm::translate(glm::mat4(1.f), {-8.f, -1.6f, -6.2f}));
    models_.emplace_back(Model(FullPath("assets/teapot/teapot.obj")), glm::translate(glm::mat4(1.f), {8.f, -1.6f, -6.2f}));
    models_.emplace_back(Model(FullPath("assets/teapot/teapot.obj")), glm::translate(glm::mat4(1.f), {16.f, -1.6f, -6.2f}));
}

void Application::Run() {
    float dt = 0;

    while(window_.IsOpen()) {
        sf::Time frame_start_time = clock_.getElapsedTime();

        UpdateScene(dt);
        DrawScene();
        window_.PollEvents();

        dt = GetFrameTime(frame_start_time); 
    }
}

void Application::UpdateScene(float dt) {
    if(window_.IsKeyPressed(sf::Keyboard::W)) {
        camera_.Move(kMovementSpeed * dt * camera_.GetDirectionVec());
    } 
    if(window_.IsKeyPressed(sf::Keyboard::S)) {
        camera_.Move(-kMovementSpeed * dt * camera_.GetDirectionVec());
    } 
    if(window_.IsKeyPressed(sf::Keyboard::D)) {
        camera_.Move(kMovementSpeed * dt * camera_.GetRightVec());
    } 
    if(window_.IsKeyPressed(sf::Keyboard::A)) {
        camera_.Move(-kMovementSpeed * dt * camera_.GetRightVec());
    } 
    if(window_.IsKeyPressed(sf::Keyboard::Space)) {
        camera_.Move(kMovementSpeed * dt * glm::vec3{0.f, 1.f, 0.f});
    } 
    if(window_.IsKeyPressed(sf::Keyboard::LShift)) {
        camera_.Move(-kMovementSpeed * dt * glm::vec3{0.f, 1.f, 0.f});
    } 

    static bool released_c = true;

    if(window_.IsKeyPressed(sf::Keyboard::C)) {
        if(released_c) {
            if(window_.GetCursorState() == CursorState::LOCKED_INSIDE) {
                window_.SetCursorState(CursorState::NORMAL);
            } else {
                window_.SetCursorState(CursorState::LOCKED_INSIDE);
            }
        }

        released_c = false; 
    } else {
        released_c = true;
    } 

    if(window_.IsKeyPressed(sf::Keyboard::Escape)) {
        window_.Close();
    }
}

void Application::DrawScene() {
    renderer_.Clear({50, 50, 50});

    for(auto& [model, trans] : models_) {
        renderer_.Render(model, camera_.GetViewProjMatrix() * trans);
    }

    window_.Display(renderer_.GetRendered());
}

void Application::CursorMovedCallback(int dx, int dy) {
    if(window_.GetCursorState() != CursorState::LOCKED_INSIDE) {
        return;
    }

    camera_.Rotate(1.f * dy * kMouseSensitivity, 1.f * dx * kMouseSensitivity);
}

void Application::WindowResizedCallback() {
    renderer_.SetScreenSize(window_.GetWidth(), window_.GetHeight());
    camera_.SetViewFrustrum(60.f, window_.GetWidth() / window_.GetHeight(), 0.1f, 100.f);
}

std::string Application::FullPath(std::string_view sw) {
    return std::filesystem::path(kAppPath).append(sw);
}

float Application::GetFrameTime(sf::Time frame_start_time) const {
    return static_cast<float>((clock_.getElapsedTime() - frame_start_time).asMilliseconds());
}

}