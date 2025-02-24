#pragma once

class Camera;

class Input
{
public:
	static void KeyBoardInput(GLFWwindow* window, int key, int scancode, int action, int mods);

	void SetCamera(Camera* cam) { camera = cam; }

private:
	Camera* camera;
};
