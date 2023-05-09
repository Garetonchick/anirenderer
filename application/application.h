#pragma once
#include <camera/camera.h>
#include <renderer/renderer.h>
#include <window/window.h>

#include <vector>

namespace ani {

class Application {
public:
    Application();
    Application(const Application& o) = delete;
    Application& operator=(const Application& o) = delete;
    Application(Application&& o) = delete;
    Application& operator=(Application&& o) = delete;
    ~Application() = default;

    void Run();

private:
    void SetupScene();
    void AddObjects();
    void AddLights();
    void UpdateScene(float dt);
    void DrawScene();

    void CursorMovedCallback(int dx, int dy);
    void WindowResizedCallback();

    std::string FullPath(std::string_view sw);
    float GetFrameTime(sf::Time frame_start_time) const;

private:
    static const std::string kAppPath;
    static constexpr float kMovementSpeed = 0.01f;
    static constexpr float kMouseSensitivity = 0.1f;

    ani::Window window_;
    ani::Renderer renderer_;
    ani::Camera camera_;
    sf::Clock clock_;

    std::vector<std::pair<Model, glm::mat4>> models_;
    std::vector<std::pair<Lamp, glm::mat4>> lamps_;
};

}  // namespace ani
