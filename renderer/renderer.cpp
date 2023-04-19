#include <renderer/renderer.h>

namespace ani {

void Renderer::Render(const Model& model, glm::mat4 trans) {
    auto triangles = model.GetTriangles();

    for(const auto& triangle : triangles) {
        Triangle nt = triangle;
        nt.Transform(trans);
        PrimitiveRenderer::Render(nt, model.GetTexture());
    }
}

}