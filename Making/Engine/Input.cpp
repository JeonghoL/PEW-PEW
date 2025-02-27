#include "pch.h"
#include "Input.h"
#include "Camera.h"
#include "MainCharacter.h"

void Input::KeyBoardInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));

	//bool wasMoving = mainCat->isMoving();
	//bool wasRunning = mainCat->shift_value();

	switch (key) {
		case GLFW_KEY_P:
			if (action == GLFW_PRESS)
				input->camera->SetStart(true);
			break;
		case GLFW_KEY_Q:
			if ((input->camera->Get_start_pos() == 0 && !input->mainCat->GetDying())/* || finish*/)
			{
				if (action == GLFW_PRESS)
					glfwSetWindowShouldClose(window, GL_TRUE);
			}
			break;
		case GLFW_KEY_LEFT_SHIFT:
			if ((input->camera->Get_start_pos() == 0 && !input->mainCat->GetDying())/* || finish*/)
			{
				if (action == GLFW_PRESS)
				{
					input->mainCat->Shift_on(true);
				}
				else if (action == GLFW_RELEASE)
				{
					input->mainCat->Shift_on(false);
				}
			}
			break;
		//case GLFW_KEY_C:
		//	if (action == GLFW_PRESS)
		//	{
		//		mainCat->setPosition();
		//	}
		//	break;
		case GLFW_KEY_D:
			if ((input->camera->Get_start_pos() == 0 && !input->mainCat->GetDying())/* || finish*/)
			{
				if (action == GLFW_PRESS)
				{
					input->mainCat->SetRight_on(true);
				}
				else if (action == GLFW_RELEASE)
				{
					input->mainCat->SetRight_on(false);
				}
			}
			break;
		case GLFW_KEY_A:
			if ((input->camera->Get_start_pos() == 0 && !input->mainCat->GetDying())/* || finish*/)
			{
				if (action == GLFW_PRESS)
				{
					input->mainCat->SetLeft_on(true);
				}
				else if (action == GLFW_RELEASE)
				{
					input->mainCat->SetLeft_on(false);
				}
			}
			break;
		case GLFW_KEY_W:
			if ((input->camera->Get_start_pos() == 0 && !input->mainCat->GetDying())/* || finish*/)
			{
				if (action == GLFW_PRESS)
				{
					input->mainCat->SetTop_on(true);
				}
				else if (action == GLFW_RELEASE)
				{
					input->mainCat->SetTop_on(false);
				}
			}
			break;
		case GLFW_KEY_S:
			if ((input->camera->Get_start_pos() == 0 && !input->mainCat->GetDying())/* || finish*/)
			{
				if (action == GLFW_PRESS)
				{
					input->mainCat->SetBottom_on(true);
				}
				else if (action == GLFW_RELEASE)
				{
					input->mainCat->SetBottom_on(false);
				}
			}
			break;
		//case GLFW_KEY_H:
		//	if ((input->camera->Get_start_pos() == 0 && !input->mainCat->GetDying())/* || finish*/)
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
		case GLFW_KEY_V:
			if ((input->camera->Get_start_pos() == 0 && !input->mainCat->GetDying())/* || finish*/)
			{
				if (action == GLFW_PRESS)
				{
					input->camera->ChangeViewType();
		
					if (input->camera->GetViewType()) {
						input->camera->SetInitialDirection(mouseDir);
					}
				}
			}
			break;
		//case GLFW_KEY_0:
		//	if (camera.Get_start_pos() == 0 && !mainCat->getdying() && !finish)
		//	{
		//		if (action == GLFW_PRESS)
		//		{
		//			cout << "x: " << mainCat->getPosition().x << endl;
		//			cout << "z: " << mainCat->getPosition().z << endl;
		//		}
		//	}
		//	break;
		//case GLFW_KEY_1:
		//	if (camera.Get_start_pos() == 0 && !mainCat->getdying() && !finish)
		//	{
		//		if (action == GLFW_PRESS)
		//		{
		//			playeranimLib.changeAnimation("Dance", player_CurrentAnim);
		//		}
		//	}
		//	break;
		case GLFW_KEY_LEFT_ALT:
			if ((input->camera->Get_start_pos() == 0 && !input->mainCat->GetDying())/* || finish*/)
			{
				if (action == GLFW_PRESS)
					input->camera->HandleAltKey(true);
				else if (action == GLFW_RELEASE)
					input->camera->HandleAltKey(false);
			}
			break;
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

void Input::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));

	if (input->camera->Get_start_pos() == 0)
		input->camera->HandleScroll(yoffset);
}

void Input::MouseFunc(GLFWwindow* window, int button, int action, int mods)
{
	Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));

	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		if (input->camera->Get_start_pos() == 0 && !input->mainCat->GetDying() /*&& !finish*/)
		{
			if (action == GLFW_PRESS)
			{
				input->mainCat->SetFiring(true);
			}
			else if (action == GLFW_RELEASE)
			{
				input->mainCat->SetFiring(false);
			}
		}
		break;
	}

	// �����ʿ�
	/*if (input->mainCat->GetFiring() && input->mainCat->GetAnimLibrary()->getcurrentAnimation() == "Run")
		input->mainCat->GetCurrentAnim().changeAnimation("FireRun", player_CurrentAnim);
	else if (input->mainCat->GetFiring() && input->mainCat->GetAnimLibrary()->getcurrentAnimation() == "Walk")
		playeranimLib.changeAnimation("FireWalk", player_CurrentAnim);
	else if (input->mainCat->GetFiring() && input->mainCat->GetAnimLibrary()->getcurrentAnimation() == "Idle")
		playeranimLib.changeAnimation("Fire", player_CurrentAnim);*/
}
