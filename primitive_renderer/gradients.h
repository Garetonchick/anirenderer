#pragma once
#include <primitive_renderer/primitives.h>

namespace ani {

class Gradients {
public:
    explicit Gradients(const Triangle& triangle);

    const glm::vec4& GetXColorSlope() const;
    const glm::vec4& GetYColorSlope() const;

private:
    glm::vec4 x_color_slope_; 
    glm::vec4 y_color_slope_;
};

}
