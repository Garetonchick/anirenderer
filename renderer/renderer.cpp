#include <renderer/renderer.h>

namespace ani {

void Renderer::Render(const Model& model, glm::mat4 trans) {
    auto triangles = model.GetTriangles();

    for(const auto& triangle : triangles) {
        Triangle nt = triangle;
        nt.Transform(trans);
        // std::cout << "Before primitive render" << std::endl;
        // std::cout << nt.GetPointA().pos << std::endl;
        // std::cout << nt.GetPointB().pos << std::endl;
        // std::cout << nt.GetPointC().pos << std::endl;
        // std::cout.flush();
        PrimitiveRenderer::Render(nt);
        // std::cout << "After primitive render" << std::endl;
        // std::cout.flush();
    }
}

}