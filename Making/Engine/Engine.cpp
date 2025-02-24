#include "pch.h"
#include "Engine.h"
#include "WindowInfo.h"
#include "Timer.h"

void Engine::Init()
{
	GET_SINGLE(WindowInfo)->Init();
	GET_SINGLE(Timer)->Init();
}

void Engine::Update()
{
	GLFWwindow* window = GET_SINGLE(WindowInfo)->GetWindow();
	while (!glfwWindowShouldClose(window)) {
		GET_SINGLE(Timer)->Update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Draw();
		ShowFps();
		// TODO

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Engine::Draw()
{

	glFinish();
}

void Engine::ShowFps()
{
	uint32 fps = GET_SINGLE(Timer)->GetFps();

	char text[100];
	sprintf_s(text, "FPS : %d", fps);

	GLFWwindow* window = GET_SINGLE(WindowInfo)->GetWindow();
	glfwSetWindowTitle(window, text);
}
