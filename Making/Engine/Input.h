#pragma once

class Camera;
class MainCharacter;

class Input
{
public:
	static void KeyBoardInput(GLFWwindow* window, int key, int scancode, int action, int mods);

	void SetCamera(Camera* cam) { camera = cam; }
	void SetMainCharacter(MainCharacter* cat) { mainCat = cat; }

private:
	Camera* camera;
	MainCharacter* mainCat;
};
