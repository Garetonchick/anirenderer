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
#include "primitive_renderer/lights.h"
#include "primitive_renderer/texture.h"
#include "utility/image.h"
#include "utility/math.h"

// #define FACE_CULLING

namespace ani {

glm::vec4 StandartShader(const ani::Texture& texture, float w, float lerp_amount,
                         const ani::EdgeWalk& left_edge, const ani::EdgeWalk& right_edge,
                         const glm::vec3& view_pos,
                         const std::vector<ani::PointLight>& point_lights) {
    glm::vec4 color = glm::clamp(
        Lerp(left_edge.GetCurrentColor(), right_edge.GetCurrentColor(), lerp_amount) * w, 0.f, 1.f);
    glm::vec2 tex_coord =
        Lerp(left_edge.GetCurrentTexCoord(), right_edge.GetCurrentTexCoord(), lerp_amount) * w;
    glm::vec4 tex_color = texture.Sample(tex_coord);
    glm::vec4 world_pos =
        Lerp(left_edge.GetCurrentWorldPos(), right_edge.GetCurrentWorldPos(), lerp_amount) * w;

    glm::vec4 frag_color = Lerp(color, tex_color, tex_color.w);
    glm::vec3 light_color(0.f);

    for (auto& light : point_lights) {
        light_color += CalculateLightValue(light, left_edge.GetNormal(), world_pos, view_pos);
    }

    return glm::clamp(frag_color * glm::vec4(light_color, 1.f), 0.f, 1.f);
}

glm::vec4 LightlessShader(const ani::Texture& texture, float w, float lerp_amount,
                          const ani::EdgeWalk& left_edge, const ani::EdgeWalk& right_edge,
                          const glm::vec3& view_pos,
                          const std::vector<ani::PointLight>& point_lights) {
    glm::vec4 color = glm::clamp(
        Lerp(left_edge.GetCurrentColor(), right_edge.GetCurrentColor(), lerp_amount) * w, 0.f, 1.f);
    glm::vec2 tex_coord =
        Lerp(left_edge.GetCurrentTexCoord(), right_edge.GetCurrentTexCoord(), lerp_amount) * w;
    glm::vec4 tex_color = texture.Sample(tex_coord);
    glm::vec4 world_pos =
        Lerp(left_edge.GetCurrentWorldPos(), right_edge.GetCurrentWorldPos(), lerp_amount) * w;

    glm::vec4 frag_color = Lerp(color, tex_color, tex_color.w);

    return frag_color;
}

PrimitiveRenderer::PrimitiveRenderer(uint32_t screen_width, uint32_t screen_height)
    : screen_(screen_width, screen_height),
      depth_buf_(screen_width, screen_height, 1.f),
      view_pos_(0.f) {
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

void PrimitiveRenderer::SetViewPos(const glm::vec3& view_pos) {
    view_pos_ = view_pos;
}

void PrimitiveRenderer::SetFaceCulling(bool enable) {
    face_culling_enabled_ = enable;
}

void PrimitiveRenderer::AddPointLight(const PointLight& light) {
    point_lights_.push_back(light);
}

void PrimitiveRenderer::Render(const Triangle& triangle, const Texture& texture,
                               FragmentShader fragment_shader) {
    RenderClippedTriangles(ClipTriangle(triangle), texture, fragment_shader);
}

const Image& PrimitiveRenderer::GetRendered() const {
    return screen_;
}

void PrimitiveRenderer::RenderClippedTriangles(const std::vector<Point>& points,
                                               const Texture& texture,
                                               FragmentShader fragment_shader) {
    for (int i = 1; i + 1 < points.size(); ++i) {
        RenderTriangle(points[0], points[i], points[i + 1], texture, fragment_shader);
    }
}

void PrimitiveRenderer::RenderTriangle(Point p1, Point p2, Point p3, const Texture& texture,
                                       FragmentShader fragment_shader) {
    Point* points[] = {&p1, &p2, &p3};

    TransformPoint(points[0]);
    TransformPoint(points[1]);
    TransformPoint(points[2]);

    if (face_culling_enabled_) {
        bool clockwise_winding_order =
            glm::cross(glm::vec3(p2.pos - p1.pos), glm::vec3(p3.pos - p1.pos)).z > 0.0f;

        if (clockwise_winding_order) {
            return;
        }
    }

    std::sort(points, points + 3,
              [](const Point* a, const Point* b) { return a->pos.y < b->pos.y; });

    Point& min_y_point = *points[0];
    Point& mid_y_point = *points[1];
    Point& max_y_point = *points[2];

    Gradients gradients(p1, p2, p3);

    EdgeWalk long_edge(min_y_point, max_y_point, gradients);
    EdgeWalk short_top_edge(min_y_point, mid_y_point, gradients);
    EdgeWalk short_bottom_edge(mid_y_point, max_y_point, gradients);

    bool righthanded = glm::cross(glm::vec3(max_y_point.pos - min_y_point.pos),
                                  glm::vec3(mid_y_point.pos - min_y_point.pos))
                           .z < 0.0f;

    ScanBetweenEdges(&long_edge, &short_top_edge, righthanded, texture, fragment_shader);
    ScanBetweenEdges(&long_edge, &short_bottom_edge, righthanded, texture, fragment_shader);
}

void PrimitiveRenderer::TransformPoint(Point* p) {
    float w = p->pos.w;  // Save w for perspective texturing
    p->pos = PerspectiveDivide(p->pos);
    p->pos = ScreenSpaceTransformMatrix(screen_.GetWidth(), screen_.GetHeight()) * p->pos;
    p->pos.w = w;
}

void PrimitiveRenderer::ScanBetweenEdges(EdgeWalk* long_edge, EdgeWalk* short_edge,
                                         bool righthanded, const Texture& texture,
                                         FragmentShader fragment_shader) {
    int32_t start_y = std::max(0, short_edge->GetBeginY());
    int32_t end_y = std::min<int32_t>(short_edge->GetEndY(), screen_.GetHeight() - 1);
    EdgeWalk* left_edge = long_edge;
    EdgeWalk* right_edge = short_edge;

    if (!righthanded) {
        std::swap(left_edge, right_edge);
    }

    for (int32_t y = start_y; y < end_y; ++y, left_edge->StepDown(), right_edge->StepDown()) {
        int start_x = left_edge->GetCurrentX();
        int end_x = std::max(right_edge->GetCurrentX(), start_x + 1);
        float lerp_amount = 0.0f;
        float lerp_step = 1.0f / (static_cast<float>(end_x) - static_cast<float>(start_x));

        for (int32_t x = start_x; x < end_x; ++x, lerp_amount += lerp_step) {
            float inv_w =
                Lerp(left_edge->GetCurrentInvW(), right_edge->GetCurrentInvW(), lerp_amount);
            float w = 1.0 / inv_w;
            float z = Lerp(left_edge->GetCurrentZ(), right_edge->GetCurrentZ(), lerp_amount) * w;

            if (z > depth_buf_(y, x)) {
                continue;
            }

            depth_buf_(y, x) = z;
            screen_.SetPixel(
                x, y,
                NormalizedColorToRGB(fragment_shader(texture, w, lerp_amount, *left_edge,
                                                     *right_edge, view_pos_, point_lights_)));
        }
    }
}

std::vector<Point> PrimitiveRenderer::ClipTriangle(const Triangle& triangle) {
    std::vector<Point> points = {triangle.GetPointA(), triangle.GetPointB(), triangle.GetPointC()};
    std::vector<Point> buf;
    buf.reserve(6);
    if (ClipAxis(&points, &buf, 0) && ClipAxis(&points, &buf, 1) && ClipAxis(&points, &buf, 2)) {
        return points;
    }

    return {};
}

bool PrimitiveRenderer::ClipAxis(std::vector<Point>* in, std::vector<Point>* buf, int axis_idx) {
    buf->clear();
    ClipBound(*in, buf, axis_idx, true);

    if (buf->empty()) {
        return false;
    }

    in->clear();
    ClipBound(*buf, in, axis_idx, false);

    return !in->empty();
}

void PrimitiveRenderer::ClipBound(const std::vector<Point>& in, std::vector<Point>* out,
                                  int axis_idx, bool positive) {
    float side = (positive ? 1 : -1);
    bool is_prev_inside = (side * in.back().pos[axis_idx] <= in.back().pos.w);
    const Point* prev_p = &in.back();

    for (const Point& p : in) {
        bool is_inside = (side * p.pos[axis_idx] <= p.pos.w);

        if (is_prev_inside ^ is_inside) {
            float cur_p_diff = p.pos.w - p.pos[axis_idx] * side;
            float prev_p_diff = prev_p->pos.w - prev_p->pos[axis_idx] * side;
            float lerp_amount = cur_p_diff / (cur_p_diff - prev_p_diff);

            out->push_back(p.Lerp(*prev_p, lerp_amount));
        }

        if (is_inside) {
            out->push_back(p);
        }

        prev_p = &p;
        is_prev_inside = is_inside;
    }
}

}  // namespace ani
