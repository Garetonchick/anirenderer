#include <iostream>
#include <primitive_renderer/edge_walk.h>
#include <cmath>

namespace ani {

EdgeWalk::EdgeWalk(Point top, Point bottom, const Gradients& gradients)
    : gradients_(gradients), normal_(top.normal) {
    begin_int_y_ = std::ceil(top.pos.y);
    end_int_y_ = std::ceil(bottom.pos.y);

    float dx = bottom.pos.x - top.pos.x;
    float dy = bottom.pos.y - top.pos.y;
    float top_inv_w = (1.f / top.pos.w);

    x_step_ = dx / dy;

    float y_prestep = begin_int_y_ - top.pos.y;
    float x_prestep = y_prestep * x_step_;

    cur_x_ = top.pos.x + x_prestep;

    cur_color_ = top.color * top_inv_w + x_prestep * gradients.GetXColorSlope() + y_prestep * gradients.GetYColorSlope();
    color_step_ = x_step_ * gradients.GetXColorSlope() + gradients.GetYColorSlope();  

    cur_tex_coord_ = top.tex_coords * top_inv_w + 
                     x_prestep * gradients.GetXTexCoordSlope() + 
                     y_prestep * gradients.GetYTexCoordSlope();
    tex_coord_step_ =  x_step_ * gradients.GetXTexCoordSlope() + gradients.GetYTexCoordSlope();

    cur_world_pos_ = top.world_pos * top_inv_w + 
                     x_prestep * gradients.GetXWorldPosSlope() +
                     y_prestep * gradients.GetYWorldPosSlope();
    world_pos_step_ = x_step_ * gradients.GetXWorldPosSlope() + gradients.GetYWorldPosSlope();

    cur_inv_w_ = top_inv_w + 
                 x_prestep * gradients.GetXInvWSlope() +
                 y_prestep * gradients.GetYInvWSlope();
    inv_w_step_ = x_step_ * gradients.GetXInvWSlope() + gradients.GetYInvWSlope();

    cur_z_ = top.pos.z * top_inv_w + 
            x_prestep * gradients.GetXZSlope() +
            y_prestep * gradients.GetYZSlope();
    z_step_ = x_step_ * gradients.GetXZSlope() + gradients.GetYZSlope(); 

    cur_int_x_ = std::ceil(cur_x_);
    cur_x_prestep_ = static_cast<float>(cur_int_x_) - cur_x_;
}

int32_t EdgeWalk::GetBeginY() const {
    return static_cast<int32_t>(begin_int_y_);
}

int32_t EdgeWalk::GetEndY() const {
    return static_cast<int32_t>(end_int_y_);
}

int32_t EdgeWalk::GetCurrentX() const {
    return cur_int_x_;
}

glm::vec4 EdgeWalk::GetCurrentColor() const {
    return cur_color_ + cur_x_prestep_ * gradients_.GetXColorSlope(); 
}

glm::vec3 EdgeWalk::GetNormal() const {
    return normal_;
}

glm::vec2 EdgeWalk::GetCurrentTexCoord() const {
    return cur_tex_coord_ + cur_x_prestep_ * gradients_.GetXTexCoordSlope(); 
}

glm::vec4 EdgeWalk::GetCurrentWorldPos() const {
    return cur_world_pos_ + cur_x_prestep_ * gradients_.GetXWorldPosSlope(); 
}

float EdgeWalk::GetCurrentInvW() const {
    return cur_inv_w_ + cur_x_prestep_ * gradients_.GetXInvWSlope(); 
}

float EdgeWalk::GetCurrentZ() const {
    return cur_z_ + cur_x_prestep_ * gradients_.GetXZSlope(); 
}

void EdgeWalk::StepDown() {
    cur_x_ += x_step_;
    UpdateAfterStep();
}

void EdgeWalk::UpdateAfterStep() {
    cur_int_x_ = std::ceil(cur_x_);
    cur_x_prestep_ = static_cast<float>(cur_int_x_) - cur_x_;
    cur_color_ += color_step_;
    cur_tex_coord_ += tex_coord_step_;
    cur_inv_w_ += inv_w_step_;
    cur_z_ += z_step_;
    cur_world_pos_ += world_pos_step_;
}

}  // namespace ani
