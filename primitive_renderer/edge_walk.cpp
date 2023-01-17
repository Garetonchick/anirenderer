#include <iostream>
#include <primitive_renderer/edge_walk.h>
#include <cmath>

namespace ani {

EdgeWalk::EdgeWalk(Point top, Point bottom, const Gradients& gradients)
    : gradients_(gradients) {
    begin_int_y_ = std::ceil(top.pos.y);
    end_int_y_ = std::ceil(bottom.pos.y);

    float dx = bottom.pos.x - top.pos.x;
    float dy = bottom.pos.y - top.pos.y;

    x_step_ = dx / dy;

    float y_prestep = begin_int_y_ - top.pos.y;
    float x_prestep = y_prestep * x_step_;

    cur_x_ = top.pos.x + x_prestep;
    cur_color_ = top.color + x_prestep * gradients.GetXColorSlope() + y_prestep * gradients.GetYColorSlope();
    color_step_ = x_step_ * gradients.GetXColorSlope() + gradients.GetYColorSlope();  

    std::cout << "Edge: " << top.pos.x << " " << top.pos.y << " " << bottom.pos.x << " " << bottom.pos.y << std::endl;
    std::cout << "Color step is: " << color_step_.r << " " << color_step_.g << " " << color_step_.b << std::endl;

    UpdateAfterStep();
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

void EdgeWalk::StepDown() {
    cur_x_ += x_step_;
    UpdateAfterStep();
}

void EdgeWalk::UpdateAfterStep() {
    cur_int_x_ = std::ceil(cur_x_);
    cur_x_prestep_ = static_cast<float>(cur_int_x_) - cur_x_;
    cur_color_ += color_step_;
}

}  // namespace ani
