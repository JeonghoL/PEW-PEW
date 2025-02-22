#include "pch.h"
#include "ShadowMapping.h"

extern const char* loadFile(const char* filename);
extern float light_angle;

ShadowMapping::ShadowMapping() : depthMapFBO(0), depthMap(0), depthMapFBO_enemy(0), depthMap_enemy(0) {}

ShadowMapping::~ShadowMapping() {
    glDeleteFramebuffers(1, &depthMapFBO);
    glDeleteTextures(1, &depthMap);
    glDeleteFramebuffers(1, &depthMapFBO_enemy);
    glDeleteTextures(1, &depthMap_enemy);
}

void ShadowMapping::init() {
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glGenFramebuffers(1, &depthMapFBO_enemy);

    glGenTextures(1, &depthMap_enemy);
    glBindTexture(GL_TEXTURE_2D, depthMap_enemy);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_enemy);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap_enemy, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    initShaders();
}

void ShadowMapping::bindFramebuffer() const {
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
}

void ShadowMapping::unbindFramebuffer() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapping::updateLightSpaceMatrix(const glm::vec3& currentCharacterPos) {
    float near_plane = 0.1f;
    float far_plane = 100.0f;
    float ortho_size = 12.0f;

    glm::vec3 adjustedLightPos = currentCharacterPos + glm::vec3(-(60.0f * cos(light_angle)), 60.0f, 0.0f + (60.0f * sin(light_angle)));
    glm::vec3 lightTarget = currentCharacterPos;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    lightProjection = glm::ortho(-ortho_size, ortho_size,
        -ortho_size, ortho_size,
        near_plane, far_plane);

    lightView = glm::lookAt(adjustedLightPos, lightTarget, up);
    lightSpaceMatrix = lightProjection * lightView;
}

void ShadowMapping::setupShader(const char* vertexName, const char* fragmentName, GLuint& shaderName)
{
    const char* vertSource = loadFile(vertexName);
    const char* fragSource = loadFile(fragmentName);

    if (!vertSource || !fragSource) {
        std::cerr << "Failed to load shader files" << std::endl;
        return;
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragSource, NULL);
    glCompileShader(fragmentShader);

    shaderName = glCreateProgram();
    glAttachShader(shaderName, vertexShader);
    glAttachShader(shaderName, fragmentShader);
    glLinkProgram(shaderName);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    free((void*)vertSource);
    free((void*)fragSource);
}

void ShadowMapping::initShaders() {
    setupShader("shadowMappingvs.glsl", "shadowMappingfs.glsl", depthShaderProgram);
    setupShader("shadowstaticMappingvs.glsl", "shadowstaticMappingfs.glsl", staticdepthShaderProgram);
}