#pragma once

#include <DirectXMath.h>

#include "Component.h"
#include "ModelData.h"

class ModelRenderer : public Component
{
public:
	ModelRenderer() = default;
	ModelRenderer(const char* filePath);
	~ModelRenderer() override = default;

	//-----< 関数 >-----//

	//初期化
	void Initialize(const char* filePath);
	//更新
	void Update() override;
	//描画
	void Draw() override;
	//ImGui
	void DebugGui() override;


	//アニメ―ション関連
	void UpdateAnimation();//アニメ―ションの更新
	void PlayAnimation(int animationIndex, bool loop = true);//アニメ―ションの再生
	void StopAnimation();//アニメ―ションの停止



	//-----< Getter, Setter >-----//
	void SetMaterialColor(const DirectX::XMFLOAT4 color) { materialColor = color; }

	[[nodiscard]] bool IsPlayAnimation() const { return isPlayAnimation; }
	[[nodiscard]] bool IsFinishAnimation() const { return finishAnimation; }

	//DirectX::XMFLOAT3 GetFrontVec() { return { transform.world._31,transform.world._32 ,transform.world._33 }; }

	[[nodiscard]] ModelData* GetModelResource() const { return model.get(); }
	//DirectX::XMFLOAT4X4& GetWorldTransform() { return transform.world; }

	[[nodiscard]] float GetPlayAnimTimer() const { return playAnimTimer; }
	[[nodiscard]] size_t GetPlayAnimMaxTimer() const { return model->animationClips.at(currentAnimationIndex).sequence.size(); }
	[[nodiscard]] float GetAnimationSpeed()const { return model->animationClips.at(currentAnimationIndex).samplingRate * animationSpeed; }
	void SetAnimationSpeed(const float speed) { animationSpeed = speed; }
	[[nodiscard]] int GetAnimationIndex() const { return currentAnimationIndex; }

	//[[nodiscard]] DirectX::XMFLOAT4X4 SearchBoneMatrix(std::string name) const
	//{
	//	DirectX::XMFLOAT4X4 mat{};
	//	DirectX::XMStoreFloat4x4(&mat, DirectX::XMLoadFloat4x4(&model->SearchBoneMatrix(std::move(name))) * DirectX::XMLoadFloat4x4(&transform.world));
	//	return mat;
	//}

	//シェーダーの差し替え
	void SetPixelShader(ID3D11PixelShader* newPixelShader) const { model->SetPixelShader(newPixelShader); }
	void SetVertexShader(ID3D11VertexShader* newVertexShader) const { model->SetVertexShader(newVertexShader); }

	//テクスチャの張替え
	void SetTexture(Texture newTexture) const { model->SetTexture(newTexture); }



private:
	//-----< 変数 >-----//

	std::shared_ptr<ModelData> model = nullptr;
	DirectX::XMFLOAT4 materialColor = { 1.0f,1.0f,1.0f,1.0f };

	float samplingRate = 1.0f;

	int currentAnimationIndex = 0;
	int keyframeIndex = 0;

	float animationSpeed = 1.0f;//アニメ―ションの再生速度(1.0が初期値)

	//-----< アニメ―ション補間 >-----//
	int oldAnimationIndex = 0;//アニメ―ションを切り替えるときに使う
	int oldKeyframeIndex = 0;//アニメ―ションを切り替えるときに使う
	bool interpolationAnim = false;//アニメ―ションの補間中か
	float interpolationRatio = 0.0f;//補間に使う
	float interpolationSpeed = 20.0f;//補間の速さ
	Animation::Keyframe interpolationKeyframe;

	float playAnimTimer = 0.0f;//アニメ―ションの再生時間

	bool isPlayAnimation = false;//アニメ―ションしているか
	bool isLoopAnimation = false;//アニメ―ションをループさせるか
	bool finishAnimation = false;//アニメ―ションが一周したときのみtrue

	DirectX::XMFLOAT4X4* worlds = nullptr;

};
