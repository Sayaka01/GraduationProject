
#include <Stdafx.h>
#include "ModelRenderer.h"
#include "ModelData.h"

#include "Component/Transform.h"
#include "GameObject/GameObject.h"

#include "System/ResourceManager.h"
#include "System/SystemManager.h"

using namespace DirectX;

//�R���X�g���N�^
ModelRenderer::ModelRenderer(const char* filePath)
{
	Initialize(filePath);
}

//������
void ModelRenderer::Initialize(const char* filePath)
{
	name = "ModelRenderer";

	modelData = ResourceManager::Instance().LoadModelResource(filePath);

}

//�X�V
void ModelRenderer::Update()
{
	UpdateAnimation();
}

//�`��
void ModelRenderer::Draw()
{
	Animation::Keyframe keyframe;
	if (!modelData->animationClips.empty())
	{
		//�A�j���\�V����������
		if (interpolationAnim)
			keyframe = interpolationKeyframe;
		else keyframe = modelData->animationClips.at(currentAnimationIndex).sequence.at(keyframeIndex);
	}

	//for (const ModelData::Mesh& mesh : meshes)
	for(int i=0;i<modelData->GetMeshesSize();i++)
	{
		if (!keyframe.nodes.empty())
		{
			//currentKeyframe = keyframe;
			DirectX::XMFLOAT4X4 w;
			//DirectX::XMFLOAT4X4 bt;
			const Animation::Keyframe::Node& meshNode{ keyframe.nodes.at(modelData->GetMesh(i).nodeIndex) };
			XMStoreFloat4x4(&w, XMLoadFloat4x4(&meshNode.globalTransform) * XMLoadFloat4x4(&parent->GetComponent<Transform>()->world));

			const size_t boneCount{ modelData->GetMesh(i).bindPose.bones.size() };
			//_ASSERT_EXPR(boneCount < MAX_BONES, L"The value of the 'bone_count' has exceeded MAX_BONES.");

			for (size_t boneIndex = 0; boneIndex < boneCount; ++boneIndex)
			{
				const Animation::Keyframe::Node& bone_node{ keyframe.nodes.at(modelData->GetBone(i,(int)boneIndex).nodeIndex) };

				//�U������Ƃ���HitSphere�悤�ɂԂ������ꏊ�̈ʒu���Z�o
				for (auto& b : boneData)
				{
					if (b.isCalc && modelData->GetBone(i, (int)boneIndex).name == b.boneName)
					{
						DirectX::XMFLOAT4X4 gt;
						XMStoreFloat4x4(&gt,
							XMLoadFloat4x4(&keyframe.nodes.at(modelData->GetBone(i, (int)boneIndex).nodeIndex + 1).globalTransform) *
							XMMatrixInverse(nullptr, XMLoadFloat4x4(&modelData->GetMesh(i).defaultGlobalTransform))
						);
						DirectX::XMFLOAT3 g = { gt._41,gt._42,gt._43 };
						DirectX::XMVECTOR Translation = DirectX::XMLoadFloat3(&g);
						DirectX::XMMATRIX World = DirectX::XMLoadFloat4x4(&w);
						DirectX::XMStoreFloat3(&b.position, DirectX::XMVector3TransformCoord(Translation, World));
					}
				}


			}
		}
	}

	modelData->Draw(parent->GetComponent<Transform>()->world, &keyframe, materialColor);

}

//ImGui
void ModelRenderer::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		if (!modelData->animationClips.empty())
		{
			if (ImGui::TreeNode("Animation"))
			{
				const int maxKeyframe = static_cast<int>(modelData->animationClips.at(currentAnimationIndex).sequence.size());
				ImGui::DragInt("keyframe", &keyframeIndex, 1, 0, maxKeyframe - 1);
				ImGui::DragInt("oldKeyframeIndex", &oldKeyframeIndex, 1, 0, maxKeyframe - 1);
				ImGui::DragFloat("interpolationRatio", &interpolationRatio, 0.1f, 0, 1.0f);
				const int maxAnimation = static_cast<int>(modelData->animationClips.size());
				static int animIndex = 0;
				ImGui::SliderInt("animationIndex", &animIndex, 0, maxAnimation - 1);
				ImGui::DragFloat("interpolationSpeed", &interpolationSpeed, 0.01f);
				ImGui::DragFloat("animationSpeed", &animationSpeed, 0.01f);

				ImGui::Checkbox("isLoopAnimation", &isLoopAnimation);
				ImGui::Checkbox("playAnimation", &isPlayAnimation);
				ImGui::Checkbox("interpolationAnim", &interpolationAnim);

				if (ImGui::Button("play"))
				{
					PlayAnimation(animIndex, isLoopAnimation);
				}
				if (ImGui::Button("stop"))
				{
					StopAnimation();
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

//�A�j���[�V�����֌W
void ModelRenderer::UpdateAnimation()
{
	if (!isPlayAnimation)return;

	finishAnimation = false;

	float elapsedTime = SystemManager::Instance().GetElapsedTime();

	//�A�j���\�V�������
	if (interpolationAnim)
	{
		//��Ԃ���
		interpolationRatio += interpolationSpeed * elapsedTime;
		if (interpolationRatio >= 1.0f)
		{
			interpolationRatio = 1.0f;
			interpolationAnim = false;
		}

		const Animation::Keyframe* keyframes[2] = {
			&modelData->animationClips.at(oldAnimationIndex).sequence.at(oldKeyframeIndex),
			&modelData->animationClips.at(currentAnimationIndex).sequence.at(keyframeIndex)
		};

		modelData->BlendAnimations(keyframes, interpolationRatio, interpolationKeyframe);
		modelData->UpdateGlobalTransform(interpolationKeyframe);

		if (interpolationAnim)return;
	}

	const float samplingRate = modelData->animationClips.at(currentAnimationIndex).samplingRate;
	playAnimTimer += samplingRate * animationSpeed * elapsedTime;
	const size_t maxKeyframe = modelData->animationClips.at(currentAnimationIndex).sequence.size();
	if (static_cast<size_t>(playAnimTimer) >= maxKeyframe && isPlayAnimation)
	{
		playAnimTimer = 0.0f;

		if (!isLoopAnimation)
		{
			isPlayAnimation = false;
			playAnimTimer = static_cast<float>(maxKeyframe - 1);
		}

		finishAnimation = true;
	}
	keyframeIndex = static_cast<int>(playAnimTimer);
}
void ModelRenderer::PlayAnimation(int animationIndex, bool loop)
{
	isLoopAnimation = loop;

	if (currentAnimationIndex == animationIndex && isPlayAnimation)return;

	interpolationAnim = true;
	interpolationRatio = 0.0f;

	isPlayAnimation = true;

	oldAnimationIndex = currentAnimationIndex;
	currentAnimationIndex = animationIndex;
	oldKeyframeIndex = keyframeIndex;

	playAnimTimer = 0.0f;
	keyframeIndex = 0;
	finishAnimation = false;
}
void ModelRenderer::StopAnimation()
{
	playAnimTimer = 0.0f;
	isPlayAnimation = false;
}
void ModelRenderer::AppendAnimation(const char* animationFilename, float samplingRate)
{
	modelData->AppendAnimation(animationFilename, samplingRate);
}

// ���[�V�����̈ړ��l���~
void ModelRenderer::StopMotionVelocity(const std::string callName, std::string currentAnim)
{
	if (nodeIndex.size() <= 0)
		return;

	Motion motion = nodeIndex.at(callName);

	//���݂̃L�[�t���[�����擾
	Animation::Keyframe::Node* node = nullptr;
	if(currentAnim == "")
	node=&(modelData->animationClips.at(currentAnimationIndex).sequence.at(keyframeIndex).nodes.at(motion.index/*�v�f���Ȃ���Η�O�X���[*/));

	//DirectX::XMMATRIX GT = DirectX::XMLoadFloat4x4(&node->globalTransform);
	//DirectX::XMMATRIX T = DirectX::XMLoadFloat4x4(&transform);
	//DirectX::XMMATRIX W = GT * T;
	//DirectX::XMFLOAT4X4 w;
	//DirectX::XMStoreFloat4x4(&w, W);

	//�O�񂩂獡��̃t���[���܂ł̈ړ��l�̍������v�Z
	//diffRootMotionTranslation.x = node->translation.x - cacheRootMotionTranslation.x;
	//diffRootMotionTranslation.y = node->translation.y - cacheRootMotionTranslation.y;
	//diffRootMotionTranslation.z = node->translation.z - cacheRootMotionTranslation.z;
	//diffRootMotionTranslation.x = w._41 - cacheRootMotionTranslation.x;
	//diffRootMotionTranslation.y = w._42 - cacheRootMotionTranslation.y;
	//diffRootMotionTranslation.z = w._43 - cacheRootMotionTranslation.z;

	//����̈ړ��l��ۑ�
	//cacheRootMotionTranslation = node->translation;
	//cacheRootMotionTranslation = DirectX::XMFLOAT3(w._41, w._42, w._43);

	
	//�ʒu����̈ʒu�֐ݒ�
	node->translation = motion.defaultPostion;
	//node->translation.x = 0;
	//node->translation.y = 0;
	//node->translation.y = firstPoseTransform.y;
	//node->translation.z = 0;
	modelData->UpdateGlobalTransform(modelData->animationClips.at(currentAnimationIndex).sequence.at(keyframeIndex));

#ifdef _DEBUG
	ImGui::Begin("node");
	ImGui::SliderFloat3("translate", &node->translation.x, -20, 20);
	ImGui::End();
#endif


	//isRootMotionCalc = false;
	return;

}

void ModelRenderer::StopMotionXZVelocity(const std::string callName, std::string currentAnim)
{
	if (nodeIndex.size() <= 0)
		return;

	Motion motion = nodeIndex.at(callName);

	//���݂̃L�[�t���[�����擾
	Animation::Keyframe::Node* node = nullptr;
	if (currentAnim == "")
		node = &(modelData->animationClips.at(currentAnimationIndex).sequence.at(keyframeIndex).nodes.at(motion.index/*�v�f���Ȃ���Η�O�X���[*/));

	//�ʒu����̈ʒu�֐ݒ�
	node->translation.x = motion.defaultPostion.x;
	node->translation.z = motion.defaultPostion.z;

	modelData->UpdateGlobalTransform(modelData->animationClips.at(currentAnimationIndex).sequence.at(keyframeIndex));

	return;
}

//���O�ƈ�v����node�̔ԍ���ۑ�
void ModelRenderer::StoreNodeIndex(std::string callName,std::string BaseAnimName,std::string rigName)
{
	Animation* defaultAnimation{nullptr};
	
	//��ʒu���擾����A�j���[�V������T��
	for (Animation& anim : modelData->animationClips)
	{
		if (anim.name == BaseAnimName)
		{
			defaultAnimation = &anim;
		}
	}
	//���߂̃L�[�t���[�����擾
	Animation::Keyframe keyframe = defaultAnimation->sequence.at(0);
	int count = 0;
	for (Animation::Keyframe::Node& node : keyframe.nodes)
	{
		if (node.name != rigName)
		{
			count++;
			continue;
		}
		nodeIndex[callName].index = count;
		nodeIndex[callName].defaultPostion = node.translation;
		return;
	}

}

