#include <primitive_renderer/primitive_renderer.h>

#include <primitive_renderer/primitives.h>
#include <primitive_renderer/edge_walk.h>
#include <utility/utility.h>
#include <cstdint>
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
    : screen_(screen_width, screen_height), depth_buf_(screen_width, screen_height, 1.f) {
    Clear({0, 255, 0});
}

void PrimitiveRenderer::Clear(const RGB& color) {
    screen_.Fill(color);
    depth_buf_.Fill(1.f);
}

void PrimitiveRenderer::SetScreenSize(uint32_t screen_width, uint32_t screen_height) {
    screen_.SetSize(screen_width, screen_height);
    depth_buf_.Resize(screen_width, screen_height);
}

void PrimitiveRenderer::Render(const Triangle& triangle, const Texture& texture) {
    auto points = ClipTriangle(triangle);
    assert(points.size() != 1 && points.size() != 2);

    for(int i = 1; i + 1 < points.size(); ++i) {
        Point p0_copy = points[0];
        Point pi_copy = points[i];
        Point p_i_next_copy = points[i + 1];
        RenderTriangle(&p0_copy, &pi_copy, &p_i_next_copy, texture);
    }
}

const Image& PrimitiveRenderer::GetRendered() const {
    return screen_;
}

void PrimitiveRenderer::RenderTriangle(Point* p1, Point* p2, Point* p3, const Texture& texture) {
    Point* points[] = {p1, p2, p3};

    TransformPoint(points[0]);
    TransformPoint(points[1]);
    TransformPoint(points[2]);

    std::sort(points, points + 3,
              [](const Point* a, const Point* b) { return a->pos.y < b->pos.y; });

    Point& min_y_point = *points[0];
    Point& mid_y_point = *points[1];
    Point& max_y_point = *points[2];

    Gradients gradients(*p1, *p2, *p3);

    EdgeWalk long_edge(min_y_point, max_y_point, gradients);
    EdgeWalk short_top_edge(min_y_point, mid_y_point, gradients);
    EdgeWalk short_bottom_edge(mid_y_point, max_y_point, gradients);

    bool righthanded = glm::cross(glm::vec3(max_y_point.pos - min_y_point.pos),
                                  glm::vec3(mid_y_point.pos - min_y_point.pos))
                           .z < 0.0f;


    // TODO: Remove preprocessor
#ifdef FACE_CULLING
    bool clockwise_winding_order =
        glm::cross(glm::vec3(p2->pos - p1->pos),
                   glm::vec3(p3->pos - p1->pos))
            .z > 0.0f;

    if (clockwise_winding_order) {
        return;
    }
#endif
    ScanBetweenEdges(&long_edge, &short_top_edge, righthanded, texture);
    ScanBetweenEdges(&long_edge, &short_bottom_edge, righthanded, texture);
}

void PrimitiveRenderer::TransformPoint(Point* p) {
    float w = p->pos.w; // Save w for perspective texturing
    p->pos = PerspectiveDivide(p->pos);
    p->pos = ScreenSpaceTransformMatrix(screen_.GetWidth(), screen_.GetHeight()) * p->pos; 
    // TODO: Don't reconstruct matrix every time
    p->pos.w = w;
}

void PrimitiveRenderer::ScanBetweenEdges(EdgeWalk* long_edge, EdgeWalk* short_edge,
                                         bool righthanded, const Texture& texture) {
    int32_t start_y = std::max(0, short_edge->GetBeginY());
    int32_t end_y = std::min<int32_t>(short_edge->GetEndY(), screen_.GetHeight() - 1);
    EdgeWalk* left_edge = long_edge;
    EdgeWalk* right_edge = short_edge;

    if (!righthanded) {
        std::swap(left_edge, right_edge);
    }

    for (int32_t y = start_y; y < end_y; ++y) {
        int start_x = left_edge->GetCurrentX();
        int end_x = right_edge->GetCurrentX();

        // assert(start_x <= end_x); TODO: investigate
        // hack to ensure that thin triangles are drawn correctly, TODO: fix this
        end_x = std::max(end_x, start_x + 1);  

        float lerp_amount = 0.0f;

        if (start_x != end_x) {
            float lerp_step = 1.0f / (static_cast<float>(end_x) - static_cast<float>(start_x)); // sus

            // std::cerr << "Before line scan" << std::endl;
            for (int32_t x = start_x; x < end_x; ++x, lerp_amount += lerp_step) {
                float inv_w = Lerp(left_edge->GetCurrentInvW(), right_edge->GetCurrentInvW(), lerp_amount); 
                float w = 1.0 / inv_w; 
                float z = Lerp(left_edge->GetCurrentZ(), right_edge->GetCurrentZ(), lerp_amount) * w;
                if(z < depth_buf_.Get(y, x)) {
                    depth_buf_.Set(y, x, z);

                    glm::vec4 color = glm::clamp(Lerp(left_edge->GetCurrentColor(),
                                                    right_edge->GetCurrentColor(), lerp_amount) * w,
                                                0.f, 1.f);
                    glm::vec2 tex_coord = Lerp(left_edge->GetCurrentTexCoord(),
                                            right_edge->GetCurrentTexCoord(), lerp_amount) * w;
                    glm::vec4 tex_color = texture.Sample(tex_coord);

                    screen_.SetPixel(x, y, NormalizedColorToRGB(Lerp(color, tex_color, tex_color.w)));
                }
            }
        }


        left_edge->StepDown();
        right_edge->StepDown();
    }
}

std::vector<Point> PrimitiveRenderer::ClipTriangle(const Triangle& triangle) {
    std::vector<Point> points = { triangle.GetPointA(), triangle.GetPointB(), triangle.GetPointC() };
    std::vector<Point> buf;
    buf.reserve(6);
    if(ClipAxis(&points, &buf, 0) && ClipAxis(&points, &buf, 1) && ClipAxis(&points, &buf, 2)) {
        return points;
    }

    return {};
}

bool PrimitiveRenderer::ClipAxis(std::vector<Point>* in, std::vector<Point>* buf, int axis_idx) {
    buf->clear();
    ClipBound(*in, buf, axis_idx, true);

    if(buf->empty()) {
        return false;
    }

    in->clear();
    ClipBound(*buf, in, axis_idx, false);

    return !in->empty(); 
}

void PrimitiveRenderer::ClipBound(const std::vector<Point>& in, std::vector<Point>* out, int axis_idx, bool positive) {
    float side = (positive ? 1 : -1);
    bool is_prev_inside = (side * in.back().pos[axis_idx] <= in.back().pos.w);
    const Point* prev_p = &in.back();

    for(const Point& p : in) {
        bool is_inside = (side * p.pos[axis_idx] <= p.pos.w);

        if(is_prev_inside ^ is_inside) {
            float cur_p_diff = p.pos.w -  p.pos[axis_idx] * side;
            float prev_p_diff = prev_p->pos.w - prev_p->pos[axis_idx] * side;
            float lerp_amount = cur_p_diff / (cur_p_diff - prev_p_diff); 

            out->push_back(p.Lerp(*prev_p, lerp_amount));
        }

        if(is_inside) {
            out->push_back(p);
        }

        prev_p = &p;
        is_prev_inside = is_inside;
    }
}

}  // namespace ani
