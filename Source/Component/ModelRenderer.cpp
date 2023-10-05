
#include <Stdafx.h>
#include "ModelRenderer.h"

#include "Component/Transform.h"
#include "GameObject/GameObject.h"

#include "System/ResourceManager.h"
#include "System/SystemManager.h"

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
		if (interpolationAnim)keyframe = interpolationKeyframe;
		else keyframe = modelData->animationClips.at(currentAnimationIndex).sequence.at(keyframeIndex);
	}
	modelData->Draw(parent->GetComponent<Transform>()->world, &keyframe, materialColor);

}

//ImGui
void ModelRenderer::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{

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
			interpolationRatio = 0.0f;
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
			//isPlayAnimation = false;
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

	if (isPlayAnimation)interpolationAnim = true;
	isPlayAnimation = true;

	oldAnimationIndex = currentAnimationIndex;
	currentAnimationIndex = animationIndex;
	oldKeyframeIndex = keyframeIndex;

	playAnimTimer = 0.0f;
	keyframeIndex = 0;
}
void ModelRenderer::StopAnimation()
{
	playAnimTimer = 0.0f;
	isPlayAnimation = false;
}

