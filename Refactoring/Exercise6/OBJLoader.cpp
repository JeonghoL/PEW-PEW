#include "OBJLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool OBJLoader::loadOBJ(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << path << std::endl;
        return false;
    }

    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec3> temp_normals;
    std::vector<glm::vec2> temp_texcoords;
    std::string line;
    unsigned int lineNumber = 0;

    while (std::getline(file, line)) {
        lineNumber++;
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            glm::vec3 vertex;
            if (iss >> vertex.x >> vertex.y >> vertex.z) {
                temp_vertices.push_back(vertex);
            }
            else {
                std::cerr << "Error parsing vertex at line " << lineNumber << std::endl;
            }
        }
        else if (type == "vn") {
            glm::vec3 normal;
            if (iss >> normal.x >> normal.y >> normal.z) {
                temp_normals.push_back(normal);
            }
            else {
                std::cerr << "Error parsing normal at line " << lineNumber << std::endl;
            }
        }
        else if (type == "vt") {
            glm::vec2 texcoord;
            if (iss >> texcoord.x >> texcoord.y) {
                temp_texcoords.push_back(texcoord);
            }
            else {
                std::cerr << "Error parsing texture coordinate at line " << lineNumber << std::endl;
            }
        }
        else if (type == "f") {
            std::string vertex1, vertex2, vertex3;
            if (!(iss >> vertex1 >> vertex2 >> vertex3)) {
                std::cerr << "Error parsing face at line " << lineNumber << std::endl;
                continue;
            }

            std::vector<std::string> vertexIndices = { vertex1, vertex2, vertex3 };
            for (const auto& vertexIndex : vertexIndices) {
                std::istringstream indexStream(vertexIndex);
                std::string indexStr;
                std::vector<int> face_indices;

                while (std::getline(indexStream, indexStr, '/')) {
                    if (!indexStr.empty()) {
                        try {
                            face_indices.push_back(std::stoi(indexStr) - 1);
                        }
                        catch (const std::exception& e) {
                            std::cerr << "Error converting index at line " << lineNumber << ": " << e.what() << std::endl;
                            face_indices.push_back(-1);
                        }
                    }
                    else {
                        face_indices.push_back(-1);
                    }
                }

                Vertex vertex;
                bool validVertex = true;

                if (!face_indices.empty() && face_indices[0] >= 0 && face_indices[0] < temp_vertices.size()) {
                    vertex.position = temp_vertices[face_indices[0]];
                }
                else {
                    std::cerr << "Invalid vertex index at line " << lineNumber << ": " << (face_indices.empty() ? -1 : face_indices[0] + 1) << std::endl;
                    validVertex = false;
                }

                if (face_indices.size() > 1 && face_indices[1] >= 0 && face_indices[1] < temp_texcoords.size()) {
                    vertex.texCoords = temp_texcoords[face_indices[1]];
                }
                else if (face_indices.size() > 1 && face_indices[1] >= 0) {
                    std::cerr << "Invalid texture coordinate index at line " << lineNumber << ": " << (face_indices[1] + 1) << std::endl;
                }

                if (face_indices.size() > 2 && face_indices[2] >= 0 && face_indices[2] < temp_normals.size()) {
                    vertex.normal = temp_normals[face_indices[2]];
                }
                else if (face_indices.size() > 2 && face_indices[2] >= 0) {
                    std::cerr << "Invalid normal index at line " << lineNumber << ": " << (face_indices[2] + 1) << std::endl;
                }

                if (validVertex) {
                    vertices.push_back(vertex);
                    ch_indices.push_back(vertices.size() - 1);
                }
            }
        }
    }

    indicesCount = static_cast<GLsizei>(ch_indices.size());

    std::cout << "Loaded OBJ file: " << path << std::endl;
    std::cout << "Loaded vertices: " << temp_vertices.size() << std::endl;
    std::cout << "Loaded texture coordinates: " << temp_texcoords.size() << std::endl;
    std::cout << "Loaded normals: " << temp_normals.size() << std::endl;
    std::cout << "Created vertices: " << vertices.size() << std::endl;
    std::cout << "Created indices: " << indicesCount << std::endl;

    file.close();
    return true;
}