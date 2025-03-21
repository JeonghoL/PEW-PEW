#include "pch.h"
#include "Engine.h"
#include "WindowInfo.h"
#include "Timer.h"
#include "Skybox.h"
#include "Camera.h"
#include "MainCharacter.h"
#include "Enemy.h"
#include "Input.h"
#include "StaticObjectManager.h"
#include "ShadowMapping.h"

void Engine::Init()
{
	GET_SINGLE(WindowInfo)->Init();
	GET_SINGLE(Timer)->Init();
	GET_SINGLE(Skybox)->Init();
	GET_SINGLE(StaticObjectManager)->Init();

	camera = new Camera();
	mainCat = new MainCharacter();

	shadowMap = new ShadowMapping();

	mainCat->SetCamera(camera);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			enemy[i][j] = new Enemy(i + 1, j);
		}
	}

	input = new Input();
	input->SetCamera(camera);
	input->SetMainCharacter(mainCat);

	GLFWwindow* window = GET_SINGLE(WindowInfo)->GetWindow();
	glfwSetWindowUserPointer(window, input);
	glfwSetKeyCallback(window, Input::KeyBoardInput);
	glfwSetScrollCallback(window, Input::Scroll_callback);
	glfwSetMouseButtonCallback(window, Input::MouseFunc);
}

void Engine::Update()
{
	GLFWwindow* window = GET_SINGLE(WindowInfo)->GetWindow();

	while (!glfwWindowShouldClose(window)) {
		GET_SINGLE(Timer)->Update();
		camera->Update();
		mainCat->Update();
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 9; ++j)
			{
				enemy[i][j]->Update(mainCat);
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Draw(window);
		ShowFps();
		
		// TODO

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Engine::Draw(GLFWwindow* window)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glfwGetCursorPos(window, &cur_x, &cur_y);
	float deltatime = GET_SINGLE(Timer)->GetDeltaTime();

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIN_W / (float)WIN_H, 0.1f, 1000.0f);
	glm::mat4 view = camera->GetViewMatrix(mainCat->GetPosition());
	if (!camera->IsAltPressed())
		mouseDir = camera->SetMouseWorldDirection(cur_x, cur_y, projection, view, mainCat->GetPosition());
	glm::vec3 viewPos = camera->GetPosition(mainCat->GetPosition());
	glm::mat4 lightSpaceMatrix = shadowMap->GetLightSpaceMatrix();
	float angle = camera->GetAngle();

	mainCat->ChangeCatAnimation(view, projection);

	DrawShadow();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Normal Pass 시작
	GET_SINGLE(Skybox)->Draw(view, projection);
	
	GET_SINGLE(StaticObjectManager)->Draw(view, projection, viewPos, lightSpaceMatrix, shadowMap->GetDepthMap());

	mainCat->Draw(view, projection, viewPos, deltatime, angle);
	mainCat->ThrowBullets(view, projection, viewPos, lightSpaceMatrix, shadowMap->GetDepthMap());

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
			enemy[i][j]->Draw(viewPos, j, deltatime, mainCat->GetPosition(), enemy[i][j], view, projection, viewPos, lightSpaceMatrix, shadowMap->GetDepthMap());
	}

	glFinish();
}

void Engine::DrawShadow()
{
	shadowMap->UpdateLightSpaceMatrix(mainCat->GetPosition());		// Shadow Pass 시작
	shadowMap->BindFramebuffer();
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(shadowMap->GetDepthShaderProgram());			// Depth map 렌더링
	glm::mat4 lightSpaceMatrix = shadowMap->GetLightSpaceMatrix();
	GLuint lightSpaceMatrixLoc = glGetUniformLocation(shadowMap->GetDepthShaderProgram(), "lightSpaceMatrix");
	glUniformMatrix4fv(lightSpaceMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	float angle = camera->GetAngle();

	mainCat->Drawshadow(angle, shadowMap->GetDepthShaderProgram(), shadowMap->GetLightSpaceMatrix());

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 9; ++j)
		{
			enemy[i][j]->DrawShadow(j, mainCat->GetPosition(), enemy[i][j], shadowMap);
		}
	}

	GET_SINGLE(StaticObjectManager)->DrawShadow(lightSpaceMatrix, shadowMap->GetStaticDepthShaderProgram());

	//for (auto& bullet : bullets)
	//{
	//	bullet->renderShadow(lightSpaceMatrix, shadowMap->GetStaticDepthShaderProgram());
	//}

	//for (int i = 0; i < 3; ++i) {
	//	for (int j = 0; j < 9; ++j) {
	//		for (auto& bullet : enemyBullets[i][j]) {
	//			bullet->renderShadow(lightSpaceMatrix, shadowMap->GetStaticDepthShaderProgram());
	//		}
	//	}
	//}

	shadowMap->UnbindFramebuffer();
	glViewport(0, 0, WIN_W, WIN_H);			// Shadow Pass 종료
}

void Engine::Release()
{
	GET_SINGLE(Skybox)->Release();
	GET_SINGLE(StaticObjectManager)->Release();
	delete input;
	delete mainCat;
	delete shadowMap;
	delete camera;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
			delete enemy[i][j];
	}
}

void Engine::ShowFps()
{
	uint32 fps = GET_SINGLE(Timer)->GetFps();

	char text[100];
	sprintf_s(text, "FPS : %d", fps);

	GLFWwindow* window = GET_SINGLE(WindowInfo)->GetWindow();
	glfwSetWindowTitle(window, text);
}
