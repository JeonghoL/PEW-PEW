#pragma once

glm::vec3 InterpolatePosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
glm::quat InterpolateRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
glm::vec3 InterpolateScale(float AnimationTime, const aiNodeAnim* pNodeAnim);
glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from);
const aiNodeAnim* FindNodeAnim(const aiAnimation* animation, const std::string& nodeName);
