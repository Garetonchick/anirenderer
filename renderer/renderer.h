#pragma once
#include <primitive_renderer/primitive_renderer.h>
#include <renderer/model.h>
#include "glm/ext/matrix_transform.hpp"

namespace ani {

class Renderer : public PrimitiveRenderer {
    using PrimitiveRenderer::PrimitiveRenderer;

public:
    void Render(const Model& model, FragmentShader fragment_shader, 
                const glm::vec3& view_pos,
                const glm::mat4& view_trans, 
                const glm::mat4& proj_trans, 
                const glm::mat4& model_trans = glm::mat4(1.f), 
                const glm::mat4& scale_trans = glm::mat4(1.f)
    );
};

}