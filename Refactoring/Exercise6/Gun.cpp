#include "Gun.h"
#include "Texture.h"

const char* gunvs = "gunvs.glsl";
const char* gunfs = "gunfs.glsl";

struct AnimInfo {
    float Duration;
    float TicksPerSecond;
    float CurrentTime;
    const aiAnimation* animation;
    const aiNode* rootNode;
    bool isPlaying;

    AnimInfo() {
        Duration = 0.0f;
        TicksPerSecond = 0.0f;
        CurrentTime = 0.0f;
        animation = nullptr;
        rootNode = nullptr;
        isPlaying = false;
    }
};

extern struct BoneInfo;
extern std::vector<BoneInfo> gun_BoneInfo;
extern float getCamerahori();
extern bool getFirstPersonView();
extern bool wallcollapsed_w(), wallcollapsed_s(), wallcollapsed_a(), wallcollapsed_d();
extern bool wallcollapsed_forward(const glm::vec3& forward);
extern bool wallcollapsed_backward(const glm::vec3& forward);
extern bool wallcollapsed_right(const glm::vec3& right);
extern bool wallcollapsed_left(const glm::vec3& right);

extern void setupBoneTransforms(std::vector<BoneInfo>& BoneInfoName, GLuint shadername);
extern void UpdateAnimation(int j, std::vector<BoneInfo>& BoneInfoName, float deltaTime, AnimInfo& currentAnim);

void loadGLBFile2(int j, std::vector<BoneInfo>& BoneInfoName, const std::string& filename, GLuint& VAO, GLuint& VBO, GLuint& VBO2, GLuint& EBO, std::vector<unsigned int>& Indices);
void setupShader(const char* vertexName, const char* fragmentName, GLuint& shaderName);
extern Texture textureLoader;
extern AnimInfo gun_CurrentAnim;

Gun::Gun()
{
    loadGLBFile2(1, gun_BoneInfo, "basepose/rifle_test.glb", VAO, VBO, VBO2, EBO, Indices);
    Texture = textureLoader.loadTexture("texture/rifle.png");
    setupShader(gunvs, gunfs, shaderprogram);
}

void Gun::draw(float deltaTime, Gun* gun, float angle, glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos, glm::mat4 lightSpaceMatrix, GLuint depthMap)
{
    UpdateAnimation(1, gun_BoneInfo, deltaTime, gun_CurrentAnim);
    glUseProgram(shaderprogram);
    setupBoneTransforms(gun_BoneInfo, shaderprogram);

    model = glm::mat4(1.0f);
    model = glm::translate(model, gun->getPosition());
    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

    ViewLoc = glGetUniformLocation(shaderprogram, "view");
    glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, &view[0][0]);
    ProjLoc = glGetUniformLocation(shaderprogram, "projection");
    glUniformMatrix4fv(ProjLoc, 1, GL_FALSE, &projection[0][0]);
    ModelLoc = glGetUniformLocation(shaderprogram, "model");
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, &model[0][0]);

    GLuint lightSpaceMatrixLoc = glGetUniformLocation(shaderprogram, "lightSpaceMatrix");
    glUniformMatrix4fv(lightSpaceMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    GLuint shadowMapLoc = glGetUniformLocation(shaderprogram, "shadowMap");
    glUniform1i(shadowMapLoc, 1);

    GLuint lightPosLoc = glGetUniformLocation(shaderprogram, "lightPos");
    GLuint viewPosLoc = glGetUniformLocation(shaderprogram, "viewPos");
    glm::vec3 lightPos{ -37.3051f + 1000.0f, 0.0f + 1000.0f, 42.5001f };
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    TextureLoc = glGetUniformLocation(shaderprogram, "catTexture");
    glUniform1i(TextureLoc, 0);

    UseTextureLoc = glGetUniformLocation(shaderprogram, "useTexture");
    glUniform1i(UseTextureLoc, 1);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Gun::drawshadow(Gun* gun, float angle, ShadowMapping* shadowMap)
{
    model = glm::mat4(1.0f);
    model = glm::translate(model, gun->getPosition());
    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    ModelLoc = glGetUniformLocation(shadowMap->getDepthShaderProgram(), "model");
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    setupBoneTransforms(gun_BoneInfo, shadowMap->getDepthShaderProgram());
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
}

void Gun::setRight_on(bool in)
{
    Right_on = in;
}

void Gun::setLeft_on(bool in)
{
    Left_on = in;
}

void Gun::setTop_on(bool in)
{
    Top_on = in;
}

void Gun::setBottom_on(bool in)
{
    Bottom_on = in;
}

void Gun::shift_on(bool in)
{
    Shift_on = in;
}

void Gun::aim_on(bool in)
{
    Aim_pose = in;
}

void Gun::walk() {
    if (!getFirstPersonView()) {
        if (Right_on && !wallcollapsed_d())
            gunPos.x += 0.005f;
        if (Left_on && !wallcollapsed_a())
            gunPos.x -= 0.005f;
        if (Top_on && !wallcollapsed_w())
            gunPos.z -= 0.005f;
        if (Bottom_on && !wallcollapsed_s())
            gunPos.z += 0.005f;
    }
    else {
        glm::vec3 forward(
            sin(getCamerahori()),
            0,
            cos(getCamerahori())
        );
        glm::vec3 right = glm::cross(forward, glm::vec3(0, 1, 0));

        glm::vec3 moveDir(0.0f);
        if (Top_on && !wallcollapsed_forward(forward)) moveDir += forward;
        if (Bottom_on && !wallcollapsed_backward(forward)) moveDir -= forward;
        if (Right_on && !wallcollapsed_right(right)) moveDir += right;
        if (Left_on && !wallcollapsed_left(right)) moveDir -= right;

        if (glm::length(moveDir) > 0) {
            moveDir = glm::normalize(moveDir);
            glm::vec3 nextPos = gunPos + moveDir * 0.005f;
            gunPos = nextPos;
        }
    }
}

void Gun::run()
{
    if (Shift_on) {
        if (!getFirstPersonView()) {
            if (Right_on && !wallcollapsed_d())
                gunPos.x += 0.012f;
            if (Left_on && !wallcollapsed_a())
                gunPos.x -= 0.012f;
            if (Top_on && !wallcollapsed_w())
                gunPos.z -= 0.012f;
            if (Bottom_on && !wallcollapsed_s())
                gunPos.z += 0.012f;
        }
        else {
            glm::vec3 forward(
                sin(getCamerahori()),
                0,
                cos(getCamerahori())
            );
            glm::vec3 right = glm::cross(forward, glm::vec3(0, 1, 0));

            glm::vec3 moveDir(0.0f);
            if (Top_on && !wallcollapsed_forward(forward)) moveDir += forward;
            if (Bottom_on && !wallcollapsed_backward(forward)) moveDir -= forward;
            if (Right_on && !wallcollapsed_right(right)) moveDir += right;
            if (Left_on && !wallcollapsed_left(right)) moveDir -= right;

            if (glm::length(moveDir) > 0) {
                moveDir = glm::normalize(moveDir);
                glm::vec3 nextPos = gunPos + moveDir * 0.01f;
                gunPos = nextPos;
            }
        }
    }
}

void Gun::die()
{

}