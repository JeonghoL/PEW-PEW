#pragma once
#include "AnimatedModel.h"

class ShadowMapping;

class MainCharacter
{
public:
	MainCharacter();
	~MainCharacter();

	void Update();
	void Draw(glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos, float deltaTime, float angle);

	void Drawshadow(float angle, GLuint depthShaderProgram, const glm::mat4& lightSpaceMatrix);

	void SetRight_on(bool in);
	void SetLeft_on(bool in);
	void SetTop_on(bool in);
	void SetBottom_on(bool in);
	void Shift_on(bool in);
	//void hitboxOnOff(bool in);

	void Walk();
	void Run();
	//void jump(bool in);
	//void stop();

	bool Shift_value() const { return _Shift; }
	//bool hitbox_ison() const { return hitbox_on; }

	//void changehitColor();

	//void setlife();

	bool IsMoving() const {
		return _Right || _Left || _Top || _Bottom;
	}

	const glm::vec3& GetPosition() const { return characterPos; }
	const glm::mat4& GetModel() const { return model; }
	const bool& GetDying() const { return dying; }

	void SetPosition() { characterPos = glm::vec3{ -44.0f, 0.0f, -48.0f }; }
	void SetDead(bool in) { dead = in; }
	void SetFinishPos() { characterPos = glm::vec3{ -5.0f, 0.0f, 5.0f }; }

	void SaveAnimations();
	void SetAnimationType(const std::string& animName);

private:
	glm::vec3 characterPos = { -37.3051f, 0.0f, 42.5001f };
	GLuint VAO, VBO, VBO2, EBO, shaderprogram, Texture;
	GLuint ViewLoc, ProjLoc, ModelLoc, TextureLoc, UseTextureLoc, colorHitLoc;
	std::vector<unsigned int> Indices;
	glm::mat4 model = { 1.0f };
	glm::vec4 hitcolor = { 1.0f, 1.0f, 1.0f, 1.0f };
	int hit_cnt = { 200 };
	bool dead = { false };
	bool dying = { false };
	float lastangle;
	int life = { 6 };
	float revive_timer = { 1200.0f };

	bool _Right = { false }; 
	bool _Left = { false };
	bool _Top = { false };
	bool _Bottom = { false };
	bool _Shift = { false };
	bool hitbox_on = { false };

	vector<BoneInfo>* player_BoneInfo;
	AnimatedModel* animModel;
	AnimInfo* player_CurrentAnim;
	AnimatedModel::AnimationLibrary* animLibrary = { nullptr };

	std::vector<std::unique_ptr<Assimp::Importer>> animationImporters;
};