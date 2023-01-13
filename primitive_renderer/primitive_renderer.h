#pragma once
#include <primitive_renderer/primitives.h>
#include <utility/array2d.h>

#include <glm/vec4.hpp>

namespace ani {
// Expects all coordinates in ndc
class PrimitiveRenderer {
public:
    PrimitiveRenderer(uint32_t screen_width, uint32_t screen_height);

    void Clear(const RGB& color);
    void SetScreenSize(uint32_t screen_width, uint32_t screen_height);
    void Render(const Point& point);
    void Render(const Segment& segment);
    void Render(const Triangle& triangle);

    const Image& GetRendered() const;

private:
    bool IsInsideScreen(const Point& p);
    bool IsInsideScreen(int32_t x, int32_t y);
    std::pair<int32_t, int32_t> GetPixelCoordinates(const Point& p);
    int32_t FromNDCToPixelCoordinate(float ndc, int32_t bound);
    ani::RGB ToRGB(const glm::vec4& color);
    glm::vec4 Interpolate(const glm::vec4& v1, const glm::vec4& v2, float coef);

private:
    Image screen_;
    Array2D<float> depth_buf_; 
};
}
