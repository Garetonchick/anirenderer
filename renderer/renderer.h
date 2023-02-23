#pragma once
#include <primitive_renderer/primitive_renderer.h>
#include <renderer/model.h>
#include "glm/ext/matrix_transform.hpp"

namespace ani {

class Renderer : public PrimitiveRenderer {
    using PrimitiveRenderer::PrimitiveRenderer;

public:
    void Render(const Model& model, glm::mat4 trans = glm::mat4(1.f));
};

}