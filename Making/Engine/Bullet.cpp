#include "pch.h"
#include "Bullet.h"
#include "stb_image.h"
#include "MainCharacter.h"
#include "Camera.h"

//extern bool FirstPersonView;
//extern float min_X[70], max_X[70], min_Z[70], max_Z[70];

Bullet::Bullet(int type, int i, int j)
{
	SetUpShader("Shaders/StaticObjectVert.glsl", "Shaders/StaticObjectFrag.glsl", shaderprogram);
	if (type == 1)
	{
		LoadBulletGLB("glb/dagger.glb");
		Texture = LoadBulletTexture("texture/dagger.png");
		b_type = type;
		bulletSpeed = 0.2f;
	}
	else if (type == 2)
	{
		LoadBulletGLB("glb/star.glb");
		Texture = LoadBulletTexture("texture/star.png");
		b_type = type;
		enemy_i = i;
		enemy_j = j;
		bulletSpeed = 0.3f;
	}
}

Bullet::~Bullet()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderprogram);
}


void Bullet::LoadBulletGLB(const std::string& filename) {
	const aiScene* scene = objectImporter.ReadFile(filename,
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace);
	if (!scene) {
		std::cout << "Failed to load GLB file: " << objectImporter.GetErrorString() << std::endl;
		return;
	}

	std::vector<float> vertexData;
	Indices.clear();

	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
			vertexData.push_back(mesh->mVertices[j].x);
			vertexData.push_back(mesh->mVertices[j].y);
			vertexData.push_back(mesh->mVertices[j].z);

			if (mesh->HasNormals()) {
				vertexData.push_back(mesh->mNormals[j].x);
				vertexData.push_back(mesh->mNormals[j].y);
				vertexData.push_back(mesh->mNormals[j].z);
			}
			else {
				vertexData.push_back(0.0f);
				vertexData.push_back(1.0f);
				vertexData.push_back(0.0f);
			}

			if (mesh->HasTextureCoords(0)) {
				vertexData.push_back(mesh->mTextureCoords[0][j].x);
				vertexData.push_back(mesh->mTextureCoords[0][j].y);
			}
			else {
				vertexData.push_back(0.0f);
				vertexData.push_back(0.0f);
			}
		}

		for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
			aiFace face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; k++)
				Indices.push_back(face.mIndices[k]);
		}
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), Indices.data(), GL_STATIC_DRAW);

	const GLsizei stride = (3 + 3 + 2) * sizeof(float);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

GLuint Bullet::LoadBulletTexture(const char* path)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format{};
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void Bullet::BulletSetting(MainCharacter* mainCharacter, Camera* camera, glm::vec3 mousePick)
{
	if (b_type == 1)
	{
		position = mainCharacter->GetPosition();
		position.y = 0.45f;

		angle = atan2(mouseDir.x, mouseDir.z);

		if (!camera->GetViewType()) {

			position.x += cos(angle) * 0.2f;
			position.z -= sin(angle) * 0.2f;

			glm::vec3 targetPos = mousePick;
			targetPos.y = 0.45f;
			direction = glm::normalize(targetPos - position);
			angle = atan2(direction.x, direction.z);
		}
		else {
			angle = camera->GetHorizontalAngle();
			float verticalAngle = camera->GetVerticalAngle();

			direction = glm::vec3(
				sin(angle) * cos(verticalAngle),
				-sin(verticalAngle),
				cos(angle) * cos(verticalAngle)
			);
		}

		model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	//else if (b_type == 2)
	//{
	//	position = enemy[enemy_i][enemy_j]->getPosition();
	//	position.y = 0.45f;
	//
	//	tPos = mainCat->getPosition();
	//	tPos.y = 0.45f;
	//	direction = glm::normalize(tPos - position);
	//	position += 0.5f * direction;
	//	angle = atan2(direction.x, direction.z);
	//}
}

//void Bullet::BulletSettingAgain(glm::vec3 Pos)
//{
//	position = enemy[enemy_i][enemy_j]->getPosition();
//	position.y = 0.45f;
//
//	tPos = Pos;
//	tPos.y = 0.45f;
//	direction = glm::normalize(tPos - position);
//	position += 0.5f * direction;
//	angle = atan2(direction.x, direction.z);
//}

void Bullet::render(const glm::mat4& orgview, const glm::mat4& orgproj, glm::vec3 viewPos,
	glm::mat4 lightSpaceMatrix, GLuint shadowMap)
{
	glUseProgram(shaderprogram);
	ViewLoc = glGetUniformLocation(shaderprogram, "view");
	glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, &orgview[0][0]);

	ProjLoc = glGetUniformLocation(shaderprogram, "projection");
	glUniformMatrix4fv(ProjLoc, 1, GL_FALSE, &orgproj[0][0]);

	ModelLoc = glGetUniformLocation(shaderprogram, "model");
	glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "lightSpaceMatrix"),
		1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glUniform1i(glGetUniformLocation(shaderprogram, "shadowMap"), 1);

	GLuint lightPosLoc = glGetUniformLocation(shaderprogram, "lightPos");
	GLuint viewPosLoc = glGetUniformLocation(shaderprogram, "viewPos");
	glm::vec3 lightPos{ -37.3051f - (1000.0f * cos(light_angle)), 0.0f + 1000.0f, 42.5001f + (1000.0f * sin(light_angle)) };
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(glGetUniformLocation(shaderprogram, "objTexture"), 0);
	glUniform1i(glGetUniformLocation(shaderprogram, "useobjTexture"), 1);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
}

void Bullet::renderShadow(const glm::mat4& lightSpaceMatrix, GLuint depthShader)
{
	glUseProgram(depthShader);
	glUniformMatrix4fv(glGetUniformLocation(depthShader, "lightSpaceMatrix"),
		1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	glUniformMatrix4fv(glGetUniformLocation(depthShader, "model"),
		1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
}

//bool Bullet::isCollapsed()
//{
//	bool check{ false };
//
//	for (int i = 0; i < 70; ++i)
//	{
//		if (min_Z[i] <= position.z && max_Z[i] >= position.z)
//		{
//			if (position.x <= max_X[i] && position.x > min_X[i])
//				check = true;
//		}
//	}
//
//	if (b_type == 1)
//	{
//		for (int i = 0; i < 3; ++i)
//		{
//			for (int j = 0; j < 9; ++j)
//			{
//				glm::vec3 pos = enemy[i][j]->getPosition();
//
//				if (!(enemy[i][j]->getstate() == 4))
//				{
//					if (position.y >= 0.0f && position.y <= 0.95f)
//					{
//						if ((position.x >= pos.x - 0.25f && position.x <= pos.x + 0.25f) &&
//							(position.z >= pos.z - 0.2f && position.z <= pos.z + 0.2f))
//						{
//							enemy[i][j]->setlife();
//							return true;
//						}
//					}
//				}
//			}
//		}
//	}
//	else if (b_type == 2)
//	{
//		glm::vec3 pos = mainCat->getPosition();
//
//		if (!mainCat->getdying())
//		{
//			if (position.y >= 0.0f && position.y <= 0.95f)
//			{
//				if ((position.x >= pos.x - 0.25f && position.x <= pos.x + 0.25f) &&
//					(position.z >= pos.z - 0.2f && position.z <= pos.z + 0.2f))
//				{
//					// ���ΰ� ĳ���� Ÿ��
//					mainCat->setlife();
//					return true;
//				}
//			}
//		}
//	}
//
//	return check;
//}

void Bullet::bulletUpdate()
{
	position += direction * bulletSpeed;

	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	if (b_type == 1)
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	else if (b_type == 2)
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
}