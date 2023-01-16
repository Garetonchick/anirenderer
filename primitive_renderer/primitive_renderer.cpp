#include <primitive_renderer/primitive_renderer.h>

#include <primitive_renderer/primitives.h>
#include <primitive_renderer/edge_walk.h>
#include <utility/utility.h>
#include <glm/geometric.hpp>

#include <limits>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <iostream>

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
        (void)point;
        throw std::runtime_error("Not implemented");
    }

    void PrimitiveRenderer::Render(const Segment& segment) {
        (void)segment;
        throw std::runtime_error("Not implemented");
    }

    void PrimitiveRenderer::Render(const Triangle& triangle) {
        const Point* points[] = {&triangle.GetPointA(), &triangle.GetPointB(), &triangle.GetPointC()};
        std::sort(points, points + 3, [](const Point* a, const Point* b){
            return a->pos.y < b->pos.y;
        });

        const Point& min_y_point = *points[0];
        const Point& mid_y_point = *points[1]; 
        const Point& max_y_point = *points[2]; 

        EdgeWalk long_edge(min_y_point, max_y_point);
        EdgeWalk short_top_edge(min_y_point, mid_y_point);
        EdgeWalk short_bottom_edge(mid_y_point, max_y_point);

        bool righthanded = glm::cross(glm::vec3(max_y_point.pos), glm::vec3(mid_y_point.pos)).z <= 0.0f;

        ScanBetweenEdges(&long_edge, &short_top_edge, righthanded);
        ScanBetweenEdges(&long_edge, &short_bottom_edge, righthanded);
    }

    const Image& PrimitiveRenderer::GetRendered() const {
        return screen_;
    }

    void PrimitiveRenderer::ScanBetweenEdges(EdgeWalk* long_edge, EdgeWalk* short_edge, bool righthanded) {
        int32_t start_y = short_edge->GetBeginY();
        int32_t end_y = short_edge->GetEndY();
        EdgeWalk* left_edge = long_edge;
        EdgeWalk* right_edge = short_edge;

        if(!righthanded) {
            std::swap(left_edge, right_edge);
        }
        
        for(int32_t y = start_y; y < end_y; ++y) {
            int start_x = left_edge->GetCurrentX();
            int end_x = right_edge->GetCurrentX();

            for(int32_t x = start_x; x < end_x; ++x) {
                screen_.SetPixel(x, y, ani::RGB{0, 255, 0});
            }

            left_edge->StepDown();
            right_edge->StepDown();
        }
    }
}
