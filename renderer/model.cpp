#include <renderer/model.h>
#include <cstdio>
#include <fstream>
#include <stdexcept>
#include "glm/geometric.hpp"
#include "utility/image.h"

namespace ani {

Model::Model(const std::string& path, const Texture& texture, const glm::mat4& scale_trans) : texture_(texture), scale_trans_(scale_trans) {
    LoadModel(path, scale_trans);
}

Model::Model(const std::string& path, const glm::vec4& color, const glm::mat4& scale_trans) : scale_trans_(scale_trans) {
    LoadModel(path, scale_trans);

    for(auto& triangle : triangles_) {
        triangle.GetPointA().color = triangle.GetPointB().color = triangle.GetPointC().color = color;
    }
}

const std::vector<Triangle>& Model::GetTriangles() const {
    return triangles_;
}

const Texture& Model::GetTexture() const {
    return texture_;
}

char* SkipSpaces(char* s) {
    while(*s && *s == ' ') {
        ++s;
    }

    return s;
}

void Model::LoadModel(const std::string& path, const glm::mat4& scale_trans) {
    std::ifstream in(path);

    if(!in) {
        throw std::runtime_error(std::string("Couldn't open file: ") + path);
    }

    std::string s;
    std::vector<glm::vec4> verticies;
    std::vector<glm::vec2> tex_coords;
    std::vector<glm::vec3> normals;

    while(std::getline(in, s)) {
        if(s.empty()) {
            continue;
        } 

        if(s[0] == 'v' && s[1] == ' ') {
            verticies.push_back({});
            glm::vec4& new_vertex = verticies.back();
            sscanf(s.c_str() + 1, "%f %f %f", &new_vertex.x, &new_vertex.y, &new_vertex.z);
            new_vertex.w = 1.f;
        } else if(s[0] == 'f' && s[1] == ' ') {
            int32_t fir_ver_idx = -1;
            int32_t fir_tex_idx = -1;
            int32_t fir_norm_idx = -1;
            int32_t last_ver_idx = -1;
            int32_t last_tex_idx = -1;
            int32_t last_norm_idx = -1;

            const char* raw_s = s.c_str() + 2;

            raw_s = GetNextVertexIndicies(raw_s, &fir_ver_idx, &fir_tex_idx, &fir_norm_idx);
            raw_s = GetNextVertexIndicies(raw_s, &last_ver_idx, &last_tex_idx, &last_norm_idx);

            while(*raw_s) {
                int32_t ver_idx = -1;
                int32_t tex_idx = -1;
                int32_t norm_idx = -1;

                raw_s = GetNextVertexIndicies(raw_s, &ver_idx, &tex_idx, &norm_idx);

                Point p1 = {.pos = verticies[fir_ver_idx], .color = RandomColor() };
                Point p2 = {.pos = verticies[last_ver_idx], .color = RandomColor() }; 
                Point p3 = {.pos = verticies[ver_idx], .color = RandomColor() };

                if(tex_idx != -1) {
                    p1.tex_coords = tex_coords[fir_tex_idx];
                    p2.tex_coords = tex_coords[last_tex_idx];
                    p3.tex_coords = tex_coords[tex_idx];
                } else {
                    p1.tex_coords = {0.f, 0.f};
                    p2.tex_coords = {0.f, 1.f};
                    p3.tex_coords = {1.f, 1.f};
                }

                if(norm_idx != -1) {
                    p1.normal = normals[fir_norm_idx];
                    p2.normal = normals[last_norm_idx];
                    p3.normal = normals[norm_idx];
                } else {
                    p1.normal = p2.normal = p3.normal 
                        = glm::normalize(-glm::cross(glm::vec3(p2.pos - p1.pos), glm::vec3(p2.pos - p3.pos)));
                }

                p1.pos = scale_trans_ * p1.pos;
                p2.pos = scale_trans_ * p2.pos;
                p3.pos = scale_trans_ * p3.pos;
                triangles_.emplace_back(p1, p2, p3);
                last_ver_idx = ver_idx;
                last_tex_idx = tex_idx;
                last_norm_idx = norm_idx;
            }
        } else if(s[0] == 'v' && s[1] == 't') {
            tex_coords.push_back({});
            glm::vec2& tex_coord = tex_coords.back();
            sscanf(s.c_str() + 3, "%f %f", &tex_coord.x, &tex_coord.y);
        } else if(s[0] == 'v' && s[1] == 'n') {
            normals.push_back({});
            glm::vec3& normal = normals.back();
            sscanf(s.c_str() + 3, "%f %f %f", &normal.x, &normal.y, &normal.z);
            normal = glm::normalize(normal);
        }
    }
}

const char* Model::GetNextVertexIndicies(const char* s, int32_t* vertex_idx, int32_t* tex_coord_idx, int32_t* normal_idx) {
    *vertex_idx = *tex_coord_idx = *normal_idx = -1;
    char* nxt = nullptr;
    *vertex_idx = strtol(s, &nxt, 10) - 1;

    if(*nxt != '/') {
        return SkipSpaces(nxt);
    }

    ++nxt;
    char* cur = nxt;
    *tex_coord_idx = strtol(cur, &nxt, 10) - 1;
    
    if(*nxt != '/') {
        return SkipSpaces(nxt);
    }

    ++nxt;
    cur = nxt;
    *normal_idx = strtol(cur, &nxt, 10) - 1;

    return SkipSpaces(nxt);
}
    
}