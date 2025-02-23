#pragma once

class Input
{
public:
	DECLARE_SINGLE(Input);

	void KeyBoardInput(GLFWwindow* window, int key, int scancode, int action, int mods);
};
