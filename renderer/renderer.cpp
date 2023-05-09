#include <renderer/renderer.h>
#include "primitive_renderer/primitive_renderer.h"

namespace ani {

void Renderer::Render(const Model& model, FragmentShader fragment_shader, const glm::vec3& view_pos, const glm::mat4& view_trans, 
const glm::mat4& proj_trans, const glm::mat4& model_trans, const glm::mat4& scale_trans) {
    SetViewPos(view_pos);

    auto triangles = model.GetTriangles();

    for(const auto& triangle : triangles) {
        Triangle nt = triangle;
        nt.Transform(view_trans, proj_trans, model_trans, scale_trans);
        PrimitiveRenderer::Render(nt, model.GetTexture(), fragment_shader);
    }
}

}
