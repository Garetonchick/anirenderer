#pragma once
#include <primitive_renderer/primitives.h>
#include <primitive_renderer/texture.h>
#include <string>
#include "glm/ext/matrix_float4x4.hpp"
#include "primitive_renderer/lights.h"

namespace ani {

class Model {
friend std::istream& operator>>(std::istream& in, Model& model); 
public:
    Model() = default;
    Model(const Model& o) = default;
    Model& operator=(const Model& o) = default;
    Model(Model&& o) = default;
    Model& operator=(Model&& o) = default;
    ~Model() = default;

    explicit Model(const std::string& path, const Texture& texture = Texture(),
                   const glm::mat4& scale_trans = glm::mat4(1.f));
    explicit Model(const std::string& path, const glm::vec4& color,
                   const glm::mat4& scale_trans = glm::mat4(1.f));

    const std::vector<Triangle>& GetTriangles() const;
    const Texture& GetTexture() const;


    void SetTexture(Texture texture);
    void SetColor(const glm::vec4& color);
    void SetScale(const glm::mat4& scale);

private:
    void LoadModel(const std::string& path, const glm::mat4& scale_trans);
    const char* GetNextVertexIndicies(const char* s, int32_t* vertex_idx, int32_t* tex_coord_idx,
                                      int32_t* normal_idx);

private:
    std::vector<Triangle> triangles_;
    Texture texture_;
    glm::mat4 scale_trans_ = glm::mat4(1.f);
};

std::istream& operator>>(std::istream& in, Model& model); 
Model LoadModel(const std::string& path, const Texture& texture = Texture(), const glm::mat4& scale_trans = glm::mat4(1.f));
Model LoadModel(const std::string& path, const glm::vec4& color, const glm::mat4& scale_trans = glm::mat4(1.f));

struct Lamp {
    Model model;
    PointLight light;
};

}  // namespace ani
