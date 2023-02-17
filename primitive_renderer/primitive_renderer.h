#pragma once
#include <primitive_renderer/primitives.h>
#include <primitive_renderer/edge_walk.h>
#include <primitive_renderer/texture.h>
#include <utility/array2d.h>

#include <glm/vec4.hpp>

#include <functional>
#include <optional>

namespace ani {
// Expects all coordinates in ndc
class PrimitiveRenderer {
public:
    PrimitiveRenderer(uint32_t screen_width, uint32_t screen_height);

    void Clear(const RGB& color);
    void SetScreenSize(uint32_t screen_width, uint32_t screen_height);
    void Render(const Point& point);
    void Render(const Segment& segment);
    void Render(Triangle triangle, const Texture& texture = Texture());

    const Image& GetRendered() const;

private:
    void TransformPoint(Point* p);
    void ScanBetweenEdges(EdgeWalk* long_edge, EdgeWalk* short_edge, bool righthanded, const Texture& texture);

private:
    Image screen_;
    Array2D<float> depth_buf_; 
};
}
