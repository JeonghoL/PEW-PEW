#include "pch.h"
#include "EnginePch.h"
#include "stb_image.h"

double cur_x = 0.0f;
double cur_y = 0.0f;

string LoadFile(const string& filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error: cannot open \"" << filename << "\"" << std::endl;
		return "";
	}

	// 파일 크기 구하기
	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0, std::ios::beg);

	// 버퍼 할당 및 파일 읽기
	std::string buffer;
	buffer.resize(size);
	file.read(&buffer[0], size);
	file.close();

	return buffer;
}

GLuint LoadTexture(const char* path)
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

void SetUpShader(const char* vertexName, const char* fragmentName, GLuint& shaderName)
{
	string vertSource = LoadFile(vertexName);
	string fragSource = LoadFile(fragmentName);

	if (vertSource.empty() || fragSource.empty()) {
		cerr << "Failed to load shader files" << endl;
		return;
	}

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertSourcePtr = vertSource.c_str();
	glShaderSource(vertexShader, 1, &vertSourcePtr, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragSourcePtr = fragSource.c_str();
	glShaderSource(fragmentShader, 1, &fragSourcePtr, NULL);
	glCompileShader(fragmentShader);

	shaderName = glCreateProgram();
	glAttachShader(shaderName, vertexShader);
	glAttachShader(shaderName, fragmentShader);
	glLinkProgram(shaderName);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

//void loadGLBFile(int j, vector<BoneInfo>& BoneInfoName, const std::string& filename, GLuint& VAO, GLuint& VBO, GLuint& VBO2, GLuint& EBO, vector<unsigned int>& Indices) {
//	const aiScene* scene = characterImporter.ReadFile(filename,
//		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices |
//		aiProcess_CalcTangentSpace);
//	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
//		std::cout << "Failed to load GLB file: " << characterImporter.GetErrorString() << std::endl;
//		return;
//	}
//
//	BoneInfoName.clear();
//	BoneInfoName.resize(MAX_BONES);
//	m_BoneNameToIndexMap.clear();
//
//	aiMatrix4x4 GlobalTransform = scene->mRootNode->mTransformation;
//	m_GlobalInverseTransform[j] = glm::inverse(glm::mat4(
//		GlobalTransform.a1, GlobalTransform.b1, GlobalTransform.c1, GlobalTransform.d1,
//		GlobalTransform.a2, GlobalTransform.b2, GlobalTransform.c2, GlobalTransform.d2,
//		GlobalTransform.a3, GlobalTransform.b3, GlobalTransform.c3, GlobalTransform.d3,
//		GlobalTransform.a4, GlobalTransform.b4, GlobalTransform.c4, GlobalTransform.d4
//	));
//
//	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
//		const aiMesh* mesh = scene->mMeshes[i];
//		if (mesh->HasBones()) {
//			for (unsigned int j = 0; j < mesh->mNumBones; j++) {
//				const aiBone* bone = mesh->mBones[j];
//				std::string boneName(bone->mName.C_Str());
//
//				if (m_BoneNameToIndexMap.find(boneName) == m_BoneNameToIndexMap.end()) {
//					unsigned int boneIndex = m_BoneNameToIndexMap.size();
//					m_BoneNameToIndexMap[boneName] = boneIndex;
//
//					BoneInfo bi;
//					bi.BoneOffset = glm::mat4(
//						bone->mOffsetMatrix.a1, bone->mOffsetMatrix.b1, bone->mOffsetMatrix.c1, bone->mOffsetMatrix.d1,
//						bone->mOffsetMatrix.a2, bone->mOffsetMatrix.b2, bone->mOffsetMatrix.c2, bone->mOffsetMatrix.d2,
//						bone->mOffsetMatrix.a3, bone->mOffsetMatrix.b3, bone->mOffsetMatrix.c3, bone->mOffsetMatrix.d3,
//						bone->mOffsetMatrix.a4, bone->mOffsetMatrix.b4, bone->mOffsetMatrix.c4, bone->mOffsetMatrix.d4
//					);
//
//					if (boneIndex < MAX_BONES) {
//						BoneInfoName[boneIndex] = bi;
//					}
//				}
//			}
//		}
//	}
//
//	parse_scene(scene);
//	//CheckBoneHierarchy(scene->mRootNode);		// 본 계층구조 확인함수
//	//PrintBoneHierarchy(scene->mRootNode);
//	//checkFileAnimation(scene);		// T-pose 캐릭터 파일에 애니메이션 있는지 확인
//
//	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
//		const aiMesh* mesh = scene->mMeshes[i];
//		LoadBones(mesh, BoneInfoName);
//	}
//
//	std::vector<float> vertexData;
//	std::vector<int> boneData;
//	unsigned int totalVertices = 0;
//
//	Indices.clear();
//
//	for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++) {
//		aiMesh* mesh = scene->mMeshes[meshIndex];
//
//		// 텍스처 좌표 추출 (동일)
//		std::vector<float> texCoords;
//		if (mesh->HasTextureCoords(0)) {
//			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
//				aiVector3D texCoord = mesh->mTextureCoords[0][i];
//				texCoords.push_back(texCoord.x);
//				texCoords.push_back(texCoord.y);
//			}
//		}
//		else {
//			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
//				texCoords.push_back(0.0f);
//				texCoords.push_back(0.0f);
//			}
//		}
//
//		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
//			aiVector3D vertex = mesh->mVertices[i];
//			vertexData.push_back(vertex.x);
//			vertexData.push_back(vertex.y);
//			vertexData.push_back(vertex.z);
//
//			if (mesh->HasNormals()) {
//				aiVector3D normal = mesh->mNormals[i];
//				vertexData.push_back(normal.x);
//				vertexData.push_back(normal.y);
//				vertexData.push_back(normal.z);
//			}
//			else {
//				vertexData.push_back(0.0f);
//				vertexData.push_back(1.0f);
//				vertexData.push_back(0.0f);
//			}
//
//			if (mesh->HasTextureCoords(0)) {
//				vertexData.push_back(texCoords[i * 2]);
//				vertexData.push_back(texCoords[i * 2 + 1]);
//			}
//			else {
//				vertexData.push_back(0.0f);
//				vertexData.push_back(0.0f);
//			}
//
//			for (int j = 0; j < MAX_NUM_BONES_PER_VERTEX; j++) {
//				vertexData.push_back(vertex_to_bones[totalVertices + i].Weights[j]);
//			}
//
//			for (int j = 0; j < MAX_NUM_BONES_PER_VERTEX; j++) {
//				boneData.push_back(vertex_to_bones[totalVertices + i].BoneIDs[j]);
//			}
//		}
//
//		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
//			aiFace face = mesh->mFaces[i];
//			for (unsigned int j = 0; j < face.mNumIndices; j++) {
//				Indices.push_back(face.mIndices[j] + totalVertices);
//			}
//		}
//
//		totalVertices += mesh->mNumVertices;
//	}
//
//	glGenVertexArrays(1, &VAO);
//	glBindVertexArray(VAO);
//
//	glGenBuffers(1, &VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
//
//	glGenBuffers(1, &VBO2);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
//	glBufferData(GL_ARRAY_BUFFER, boneData.size() * sizeof(int), boneData.data(), GL_STATIC_DRAW);
//
//	glGenBuffers(1, &EBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), Indices.data(), GL_STATIC_DRAW);
//
//	const GLsizei vertexStride = (3 + 3 + 2 + 4) * sizeof(float);  // position + normal + texcoord + weights
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexStride, (void*)0);  // position
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexStride, (void*)(3 * sizeof(float)));  // normal
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexStride, (void*)(6 * sizeof(float)));  // texcoord
//	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, vertexStride, (void*)(8 * sizeof(float)));  // weights
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
//	glBufferData(GL_ARRAY_BUFFER, boneData.size() * sizeof(int), boneData.data(), GL_STATIC_DRAW);
//	glVertexAttribIPointer(3, 4, GL_INT, 0, (void*)0);
//
//	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);
//	glEnableVertexAttribArray(2);
//	glEnableVertexAttribArray(3);
//	glEnableVertexAttribArray(4);
//
//	glBindVertexArray(0);
//}