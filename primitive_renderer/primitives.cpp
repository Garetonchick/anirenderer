#include <primitive_renderer/primitives.h>
#include <stdexcept>

namespace ani {

Triangle::Triangle(const Point& a, const Point& b, const Point& c) {
    points_[0] = a;
    points_[1] = b;
    points_[2] = c;
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
