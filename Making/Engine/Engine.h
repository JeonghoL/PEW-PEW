#pragma once

class Camera;
class MainCharacter;
class Input;
class StaticObject;
class ShadowMapping;
class Enemy;

class Engine
{
public:
	void Init();
	void Update();
	void Render(GLFWwindow* window);
	void RenderShadow();
	void Release();

private:
	void ShowFps();

private:
	Camera* camera = { nullptr };
	MainCharacter* mainCat = { nullptr };
	Input* input = { nullptr };
	ShadowMapping* shadowMap = { nullptr };
	Enemy* enemy[3][9] = { nullptr };
};
