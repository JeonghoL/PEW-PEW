#pragma once
#include "LoadGLB.h"
#include "Animation.h"

#define STB_IMAGE_IMPLEMENTATION

#define PI 3.141592
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

extern MainCharacter* mainCat;

const char* objectvs = "objectvs.glsl";
const char* objectfs = "objectfs.glsl";

GLuint vert = 0;
GLuint frag = 0;
GLuint prog = 0;

float min_X[70], max_X[70], min_Z[70], max_Z[70];
extern Enemy* enemy[3][9];

void setAnimation()
{
	playeranimLib.loadAnimation("Die", "animation/cat_animation_die.glb",playerAnimationImporters);
	playeranimLib.loadAnimation("Idle", "animation/cat_animation_idle.glb", playerAnimationImporters);
	playeranimLib.loadAnimation("Walk", "animation/cat_animation_walking.glb", playerAnimationImporters);
	playeranimLib.loadAnimation("Run", "animation/cat_animation_run.glb", playerAnimationImporters);
	playeranimLib.loadAnimation("Hit", "animation/cat_animation_hit.glb", playerAnimationImporters);
	playeranimLib.loadAnimation("Jump", "animation/cat_animation_jump.glb", playerAnimationImporters);
	playeranimLib.loadAnimation("AimIdle", "animation/cat_animation_idle_aim.glb", playerAnimationImporters);
	playeranimLib.loadAnimation("AimWalk", "animation/cat_animation_walking_aim.glb", playerAnimationImporters);
	playeranimLib.loadAnimation("Fire", "animation/cat_animation_firing_aim.glb", playerAnimationImporters);
	playeranimLib.loadAnimation("FireWalk", "animation/cat_animation_firing_walk.glb", playerAnimationImporters);
	playeranimLib.loadAnimation("FireRun", "animation/cat_animation_firing_run.glb", playerAnimationImporters);
	playeranimLib.loadAnimation("Dance", "animation/cat_animation_dance.glb", playerAnimationImporters);

	for (int j = 0; j < 9; ++j) {
		enemyanimLib[0][j].loadAnimation("Idle", "animation/alien_animation_idle.glb", enemy1AnimationImporters);
		enemyanimLib[0][j].loadAnimation("Run", "animation/alien_animation_run.glb", enemy1AnimationImporters);
		enemyanimLib[0][j].loadAnimation("Attack", "animation/alien_animation_attack.glb", enemy1AnimationImporters);
		enemyanimLib[0][j].loadAnimation("Hit", "animation/alien_animation_hit.glb", enemy1AnimationImporters);
		enemyanimLib[0][j].loadAnimation("Death", "animation/alien_animation_death.glb", enemy1AnimationImporters);
		enemyanimLib[0][j].loadAnimation("Dance", "animation/cat_animation_dance.glb", enemy1AnimationImporters);

		enemyanimLib[1][j].loadAnimation("Idle", "animation/alien_animation_idle.glb", enemy2AnimationImporters);
		enemyanimLib[1][j].loadAnimation("Run", "animation/alien_animation_run.glb", enemy2AnimationImporters);
		enemyanimLib[1][j].loadAnimation("Attack", "animation/alien_animation_attack.glb", enemy2AnimationImporters);
		enemyanimLib[1][j].loadAnimation("Hit", "animation/alien_animation_hit.glb", enemy2AnimationImporters);
		enemyanimLib[1][j].loadAnimation("Death", "animation/alien_animation_death.glb", enemy2AnimationImporters);
		enemyanimLib[1][j].loadAnimation("Dance", "animation/cat_animation_dance.glb", enemy2AnimationImporters);

		enemyanimLib[2][j].loadAnimation("Idle", "animation/alien_animation_idle.glb", enemy3AnimationImporters);
		enemyanimLib[2][j].loadAnimation("Run", "animation/alien_animation_run.glb", enemy3AnimationImporters);
		enemyanimLib[2][j].loadAnimation("Attack", "animation/alien_animation_attack.glb", enemy3AnimationImporters);
		enemyanimLib[2][j].loadAnimation("Hit", "animation/alien_animation_hit.glb", enemy3AnimationImporters);
		enemyanimLib[2][j].loadAnimation("Death", "animation/alien_animation_death.glb", enemy3AnimationImporters);
		enemyanimLib[2][j].loadAnimation("Dance", "animation/cat_animation_dance.glb", enemy3AnimationImporters);
	}
}

void setCollision()
{
	for (int i = 1; i <= 70; i++) {
		string filename = "collision/collision_0" + to_string(i) + ".glb";
		getModelBounds(filename, min_X[i - 1], max_X[i - 1], min_Z[i - 1], max_Z[i - 1]);
	}
}

bool wallcollapsed_w()
{
	glm::vec3 pos = mainCat->getPosition();
	bool check = false;
	glm::vec3 other_pos;
	
	for (int i = 0; i < 70; ++i)
	{
		if (min_X[i] <= pos.x && max_X[i] >= pos.x)
		{
			if (pos.z - 0.25f <= max_Z[i] && pos.z - 0.25f > min_Z[i])
				check = true;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{			
			other_pos = enemy[i][j]->getPosition();
			if (enemy[i][j]->getlife() > 0)
			{
				if (abs(pos.x - other_pos.x) < 0.5f)
				{
					if (other_pos.z - pos.z > -0.4f && other_pos.z - pos.z < 0.0f)
						check = true;
				}
			}
		}
	}

	return check;
}

bool wallcollapsed_s()
{
	glm::vec3 pos = mainCat->getPosition();
	bool check = false;
	glm::vec3 other_pos;

	for (int i = 0; i < 70; ++i)
	{
		if (min_X[i] <= pos.x && max_X[i] >= pos.x)
		{
			if (pos.z + 0.25f <= max_Z[i] && pos.z + 0.25f > min_Z[i])
				check = true;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{			
			other_pos = enemy[i][j]->getPosition();
			if (enemy[i][j]->getlife() > 0)
			{
				if (abs(pos.x - other_pos.x) < 0.5f)
				{
					if (other_pos.z - pos.z < 0.4f && other_pos.z - pos.z > 0.0f)
						check = true;
				}
			}
		}
	}

	return check;
}

bool wallcollapsed_a()
{
	glm::vec3 pos = mainCat->getPosition();
	bool check = false;
	glm::vec3 other_pos;

	for (int i = 0; i < 70; ++i)
	{
		if (min_Z[i] <= pos.z && max_Z[i] >= pos.z)
		{
			if (pos.x - 0.25f <= max_X[i] && pos.x - 0.25f > min_X[i])
				check = true;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			other_pos = enemy[i][j]->getPosition();
			if (enemy[i][j]->getlife() > 0)
			{
				if (abs(pos.z - other_pos.z) < 0.4f)
				{
					if (other_pos.x - pos.x > -0.5f && other_pos.x - pos.x < 0.0f)
						check = true;
				}
			}
		}
	}

	return check;
}

bool wallcollapsed_d()
{
	glm::vec3 pos = mainCat->getPosition();
	bool check = false;
	glm::vec3 other_pos;

	for (int i = 0; i < 70; ++i)
	{
		if (min_Z[i] <= pos.z && max_Z[i] >= pos.z)
		{
			if (pos.x + 0.25f <= max_X[i] && pos.x + 0.25f > min_X[i])
				check = true;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{			
			other_pos = enemy[i][j]->getPosition();
			if (enemy[i][j]->getlife() > 0)
			{
				if (abs(pos.z - other_pos.z) < 0.4f)
				{
					if (other_pos.x - pos.x < 0.5f && other_pos.x - pos.x > 0.0f)
						check = true;
				}
			}
		}
	}

	return check;
}

bool wallcollapsed_forward(const glm::vec3& forward) {
	glm::vec3 pos = mainCat->getPosition();
	glm::vec3 nextPos = pos + forward * 0.25f;
	bool check = false;
	glm::vec3 other_pos;

	for (int i = 0; i < 70; ++i) {
		if (min_X[i] <= nextPos.x && max_X[i] >= nextPos.x) {
			if (nextPos.z <= max_Z[i] && nextPos.z > min_Z[i])
				check = true;
		}
	}

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 9; ++j) {
			other_pos = enemy[i][j]->getPosition();
			if (enemy[i][j]->getlife() > 0) {
				if (abs(nextPos.x - other_pos.x) < 0.5f) {
					if (other_pos.z - nextPos.z > -0.4f && other_pos.z - nextPos.z < 0.4f)
						check = true;
				}
			}
		}
	}
	return check;
}

bool wallcollapsed_backward(const glm::vec3& forward) {
	glm::vec3 pos = mainCat->getPosition();
	glm::vec3 nextPos = pos - forward * 0.25f;
	bool check = false;
	glm::vec3 other_pos;

	for (int i = 0; i < 70; ++i) {
		if (min_X[i] <= nextPos.x && max_X[i] >= nextPos.x) {
			if (nextPos.z <= max_Z[i] && nextPos.z > min_Z[i])
				check = true;
		}
	}

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 9; ++j) {
			other_pos = enemy[i][j]->getPosition();
			if (enemy[i][j]->getlife() > 0) {
				if (abs(nextPos.x - other_pos.x) < 0.5f) {
					if (other_pos.z - nextPos.z > -0.4f && other_pos.z - nextPos.z < 0.4f)
						check = true;
				}
			}
		}
	}
	return check;
}

bool wallcollapsed_right(const glm::vec3& right) {
	glm::vec3 pos = mainCat->getPosition();
	glm::vec3 nextPos = pos + right * 0.25f;
	bool check = false;
	glm::vec3 other_pos;

	for (int i = 0; i < 70; ++i) {
		if (min_Z[i] <= nextPos.z && max_Z[i] >= nextPos.z) {
			if (nextPos.x <= max_X[i] && nextPos.x > min_X[i])
				check = true;
		}
	}

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 9; ++j) {
			other_pos = enemy[i][j]->getPosition();
			if (enemy[i][j]->getlife() > 0) {
				if (abs(nextPos.z - other_pos.z) < 0.4f) {
					if (other_pos.x - nextPos.x > -0.5f && other_pos.x - nextPos.x < 0.5f)
						check = true;
				}
			}
		}
	}
	return check;
}

bool wallcollapsed_left(const glm::vec3& right) {
	glm::vec3 pos = mainCat->getPosition();
	glm::vec3 nextPos = pos - right * 0.25f;
	bool check = false;
	glm::vec3 other_pos;

	for (int i = 0; i < 70; ++i) {
		if (min_Z[i] <= nextPos.z && max_Z[i] >= nextPos.z) {
			if (nextPos.x <= max_X[i] && nextPos.x > min_X[i])
				check = true;
		}
	}

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 9; ++j) {
			other_pos = enemy[i][j]->getPosition();
			if (enemy[i][j]->getlife() > 0) {
				if (abs(nextPos.z - other_pos.z) < 0.4f) {
					if (other_pos.x - nextPos.x > -0.5f && other_pos.x - nextPos.x < 0.5f)
						check = true;
				}
			}
		}
	}
	return check;
}