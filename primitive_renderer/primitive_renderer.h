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
    explicit PrimitiveRenderer(uint32_t screen_width = 800, uint32_t screen_height = 600);

    void Clear(const RGB& color);
    void SetScreenSize(uint32_t screen_width, uint32_t screen_height);
    void Render(const Triangle& triangle, const Texture& texture = Texture());

    const Image& GetRendered() const;

private:
    void RenderTriangle(Point* p1, Point* p2, Point* p3, const Texture& texture);
    void TransformPoint(Point* p);
    void ScanBetweenEdges(EdgeWalk* long_edge, EdgeWalk* short_edge, bool righthanded, const Texture& texture);
    std::vector<Point> ClipTriangle(const Triangle& triangle);
    bool ClipAxis(std::vector<Point>* in, std::vector<Point>* buf, int axis_idx);
    void ClipBound(const std::vector<Point>& in, std::vector<Point>* out, int axis_idx, bool positive);

private:
    Image screen_;
    Array2D<float> depth_buf_; 
};
}
