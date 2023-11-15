#pragma once

#include <DirectXMath.h>
#include <SimpleMath.h>

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
	void AppendAnimation(const char* animationFilename, float samplingRate = 0.0f);//アニメーションの追加


	//-----< Getter, Setter >-----//
	void SetMaterialColor(const DirectX::XMFLOAT4 color) { materialColor = color; }

	[[nodiscard]] bool IsPlayAnimation() const { return isPlayAnimation; }
	[[nodiscard]] bool IsFinishAnimation() const { return finishAnimation; }

	//DirectX::XMFLOAT3 GetFrontVec() { return { transform.world._31,transform.world._32 ,transform.world._33 }; }

	[[nodiscard]] ModelData* GetModelResource() const { return modelData.get(); }
	//DirectX::XMFLOAT4X4& GetWorldTransform() { return transform.world; }

	[[nodiscard]] float GetPlayAnimTimer() const { return playAnimTimer; }
	[[nodiscard]] size_t GetPlayAnimMaxTimer() const { return modelData->animationClips.at(currentAnimationIndex).sequence.size(); }
	[[nodiscard]] float GetAnimationSpeed()const { return modelData->animationClips.at(currentAnimationIndex).samplingRate * animationSpeed; }
	void SetAnimationSpeed(const float speed) { animationSpeed = speed; }
	[[nodiscard]] int GetAnimationIndex() const { return currentAnimationIndex; }

	//[[nodiscard]] DirectX::XMFLOAT4X4 SearchBoneMatrix(std::string name) const
	//{
	//	DirectX::XMFLOAT4X4 mat{};
	//	DirectX::XMStoreFloat4x4(&mat, DirectX::XMLoadFloat4x4(&modelData->SearchBoneMatrix(std::move(name))) * DirectX::XMLoadFloat4x4(&transform.world));
	//	return mat;
	//}

	//シェーダーの差し替え
	void SetPixelShader(ID3D11PixelShader* newPixelShader) const { modelData->SetPixelShader(newPixelShader); }
	void SetVertexShader(ID3D11VertexShader* newVertexShader) const { modelData->SetVertexShader(newVertexShader); }

	//テクスチャの張替え
	void SetTexture(Texture newTexture) const { modelData->SetTexture(newTexture); }

	// モーションの移動値を停止
	void StopMotionVelocity(const std::string nodeName, std::string currentAnim = "");

	//名前と一致するnodeの番号を保存
	void StoreNodeIndex(std::string callName, std::string BaseAnimName, std::string rigName);

private:
	//-----< 変数 >-----//

	std::shared_ptr<ModelData> modelData = nullptr;
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

	struct Motion
	{
		int index = -1;
		std::string name;
		std::string rigName;
		DirectX::SimpleMath::Vector3 defaultPostion{ 0,0,0 };//基準の腰の位置
	};

	std::map<std::string, Motion> nodeIndex;//string(ノードの名前),int(indexの保存)
public:
	//特定の骨の位置を取得するための構造体
	struct BonePositionData
	{
	public:
		// ---< 関数 >---
		// name:呼び出す際の名前　boneName:骨の本来の名前
		BonePositionData(std::string name, std::string boneName) :name(name), boneName(boneName) {}
		void SetIsCalc(bool b) { isCalc = b; }

		// ---< 変数 >---

		bool isCalc{ false };//計算するかどうか
		std::string name;//呼び出す際の名前
		std::string boneName;//骨の名前
		DirectX::XMFLOAT3 position{};//骨のワールド座標
	};

	// 骨の位置データを格納
	std::vector<BonePositionData> boneData;

	// 名前から位置を取得(name:呼び出す際の名前)
	DirectX::XMFLOAT3 GetBonePositionFromName(std::string name)
	{
		for (auto& bone : boneData)
		{
			if (bone.name == name)
				return bone.position;
		}

		//取得したい骨の位置の名前が登録されていません
		return  { 0.0f,0.0f,0.0f };
	}

	//取得したい骨の情報を登録
	//name:呼び出し用の名前、boneName:モデルの骨の名前
	void AddBonePositionData(std::string name, std::string boneName)
	{
		boneData.emplace_back(name, boneName);
	}

	BonePositionData* GetBoneData(std::string name)
	{
		for (auto& bone : boneData)
		{
			if (bone.name == name)
				return &bone;
		}
		return nullptr;
	}
};
