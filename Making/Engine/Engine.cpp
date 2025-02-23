#include "pch.h"
#include "Engine.h"
#include "WindowInfo.h"

void Engine::Init()
{
	GET_SINGLE(WindowInfo)->Init();

}

void Engine::Update()
{
	GLFWwindow* window = GET_SINGLE(WindowInfo)->GetWindow();
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// TODO

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

//void Engine::ShowFps()
//{
//	uint32 fps = GET_SINGLE(Timer)->GetFps();
//
//	WCHAR text[100] = L"";
//	wsprintf(text, L"FPS : %d", fps);
//
//	SetWindowText(_window.hwnd, text);
//}
