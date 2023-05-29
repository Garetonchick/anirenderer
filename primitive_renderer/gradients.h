#pragma once
#include <primitive_renderer/primitives.h>

namespace ani {

class Gradients {
public:
    Gradients(const Point& a, const Point& b, const Point& c);
    Gradients(const Gradients& o) = default;
    Gradients& operator=(const Gradients& o) = default;
    Gradients(Gradients&& o) = delete;
    Gradients& operator=(Gradients&& o) = delete;

    const glm::vec4& GetXColorSlope() const;
    const glm::vec4& GetYColorSlope() const;
    const glm::vec2& GetXTexCoordSlope() const;
    const glm::vec2& GetYTexCoordSlope() const;
    const glm::vec4& GetXWorldPosSlope() const;
    const glm::vec4& GetYWorldPosSlope() const;
    float GetXInvWSlope() const;
    float GetYInvWSlope() const;
    float GetXZSlope() const;
    float GetYZSlope() const;

private:
    glm::vec4 x_color_slope_;
    glm::vec4 y_color_slope_;
    glm::vec2 x_tex_coord_slope_;
    glm::vec2 y_tex_coord_slope_;
    glm::vec4 x_world_pos_slope_;
    glm::vec4 y_world_pos_slope_;
    float x_inv_w_slope_;
    float y_inv_w_slope_;
    float x_z_slope_;
    float y_z_slope_;
};

}  // namespace ani
