#pragma once
//#include "ShadowMapping.h"
#include "AnimatedModel.h"

class ShadowMapping;

class MainCharacter
{
public:
	MainCharacter();
	~MainCharacter();

	void Update();
	void Draw(glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos, float deltaTime, float angle);

	void Drawshadow(MainCharacter* mainCat, float angle, ShadowMapping* shadowMap);

	void setRight_on(bool in);
	void setLeft_on(bool in);
	void setTop_on(bool in);
	void setBottom_on(bool in);
	//void shift_on(bool in);
	//void hitboxOnOff(bool in);

	void walk();
	//void run();
	//void jump(bool in);
	//void stop();

	//bool shift_value() const { return Shift_on; }
	//bool hitbox_ison() const { return hitbox_on; }

	//void changehitColor();

	//void setlife();

	bool isMoving() const {
		return Right_on || Left_on || Top_on || Bottom_on;
	}

	const glm::vec3& getPosition() const { return characterPos; }
	const glm::mat4& getmodel() const { return model; }
	const bool& getdying() const { return dying; }

	void setPosition() { characterPos = glm::vec3{ -44.0f, 0.0f, -48.0f }; }
	void setdead(bool in) { dead = in; }
	void setFinishPos() { characterPos = glm::vec3{ -5.0f, 0.0f, 5.0f }; }

	void saveAnimations();
	void setAnimationType(const std::string& animName);

private:
	glm::vec3 characterPos{ -37.3051f, 0.0f, 42.5001f };
	GLuint VAO, VBO, VBO2, EBO, shaderprogram, Texture;
	GLuint ViewLoc, ProjLoc, ModelLoc, TextureLoc, UseTextureLoc, colorHitLoc;
	std::vector<unsigned int> Indices;
	glm::mat4 model{ 1.0f };
	glm::vec4 hitcolor{ 1.0f, 1.0f, 1.0f, 1.0f };
	int hit_cnt{ 200 };
	bool dead{ false };
	bool dying{ false };
	float lastangle;
	int life{ 6 };
	float revive_timer{ 1200.0f };

	bool Right_on{ false }, Left_on{ false }, Top_on{ false }, Bottom_on{ false }, Shift_on{ false }, hitbox_on{ false };

	vector<BoneInfo>* player_BoneInfo;
	AnimatedModel* animModel;
	AnimInfo* player_CurrentAnim;
	AnimatedModel::AnimationLibrary* animLibrary = { nullptr };

	std::vector<std::unique_ptr<Assimp::Importer>> animationImporters;
};