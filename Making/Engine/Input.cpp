#include "pch.h"
#include "Input.h"
#include "Camera.h"

void Input::KeyBoardInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));

	//bool wasMoving = mainCat->isMoving();
	//bool wasRunning = mainCat->shift_value();

	switch (key) {
		//case GLFW_KEY_P:
		//	if (action == GLFW_PRESS)
		//	{
		//		start = true;
		//	}
		//	break;
		case GLFW_KEY_Q:
			//if ((camera.get_start_pos() != 0 && !mainCat->getdying()) || finish)
			//{
			//	if (action == GLFW_PRESS)
			//		glfwSetWindowShouldClose(window, GL_TRUE);
			//}
			if (action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_RIGHT:
			if (action == GLFW_PRESS)
			{
				input->camera->RotRight(true);
			}
			else if (action == GLFW_RELEASE)
			{
				input->camera->RotRight(false);
			}
			break;
		case GLFW_KEY_LEFT:
			if (action == GLFW_PRESS)
			{
				input->camera->RotLeft(true);
			}
			else if (action == GLFW_RELEASE)
			{
				input->camera->RotLeft(false);
			}
			break;
		case GLFW_KEY_UP:
			if (action == GLFW_PRESS)
			{
				input->camera->RotUp(true);
			}
			else if (action == GLFW_RELEASE)
			{
				input->camera->RotUp(false);
			}
			break;
		case GLFW_KEY_DOWN:
			if (action == GLFW_PRESS)
			{
				input->camera->RotDown(true);
			}
			else if (action == GLFW_RELEASE)
			{
				input->camera->RotDown(false);
			}
			break;
		//case GLFW_KEY_LEFT_SHIFT:
		//	if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		//	{
		//		if (action == GLFW_PRESS)
		//		{
		//			mainCat->shift_on(true);
		//		}
		//		else if (action == GLFW_RELEASE)
		//		{
		//			mainCat->shift_on(false);
		//		}
		//	}
		//	break;
		//case GLFW_KEY_C:
		//	if (action == GLFW_PRESS)
		//	{
		//		mainCat->setPosition();
		//	}
		//	break;
		//case GLFW_KEY_D:
		//	if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		//	{
		//		if (action == GLFW_PRESS)
		//		{
		//			mainCat->setRight_on(true);
		//		}
		//		else if (action == GLFW_RELEASE)
		//		{
		//			mainCat->setRight_on(false);
		//		}
		//	}
		//	break;
		//case GLFW_KEY_A:
		//	if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		//	{
		//		if (action == GLFW_PRESS)
		//		{
		//			mainCat->setLeft_on(true);
		//		}
		//		else if (action == GLFW_RELEASE)
		//		{
		//			mainCat->setLeft_on(false);
		//		}
		//	}
		//	break;
		//case GLFW_KEY_W:
		//	if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		//	{
		//		if (action == GLFW_PRESS)
		//		{
		//			mainCat->setTop_on(true);
		//		}
		//		else if (action == GLFW_RELEASE)
		//		{
		//			mainCat->setTop_on(false);
		//		}
		//	}
		//	break;
		//case GLFW_KEY_S:
		//	if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		//	{
		//		if (action == GLFW_PRESS)
		//		{
		//			mainCat->setBottom_on(true);
		//		}
		//		else if (action == GLFW_RELEASE)
		//		{
		//			mainCat->setBottom_on(false);
		//		}
		//	}
		//	break;
		//case GLFW_KEY_H:
		//	if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		//	{
		//		if (action == GLFW_PRESS)
		//		{
		//			if (mainCat->hitbox_ison())
		//				mainCat->hitboxOnOff(false);
		//			else
		//				mainCat->hitboxOnOff(true);
		//		}
		//	}
		//	break;
		//case GLFW_KEY_V:
		//	if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		//	{
		//		if (action == GLFW_PRESS)
		//		{
		//			FirstPersonView = !FirstPersonView;
		//
		//			if (FirstPersonView) {
		//				camera.setInitialDirection(mouseDir);
		//			}
		//		}
		//	}
		//	break;
		//case GLFW_KEY_0:
		//	if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		//	{
		//		if (action == GLFW_PRESS)
		//		{
		//			cout << "x: " << mainCat->getPosition().x << endl;
		//			cout << "z: " << mainCat->getPosition().z << endl;
		//		}
		//	}
		//	break;
		//case GLFW_KEY_1:
		//	if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		//	{
		//		if (action == GLFW_PRESS)
		//		{
		//			playeranimLib.changeAnimation("Dance", player_CurrentAnim);
		//		}
		//	}
		//	break;
		//case GLFW_KEY_LEFT_ALT:
		//	if (camera.get_start_pos() == 0 && !mainCat->getdying() && !finish)
		//	{
		//		if (action == GLFW_PRESS)
		//			camera.handleAltKey(true);
		//		else if (action == GLFW_RELEASE)
		//			camera.handleAltKey(false);
		//	}
		//	break;
		//case GLFW_KEY_EQUAL:
		//	if (action == GLFW_PRESS)
		//	{
		//		if (mods == GLFW_MOD_SHIFT)
		//		{
		//			if (soundVol < 1.0f)
		//			{
		//				soundVol += 0.05f;
		//				std::cout << "Sound: " << soundVol << std::endl;
		//			}
		//		}
		//	}
		//	break;
		//case GLFW_KEY_MINUS:
		//	if (action == GLFW_PRESS)
		//	{
		//		if (soundVol > 0.0f)
		//		{
		//			soundVol -= 0.05f;
		//			std::cout << "Sound: " << soundVol << std::endl;
		//		}
		//	}
		//	break;
	}

	//bool isMovingNow = mainCat->isMoving();
	//bool isRunningNow = mainCat->shift_value();
	//
	//if (wasMoving != isMovingNow || (isMovingNow && wasRunning != isRunningNow)) {
	//	if (!firing_induration)
	//	{
	//		if (isMovingNow) {
	//			if (isRunningNow)
	//			{
	//				playeranimLib.changeAnimation("Run", player_CurrentAnim);
	//			}
	//			else
	//			{
	//				playeranimLib.changeAnimation("Walk", player_CurrentAnim);
	//			}
	//		}
	//		else
	//		{
	//			playeranimLib.changeAnimation("Idle", player_CurrentAnim);
	//		}
	//	}
	//}
}
