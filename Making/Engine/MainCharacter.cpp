#include "pch.h"
#include "MainCharacter.h"
//#include "Texture.h"

//struct AnimInfo {
//    float Duration;
//    float TicksPerSecond;
//    float CurrentTime;
//    const aiAnimation* animation;
//    const aiNode* rootNode;
//    bool isPlaying;
//
//    AnimInfo() {
//        Duration = 0.0f;
//        TicksPerSecond = 0.0f;
//        CurrentTime = 0.0f;
//        animation = nullptr;
//        rootNode = nullptr;
//        isPlaying = false;
//    }
//};

//extern struct BoneInfo;
//extern std::vector<BoneInfo> player_BoneInfo;
//extern float getCamerahori();
//extern bool getFirstPersonView();
//extern bool wallcollapsed_w(), wallcollapsed_s(), wallcollapsed_a(), wallcollapsed_d();
//extern bool wallcollapsed_forward(const glm::vec3& forward);
//extern bool wallcollapsed_backward(const glm::vec3& forward);
//extern bool wallcollapsed_right(const glm::vec3& right);
//extern bool wallcollapsed_left(const glm::vec3& right);
//
//extern void setupBoneTransforms(std::vector<BoneInfo>& BoneInfoName, GLuint shadername);
//extern void UpdateAnimation(int j, std::vector<BoneInfo>& BoneInfoName, float deltaTime, AnimInfo& currentAnim);
//
//extern void loadGLBFile(int j, std::vector<BoneInfo>& BoneInfoName, const std::string& filename, GLuint& VAO, GLuint& VBO, GLuint& VBO2, GLuint& EBO, std::vector<unsigned int>& Indices);
//void setupShader(const char* vertexName, const char* fragmentName, GLuint& shaderName);
//extern Texture textureLoader;
//extern AnimInfo player_CurrentAnim;
//extern bool firing;
//extern bool firing_induration;
//extern float light_angle;
//extern bool finish;

void MainCharacter::Init()
{
    //loadGLBFile(0, player_BoneInfo, "basepose/cat_Tpose.glb", VAO, VBO, VBO2, EBO, Indices);
    Texture = LoadTexture("Texture/CatTexture.png");
    SetUpShader("Shaders/CatVert.glsl", "Shaders/CatFrag.glsl", shaderprogram);
}

void MainCharacter::Update()
{

}

void MainCharacter::Draw(glm::mat4 view, glm::mat4 projection)
{
    //if (!dead)
    //{
        //UpdateAnimation(0, player_BoneInfo, deltaTime, player_CurrentAnim);
        glUseProgram(shaderprogram);
        //setupBoneTransforms(player_BoneInfo, shaderprogram);

        //if (!dying)
        //    lastangle = angle;

        model = glm::mat4(1.0f);
        model = glm::translate(model, characterPos);
        //if (!dying)
        //    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        //else
        //    model = glm::rotate(model, lastangle, glm::vec3(0.0f, 1.0f, 0.0f));

        //if (finish)
        //    model = glm::scale(model, glm::vec3(20.0f));

        ViewLoc = glGetUniformLocation(shaderprogram, "view");
        glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, &view[0][0]);
        ProjLoc = glGetUniformLocation(shaderprogram, "projection");
        glUniformMatrix4fv(ProjLoc, 1, GL_FALSE, &projection[0][0]);
        ModelLoc = glGetUniformLocation(shaderprogram, "model");
        glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, &model[0][0]);

        //GLuint lightSpaceMatrixLoc = glGetUniformLocation(shaderprogram, "lightSpaceMatrix");
        //glUniformMatrix4fv(lightSpaceMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

        glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, depthMap);
        //GLuint shadowMapLoc = glGetUniformLocation(shaderprogram, "shadowMap");
        //glUniform1i(shadowMapLoc, 1);

        //GLuint lightPosLoc = glGetUniformLocation(shaderprogram, "lightPos");
        //glm::vec3 lightPos{ -37.3051f - (1000.0f * cos(light_angle)), 0.0f + 1000.0f, 42.5001f + (1000.0f * sin(light_angle)) };
        //glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        TextureLoc = glGetUniformLocation(shaderprogram, "catTexture");
        glUniform1i(TextureLoc, 0);

        colorHitLoc = glGetUniformLocation(shaderprogram, "colorHit");
        glUniform4fv(colorHitLoc, 1, glm::value_ptr(hitcolor));

        UseTextureLoc = glGetUniformLocation(shaderprogram, "useTexture");
        glUniform1i(UseTextureLoc, 1);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //if (hitcolor == glm::vec4(1.0f, 0.6f, 0.6f, 1.0f))
        //{
        //    hit_cnt -= 1;
        //}

        //if (hit_cnt == 0)
        //{
        //    hitcolor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        //    hit_cnt = 200;
        //}
    //}
    //else if (dead && revive_timer > 0)
    //{
    //    revive_timer -= 1.0f;
    //}
    //else if (dead && revive_timer == 0)
    //{
    //    life = { 6 };
    //    dead = { false };
    //    dying = { false };
    //    revive_timer = { 1200.0f };
    //    characterPos = { -37.3051f, 0.0f, 42.5001f };
    //    hit_cnt = { 200 };
    //    firing = false;
    //    firing_induration = false;
    //}
}

//void MainCharacter::drawshadow(MainCharacter* mainCat, float angle, ShadowMapping* shadowMap)
//{
//    if (!dead)
//    {
//        model = glm::mat4(1.0f);
//        model = glm::translate(model, mainCat->getPosition());
//        if (!dying)
//            model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
//        else
//            model = glm::rotate(model, lastangle, glm::vec3(0.0f, 1.0f, 0.0f));
//        if (finish)
//            model = glm::scale(model, glm::vec3(20.0f));
//        ModelLoc = glGetUniformLocation(shadowMap->getDepthShaderProgram(), "model");
//        glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));
//        setupBoneTransforms(player_BoneInfo, shadowMap->getDepthShaderProgram());
//        glBindVertexArray(VAO);
//        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
//    }
//}

//void MainCharacter::setRight_on(bool in)
//{
//    Right_on = in;
//}
//
//void MainCharacter::setLeft_on(bool in)
//{
//    Left_on = in;
//}
//
//void MainCharacter::setTop_on(bool in)
//{
//    Top_on = in;
//}
//
//void MainCharacter::setBottom_on(bool in)
//{
//    Bottom_on = in;
//}
//
//void MainCharacter::shift_on(bool in)
//{
//    Shift_on = in;
//}
//
//void MainCharacter::hitboxOnOff(bool in)
//{
//    hitbox_on = in;
//}

//void MainCharacter::walk() {
//    if (!getFirstPersonView()) {
//        if (Right_on && !wallcollapsed_d())
//            characterPos.x += 0.005f;
//        if (Left_on && !wallcollapsed_a())
//            characterPos.x -= 0.005f;
//        if (Top_on && !wallcollapsed_w())
//            characterPos.z -= 0.005f;
//        if (Bottom_on && !wallcollapsed_s())
//            characterPos.z += 0.005f;
//    }
//    else {
//        glm::vec3 forward(
//            sin(getCamerahori()),
//            0,
//            cos(getCamerahori())
//        );
//        glm::vec3 right = glm::cross(forward, glm::vec3(0, 1, 0));
//
//        glm::vec3 moveDir(0.0f);
//        if (Top_on && !wallcollapsed_forward(forward)) moveDir += forward;
//        if (Bottom_on && !wallcollapsed_backward(forward)) moveDir -= forward;
//        if (Right_on && !wallcollapsed_right(right)) moveDir += right;
//        if (Left_on && !wallcollapsed_left(right)) moveDir -= right;
//
//        if (glm::length(moveDir) > 0) {
//            moveDir = glm::normalize(moveDir);
//            glm::vec3 nextPos = characterPos + moveDir * 0.005f;
//            characterPos = nextPos;
//        }
//    }
//}

//void MainCharacter::run()
//{
//    if (Shift_on) {
//        if (!getFirstPersonView()) {
//            if (Right_on && !wallcollapsed_d())
//                characterPos.x += 0.012f;
//            if (Left_on && !wallcollapsed_a())
//                characterPos.x -= 0.012f;
//            if (Top_on && !wallcollapsed_w())
//                characterPos.z -= 0.012f;
//            if (Bottom_on && !wallcollapsed_s())
//                characterPos.z += 0.012f;
//        }
//        else {
//            glm::vec3 forward(
//                sin(getCamerahori()),
//                0,
//                cos(getCamerahori())
//            );
//            glm::vec3 right = glm::cross(forward, glm::vec3(0, 1, 0));
//
//            glm::vec3 moveDir(0.0f);
//            if (Top_on && !wallcollapsed_forward(forward)) moveDir += forward;
//            if (Bottom_on && !wallcollapsed_backward(forward)) moveDir -= forward;
//            if (Right_on && !wallcollapsed_right(right)) moveDir += right;
//            if (Left_on && !wallcollapsed_left(right)) moveDir -= right;
//
//            if (glm::length(moveDir) > 0) {
//                moveDir = glm::normalize(moveDir);
//                glm::vec3 nextPos = characterPos + moveDir * 0.01f;
//                characterPos = nextPos;
//            }
//        }
//    }
//}

//void MainCharacter::jump(bool in)
//{
//    /*if (in)
//        characterPos.y += 0.05;*/   // 점프는 나중에
//}

//void MainCharacter::stop()
//{
//    Right_on = { false };
//    Left_on = { false };
//    Top_on = { false };
//    Bottom_on = { false };
//    Shift_on = { false };
//    hitbox_on = { false };
//}

//void MainCharacter::changehitColor()
//{
//    if (hitcolor.g == 1.0f && hitcolor.b == 1.0f)
//    {
//        hitcolor.g = 0.6f;
//        hitcolor.b = 0.6f;
//    }
//    else if (hitcolor.g == 0.6f && hitcolor.b == 0.6f)
//    {
//        hitcolor.g = 1.0f;
//        hitcolor.b = 1.0f;
//    }
//}

//void MainCharacter::setlife()
//{
//    if (life > 1)
//    {
//        life -= 1;
//        hitcolor = glm::vec4(1.0f, 0.6f, 0.6f, 1.0f);
//    }
//    else if (life > 0)
//    {
//        life -= 1;
//        hit_cnt = 200;
//        hitcolor = glm::vec4(1.0f, 0.6f, 0.6f, 1.0f);
//        dying = true;
//        Right_on = { false };
//        Left_on = { false };
//        Top_on = { false };
//        Bottom_on = { false };
//        Shift_on = { false };
//        hitbox_on = { false };
//    }
//}