#pragma once
#include <primitive_renderer/primitives.h>
#include <primitive_renderer/edge_walk.h>
#include <primitive_renderer/texture.h>
#include <primitive_renderer/lights.h>
#include <utility/array2d.h>

#include <glm/vec4.hpp>

#include <functional>
#include <optional>

namespace ani {

using FragmentShader = glm::vec4(const ani::Texture& texture, float w, float lerp_amount,
                                 const ani::EdgeWalk& left_edge, const ani::EdgeWalk& right_edge,
                                 const glm::vec3& view_pos,
                                 const std::vector<ani::PointLight>& point_lights);

glm::vec4 StandartShader(const ani::Texture& texture, float w, float lerp_amount,
                         const ani::EdgeWalk& left_edge, const ani::EdgeWalk& right_edge,
                         const glm::vec3& view_pos,
                         const std::vector<ani::PointLight>& point_lights);

glm::vec4 LightlessShader(const ani::Texture& texture, float w, float lerp_amount,
                          const ani::EdgeWalk& left_edge, const ani::EdgeWalk& right_edge,
                          const glm::vec3& view_pos,
                          const std::vector<ani::PointLight>& point_lights);

class PrimitiveRenderer {
public:
    explicit PrimitiveRenderer(uint32_t screen_width = 800, uint32_t screen_height = 600);

    void Clear(RGB color = kDefaultColor);
    void SetScreenSize(uint32_t screen_width, uint32_t screen_height);
    void SetViewPos(const glm::vec3& view_pos);
    void SetFaceCulling(bool enable);
    void AddPointLight(const PointLight& light);
    void Render(const Triangle& triangle, const Texture& texture = Texture(),
                FragmentShader fragment_shader = StandartShader);
    const Image& GetRendered() const;

private:
    void RenderClippedTriangles(const std::vector<Point>& points, const Texture& texture,
                                FragmentShader fragment_shader);
    void RenderTriangle(Point p1, Point p2, Point p3, const Texture& texture,
                        FragmentShader fragment_shader);
    void TransformClippedToScreen(Point* p);
    void ScanBetweenEdges(EdgeWalk* long_edge, EdgeWalk* short_edge, bool righthanded,
                          const Texture& texture, FragmentShader fragment_shader);
    std::vector<Point> ClipTriangle(const Triangle& triangle);
    bool ClipAxis(std::vector<Point>* in, std::vector<Point>* buf, int axis_idx);
    void ClipBound(const std::vector<Point>& in, std::vector<Point>* out, int axis_idx,
                   bool positive);
    glm::mat4 CalcScreenSpaceTransformMatrix(int32_t width, int32_t height);

private:
    static constexpr const RGB kDefaultColor = {50, 50, 50};

    Image screen_;
    Array2D<float> depth_buf_;
    std::vector<PointLight> point_lights_;
    glm::vec3 view_pos_;
    bool face_culling_enabled_ = false;
};
}  // namespace ani
