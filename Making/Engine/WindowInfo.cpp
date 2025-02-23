#include "pch.h"
#include "WindowInfo.h"
#include "Input.h"

void WindowInfo::Init()
{
	glfwInit();
	window = glfwCreateWindow(WIN_W, WIN_H, basename, NULL, NULL);
	glfwSetWindowPos(window, WIN_X, WIN_Y);
	glfwMakeContextCurrent(window);
	glewInit();
	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0F, 1.0F, 1.0F, 1.0F);

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		GET_SINGLE(Input)->KeyBoardInput(window, key, scancode, action, mods);
		});
}
