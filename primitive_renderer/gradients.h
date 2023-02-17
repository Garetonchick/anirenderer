#pragma once
#include <primitive_renderer/primitives.h>

namespace ani {

class Gradients {
public:
    explicit Gradients(const Triangle& triangle);

    const glm::vec4& GetXColorSlope() const;
    const glm::vec4& GetYColorSlope() const;
    const glm::vec2& GetXTexCoordSlope() const;
    const glm::vec2& GetYTexCoordSlope() const;

private:
    glm::vec4 x_color_slope_; 
    glm::vec4 y_color_slope_;
    glm::vec2 x_tex_coord_slope_;
    glm::vec2 y_tex_coord_slope_;
};

}
