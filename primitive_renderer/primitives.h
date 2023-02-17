#pragma once
#include <utility/utility.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cstdint>
#include <array>

namespace ani {

struct Point {
    glm::vec4 pos;
    glm::vec4 color;
    glm::vec2 tex_coords;
};

struct Segment {
    Point a;
    Point b;
};

struct Triangle {
public:
    Triangle(const Point& a, const Point& b, const Point& c);

    void Transform(const glm::mat4& trans);

    Point& GetPointA();
    Point& GetPointB();
    Point& GetPointC();
    Point& GetPoint(int32_t idx);

    const Point& GetPointA() const;
    const Point& GetPointB() const;
    const Point& GetPointC() const;
    const Point& GetPoint(int32_t idx) const;


private:
    Point points_[3];
};

}  // namespace ani
