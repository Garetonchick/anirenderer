#pragma once
#include <primitive_renderer/primitives.h>

namespace ani {

class EdgeWalk {
public:
    EdgeWalk(Point top, Point bottom);

    int32_t GetBeginY() const;
    int32_t GetEndY() const;
    int32_t GetCurrentX() const;

    void StepDown();

private:
    float begin_int_y_;
    float end_int_y_;
    float cur_x_;
    float x_step_;
};

}  // namespace ani
