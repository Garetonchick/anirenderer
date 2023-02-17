#include <primitive_renderer/primitive_renderer.h>

#include <primitive_renderer/primitives.h>
#include <primitive_renderer/edge_walk.h>
#include <utility/utility.h>
#include <glm/geometric.hpp>
#include <glm/common.hpp>

#include <limits>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "utility/image.h"
#include "utility/math.h"

// #define FACE_CULLING

namespace ani {
PrimitiveRenderer::PrimitiveRenderer(uint32_t screen_width, uint32_t screen_height)
    : screen_(screen_width, screen_height), depth_buf_(screen_width, screen_height, -1.f) {
}

void PrimitiveRenderer::Clear(const RGB& color) {
    screen_.Fill(color);
}

void PrimitiveRenderer::SetScreenSize(uint32_t screen_width, uint32_t screen_height) {
    screen_.SetSize(screen_width, screen_height);
    depth_buf_.Resize(screen_width, screen_height);
}

void PrimitiveRenderer::Render(const Point& point) {
    (void)point;
    throw std::runtime_error("Not implemented");
}

void PrimitiveRenderer::Render(const Segment& segment) {
    (void)segment;
    throw std::runtime_error("Not implemented");
}

void PrimitiveRenderer::Render(Triangle triangle, const Texture& texture) {
    Point* points[] = {&triangle.GetPointA(), &triangle.GetPointB(), &triangle.GetPointC()};

    TransformPoint(points[0]);
    TransformPoint(points[1]);
    TransformPoint(points[2]);

    std::sort(points, points + 3,
              [](const Point* a, const Point* b) { return a->pos.y < b->pos.y; });

    Point& min_y_point = *points[0];
    Point& mid_y_point = *points[1];
    Point& max_y_point = *points[2];

    Gradients gradients(triangle);

    EdgeWalk long_edge(min_y_point, max_y_point, gradients);
    EdgeWalk short_top_edge(min_y_point, mid_y_point, gradients);
    EdgeWalk short_bottom_edge(mid_y_point, max_y_point, gradients);

    bool righthanded = glm::cross(glm::vec3(max_y_point.pos - min_y_point.pos),
                                  glm::vec3(mid_y_point.pos - min_y_point.pos))
                           .z < 0.0f;

    // TODO: Remove preprocessor
#ifdef FACE_CULLING
    bool clockwise_winding_order =
        glm::cross(glm::vec3(triangle.GetPointB().pos - triangle.GetPointA().pos),
                   glm::vec3(triangle.GetPointC().pos - triangle.GetPointA().pos))
            .z > 0.0f;

    if (clockwise_winding_order) {
        return;
    }
#endif

    ScanBetweenEdges(&long_edge, &short_top_edge, righthanded, texture);
    ScanBetweenEdges(&long_edge, &short_bottom_edge, righthanded, texture);
}

const Image& PrimitiveRenderer::GetRendered() const {
    return screen_;
}

void PrimitiveRenderer::TransformPoint(Point* p) {
    float w = p->pos.w; // Save w for perspective texturing
    p->pos = PerspectiveDivide(p->pos);
    // TODO: Don't reconstruct matrix every time
    p->pos = ScreenSpaceTransformMatrix(screen_.GetWidth(), screen_.GetHeight()) * p->pos; 
    p->pos.w = w;
}

void PrimitiveRenderer::ScanBetweenEdges(EdgeWalk* long_edge, EdgeWalk* short_edge,
                                         bool righthanded, const Texture& texture) {
    int32_t start_y = short_edge->GetBeginY();
    int32_t end_y = short_edge->GetEndY();
    EdgeWalk* left_edge = long_edge;
    EdgeWalk* right_edge = short_edge;

    if (!righthanded) {
        std::swap(left_edge, right_edge);
    }

    for (int32_t y = start_y; y < end_y; ++y) {
        int start_x = left_edge->GetCurrentX();
        int end_x = right_edge->GetCurrentX();

        assert(start_x <= end_x);
        // hack to ensure that thin triangles are drawn correctly, TODO: fix this
        end_x = std::max(end_x, start_x + 1);  

        float lerp_amount = 0.0f;

        if (start_x != end_x) {
            float lerp_step = 1.0f / (static_cast<float>(end_x) - static_cast<float>(start_x));

            for (int32_t x = start_x; x < end_x; ++x, lerp_amount += lerp_step) {
                glm::vec4 color = glm::clamp(Lerp(left_edge->GetCurrentColor(),
                                                  right_edge->GetCurrentColor(), lerp_amount),
                                             0.f, 1.f);
                glm::vec2 tex_coord = Lerp(left_edge->GetCurrentTexCoord(),
                                          right_edge->GetCurrentTexCoord(), lerp_amount);
                glm::vec4 tex_color = texture.Sample(tex_coord);
                screen_.SetPixel(x, y, NormalizedColorToRGB(tex_color));
                //screen_.SetPixel(x, y, NormalizedColorToRGB({tex_coord.x, tex_coord.y, 0.f, 1.f}));
            }
        }

        left_edge->StepDown();
        right_edge->StepDown();
    }
}
}  // namespace ani
