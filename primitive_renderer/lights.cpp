#include "lights.h"
#include "glm/geometric.hpp"

#include <utility/math.h>

#include <algorithm>

namespace ani {

glm::vec3 CalculateLightValue(const PointLight& light, glm::vec3 normal, glm::vec3 frag_pos, glm::vec3 view_pos) {
    glm::vec3 light_color = light.color.ambient;
    glm::vec3 light_dir = glm::normalize(frag_pos - light.pos);
    glm::vec3 view_dir = glm::normalize(frag_pos - view_pos);
    glm::vec3 reflection = glm::reflect(-light_dir, normal);
    float diffuse_coef = std::max(glm::dot(-light_dir, normal), 0.f); 
    float specular_coef = Bpow(std::max(glm::dot(reflection, view_dir), 0.f), 32);
    light_color += light.color.diffuse * diffuse_coef;
    light_color += light.color.specular * specular_coef;

    return light_color;
}

}