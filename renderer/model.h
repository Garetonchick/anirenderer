#pragma once
#include <primitive_renderer/primitives.h>

namespace ani {

class Model {
public:
    Model(const char* path);
    const std::vector<Triangle>& GetTriangles() const;

private:
    std::vector<Triangle> triangles_;
};

}