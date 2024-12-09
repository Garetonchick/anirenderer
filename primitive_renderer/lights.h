#pragma once
#include <glm/vec3.hpp>

namespace ani {
struct LightColor {
    glm::vec3 ambient = glm::vec3(0.1f);
    glm::vec3 diffuse = glm::vec3(0.6f);
    glm::vec3 specular = glm::vec3(0.3f);
};

struct PointLight {
    glm::vec3 pos = glm::vec3(0.f);
    LightColor color;
};

struct DirectionalLight {
    glm::vec3 direction;
    LightColor color;
};

glm::vec3 CalculateLightValue(const PointLight& light, glm::vec3 normal, glm::vec3 frag_pos,
                              glm::vec3 view_dir);
}  // namespace ani
