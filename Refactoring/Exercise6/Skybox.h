#pragma once

class Skybox
{
public:
	Skybox();
	~Skybox();
	unsigned int loadCubemap(std::vector<std::string> faces);
	void setskyboxVertices();

	void setskyboxupShader(const char* vertexName, const char* fragmentName, GLuint& shaderName);

	void draw(const glm::mat4& view, const glm::mat4& projection);

private:
	std::vector<std::string> faces;
	float skyboxVertices[108];
	unsigned int cubemapTexture;
	GLuint VAO, VBO, shaderprogram;
	GLuint ViewLoc, ProjLoc;
};