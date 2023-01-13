#include <primitive_renderer/primitive_renderer.h>

#include <limits>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "primitive_renderer/primitives.h"
#include "utility/utility.h"

namespace ani {
    PrimitiveRenderer::PrimitiveRenderer(uint32_t screen_width, uint32_t screen_height) : 
        screen_(screen_width, screen_height), depth_buf_(screen_width, screen_height, -1.f) {
        }

    void PrimitiveRenderer::Clear(const RGB& color) {
        screen_.Fill(color); 
    }

    void PrimitiveRenderer::SetScreenSize(uint32_t screen_width, uint32_t screen_height) {
        screen_.SetSize(screen_width, screen_height);
        depth_buf_.Resize(screen_width, screen_height);
    }

    void PrimitiveRenderer::Render(const Point& point) {
        if(!IsInsideScreen(point)) {
            return;
        }

        auto [x, y] = GetPixelCoordinates(point); 

        if(!IsInsideScreen(x, y)) {
            return;
        }

        screen_.SetPixel(x, y, ToRGB(point.color));
    }

    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
    void PrimitiveRenderer::Render(const Segment& segment) {
        auto [x0, y0] = GetPixelCoordinates(segment.points[0]);  
        auto [x1, y1] = GetPixelCoordinates(segment.points[1]);  
        int dx = x1 - x0;
        int dy = y1 - y0;
        int d = 2 * dy - dx;
        int y = y0;

        for(int x = x0; x <= x1; ++x) {
            screen_.SetPixel(
                    x, 
                    y, 
                    ToRGB(Interpolate(segment.points[0].color, segment.points[1].color, static_cast<float>(x1 - x) / static_cast<float>(dx)))
            );

            if(d > 0) {
                ++y;
                d -= 2 * dx;
            } else {
                d += 2 * dy;
            }
        }
    }

    std::pair<int32_t, int32_t> PrimitiveRenderer::FindMinMaxIntersections(float x1, float y1, float x2, float y2, int32_t y) {
        float ndc_y = FromPixelCoordinateToNDC(y, screen_.GetHeight());        

        if(y1 > y2) {
            std::swap(y1, y2);
            std::swap(x1, x2);
        }

        if(y1 > ndc_y || y2 < ndc_y) {
            return {std::numeric_limits<int32_t>::max(), -1};
        }

        float dy = y2 - y1;

        if(std::abs(dy) < kEpsilon) {
            return {FromNDCToPixelCoordinate(std::min(x1, x2), screen_.GetWidth()), FromNDCToPixelCoordinate(std::max(x1, x2), screen_.GetWidth())};
        }

        float k = (x2 - x1) / dy;
        float b = x1 - k * y1;
        float ndc_x = k * ndc_y + b;
        std::cerr << "y is:" << y << std::endl;
        std::cerr << "Ndc y is:" << ndc_y << std::endl;
        std::cerr << "Ndc x is:" << ndc_x << std::endl;
        int32_t x = FromNDCToPixelCoordinate(ndc_x, screen_.GetWidth());

        return {x, x}; 
    }

    std::pair<int32_t, int32_t> PrimitiveRenderer::GetScanBounds(const Triangle& triangle, int32_t y) {
        auto [edge1_mn, edge1_mx] = FindMinMaxIntersections(triangle.points[0].pos.x, triangle.points[0].pos.y, triangle.points[1].pos.x, triangle.points[1].pos.y, y);
        auto [edge2_mn, edge2_mx] = FindMinMaxIntersections(triangle.points[1].pos.x, triangle.points[1].pos.y, triangle.points[2].pos.x, triangle.points[2].pos.y, y);
        auto [edge3_mn, edge3_mx] = FindMinMaxIntersections(triangle.points[0].pos.x, triangle.points[0].pos.y, triangle.points[2].pos.x, triangle.points[2].pos.y, y);
        
        return {std::min({edge1_mn, edge2_mn, edge3_mn}), std::max({edge1_mx, edge2_mx, edge3_mx})};
        //return {std::min({edge2_mn, edge3_mn}), std::max({edge2_mx, edge3_mx})};
        //return {edge3_mn, edge3_mx};
    }

    void PrimitiveRenderer::Render(const Triangle& triangle) {
        int32_t mny = FromNDCToPixelCoordinate(std::min({triangle.points[0].pos.y, triangle.points[1].pos.y, triangle.points[2].pos.y}), screen_.GetHeight());
        int32_t mxy = FromNDCToPixelCoordinate(std::max({triangle.points[0].pos.y, triangle.points[1].pos.y, triangle.points[2].pos.y}), screen_.GetHeight());
        std::cerr << "Y bounds are: " << mny << ", " << mxy << std::endl;

        for(int32_t y = mny; y <= mxy; ++y) {
            auto [mnx, mxx] = GetScanBounds(triangle, y); 

            for(int32_t x = mnx; x <= mxx; ++x) {
                screen_.SetPixel(x, y, ToRGB(triangle.points[0].color));
            }
        }
    }

    const Image& PrimitiveRenderer::GetRendered() const {
        return screen_;
    }

    bool PrimitiveRenderer::IsInsideScreen(const Point& p) {
        return p.pos.x >= -1.f && p.pos.x <= 1.f &&
               p.pos.y >= -1.f && p.pos.y <= 1.f &&
               p.pos.z >= -1.f && p.pos.z <= 1.f; 
    }

    bool PrimitiveRenderer::IsInsideScreen(int32_t x, int32_t y) {
        return x >= 0 && static_cast<uint32_t>(x) < screen_.GetWidth() && 
               y >= 0 && static_cast<uint32_t>(y) < screen_.GetHeight();
    }

    std::pair<int32_t, int32_t> PrimitiveRenderer::GetPixelCoordinates(const Point& p) {
        return {((p.pos.x + 1.f) * 0.5f) * static_cast<float>(screen_.GetWidth()) - 0.5f, 
                ((p.pos.y + 1.f) * 0.5f) * static_cast<float>(screen_.GetHeight()) - 0.5f};
    }

    int32_t PrimitiveRenderer::FromNDCToPixelCoordinate(float ndc, int32_t bound) {
        return std::clamp<int32_t>((ndc + 1.f) * 0.5f * static_cast<float>(bound) - 0.5f, 0, bound - 1);
    }

    float PrimitiveRenderer::FromPixelCoordinateToNDC(int32_t cord, int32_t bound) {
        return (static_cast<float>(cord) + 0.5f) / static_cast<float>(bound) * 2.f - 1.f;
    }

    ani::RGB PrimitiveRenderer::ToRGB(const glm::vec4& color) {
        return ani::RGB{static_cast<uint8_t>(color.r * 255.f), 
                        static_cast<uint8_t>(color.g * 255.f), 
                        static_cast<uint8_t>(color.b * 255.f)}; 
    }

    glm::vec4 PrimitiveRenderer::Interpolate(const glm::vec4& v1, const glm::vec4& v2, float coef) {
        return glm::vec4{v1.x * coef + v2.x * (1.f - coef), 
                         v1.y * coef + v2.y * (1.f - coef),
                         v1.z * coef + v2.z * (1.f - coef),
                         v1.w * coef + v2.w * (1.f - coef)};
    }
}
