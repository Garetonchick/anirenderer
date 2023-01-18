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
    x_color_slope_ = (y_dlt_ac * (b.color - c.color) + y_dlt_bc * (c.color - a.color)) / partial_denominator_for_x_slope;
    y_color_slope_ = (x_dlt_ac * (b.color - c.color) + x_dlt_bc * (c.color - a.color)) / partial_denominator_for_y_slope;
}

const glm::vec4& Gradients::GetXColorSlope() const {
    return x_color_slope_;
}

const glm::vec4& Gradients::GetYColorSlope() const {
    return y_color_slope_;
}

}
