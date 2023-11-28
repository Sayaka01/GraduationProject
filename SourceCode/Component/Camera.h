#pragma once

#pragma once

#include <DirectXMath.h>
#include <SimpleMath.h>

#include "Component.h"

#include "System/ConstantBuffer.h"

#include "CameraData.h"

class GameObject;

class Camera : public Component
{
public:
	Camera() = default;
	~Camera() override = default;

	//-----< �֐� >-----//

	//������
	void Initialize() override;
	//�X�V
	void Update() override;
	//�`��
	void Draw3D() override;
	//ImGui
	void DebugGui() override;

	//�萔�o�b�t�@�̍X�V
	void UpdateBuffer() const;

	void MoveCameraAngle();

	void Tracking() const;

	//---- �R���g���[���[��R�X�e�B�b�N��X���̈ړ��l���擾 ----
	//�i�߂�l�F���͂�����Ƃ�true�j
	bool SetAngleToX();

	//---- �R���g���[���[��R�X�e�B�b�N��Y���̈ړ��l���擾 ----
	//�i�߂�l�F���͂�����Ƃ�true�j
	bool SetAngleToY();

	// �v���C���[�̃X�e�[�g�ɂ���ăJ�����̋����𓮂���
	void ChangeRange();

	bool LockOnTarget(DirectX::XMFLOAT3 targetPosition, float rate);

	//void ResetDefault();

	//---- �J�����̈ʒu�ƒ����_���Z�o���ݒ� ----
	//�i�߂�l�F���͂�����Ƃ�true�j
	void SetEyePoint();

	//-----< Getter, Setter >-----//

	[[nodiscard]] DirectX::XMFLOAT4X4 GetView() const { return cameraData->GetView(); }
	[[nodiscard]] DirectX::XMFLOAT4X4 GetProjection() const { return cameraData->GetProjection(); }
	[[nodiscard]] DirectX::XMFLOAT4X4 GetViewProjection() const { return cameraData->GetViewProjection(); }

	void SetTarget(const DirectX::XMFLOAT3 target) const { cameraData->SetTarget(target); }
	[[nodiscard]] DirectX::XMFLOAT3 GetTarget() const { return cameraData->GetTarget(); }
	
	[[nodiscard]] DirectX::XMFLOAT3 GetEye() const { return cameraData->GetEye(); }

	void SetTargetObj(GameObject* obj) 
	{ 
		if (obj) 
		{
			targetObj = obj;
			tracking = true;
		}
	}
	void SetIsTracking(bool isTracking) { tracking = isTracking; }

	//model��ǔ�����Ƃ���model�̌��_����̕␳�l���Z�b�g����
	void SetTargetCorrection(DirectX::XMFLOAT3 correction) { targetCorrection = correction; }

	[[nodiscard]] DirectX::XMFLOAT3 GetCameraFront() const { return cameraData->GetCameraFront(); }
	[[nodiscard]] DirectX::XMFLOAT3 GetCameraUp() const { return cameraData->GetCameraUp(); }
	[[nodiscard]] DirectX::XMFLOAT3 GetCameraRight() const { return cameraData->GetCameraRight(); }


private:
	//-----< �ϐ� >-----//

	std::unique_ptr<CameraData> cameraData = nullptr;

	float rotationSpeed = 80;
	//float rotationSpeed = DirectX::XMConvertToRadians(90);

	GameObject* targetObj = nullptr;
	bool tracking = false;

	//model��ǔ�����Ƃ���model�̌��_����̕␳�l
	DirectX::XMFLOAT3 targetCorrection = { 0.0f,10.0f,0.0f };

	float maxDegree = 85.0f;
	float minDegree = -12.5f;
	

	float FovRadY = DirectX::XMConvertToRadians(45.0f);//����̍L��
	float nearZ = 0.1f;//�J�����̍ŋߋ���
	float farZ = 2000.0f;//�J�����̍ŉ�����

	float verticalDegree{ 0.0f };//�c�����̉�]���x
	float horizonDegree{ 0.0f };//�������̉�]���x
	//DirectX::XMFLOAT3 eyeVector{ 0,0,0 };//target����J�����ւ̃x�N�g��

	DirectX::SimpleMath::Vector3 fetchFront{};//�O�����֌����Ƃ��̑O������ۑ�(�Œ肵�Ȃ���Έꐶ�܂�葱���邽��)
	bool lockingOn{ false };//���b�N�I������
	float rateInCamera{ 0.01f };   //���b�N�I���J�����Ȃǂ̃C�[�W���O���[�g
	float lockonDegree{ 0.0f };
};