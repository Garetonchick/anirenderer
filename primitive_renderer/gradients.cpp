#include <primitive_renderer/gradients.h>

namespace ani {

Gradients::Gradients(const Triangle& triangle) {
    const Point& a = triangle.GetPointA();
    const Point& b = triangle.GetPointB();
    const Point& c = triangle.GetPointC();
    float x_dlt_bc = b.pos.x - c.pos.x;
    float x_dlt_ac = a.pos.x - c.pos.x;
    float y_dlt_bc = b.pos.y - c.pos.y;
    float y_dlt_ac = a.pos.y - c.pos.y;

    float partial_denominator_for_x_slope = x_dlt_bc * y_dlt_ac - x_dlt_ac * y_dlt_bc;
    float partial_denominator_for_y_slope = -partial_denominator_for_x_slope;

    // TODO: Optimize number of divisions
    auto calc_x_slope = [&](const auto& val_a, const auto& val_b, const auto& val_c) {
        return (y_dlt_ac * (val_b - val_c) + y_dlt_bc * (val_c - val_a)) / partial_denominator_for_x_slope;
    };

    auto calc_y_slope = [&](const auto& val_a, const auto& val_b, const auto& val_c) {
        return (x_dlt_ac * (val_b - val_c) + x_dlt_bc * (val_c - val_a)) / partial_denominator_for_y_slope;
    };

    x_color_slope_ = calc_x_slope(a.color, b.color, c.color); 
    y_color_slope_ = calc_y_slope(a.color, b.color, c.color); 

    x_tex_coord_slope_ = calc_x_slope(a.tex_coords, b.tex_coords, c.tex_coords); 
    y_tex_coord_slope_ = calc_y_slope(a.tex_coords, b.tex_coords, c.tex_coords); 
}

const glm::vec4& Gradients::GetXColorSlope() const {
    return x_color_slope_;
}

const glm::vec4& Gradients::GetYColorSlope() const {
    return y_color_slope_;
}

const glm::vec2& Gradients::GetXTexCoordSlope() const {
    return x_tex_coord_slope_;
}

const glm::vec2& Gradients::GetYTexCoordSlope() const {
    return y_tex_coord_slope_;
}

}
