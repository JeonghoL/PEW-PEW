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
	void Draw(GLFWwindow* window);
	void DrawShadow();
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
