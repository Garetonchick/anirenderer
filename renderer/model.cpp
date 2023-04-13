#include <renderer/model.h>
#include <fstream>
#include <stdexcept>
#include "utility/image.h"

namespace ani {

Model::Model(const std::string& path) {
    std::ifstream in(path);

    if(!in) {
        throw std::runtime_error(std::string("Couldn't open file: ") + path);
    }

    std::string s;
    std::vector<glm::vec4> verticies;

    while(std::getline(in, s)) {
        if(s.empty()) {
            continue;
        } 

        if(s[0] == 'v') {
            verticies.push_back({});
            glm::vec4& new_vertex = verticies.back();
            sscanf(s.c_str() + 1, "%f %f %f", &new_vertex.x, &new_vertex.y, &new_vertex.z);
            new_vertex.w = 1.f;
        } else if(s[0] == 'f') {
            int i1, i2, i3;
            sscanf(s.c_str() + 1, "%d %d %d", &i1, &i2, &i3);
            --i1; --i2; --i3;
            assert(std::min({i1, i2, i3}) >= 0 && std::max({i1, i2, i3}) < verticies.size());

            Point p1 = {.pos = verticies[i1], .color = RandomColor()};
            Point p2 = {.pos = verticies[i2], .color = RandomColor()};
            Point p3 = {.pos = verticies[i3], .color = RandomColor()};

            triangles_.emplace_back(p1, p2, p3);
        }
    }
}

const std::vector<Triangle>& Model::GetTriangles() const {
    return triangles_;
}

}