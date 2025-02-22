#pragma once

class Bullet;
class ShadowMapping;

class Enemy
{
public:
	Enemy(int num, int POINT);
    ~Enemy();

    void draw(int POINT, float deltaTime, Enemy* enemy, glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos, glm::mat4 lightSpaceMatrix, GLuint depthMap);
    void drawshadow(int POINT, Enemy* enemy, ShadowMapping* shadowMap, float deltaTime);

    void movetoward();
    void lookupdate();

    bool wallcollapsed_s();
    bool wallcollapsed_w();
    bool wallcollapsed_d();
    bool wallcollapsed_a();

    void setAnimation();

    void setlife();

    const glm::vec3& getPosition() const { return enemypos; }
    const glm::mat4& getmodel() const { return model; }
    const int& getstate() const { return state; }
    const int& getlife() const { return life; }
    const bool& getdead() const { return dead; }
    const float& getlastangle() const { return lastangle; }

    void setdie();
    void setrevive_timer();

private:
    glm::vec3 enemypos, targetpos;
    bool aim_target{ false };
    GLuint VAO, VBO, VBO2, EBO, shaderprogram, Texture;
    GLuint ViewLoc, ProjLoc, ModelLoc, TextureLoc, UseTextureLoc, colorHitLoc;
    GLuint lVAO, lVBO, shaderprogram2;
    std::vector<unsigned int> Indices;
	glm::mat4 model{ 1.0f };
    glm::vec4 hitcolor{ 1.0f, 1.0f, 1.0f, 1.0f };
    float lastangle;
    int type{ 0 };
    int point{ -1 };
    int state{ 0 };     // 0: Idle, 1: Run, 2:Attack, 3: Hit, 4: Die, 5: Dance
    int life{ 6 };
    int hit_cnt{ 200 };
    float revive_timer{ 2400.0f };   // 240Hz 모니터 기준 10초
    bool dead{ false };
    bool fire{ false };
    bool newbullet{ false };

    Bullet* newBullet;
    glm::vec3 tPos;
};