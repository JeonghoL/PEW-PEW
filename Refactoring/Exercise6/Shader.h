#pragma once
#include "./common.c"

void setupShader(const char* vertexName, const char* fragmentName, GLuint& shaderName) {
	const char* vertSource = loadFile(vertexName);
	const char* fragSource = loadFile(fragmentName);

	if (!vertSource || !fragSource) {
		std::cerr << "Failed to load shader files" << std::endl;
		return;
	}

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragSource, NULL);
	glCompileShader(fragmentShader);

	shaderName = glCreateProgram();
	glAttachShader(shaderName, vertexShader);
	glAttachShader(shaderName, fragmentShader);
	glLinkProgram(shaderName);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	free((void*)vertSource);
	free((void*)fragSource);
}

void drawStaticobject(const GLuint& shadername, const GLuint& VAOname, const GLuint& texturename, const std::vector<unsigned int>& indicesname,
	const glm::mat4& orgview, const glm::mat4& orgproj, const glm::mat4& modelname, GLuint& viewlocname, GLuint& projlocname, GLuint& modellocname)
{
	glUseProgram(shadername);

	viewlocname = glGetUniformLocation(shadername, "view");
	glUniformMatrix4fv(viewlocname, 1, GL_FALSE, &orgview[0][0]);

	projlocname = glGetUniformLocation(shadername, "projection");
	glUniformMatrix4fv(projlocname, 1, GL_FALSE, &orgproj[0][0]);

	modellocname = glGetUniformLocation(shadername, "model");
	glUniformMatrix4fv(modellocname, 1, GL_FALSE, glm::value_ptr(modelname));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturename);
	glUniform1i(glGetUniformLocation(shadername, "objTexture"), 0);
	glUniform1i(glGetUniformLocation(shadername, "useobjTexture"), 1);

	glBindVertexArray(VAOname);
	glDrawElements(GL_TRIANGLES, indicesname.size(), GL_UNSIGNED_INT, 0);
}