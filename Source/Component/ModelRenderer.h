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

	//�V�F�[�_�[�̍����ւ�
	void SetPixelShader(ID3D11PixelShader* newPixelShader) const { model->SetPixelShader(newPixelShader); }
	void SetVertexShader(ID3D11VertexShader* newVertexShader) const { model->SetVertexShader(newVertexShader); }

	//�e�N�X�`���̒��ւ�
	void SetTexture(Texture newTexture) const { model->SetTexture(newTexture); }



private:
	//-----< �ϐ� >-----//

	std::shared_ptr<ModelData> model = nullptr;
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

};
