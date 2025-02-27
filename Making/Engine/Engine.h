#pragma once

class Camera;
class MainCharacter;
class Input;
class StaticObject;

class Engine
{
public:
	void Init();
	void Update();
	void Draw(GLFWwindow* window);
	void Release();

private:
	void ShowFps();

private:
	Camera* camera = { nullptr };
	MainCharacter* mainCat = { nullptr };
	Input* input = { nullptr };
};
