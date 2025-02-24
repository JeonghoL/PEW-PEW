#pragma once

class Camera;
class MainCharacter;
class Input;

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
	unique_ptr<Camera> camera;
	unique_ptr<MainCharacter> mainCat;
	unique_ptr<Input> input;
};
