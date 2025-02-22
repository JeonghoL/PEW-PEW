#pragma once

struct BoneInfo {
	glm::mat4 BoneOffset;       
	glm::mat4 FinalTransformation; 
	glm::mat4 LocalTransform;    
	aiNodeAnim* nodeAnim;        

	BoneInfo() {
		BoneOffset = glm::mat4(1.0f);
		FinalTransformation = glm::mat4(1.0f);
		LocalTransform = glm::mat4(1.0f);
		nodeAnim = nullptr;
	}
};

vector<BoneInfo> player_BoneInfo, enemy_BoneInfo[3];
glm::mat4 m_GlobalInverseTransform[5];
Assimp::Importer characterImporter; 
Assimp::Importer objectImporter;    
vector<unsigned int> colli_indices;

void LoadBones(const aiMesh* mesh, vector<BoneInfo>& BoneInfoName) {
	for (unsigned int i = 0; i < mesh->mNumBones; i++) {
		string BoneName(mesh->mBones[i]->mName.data);
		if (m_BoneNameToIndexMap.find(BoneName) != m_BoneNameToIndexMap.end()) {
			unsigned int BoneIndex = m_BoneNameToIndexMap[BoneName];

			aiMatrix4x4& offset = mesh->mBones[i]->mOffsetMatrix;

			BoneInfoName[BoneIndex].BoneOffset = glm::mat4(
				offset.a1, offset.b1, offset.c1, offset.d1,
				offset.a2, offset.b2, offset.c2, offset.d2,
				offset.a3, offset.b3, offset.c3, offset.d3,
				offset.a4, offset.b4, offset.c4, offset.d4
			);
		}
	}
}

void loadStaticGLB(const std::string& filename, GLuint& VAO, GLuint& VBO, GLuint& EBO, vector<unsigned int>& Indices) {
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

void getModelBounds(const std::string& filename, float& minX, float& maxX, float& minZ, float& maxZ) {
	const aiScene* scene = objectImporter.ReadFile(filename,
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace);

	if (!scene) {
		std::cout << "Failed to load GLB file: " << objectImporter.GetErrorString() << std::endl;
		return;
	}

	minX = std::numeric_limits<float>::max();
	maxX = std::numeric_limits<float>::lowest();
	minZ = std::numeric_limits<float>::max();
	maxZ = std::numeric_limits<float>::lowest();

	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
			float x = mesh->mVertices[j].x;
			float z = mesh->mVertices[j].z;

			minX = std::min(minX, x);
			maxX = std::max(maxX, x);
			minZ = std::min(minZ, z);
			maxZ = std::max(maxZ, z);
		}
	}
}

void loadGLBFile(int j, vector<BoneInfo>& BoneInfoName, const std::string& filename, GLuint& VAO, GLuint& VBO, GLuint& VBO2, GLuint& EBO, vector<unsigned int>& Indices) {
	const aiScene* scene = characterImporter.ReadFile(filename,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices |
		aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Failed to load GLB file: " << characterImporter.GetErrorString() << std::endl;
		return;
	}

	BoneInfoName.clear();
	BoneInfoName.resize(MAX_BONES);
	m_BoneNameToIndexMap.clear();

	aiMatrix4x4 GlobalTransform = scene->mRootNode->mTransformation;
	m_GlobalInverseTransform[j] = glm::inverse(glm::mat4(
		GlobalTransform.a1, GlobalTransform.b1, GlobalTransform.c1, GlobalTransform.d1,
		GlobalTransform.a2, GlobalTransform.b2, GlobalTransform.c2, GlobalTransform.d2,
		GlobalTransform.a3, GlobalTransform.b3, GlobalTransform.c3, GlobalTransform.d3,
		GlobalTransform.a4, GlobalTransform.b4, GlobalTransform.c4, GlobalTransform.d4
	));

	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		const aiMesh* mesh = scene->mMeshes[i];
		if (mesh->HasBones()) {
			for (unsigned int j = 0; j < mesh->mNumBones; j++) {
				const aiBone* bone = mesh->mBones[j];
				std::string boneName(bone->mName.C_Str());

				if (m_BoneNameToIndexMap.find(boneName) == m_BoneNameToIndexMap.end()) {
					unsigned int boneIndex = m_BoneNameToIndexMap.size();
					m_BoneNameToIndexMap[boneName] = boneIndex;

					BoneInfo bi;
					bi.BoneOffset = glm::mat4(
						bone->mOffsetMatrix.a1, bone->mOffsetMatrix.b1, bone->mOffsetMatrix.c1, bone->mOffsetMatrix.d1,
						bone->mOffsetMatrix.a2, bone->mOffsetMatrix.b2, bone->mOffsetMatrix.c2, bone->mOffsetMatrix.d2,
						bone->mOffsetMatrix.a3, bone->mOffsetMatrix.b3, bone->mOffsetMatrix.c3, bone->mOffsetMatrix.d3,
						bone->mOffsetMatrix.a4, bone->mOffsetMatrix.b4, bone->mOffsetMatrix.c4, bone->mOffsetMatrix.d4
					);

					if (boneIndex < MAX_BONES) {
						BoneInfoName[boneIndex] = bi;
					}
				}
			}
		}
	}

	parse_scene(scene);
	//CheckBoneHierarchy(scene->mRootNode);		// 본 계층구조 확인함수
	//PrintBoneHierarchy(scene->mRootNode);
	//checkFileAnimation(scene);		// T-pose 캐릭터 파일에 애니메이션 있는지 확인

	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		const aiMesh* mesh = scene->mMeshes[i];
		LoadBones(mesh, BoneInfoName);
	}

	std::vector<float> vertexData;
	std::vector<int> boneData;
	unsigned int totalVertices = 0;

	Indices.clear();

	for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++) {
		aiMesh* mesh = scene->mMeshes[meshIndex];

		// 텍스처 좌표 추출 (동일)
		std::vector<float> texCoords;
		if (mesh->HasTextureCoords(0)) {
			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
				aiVector3D texCoord = mesh->mTextureCoords[0][i];
				texCoords.push_back(texCoord.x);
				texCoords.push_back(texCoord.y);
			}
		}
		else {
			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
				texCoords.push_back(0.0f);
				texCoords.push_back(0.0f);
			}
		}

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			aiVector3D vertex = mesh->mVertices[i];
			vertexData.push_back(vertex.x);
			vertexData.push_back(vertex.y);
			vertexData.push_back(vertex.z);

			if (mesh->HasNormals()) {
				aiVector3D normal = mesh->mNormals[i];
				vertexData.push_back(normal.x);
				vertexData.push_back(normal.y);
				vertexData.push_back(normal.z);
			}
			else {
				vertexData.push_back(0.0f);
				vertexData.push_back(1.0f);
				vertexData.push_back(0.0f);
			}

			if (mesh->HasTextureCoords(0)) {
				vertexData.push_back(texCoords[i * 2]);
				vertexData.push_back(texCoords[i * 2 + 1]);
			}
			else {
				vertexData.push_back(0.0f);
				vertexData.push_back(0.0f);
			}

			for (int j = 0; j < MAX_NUM_BONES_PER_VERTEX; j++) {
				vertexData.push_back(vertex_to_bones[totalVertices + i].Weights[j]);
			}

			for (int j = 0; j < MAX_NUM_BONES_PER_VERTEX; j++) {
				boneData.push_back(vertex_to_bones[totalVertices + i].BoneIDs[j]);
			}
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				Indices.push_back(face.mIndices[j] + totalVertices);
			}
		}

		totalVertices += mesh->mNumVertices;
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, boneData.size() * sizeof(int), boneData.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), Indices.data(), GL_STATIC_DRAW);

	const GLsizei vertexStride = (3 + 3 + 2 + 4) * sizeof(float);  // position + normal + texcoord + weights

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexStride, (void*)0);  // position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexStride, (void*)(3 * sizeof(float)));  // normal
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexStride, (void*)(6 * sizeof(float)));  // texcoord
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, vertexStride, (void*)(8 * sizeof(float)));  // weights

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, boneData.size() * sizeof(int), boneData.data(), GL_STATIC_DRAW);
	glVertexAttribIPointer(3, 4, GL_INT, 0, (void*)0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);
}