
#include <Stdafx.h>
#include "ModelRenderer.h"

#include "Component/Transform.h"
#include "GameObject/GameObject.h"

#include "System/ResourceManager.h"
#include "System/SystemManager.h"

//コンストラクタ
ModelRenderer::ModelRenderer(const char* filePath)
{
	Initialize(filePath);
}

//初期化
void ModelRenderer::Initialize(const char* filePath)
{
	name = "ModelRenderer";

	modelData = ResourceManager::Instance().LoadModelResource(filePath);

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
	if (!modelData->animationClips.empty())
	{
		//アニメ―ションがある
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
void ModelRenderer::AppendAnimation(const char* animationFilename, float samplingRate)
{
	modelData->AppendAnimation(animationFilename, samplingRate);
}

