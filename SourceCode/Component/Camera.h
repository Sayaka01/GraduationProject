#pragma once

#pragma once

#include <DirectXMath.h>

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
	void Draw() override;
	//ImGui
	void DebugGui() override;

	//�萔�o�b�t�@�̍X�V
	void UpdateBuffer() const;

	void MoveCameraAngle() const;

	void Tracking() const;


	//-----< Getter, Setter >-----//

	[[nodiscard]] DirectX::XMFLOAT4X4 GetView() const { return cameraData->GetView(); }
	[[nodiscard]] DirectX::XMFLOAT4X4 GetProjection() const { return cameraData->GetProjection(); }
	[[nodiscard]] DirectX::XMFLOAT4X4 GetViewProjection() const { return cameraData->GetViewProjection(); }

	void SetTarget(const DirectX::XMFLOAT3 target) const { cameraData->SetTarget(target); }
	[[nodiscard]] DirectX::XMFLOAT3 GetTarget() const { return cameraData->GetTarget(); }

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

	float rotationSpeed = DirectX::XMConvertToRadians(90);

	GameObject* targetObj = nullptr;
	bool tracking = false;

	//model��ǔ�����Ƃ���model�̌��_����̕␳�l
	DirectX::XMFLOAT3 targetCorrection = { 0.0f,10.0f,0.0f };

	float maxDegree = 85.0f;
	float minDegree = -12.5f;
	

	float FovRadY = DirectX::XMConvertToRadians(45.0f);//����̍L��
	float nearZ = 0.1f;//�J�����̍ŋߋ���
	float farZ = 2000.0f;//�J�����̍ŉ�����

};