#include <window/window.h>
#include <primitive_renderer/primitive_renderer.h>

#include <iostream>
#include <vector>
#include <random>

struct Star {
    ani::Point point;
    float speed;
    float ttl;
};

const float kStarMaxTimeToLive = 4.4f;
const float kStarMinTimeToLive = 1.2f;
const float kStarSpeed = 0.01f;
const int kMaxStars = 2000;

int main() {
    ani::Window window(800, 800);

    size_t frames = 0;

    sf::Clock clock;

    ani::Image image;
    ani::PrimitiveRenderer renderer(window.GetWidth(), window.GetHeight());
    std::mt19937 rnd(4444);
    std::uniform_real_distribution<float> dist(-1.f, 1.f);
    std::uniform_real_distribution<float> live_dist(kStarMinTimeToLive, kStarMaxTimeToLive);
    std::vector<Star> stars;
    float frame_time = 0;

    while (window.IsOpen()) {
        float frame_start_time = clock.getElapsedTime().asSeconds();

        window.PollEvents();
        renderer.SetScreenSize(window.GetWidth(), window.GetHeight());
        renderer.Clear({0, 0, 0});

        for (size_t i = 0; i < stars.size(); ++i) {
            float depth = (stars[i].point.pos.z + 1.f) * 0.5f;
            stars[i].ttl -= frame_time;
            stars[i].point.pos.x *= (1.f + stars[i].speed / depth);
            stars[i].point.pos.y *= (1.f + stars[i].speed / depth);

            if (stars[i].ttl <= 0.f) {
                std::swap(stars[i], stars.back());
                stars.pop_back();
                --i;
            }
        }

        while (stars.size() < kMaxStars) {
            stars.push_back(Star{
                .point = {.pos = {dist(rnd), dist(rnd), dist(rnd)}, .color = {1.f, 1.f, 1.f, 1.f}},
                .speed = kStarSpeed,
                .ttl = live_dist(rnd)});
        }

        for (const auto& star : stars) {
            renderer.Render(star.point);
        }

        const auto& rendered = renderer.GetRendered();

        if (rendered.GetWidth() != window.GetWidth() ||
            rendered.GetHeight() != window.GetHeight()) {
            assert(false && "Why?");
        }

        window.Display(rendered);
        ++frames;
        frame_time = clock.getElapsedTime().asSeconds() - frame_start_time;

        if (frames & 8) {
            std::cerr << "FPS: "
                      << static_cast<size_t>(static_cast<float>(frames) /
                                             clock.getElapsedTime().asSeconds())
                      << std::endl;

            if (frames > 100) {
                frames = 0;
                clock.restart();
            }
        }
    }

    return 0;
}
