#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

class OBJLoader {
public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

    std::vector<Vertex> vertices;
    std::vector<unsigned int> ch_indices;
    GLsizei indicesCount;

    bool loadOBJ(const std::string& path);
    GLsizei getIndicesCount() const { return indicesCount; }  // Getter 메서드 추가
};