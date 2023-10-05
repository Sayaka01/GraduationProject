
#include <Stdafx.h>
#include "ModelRenderer.h"

#include "Component/Transform.h"

#include "Manager/ResourceManager.h"
#include "Manager/SystemManager.h"

//コンストラクタ
ModelRenderer::ModelRenderer(const char* filePath)
{
	Initialize(filePath);
}

//初期化
void ModelRenderer::Initialize(const char* filePath)
{
	name = "ModelRenderer";

	model = ResourceManager::Instance().LoadModelResource(filePath);

}

//更新
void ModelRenderer::Update()
{
	UpdateAnimation();
}

//描画
void ModelRenderer::Draw()
{
	Animation::Keyframe keyframe;
	if (!model->animationClips.empty())
	{
		//アニメ―ションがある
		if (interpolationAnim)keyframe = interpolationKeyframe;
		else keyframe = model->animationClips.at(currentAnimationIndex).sequence.at(keyframeIndex);
	}

	model->Draw(parent->GetComponent<Transform>()->world, &keyframe, materialColor);

}

//ImGui
void ModelRenderer::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{

		ImGui::TreePop();
	}
}

//アニメーション関係
void ModelRenderer::UpdateAnimation()
{
	if (!isPlayAnimation)return;

	finishAnimation = false;

	float elapsedTime = SystemManager::Instance().GetElapsedTime();

	//アニメ―ション補間
	if (interpolationAnim)
	{
		//補間する
		interpolationRatio += interpolationSpeed * elapsedTime;
		if (interpolationRatio >= 1.0f)
		{
			interpolationRatio = 0.0f;
			interpolationAnim = false;
		}

		const Animation::Keyframe* keyframes[2] = {
			&model->animationClips.at(oldAnimationIndex).sequence.at(oldKeyframeIndex),
			&model->animationClips.at(currentAnimationIndex).sequence.at(keyframeIndex)
		};

		model->BlendAnimations(keyframes, interpolationRatio, interpolationKeyframe);
		model->UpdateGlobalTransform(interpolationKeyframe);

		if (interpolationAnim)return;
	}

	const float samplingRate = model->animationClips.at(currentAnimationIndex).samplingRate;
	playAnimTimer += samplingRate * animationSpeed * elapsedTime;
	const size_t maxKeyframe = model->animationClips.at(currentAnimationIndex).sequence.size();
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

