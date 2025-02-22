#pragma once
#include "AnimationInterpolation.h"

struct AnimInfo {
    float Duration;
    float TicksPerSecond;
    float CurrentTime;
    const aiAnimation* animation;
    const aiNode* rootNode;
    bool isPlaying;

    AnimInfo() {
        Duration = 0.0f;
        TicksPerSecond = 0.0f;
        CurrentTime = 0.0f;
        animation = nullptr;
        rootNode = nullptr;
        isPlaying = false;
    }
};

vector<unique_ptr<Assimp::Importer>> playerAnimationImporters, enemy1AnimationImporters, enemy2AnimationImporters, enemy3AnimationImporters;

struct AnimationLibrary {
    map<std::string, AnimInfo> animations;
    std::string currentAnimationName;

    void loadAnimation(const std::string& name, const std::string& filename,
        vector<unique_ptr<Assimp::Importer>>& importers) {
        importers.push_back(make_unique<Assimp::Importer>());
        const aiScene* scene = importers.back()->ReadFile(filename,
            aiProcess_Triangulate | aiProcess_FlipUVs);

        if (scene->mAnimations[0]->mNumChannels > 0) {
            aiNodeAnim* channel = scene->mAnimations[0]->mChannels[0];
        }

        if (scene && scene->HasAnimations()) {
            AnimInfo animInfo;
            animInfo.animation = scene->mAnimations[0];
            animInfo.Duration = scene->mAnimations[0]->mDuration;
            animInfo.TicksPerSecond = (scene->mAnimations[0]->mTicksPerSecond != 0) ?
                scene->mAnimations[0]->mTicksPerSecond : 24.0f;
            animInfo.rootNode = scene->mRootNode;

            for (unsigned int i = 0; i < scene->mAnimations[0]->mNumChannels; i++) {
                std::string animBoneName = scene->mAnimations[0]->mChannels[i]->mNodeName.data;
                if (m_BoneNameToIndexMap.find(animBoneName) == m_BoneNameToIndexMap.end()) {
                }
            }

            animations[name] = animInfo;
        }
        else
            cout << "Unloaded animation: " << name << endl;
    }

    void changeAnimation(const std::string& name, AnimInfo& currentAnim) {
        auto it = animations.find(name);
        if (it != animations.end()) {
            currentAnim = it->second;
            currentAnim.CurrentTime = 0.0f;
            currentAnim.isPlaying = true;
            currentAnimationName = name;
        }
    }

    const std::string& getcurrentAnimation() const {
        return currentAnimationName;
    }
};

AnimationLibrary playeranimLib, enemyanimLib[3][9];

void CalculateNodeTransform(int j, vector<BoneInfo>& BoneInfoName, const aiNode* pNode, const glm::mat4& ParentTransform, const AnimInfo& currentAnim) {
    std::string NodeName(pNode->mName.data);
    glm::mat4 NodeTransformation = aiMatrix4x4ToGlm(pNode->mTransformation);
    const aiNodeAnim* pNodeAnim = FindNodeAnim(currentAnim.animation, NodeName);

    if (pNodeAnim) {
        glm::vec3 Scaling = InterpolateScale(currentAnim.CurrentTime, pNodeAnim);
        glm::mat4 ScalingM = glm::scale(glm::mat4(1.0f), Scaling);

        glm::quat RotationQ = InterpolateRotation(currentAnim.CurrentTime, pNodeAnim);
        glm::mat4 RotationM = glm::mat4_cast(RotationQ);

        glm::vec3 Translation = InterpolatePosition(currentAnim.CurrentTime, pNodeAnim);
        glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), Translation);

        NodeTransformation = TranslationM * RotationM * ScalingM;
    }

    glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

    if (m_BoneNameToIndexMap.find(NodeName) != m_BoneNameToIndexMap.end()) {
        unsigned int BoneIndex = m_BoneNameToIndexMap[NodeName];
        BoneInfoName[BoneIndex].LocalTransform = NodeTransformation;
        BoneInfoName[BoneIndex].FinalTransformation = m_GlobalInverseTransform[j] * GlobalTransformation * BoneInfoName[BoneIndex].BoneOffset;
    }

    for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
        CalculateNodeTransform(j, BoneInfoName, pNode->mChildren[i], GlobalTransformation, currentAnim);
    }
}

void UpdateAnimation(int j, vector<BoneInfo>& BoneInfoName, float deltaTime, AnimInfo& currentAnim) {
    if (!currentAnim.animation || !currentAnim.isPlaying) {
        return;
    }
    currentAnim.CurrentTime = fmod(currentAnim.CurrentTime + deltaTime * currentAnim.TicksPerSecond, currentAnim.Duration);
    CalculateNodeTransform(j, BoneInfoName, currentAnim.rootNode, glm::mat4(1.0f), currentAnim);
}

void setupBoneTransforms(vector<BoneInfo>& BoneInfoName, GLuint shadername) {
    for (unsigned int i = 0; i < BoneInfoName.size(); i++) {
        std::string uniformName = "gBones[" + std::to_string(i) + "]";
        GLint boneTransformLoc = glGetUniformLocation(shadername, uniformName.c_str());
        if (boneTransformLoc != -1) {
            glUniformMatrix4fv(boneTransformLoc, 1, GL_FALSE,
                glm::value_ptr(BoneInfoName[i].FinalTransformation));
        }
    }
}