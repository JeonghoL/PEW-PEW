#pragma once

class ShadowMapping {
private:
    const unsigned int SHADOW_WIDTH = 4096;
    const unsigned int SHADOW_HEIGHT = 4096;

    unsigned int depthMapFBO;
    unsigned int depthMap;

    unsigned int depthMapFBO_enemy;
    unsigned int depthMap_enemy;

    glm::mat4 lightProjection, lightProjection_enemy;
    glm::mat4 lightView, lightView_enemy;
    glm::mat4 lightSpaceMatrix, lightSpaceMatrix_enemy;
    GLuint depthShaderProgram;
    GLuint staticdepthShaderProgram;

    void initShaders();

public:
    ShadowMapping();
    ~ShadowMapping();
    void init();

    unsigned int getDepthMapFBO() const { return depthMapFBO; }
    unsigned int getDepthMap() const { return depthMap; }

    unsigned int getDepthMapFBO_Enemy() const { return depthMapFBO_enemy; }
    unsigned int getDepthMap_Enemy() const { return depthMap_enemy; }

    void bindFramebuffer() const;
    void unbindFramebuffer() const;

    glm::mat4 getLightSpaceMatrix() const { return lightSpaceMatrix; }
    glm::mat4 getLightSpaceMatrix_Enemy() const { return lightSpaceMatrix_enemy; }

    void updateLightSpaceMatrix(const glm::vec3& currentCharacterPos);

    GLuint getDepthShaderProgram() const { return depthShaderProgram; }
    GLuint getstaticDepthShaderProgram() const { return staticdepthShaderProgram; }
};