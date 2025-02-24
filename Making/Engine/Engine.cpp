#include "pch.h"
#include "Engine.h"
#include "WindowInfo.h"
#include "Timer.h"
#include "Skybox.h"
#include "Camera.h"
#include "MainCharacter.h"
#include "Input.h"

void Engine::Init()
{
	GET_SINGLE(WindowInfo)->Init();
	GET_SINGLE(Timer)->Init();
	GET_SINGLE(Skybox)->Init();

	camera = make_unique<Camera>();
	mainCat = make_unique<MainCharacter>();
	input = make_unique<Input>();
	input->SetCamera(camera.get());

	GLFWwindow* window = GET_SINGLE(WindowInfo)->GetWindow();
	glfwSetWindowUserPointer(window, input.get());
	glfwSetKeyCallback(window, Input::KeyBoardInput);
}

void Engine::Update()
{
	GLFWwindow* window = GET_SINGLE(WindowInfo)->GetWindow();

	while (!glfwWindowShouldClose(window)) {
		//GET_SINGLE(Timer)->Update();
		camera->Update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Draw(window);
		//ShowFps();
		// TODO

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Engine::Draw(GLFWwindow* window)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glfwGetCursorPos(window, &cur_x, &cur_y);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIN_W / (float)WIN_H, 0.1f, 1000.0f);
	glm::mat4 view = camera->getViewMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GET_SINGLE(Skybox)->Draw(view, projection);
	mainCat->Draw(view, projection);

	glFinish();
}

void Engine::Release()
{
	GET_SINGLE(Skybox)->Release();
}

void Engine::ShowFps()
{
	uint32 fps = GET_SINGLE(Timer)->GetFps();

	char text[100];
	sprintf_s(text, "FPS : %d", fps);

	GLFWwindow* window = GET_SINGLE(WindowInfo)->GetWindow();
	glfwSetWindowTitle(window, text);
}
