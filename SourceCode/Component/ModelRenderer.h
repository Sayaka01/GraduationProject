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

	//-----< �֐� >-----//

	//������
	void Initialize(const char* filePath);
	//�X�V
	void Update() override;
	//�`��
	void Draw() override;
	//ImGui
	void DebugGui() override;


	//�A�j���\�V�����֘A
	void UpdateAnimation();//�A�j���\�V�����̍X�V
	void PlayAnimation(int animationIndex, bool loop = true);//�A�j���\�V�����̍Đ�
	void StopAnimation();//�A�j���\�V�����̒�~
	void AppendAnimation(const char* animationFilename, float samplingRate = 0.0f);//�A�j���[�V�����̒ǉ�


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

	//�V�F�[�_�[�̍����ւ�
	void SetPixelShader(ID3D11PixelShader* newPixelShader) const { modelData->SetPixelShader(newPixelShader); }
	void SetVertexShader(ID3D11VertexShader* newVertexShader) const { modelData->SetVertexShader(newVertexShader); }

	//�e�N�X�`���̒��ւ�
	void SetTexture(Texture newTexture) const { modelData->SetTexture(newTexture); }

	// ���[�V�����̈ړ��l���~
	void StopMotionVelocity(const std::string nodeName, std::string currentAnim = "");

	//���O�ƈ�v����node�̔ԍ���ۑ�
	void StoreNodeIndex(std::string callName, std::string BaseAnimName, std::string rigName);

private:
	//-----< �ϐ� >-----//

	std::shared_ptr<ModelData> modelData = nullptr;
	DirectX::XMFLOAT4 materialColor = { 1.0f,1.0f,1.0f,1.0f };

	float samplingRate = 1.0f;

	int currentAnimationIndex = 0;
	int keyframeIndex = 0;

	float animationSpeed = 1.0f;//�A�j���\�V�����̍Đ����x(1.0�������l)

	//-----< �A�j���\�V������� >-----//
	int oldAnimationIndex = 0;//�A�j���\�V������؂�ւ���Ƃ��Ɏg��
	int oldKeyframeIndex = 0;//�A�j���\�V������؂�ւ���Ƃ��Ɏg��
	bool interpolationAnim = false;//�A�j���\�V�����̕�Ԓ���
	float interpolationRatio = 0.0f;//��ԂɎg��
	float interpolationSpeed = 20.0f;//��Ԃ̑���
	Animation::Keyframe interpolationKeyframe;

	float playAnimTimer = 0.0f;//�A�j���\�V�����̍Đ�����

	bool isPlayAnimation = false;//�A�j���\�V�������Ă��邩
	bool isLoopAnimation = false;//�A�j���\�V���������[�v�����邩
	bool finishAnimation = false;//�A�j���\�V��������������Ƃ��̂�true

	DirectX::XMFLOAT4X4* worlds = nullptr;

	struct Motion
	{
		int index = -1;
		std::string name;
		std::string rigName;
		DirectX::SimpleMath::Vector3 defaultPostion{ 0,0,0 };//��̍��̈ʒu
	};

	std::map<std::string, Motion> nodeIndex;//string(�m�[�h�̖��O),int(index�̕ۑ�)
public:
	//����̍��̈ʒu���擾���邽�߂̍\����
	struct BonePositionData
	{
	public:
		// ---< �֐� >---
		// name:�Ăяo���ۂ̖��O�@boneName:���̖{���̖��O
		BonePositionData(std::string name, std::string boneName) :name(name), boneName(boneName) {}
		void SetIsCalc(bool b) { isCalc = b; }

		// ---< �ϐ� >---

		bool isCalc{ false };//�v�Z���邩�ǂ���
		std::string name;//�Ăяo���ۂ̖��O
		std::string boneName;//���̖��O
		DirectX::XMFLOAT3 position{};//���̃��[���h���W
	};

	// ���̈ʒu�f�[�^���i�[
	std::vector<BonePositionData> boneData;

	// ���O����ʒu���擾(name:�Ăяo���ۂ̖��O)
	DirectX::XMFLOAT3 GetBonePositionFromName(std::string name)
	{
		for (auto& bone : boneData)
		{
			if (bone.name == name)
				return bone.position;
		}

		//�擾���������̈ʒu�̖��O���o�^����Ă��܂���
		return  { 0.0f,0.0f,0.0f };
	}

	//�擾���������̏���o�^
	//name:�Ăяo���p�̖��O�AboneName:���f���̍��̖��O
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
