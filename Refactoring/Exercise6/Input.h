#pragma once
#include "Setting.h"
#include "Bullet.h"

Camera camera;
extern AnimInfo player_CurrentAnim, enemy_CurrentAnim[3][9];

extern vector<Bullet*> bullets;
extern Enemy* enemy[3][9];
extern ISound* bgm;

glm::vec3 mouseDir;
glm::vec3 mousePick;
double cur_x, cur_y;
bool FirstPersonView{ false };
bool firing{ false };
bool firing_induration{ false };
bool start{ false };
float soundVol{ 0.05f };
bool bullet_num[3]{ false };
float cloud_pos_z{ 0.0f };
bool cloud_go{ false };
float light_angle{ 0.0f };
bool finish{ false };
bool set_fin{ false };
float end_y{ -30.0f };
bool send_end{ false };

void keyFunc(GLFWwindow* window, int key, int scancode, int action, int mods) {
	bool wasMoving = mainCat->isMoving();
	bool wasRunning = mainCat->shift_value();

	switch (key) {
	case GLFW_KEY_P:
		if (action == GLFW_PRESS)
		{
			start = true;
		}
		break;
	case GLFW_KEY_Q:
		if ((camera.get_start_pos() != 0 && !mainCat->getdying()) || finish)
		{
			if (action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GL_TRUE);
		}
		break;
	case GLFW_KEY_LEFT_SHIFT: 
	if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		{
		if (action == GLFW_PRESS)
		{
			mainCat->shift_on(true);
		}
		else if (action == GLFW_RELEASE)
		{
			mainCat->shift_on(false);
		}
		}
		break;
	case GLFW_KEY_C:
		if (action == GLFW_PRESS)
		{
			mainCat->setPosition();
		}
		break;
	case GLFW_KEY_D:
		if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		{
		if (action == GLFW_PRESS)
		{
			mainCat->setRight_on(true);
		}
		else if (action == GLFW_RELEASE)
		{
			mainCat->setRight_on(false);
		}
		}
		break;
	case GLFW_KEY_A:
		if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		{
		if (action == GLFW_PRESS)
		{
			mainCat->setLeft_on(true);
		}
		else if (action == GLFW_RELEASE)
		{
			mainCat->setLeft_on(false);
		}
		}
		break;
	case GLFW_KEY_W:
		if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		{
		if (action == GLFW_PRESS)
		{
			mainCat->setTop_on(true);
		}
		else if (action == GLFW_RELEASE)
		{
			mainCat->setTop_on(false);
		}
		}
		break;
	case GLFW_KEY_S:
		if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		{
		if (action == GLFW_PRESS)
		{
			mainCat->setBottom_on(true);
		}
		else if (action == GLFW_RELEASE)
		{
			mainCat->setBottom_on(false);
		}
		}
		break;
	case GLFW_KEY_H:
		if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		{
			if (action == GLFW_PRESS)
			{
				if (mainCat->hitbox_ison())
					mainCat->hitboxOnOff(false);
				else
					mainCat->hitboxOnOff(true);
			}
		}
		break;
	case GLFW_KEY_V:
		if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		{
		if (action == GLFW_PRESS)
		{
			FirstPersonView = !FirstPersonView;

			if (FirstPersonView) {
				camera.setInitialDirection(mouseDir);
			}
		}
		}
		break;
	case GLFW_KEY_0:
		if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		{
		if (action == GLFW_PRESS)
		{
			cout << "x: " << mainCat->getPosition().x << endl;
			cout << "z: " << mainCat->getPosition().z << endl;
		}
		}
		break;
	case GLFW_KEY_1:
		if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		{
		if (action == GLFW_PRESS)
		{
			playeranimLib.changeAnimation("Dance", player_CurrentAnim);
		}
		}
		break;
	case GLFW_KEY_LEFT_ALT:
		if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		{
		if (action == GLFW_PRESS)
			camera.handleAltKey(true);
		else if (action == GLFW_RELEASE)
			camera.handleAltKey(false);
		}
		break;
	case GLFW_KEY_EQUAL:
		if (action == GLFW_PRESS)
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				if (soundVol < 1.0f)
				{
					soundVol += 0.05f;
					std::cout << "Sound: " << soundVol << std::endl;
				}
			}
		}
		break;
	case GLFW_KEY_MINUS:
		if (action == GLFW_PRESS)
		{
			if (soundVol > 0.0f)
			{
				soundVol -= 0.05f;
				std::cout << "Sound: " << soundVol << std::endl;
			}
		}
		break;
	}

	bool isMovingNow = mainCat->isMoving();
	bool isRunningNow = mainCat->shift_value();

	if (wasMoving != isMovingNow || (isMovingNow && wasRunning != isRunningNow)) {		
		if(!firing_induration)
		{
			if (isMovingNow) {
				if (isRunningNow)
				{
					playeranimLib.changeAnimation("Run", player_CurrentAnim);
				}
				else
				{
					playeranimLib.changeAnimation("Walk", player_CurrentAnim);
				}
			}
			else
			{
				playeranimLib.changeAnimation("Idle", player_CurrentAnim);
			}
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.handleScroll(yoffset);
}

bool getFirstPersonView()
{
	return FirstPersonView;
}

float getCamerahori()
{
	return camera.get_horizontal_angle();
}

void mouseFunc(GLFWwindow* window, int button, int action, int mods)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		{
		if (action == GLFW_PRESS)
		{
			firing = true;
		}
		else if (action == GLFW_RELEASE)
		{
			firing = false;
		}
		}
		break;
	}

	if (firing && playeranimLib.getcurrentAnimation() == "Run")
		playeranimLib.changeAnimation("FireRun", player_CurrentAnim);
	else if (firing && playeranimLib.getcurrentAnimation() == "Walk")
		playeranimLib.changeAnimation("FireWalk", player_CurrentAnim);
	else if (firing && playeranimLib.getcurrentAnimation() == "Idle")
		playeranimLib.changeAnimation("Fire", player_CurrentAnim);
}

void changeMainCharacterAnimation(const glm::mat4& view, const glm::mat4& projection)
{
	float firetimer;

	if (!mainCat->getdying())
	{
		if (playeranimLib.getcurrentAnimation() == "FireRun")
		{
			firing_induration = true;
			firetimer = player_CurrentAnim.Duration * 0.56f;

			if (player_CurrentAnim.CurrentTime >= firetimer && !bullet_num[0])
			{
				mousePick = camera.getMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->bulletSetting();
				bullets.push_back(newBullet);
				bullet_num[0] = true;
			}

			if (player_CurrentAnim.CurrentTime >= firetimer + 150.0f && !bullet_num[1])
			{
				mousePick = camera.getMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->bulletSetting();
				bullets.push_back(newBullet);
				bullet_num[1] = true;
			}

			if (player_CurrentAnim.CurrentTime >= firetimer + 300.0f && !bullet_num[2])
			{
				mousePick = camera.getMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->bulletSetting();
				bullets.push_back(newBullet);
				bullet_num[2] = true;
			}

			if (player_CurrentAnim.CurrentTime + 10.0f >= player_CurrentAnim.Duration)
			{
				if (!firing)
				{
					if (mainCat->isMoving())
					{
						if (mainCat->shift_value())
							playeranimLib.changeAnimation("Run", player_CurrentAnim);
						else
							playeranimLib.changeAnimation("Walk", player_CurrentAnim);
					}
					else
						playeranimLib.changeAnimation("Idle", player_CurrentAnim);
					firing_induration = false;
				}
				else
				{
					if (mainCat->isMoving())
					{
						if (!mainCat->shift_value())
							playeranimLib.changeAnimation("FireWalk", player_CurrentAnim);
						else
							playeranimLib.changeAnimation("FireRun", player_CurrentAnim);
					}
					else
						playeranimLib.changeAnimation("Fire", player_CurrentAnim);
				}

				for (bool& bullet_nums : bullet_num)
					bullet_nums = false;
			}
		}
		else if (playeranimLib.getcurrentAnimation() == "FireWalk")
		{
			firing_induration = true;
			firetimer = player_CurrentAnim.Duration * 0.56f;

			if (player_CurrentAnim.CurrentTime >= firetimer && !bullet_num[0])
			{
				mousePick = camera.getMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->bulletSetting();
				bullets.push_back(newBullet);
				bullet_num[0] = true;
			}

			if (player_CurrentAnim.CurrentTime >= firetimer + 150.0f && !bullet_num[1])
			{
				mousePick = camera.getMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->bulletSetting();
				bullets.push_back(newBullet);
				bullet_num[1] = true;
			}

			if (player_CurrentAnim.CurrentTime >= firetimer + 300.0f && !bullet_num[2])
			{
				mousePick = camera.getMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->bulletSetting();
				bullets.push_back(newBullet);
				bullet_num[2] = true;
			}

			if (player_CurrentAnim.CurrentTime + 10.0f >= player_CurrentAnim.Duration)
			{
				if (!firing)
				{
					if (mainCat->isMoving())
					{
						if (mainCat->shift_value())
							playeranimLib.changeAnimation("Run", player_CurrentAnim);
						else
							playeranimLib.changeAnimation("Walk", player_CurrentAnim);
					}
					else
						playeranimLib.changeAnimation("Idle", player_CurrentAnim);
					firing_induration = false;
				}
				else
				{
					if (mainCat->isMoving())
					{
						if (mainCat->shift_value())
							playeranimLib.changeAnimation("FireRun", player_CurrentAnim);
						else
							playeranimLib.changeAnimation("FireWalk", player_CurrentAnim);
					}
					else
						playeranimLib.changeAnimation("Fire", player_CurrentAnim);
				}

				for (bool& bullet_nums : bullet_num)
					bullet_nums = false;
			}
		}
		else if (playeranimLib.getcurrentAnimation() == "Fire")
		{
			firing_induration = true;
			firetimer = player_CurrentAnim.Duration * 0.56f;

			if (player_CurrentAnim.CurrentTime >= firetimer && !bullet_num[0])
			{
				mousePick = camera.getMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->bulletSetting();
				bullets.push_back(newBullet);
				bullet_num[0] = true;
			}

			if (player_CurrentAnim.CurrentTime >= firetimer + 150.0f && !bullet_num[1])
			{
				mousePick = camera.getMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->bulletSetting();
				bullets.push_back(newBullet);
				bullet_num[1] = true;
			}

			if (player_CurrentAnim.CurrentTime >= firetimer + 300.0f && !bullet_num[2])
			{
				mousePick = camera.getMousePicking(cur_x, cur_y, projection, view);
				Bullet* newBullet = new Bullet(1, 0, 0);
				newBullet->bulletSetting();
				bullets.push_back(newBullet);
				bullet_num[2] = true;
			}

			if (player_CurrentAnim.CurrentTime + 10.0f >= player_CurrentAnim.Duration)
			{
				if (!firing)
				{
					if (mainCat->isMoving())
					{
						if (mainCat->shift_value())
							playeranimLib.changeAnimation("Run", player_CurrentAnim);
						else
							playeranimLib.changeAnimation("Walk", player_CurrentAnim);
					}
					else
						playeranimLib.changeAnimation("Idle", player_CurrentAnim);
					firing_induration = false;
				}
				else
				{
					if (mainCat->isMoving())
					{
						if (mainCat->shift_value())
							playeranimLib.changeAnimation("FireRun", player_CurrentAnim);
						else
							playeranimLib.changeAnimation("FireWalk", player_CurrentAnim);
					}
					else
						playeranimLib.changeAnimation("Fire", player_CurrentAnim);
				}

				for (bool& bullet_nums : bullet_num)
					bullet_nums = false;
			}
		}
		else if (playeranimLib.getcurrentAnimation() == "Die")
			playeranimLib.changeAnimation("Idle", player_CurrentAnim);
	}
	else
	{
		if (playeranimLib.getcurrentAnimation() != "Die")
			playeranimLib.changeAnimation("Die", player_CurrentAnim);
		else
		{
			if (player_CurrentAnim.CurrentTime + 10 >= player_CurrentAnim.Duration)
				mainCat->setdead(true);
		}
	}
}

void changeEnemiesAnimation()
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if (enemy[i][j]->getstate() == 0)
			{
				if (enemyanimLib[i][j].getcurrentAnimation() != "Idle")
					enemyanimLib[i][j].changeAnimation("Idle", enemy_CurrentAnim[i][j]);
			}
			else if (enemy[i][j]->getstate() == 1)
			{
				if (enemyanimLib[i][j].getcurrentAnimation() != "Run")
					enemyanimLib[i][j].changeAnimation("Run", enemy_CurrentAnim[i][j]);
			}
			else if (enemy[i][j]->getstate() == 2)
			{
				if (enemyanimLib[i][j].getcurrentAnimation() != "Attack")
					enemyanimLib[i][j].changeAnimation("Attack", enemy_CurrentAnim[i][j]);
			}
			else if (enemy[i][j]->getstate() == 3)
			{
				if (enemyanimLib[i][j].getcurrentAnimation() == "Idle")				
					enemyanimLib[i][j].changeAnimation("Hit", enemy_CurrentAnim[i][j]);
			}
			else if (enemy[i][j]->getstate() == 4)
			{
				if (enemyanimLib[i][j].getcurrentAnimation() != "Death")
					enemyanimLib[i][j].changeAnimation("Death", enemy_CurrentAnim[i][j]);
			}
			else if (enemy[i][j]->getstate() == 5)
			{
				if (enemyanimLib[i][j].getcurrentAnimation() != "Dance")
					enemyanimLib[i][j].changeAnimation("Dance", enemy_CurrentAnim[i][j]);
			}
		}
	}
}