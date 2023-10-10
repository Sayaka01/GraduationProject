
#include "Stdafx.h"

#include  "Camera.h"

#include "GameObject/GameObject.h"
#include "System/Common.h"

#include "Component/Transform.h"
#include "System/SystemManager.h"
#include "System/UserFunction.h"

//èâä˙âª
void Camera::Initialize()
{
	name = "Camera";

	cameraData = std::make_unique<CameraData>();
	constexpr float aspect = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
	cameraData->SetPerspectiveFov(FovRadY, aspect, nearZ, farZ);

	cameraData->SetAngle({ DirectX::XMConvertToRadians(10),0.0f,0.0f });

	cameraData->Initialize();

}

//çXêV
void Camera::Update()
{
	MoveCameraAngle();

	Tracking();

	cameraData->Update();
}

//ï`âÊ
void Camera::Draw()
{
	UpdateBuffer();
}

//ImGui
void Camera::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		cameraData->DebugGui();
		ImGui::TreePop();
	}
}

void Camera::UpdateBuffer() const
{
	cameraData->UpdateBuffer(parent->GetComponent<Transform>()->pos);
}

void Camera::MoveCameraAngle() const
{
	const GamePad& gamepad = SystemManager::Instance().GetGamePad();

	const float rx = gamepad.GetAxisRX();
	const float ry = gamepad.GetAxisRY();
	DirectX::XMFLOAT3 angle = cameraData->GetAngle();

	const float elapsedTime = SystemManager::Instance().GetElapsedTime();

	angle.x -= ry * rotationSpeed * elapsedTime;
	angle.y += rx * rotationSpeed * elapsedTime;


	angle.x = Clamp(angle.x, DirectX::XMConvertToRadians(minDegree), DirectX::XMConvertToRadians(maxDegree));
	angle.y = CirculateRadian(angle.y);

	cameraData->SetAngle(angle);
}

void Camera::Tracking() const
{
	if (!tracking)return;

	cameraData->SetTargetCorrection(targetCorrection);
	cameraData->SetTarget(targetObj->GetComponent<Transform>()->pos);
}
