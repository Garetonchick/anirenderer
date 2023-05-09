#pragma once
#include <primitive_renderer/primitives.h>
#include <primitive_renderer/gradients.h>

namespace ani {

class EdgeWalk {
public:
    EdgeWalk(Point top, Point bottom, const Gradients& gradients);

    int32_t GetBeginY() const;
    int32_t GetEndY() const;
    int32_t GetCurrentX() const;
    glm::vec4 GetCurrentColor() const;
    glm::vec3 GetNormal() const;
    glm::vec2 GetCurrentTexCoord() const;
    glm::vec4 GetCurrentWorldPos() const;
    float GetCurrentInvW() const;
    float GetCurrentZ() const;

    void StepDown();

private:
    void UpdateAfterStep();
    
private:
    const Gradients& gradients_;
    glm::vec3 normal_;

    float begin_int_y_;
    float end_int_y_;

    float cur_x_;
    int32_t cur_int_x_;
    float cur_x_prestep_;
    float x_step_;

    glm::vec4 cur_color_;
    glm::vec4 color_step_;

    glm::vec2 cur_tex_coord_;
    glm::vec2 tex_coord_step_;

    glm::vec4 cur_world_pos_;
    glm::vec4 world_pos_step_;

    float cur_inv_w_;
    float inv_w_step_;

    float cur_z_;
    float z_step_;
};

}  // namespace ani
