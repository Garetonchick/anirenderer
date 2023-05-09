#include <primitive_renderer/gradients.h>

namespace ani {

Gradients::Gradients(const Point& a, const Point& b, const Point& c) {
    float x_dlt_bc = b.pos.x - c.pos.x;
    float x_dlt_ac = a.pos.x - c.pos.x;
    float y_dlt_bc = b.pos.y - c.pos.y;
    float y_dlt_ac = a.pos.y - c.pos.y;

    float a_inv_w = 1.f / a.pos.w;
    float b_inv_w = 1.f / b.pos.w;
    float c_inv_w = 1.f / c.pos.w;

    float partial_denominator_for_x_slope = x_dlt_bc * y_dlt_ac - x_dlt_ac * y_dlt_bc;
    float partial_denominator_for_y_slope = -partial_denominator_for_x_slope;

    auto calc_x_slope = [&](auto val_a, auto val_b, auto val_c, bool perspective = true) {
        if(perspective) {
            val_a *= a_inv_w;
            val_b *= b_inv_w;
            val_c *= c_inv_w;
        }
        return (y_dlt_ac * (val_b - val_c) + y_dlt_bc * (val_c - val_a)) / partial_denominator_for_x_slope;
    };

    auto calc_y_slope = [&](auto val_a, auto val_b, auto val_c, bool perspective = true) {
        if(perspective) {
            val_a *= a_inv_w;
            val_b *= b_inv_w;
            val_c *= c_inv_w;
        }
        return (x_dlt_ac * (val_b - val_c) + x_dlt_bc * (val_c - val_a)) / partial_denominator_for_y_slope;
    };

    x_inv_w_slope_ = calc_x_slope(a_inv_w, b_inv_w, c_inv_w, false); 
    y_inv_w_slope_ = calc_y_slope(a_inv_w, b_inv_w, c_inv_w, false); 

    x_color_slope_ = calc_x_slope(a.color, b.color, c.color);
    y_color_slope_ = calc_y_slope(a.color, b.color, c.color); 

    x_tex_coord_slope_ = calc_x_slope(a.tex_coords, b.tex_coords, c.tex_coords); 
    y_tex_coord_slope_ = calc_y_slope(a.tex_coords, b.tex_coords, c.tex_coords); 

    x_world_pos_slope_ = calc_x_slope(a.world_pos, b.world_pos, c.world_pos);
    y_world_pos_slope_ = calc_y_slope(a.world_pos, b.world_pos, c.world_pos);

    x_z_slope_ = calc_x_slope(a.pos.z, b.pos.z, c.pos.z);
    y_z_slope_ = calc_y_slope(a.pos.z, b.pos.z, c.pos.z);
}

const glm::vec4& Gradients::GetXColorSlope() const {
    return x_color_slope_;
}

const glm::vec4& Gradients::GetYColorSlope() const {
    return y_color_slope_;
}

const glm::vec4& Gradients::GetXWorldPosSlope() const {
    return x_world_pos_slope_;
}

const glm::vec4& Gradients::GetYWorldPosSlope() const {
    return y_world_pos_slope_;
}

const glm::vec2& Gradients::GetXTexCoordSlope() const {
    return x_tex_coord_slope_;
}

const glm::vec2& Gradients::GetYTexCoordSlope() const {
    return y_tex_coord_slope_;
}

float Gradients::GetXInvWSlope() const {
    return x_inv_w_slope_;
}

float Gradients::GetYInvWSlope() const {
    return y_inv_w_slope_;
}

float Gradients::GetXZSlope() const {
    return x_z_slope_;
}
float Gradients::GetYZSlope() const {
    return y_z_slope_;
}

}
