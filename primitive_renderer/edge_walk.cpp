#include <primitive_renderer/edge_walk.h>
#include <cmath>

namespace ani {

EdgeWalk::EdgeWalk(Point top, Point bottom) {
    begin_int_y_ = std::ceil(top.pos.y);
    end_int_y_ = std::ceil(bottom.pos.y);

    float dx = bottom.pos.x - top.pos.x;
    float dy = bottom.pos.y - top.pos.y;

    x_step_ = dx / dy;

    float y_prestep = begin_int_y_ - top.pos.y;
    float x_prestep = y_prestep * x_step_;

    cur_x_ = top.pos.x + x_prestep;
}

int32_t EdgeWalk::GetBeginY() const {
    return static_cast<int32_t>(begin_int_y_);
}

int32_t EdgeWalk::GetEndY() const {
    return static_cast<int32_t>(end_int_y_);
}

int32_t EdgeWalk::GetCurrentX() const {
    return static_cast<int32_t>(std::ceil(cur_x_));
}

void EdgeWalk::StepDown() {
    cur_x_ += x_step_;
}

}  // namespace ani
