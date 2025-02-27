#pragma once

struct VertexBoneData {
	unsigned int BoneIDs[MAX_NUM_BONES_PER_VERTEX];
	float Weights[MAX_NUM_BONES_PER_VERTEX];

	VertexBoneData() {
		for (int i = 0; i < MAX_NUM_BONES_PER_VERTEX; i++) {
			BoneIDs[i] = -1;
			Weights[i] = 0.0f;
		}
	}

	void AddBoneData(unsigned int BoneID, float Weight) {
		const float MIN_WEIGHT = 0.000001f;
		if (Weight < MIN_WEIGHT)
			return;

		int emptySlot = -1;
		for (int i = 0; i < MAX_NUM_BONES_PER_VERTEX; i++) {
			if (Weights[i] == 0.0f) {
				emptySlot = i;
				break;
			}
		}

		if (emptySlot != -1) {
			BoneIDs[emptySlot] = BoneID;
			Weights[emptySlot] = Weight;
		}
		else {
			float minWeight = Weights[0];
			int minWeightIndex = 0;
			for (int i = 1; i < MAX_NUM_BONES_PER_VERTEX; i++) {
				if (Weights[i] < minWeight) {
					minWeight = Weights[i];
					minWeightIndex = i;
				}
			}
			if (Weight > minWeight) {
				BoneIDs[minWeightIndex] = BoneID;
				Weights[minWeightIndex] = Weight;
			}
		}
	}

	void Normalize() {
		float total = 0.0f;
		int validBones = 0;

		for (int i = 0; i < MAX_NUM_BONES_PER_VERTEX; i++) {
			if (BoneIDs[i] != -1 && Weights[i] > 0.0f) {
				total += Weights[i];
				validBones++;
			}
		}

		if (total > 0.0f) {
			for (int i = 0; i < MAX_NUM_BONES_PER_VERTEX; i++) {
				if (BoneIDs[i] != -1) {
					Weights[i] /= total;
				}
			}
		}
		else {
			BoneIDs[0] = 0;
			Weights[0] = 1.0f;

			for (int i = 1; i < MAX_NUM_BONES_PER_VERTEX; i++) {
				BoneIDs[i] = -1;
				Weights[i] = 0.0f;
			}
		}

		float finalTotal = 0.0f;
		for (int i = 0; i < MAX_NUM_BONES_PER_VERTEX; i++) {
			if (BoneIDs[i] != -1) {
				finalTotal += Weights[i];
			}
		}

		if (abs(finalTotal - 1.0f) > 0.0001f) {
			for (int i = 0; i < MAX_NUM_BONES_PER_VERTEX; i++) {
				if (BoneIDs[i] != -1) {
					Weights[i] += (1.0f - finalTotal);
					break;
				}
			}
		}
	}
};

extern std::vector<VertexBoneData> vertex_to_bones;
extern std::vector<int> mesh_base_vertex;
extern std::map<std::string, unsigned int> m_BoneNameToIndexMap;

const unsigned int MAX_BONES = 100;

void NormalizeBoneWeights();
void CheckWeights();
void parse_single_bone(int mesh_index, const aiBone* pBone);
void parsh_mesh_bones(int mesh_index, const aiMesh* pMesh);
void parse_meshes(const aiScene* pScene);
void parse_scene(const aiScene* pScene);
int get_bone_id(const aiBone* pBone);