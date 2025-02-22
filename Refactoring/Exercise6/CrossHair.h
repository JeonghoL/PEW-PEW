#pragma once

class CrossHair
{
public:
	CrossHair(int Viewtype, float size, GLFWwindow* win);
	~CrossHair();

	void setupCrosshairShaders(const char* vertexName, const char* fragmentName);

	void draw();
private:
	GLuint VAO, VBO, EBO, shaderProgram;
	std::vector<float> vertices;
	const float WIN_W = 2560.0f;
	const float WIN_H = 1440.0f;
	int m_Viewtype;
	GLFWcursor* customCursor;
	GLFWwindow* window;
};