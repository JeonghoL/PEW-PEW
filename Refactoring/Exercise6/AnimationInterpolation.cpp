#include "pch.h"
#include "AnimationInterpolation.h"

glm::vec3 InterpolatePosition(float AnimationTime, const aiNodeAnim* pNodeAnim) {
	if (pNodeAnim->mNumPositionKeys == 1) {
		return glm::vec3(pNodeAnim->mPositionKeys[0].mValue.x,
			pNodeAnim->mPositionKeys[0].mValue.y,
			pNodeAnim->mPositionKeys[0].mValue.z);
	}

	const aiVector3D& basePos = pNodeAnim->mPositionKeys[0].mValue;
	glm::vec3 basePosition(basePos.x, basePos.y, basePos.z);
	float baseLength = glm::length(basePosition);

	unsigned int PositionIndex = 0;
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			PositionIndex = i;
			break;
		}
	}

	unsigned int NextPositionIndex = (PositionIndex + 1);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime -
		pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;

	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	glm::vec3 currentPos = glm::vec3(
		Start.x + Factor * (End.x - Start.x),
		Start.y + Factor * (End.y - Start.y),
		Start.z + Factor * (End.z - Start.z)
	);

	float currentLength = glm::length(currentPos);
	if (currentLength > 0.0f) {
		currentPos = glm::normalize(currentPos) * baseLength;
	}

	return currentPos;
}

glm::quat InterpolateRotation(float AnimationTime, const aiNodeAnim* pNodeAnim) {
	if (pNodeAnim->mNumRotationKeys == 1) {
		const aiQuaternion& rot = pNodeAnim->mRotationKeys[0].mValue;
		return glm::quat(rot.w, rot.x, rot.y, rot.z);
	}

	unsigned int currentFrame = 0;
	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime < pNodeAnim->mRotationKeys[i + 1].mTime) {
			currentFrame = i;
			break;
		}
	}

	unsigned int nextFrame = (currentFrame + 1) % pNodeAnim->mNumRotationKeys;

	float frameStart = pNodeAnim->mRotationKeys[currentFrame].mTime;
	float frameEnd = pNodeAnim->mRotationKeys[nextFrame].mTime;
	float factor = (AnimationTime - frameStart) / (frameEnd - frameStart);

	const aiQuaternion& start = pNodeAnim->mRotationKeys[currentFrame].mValue;
	const aiQuaternion& end = pNodeAnim->mRotationKeys[nextFrame].mValue;

	float dot = start.x * end.x + start.y * end.y + start.z * end.z + start.w * end.w;

	aiQuaternion correctedEnd = dot < 0 ? aiQuaternion(-end.w, -end.x, -end.y, -end.z) : end;


	aiQuaternion out;
	aiQuaternion::Interpolate(out, start, end, factor);
	return glm::quat(out.w, out.x, out.y, out.z);
}

glm::vec3 InterpolateScale(float AnimationTime, const aiNodeAnim* pNodeAnim) {
	if (pNodeAnim->mNumScalingKeys == 1) {
		return glm::vec3(pNodeAnim->mScalingKeys[0].mValue.x,
			pNodeAnim->mScalingKeys[0].mValue.y,
			pNodeAnim->mScalingKeys[0].mValue.z);
	}

	unsigned int ScalingIndex = 0;
	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			ScalingIndex = i;
			break;
		}
	}

	unsigned int NextScalingIndex = (ScalingIndex + 1);

	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime -
		pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;


	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;

	return glm::vec3(Start.x + Factor * Delta.x,
		Start.y + Factor * Delta.y,
		Start.z + Factor * Delta.z);
}

glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from) {
	glm::mat4 to;
	to[0][0] = from.a1; to[0][1] = from.b1; to[0][2] = from.c1; to[0][3] = from.d1;
	to[1][0] = from.a2; to[1][1] = from.b2; to[1][2] = from.c2; to[1][3] = from.d2;
	to[2][0] = from.a3; to[2][1] = from.b3; to[2][2] = from.c3; to[2][3] = from.d3;
	to[3][0] = from.a4; to[3][1] = from.b4; to[3][2] = from.c4; to[3][3] = from.d4;
	return to;
}

const aiNodeAnim* FindNodeAnim(const aiAnimation* animation, const std::string& nodeName) {
	if (!animation) {
		return nullptr;
	}

	for (unsigned int i = 0; i < animation->mNumChannels; i++) {
		const aiNodeAnim* nodeAnim = animation->mChannels[i];
		if (std::string(nodeAnim->mNodeName.data) == nodeName) {
			return nodeAnim;
		}
	}
	return nullptr;
}