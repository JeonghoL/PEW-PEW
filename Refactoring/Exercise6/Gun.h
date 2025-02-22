#pragma once
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/scene.h>
#include "ShadowMapping.h"

class MainCharacter;

class Gun
{
public:
	Gun();

	void draw(float deltaTime, Gun* gun, float angle, glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos, glm::mat4 lightSpaceMatrix, GLuint depthMap);

	void drawshadow(Gun* gun, float angle, ShadowMapping* shadowMap);

	void setRight_on(bool in);
	void setLeft_on(bool in);
	void setTop_on(bool in);
	void setBottom_on(bool in);
	void shift_on(bool in);
	void aim_on(bool in);

	void walk();
	void run();
	void die();

	bool shift_value() const { return Shift_on; }
	bool Aim_ison() const { return Aim_pose; }

	bool isMoving() const {
		return Right_on || Left_on || Top_on || Bottom_on;
	}

	const glm::vec3& getPosition() const { return gunPos; }
	const glm::mat4& getmodel() const { return model; }

private:
	glm::vec3 gunPos{ -37.3051f, 0.0f, 42.5001f };
	GLuint VAO, VBO, VBO2, EBO, shaderprogram, Texture;
	GLuint ViewLoc, ProjLoc, ModelLoc, TextureLoc, UseTextureLoc, colorHitLoc;
	std::vector<unsigned int> Indices;
	glm::mat4 model{ 1.0f };

	bool Right_on{ false }, Left_on{ false }, Top_on{ false }, Bottom_on{ false }, Shift_on{ false }, Aim_pose{ false };
};