#pragma once

class Enemy;

class HitboxRenderer {
private:
    GLuint VBO, VAO, hitboxsh;
    glm::mat4 model{ 1.0f };
    std::vector<float> vertices;

    void setupHitboxBuffers();
    void setupHitboxShaders(const char* vertexName, const char* fragmentName);

public:
    HitboxRenderer();

    ~HitboxRenderer();

    void rendercat(float angle, const glm::mat4& view, const glm::mat4& projection);
    void renderenemy(float angle, const glm::mat4& view, const glm::mat4& projection, Enemy* enemy);

public:
    const char* hitboxvs = "HitboxVert.glsl";
    const char* hitboxfs = "HitboxFrag.glsl";
};