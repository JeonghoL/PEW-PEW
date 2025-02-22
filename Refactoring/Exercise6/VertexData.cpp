#include "pch.h"
#include "VertexData.h"

std::vector<VertexBoneData> vertex_to_bones;
std::vector<int> mesh_base_vertex;
std::map<std::string, unsigned int> m_BoneNameToIndexMap;

void NormalizeBoneWeights() {
	for (auto& vertex : vertex_to_bones) {
		float total = 0.0f;
		for (int i = 0; i < MAX_NUM_BONES_PER_VERTEX; i++) {
			if (vertex.Weights[i] < 0.01f) {
				vertex.Weights[i] = 0.0f;
			}
			total += vertex.Weights[i];
		}

		if (total > 0.0f) {
			for (int i = 0; i < MAX_NUM_BONES_PER_VERTEX; i++) {
				vertex.Weights[i] /= total;
			}
		}
		else {
			vertex.BoneIDs[0] = 0;
			vertex.Weights[0] = 1.0f;
		}
	}
}

void CheckWeights()
{
	for (size_t i = 0; i < vertex_to_bones.size(); i++) {
		float total = 0.0f;
		for (int j = 0; j < MAX_NUM_BONES_PER_VERTEX; j++) {
			total += vertex_to_bones[i].Weights[j];
		}
		if (total < 0.99f || total > 1.01f) {
			std::cout << "Vertex " << i << " has invalid weight total: " << total << std::endl;
		}
	}
}

void parse_single_bone(int mesh_index, const aiBone* pBone) {
	if (!pBone) return;

	int bone_id = get_bone_id(pBone);
	std::string bone_name(pBone->mName.C_Str());

	for (unsigned int i = 0; i < pBone->mNumWeights; ++i) {
		const aiVertexWeight& vw = pBone->mWeights[i];
		unsigned int global_vertex_id = mesh_base_vertex[mesh_index] + vw.mVertexId;

		vertex_to_bones[global_vertex_id].AddBoneData(bone_id, vw.mWeight);
	}
}

void parsh_mesh_bones(int mesh_index, const aiMesh* pMesh)
{
	for (int i = 0; i < pMesh->mNumBones; ++i)
	{
		parse_single_bone(mesh_index, pMesh->mBones[i]);
	}
}

void parse_meshes(const aiScene* pScene)
{
	int total_vertices = 0;
	int total_indices = 0;
	int total_bones = 0;

	mesh_base_vertex.resize(pScene->mNumMeshes);

	for (int i = 0; i < pScene->mNumMeshes; ++i)
	{
		const aiMesh* pMesh = pScene->mMeshes[i];
		int num_vertices = pMesh->mNumVertices;
		int num_indices = pMesh->mNumFaces * 3;
		int num_bones = pMesh->mNumBones;
		mesh_base_vertex[i] = total_vertices;

		total_vertices += num_vertices;
		total_indices += num_indices;
		total_bones += num_bones;

		vertex_to_bones.resize(total_vertices);

		if (pMesh->HasBones()) {
			parsh_mesh_bones(i, pMesh);
		}
	}
	NormalizeBoneWeights();
	CheckWeights();
}

void parse_scene(const aiScene* pScene)
{
	parse_meshes(pScene);
}

int get_bone_id(const aiBone* pBone)
{
	int bone_id = 0;
	std::string bone_name(pBone->mName.C_Str());

	if (m_BoneNameToIndexMap.find(bone_name) == m_BoneNameToIndexMap.end())
	{
		if (m_BoneNameToIndexMap.size() >= MAX_BONES) {
			std::cerr << "Error: Exceeded maximum number of bones (" << MAX_BONES << ")" << std::endl;
			return 0;
		}

		bone_id = static_cast<int>(m_BoneNameToIndexMap.size());
		m_BoneNameToIndexMap[bone_name] = bone_id;

		std::cout << "Adding new bone: " << bone_name << " with ID: " << bone_id << std::endl;
	}
	else
	{
		bone_id = m_BoneNameToIndexMap[bone_name];
	}

	if (bone_id >= MAX_BONES) {
		std::cerr << "Warning: Bone ID " << bone_id << " exceeds MAX_BONES (" << MAX_BONES << ")" << std::endl;
		return 0;
	}

	return bone_id;
}