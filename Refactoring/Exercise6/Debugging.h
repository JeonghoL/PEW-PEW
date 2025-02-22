#pragma once
using namespace std;

void CheckZeroWeightVertices() {
	int zeroWeightCount = 0;

	for (size_t i = 0; i < vertex_to_bones.size(); i++) {
		float totalWeight = 0.0f;
		for (int j = 0; j < MAX_NUM_BONES_PER_VERTEX; j++) {
			totalWeight += vertex_to_bones[i].Weights[j];
		}

		if (totalWeight < 0.01f) {
			zeroWeightCount++;
			if (zeroWeightCount <= 10) { 
				std::cout << "Vertex " << i << " has zero total weight:" << std::endl;
				std::cout << "Bone IDs and Weights:" << std::endl;
				for (int j = 0; j < MAX_NUM_BONES_PER_VERTEX; j++) {
					std::cout << "  Bone ID: " << vertex_to_bones[i].BoneIDs[j]
						<< ", Weight: " << vertex_to_bones[i].Weights[j] << std::endl;
				}
			}
		}
	}

	std::cout << "\nTotal vertices with zero weights: " << zeroWeightCount
		<< " out of " << vertex_to_bones.size() << std::endl;
}

void checkFileAnimation(const aiScene* scene) {
	if (!scene || !scene->mAnimations || scene->mNumAnimations == 0)
		cout << "No animations found in the scene." << endl;
	else
		cout << "Scene has animation" << endl;
}

void CheckBoneHierarchy(const aiNode* node, int depth = 0, const std::string& parentName = "root") {
	std::string indent(depth * 2, ' ');
	std::string nodeName = node->mName.C_Str();

	std::cout << indent << "Node: " << nodeName << " (Parent: " << parentName << ")";

	if (m_BoneNameToIndexMap.find(nodeName) != m_BoneNameToIndexMap.end()) {
		int boneIndex = m_BoneNameToIndexMap[nodeName];
		std::cout << " [Bone ID: " << boneIndex << "]";
	}
	std::cout << std::endl;

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		CheckBoneHierarchy(node->mChildren[i], depth + 1, nodeName);
	}
}

void PrintBoneHierarchy(const aiNode* node, int depth = 0) {
	std::string indent(depth * 2, ' ');
	std::cout << indent << "Node: " << node->mName.C_Str();

	if (m_BoneNameToIndexMap.find(node->mName.C_Str()) != m_BoneNameToIndexMap.end()) {
		std::cout << " (Bone Index: " << m_BoneNameToIndexMap[node->mName.C_Str()] << ")";
	}

	aiMatrix4x4 transform = node->mTransformation;
	std::cout << "\nTransform Matrix: \n";
	std::cout << indent << transform.a1 << " " << transform.a2 << " " << transform.a3 << " " << transform.a4 << "\n";
	std::cout << indent << transform.b1 << " " << transform.b2 << " " << transform.b3 << " " << transform.b4 << "\n";
	std::cout << indent << transform.c1 << " " << transform.c2 << " " << transform.c3 << " " << transform.c4 << "\n";
	std::cout << indent << transform.d1 << " " << transform.d2 << " " << transform.d3 << " " << transform.d4 << "\n";

	std::cout << std::endl;

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		PrintBoneHierarchy(node->mChildren[i], depth + 1);
	}
}