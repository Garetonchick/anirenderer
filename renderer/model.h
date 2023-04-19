#pragma once
#include <primitive_renderer/primitives.h>
#include <primitive_renderer/texture.h>

namespace ani {

class Model {
public:
    explicit Model(const std::string& path, const Texture& texture = Texture());

    const std::vector<Triangle>& GetTriangles() const;
    const Texture& GetTexture() const;

private:
    const char* GetNextVertexIndicies(const char* s, int32_t* vertex_idx, int32_t* tex_coord_idx);

private:
    std::vector<Triangle> triangles_;
    Texture texture_;
};

}