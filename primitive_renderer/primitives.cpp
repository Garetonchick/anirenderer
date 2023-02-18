#include <primitive_renderer/primitives.h>
#include <utility/math.h>
#include <stdexcept>

namespace ani {

Point Point::Lerp(const Point& o, float lerp_amount) const {
    Point p;
    p.pos = ani::Lerp(pos, o.pos, lerp_amount);
    p.color = ani::Lerp(color, o.color, lerp_amount);
    p.tex_coords = ani::Lerp(tex_coords, o.tex_coords, lerp_amount);

    return p;
}

Triangle::Triangle(const Point& a, const Point& b, const Point& c) {
    points_[0] = a;
    points_[1] = b;
    points_[2] = c;
}

void Triangle::Transform(const glm::mat4& trans) {
    points_[0].pos = trans * points_[0].pos;
    points_[1].pos = trans * points_[1].pos;
    points_[2].pos = trans * points_[2].pos;
}

Point& Triangle::GetPointA() {
    return points_[0];
}

Point& Triangle::GetPointB() {
    return points_[1];
}

Point& Triangle::GetPointC() {
    return points_[2];
}

Point& Triangle::GetPoint(int32_t idx) {
    if(idx < 0 || idx >= 3) {
        throw std::domain_error("Triangle::GetPoint: Index out of bounds");
    }

    return points_[idx];
}

const Point& Triangle::GetPointA() const {
    return points_[0];
}

const Point& Triangle::GetPointB() const {
    return points_[1];
}

const Point& Triangle::GetPointC() const {
    return points_[2];
}

const Point& Triangle::GetPoint(int32_t idx) const {
    if(idx < 0 || idx >= 3) {
        throw std::domain_error("Triangle::GetPoint: Index out of bounds");
    }

    return points_[idx];
}

}
