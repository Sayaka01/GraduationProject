#include "Stdafx.h"
#include "CameraData.h"

#include "System/Common.h"
#include "System/UserFunction.h"

#include "System/SystemManager.h"

CameraData::CameraData()
{
	constantBuffer.Initialize(SystemManager::Instance().GetDevice(), &cameraConstants);
}

CameraData::~CameraData()
{

}

void CameraData::Initialize()
{
	//カメラの回転値を回転行列に変換
	const DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//回転行列から前方向ベクトルを取り出す
	const DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front{};
	DirectX::XMStoreFloat3(&front, Front);

	DirectX::XMFLOAT3 corEye{};

	//各視点のカメラ位置（eye）と注視点（target）を求める
	corTarget.x = target.x + targetCorrection.x * front.x;
	corTarget.y = target.y + targetCorrection.y;
	corTarget.z = target.z + targetCorrection.z * front.z;
	corEye = corTarget - front * range;
	
	SetLookAt(corEye, corTarget, { 0.0f,1,0.0001f });

}

void CameraData::Update()
{
	//カメラの回転値を回転行列に変換
	const DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//回転行列から前方向ベクトルを取り出す
	const DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front{};
	DirectX::XMStoreFloat3(&front, Front);

	DirectX::XMFLOAT3 corEye{};

	//各視点のカメラ位置（eye）と注視点（target）を求める
	corTarget.x = target.x + targetCorrection.x * front.x;
	corTarget.y = target.y + targetCorrection.y;
	corTarget.z = target.z + targetCorrection.z * front.z;
	corEye = corTarget - front * range;
	
	//カメラシェイク
	if (shakeTimer > 0.0f)
	{
		shakeTimer -= SystemManager::Instance().GetElapsedTime();
		if (shakeTimer < 0.0f) shakeTimer = 0.0f;
		Shake();
	}
	corEye += provisionalTarget * shakeTimer * 0.1f;
	DirectX::XMFLOAT3 t = corTarget + provisionalTarget * shakeTimer * 0.1f;

	//カメラの視点と注視点を設定
	SetLookAt(corEye, t, { 0.0f,1,0.0001f });

}

//指定方向を向く
void CameraData::SetLookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up)
{
	//視点、注視点、上方向からビュー行列,ビュープロジェクション行列を作成
	DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&pos);
	DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Pos, Target, Up);
	DirectX::XMStoreFloat4x4(&view, View);
	DirectX::XMStoreFloat4x4(&viewProjection, View * P);


	//ビューを逆行列化し、ワールド座標に戻す
	DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, World);

	//カメラの方向を取り出す
	this->right.x = world._11;
	this->right.y = world._12;
	this->right.z = world._13;

	this->up.x = world._21;
	this->up.y = world._22;
	this->up.z = world._23;

	this->front.x = world._31;
	this->front.y = world._32;
	this->front.z = world._33;
	
}

void CameraData::UpdateBuffer(const DirectX::XMFLOAT3 pos)
{
	cameraConstants.viewProjection = viewProjection;
	cameraConstants.position = { pos.x,pos.y,pos.z,0.0f };

	constantBuffer.SetConstantBuffer(SystemManager::Instance().GetDeviceContext(), ConstantBuffer::ShaderType::ALL, ConstantBuffer::UsageType::Camera, &cameraConstants);
}

void CameraData::DebugGui()
{
	ImGui::DragFloat("range", &range, 0.1f);
	//ImGui::DragFloat3("pos", &eye.x, 0.01f);
	ImGui::DragFloat3("target", &target.x, 0.01f);
	ImGui::DragFloat3("angle", &angle.x, 0.01f);
	ImGui::DragFloat3("up", &up.x, 0.01f);
	ImGui::DragFloat3("front", &front.x, 0.01f);
	ImGui::DragFloat3("provisionalTarget", &provisionalTarget.x, 0.01f);

	ImGui::DragFloat3("ShakePower", &shakePower.x, 0.1f);

	if (ImGui::Button("Shake"))
	{
		shakeTimer = maxShakeTime;
	}
}

void CameraData::SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ)
{
	//パースペクティブ投影行列を作成
	XMMATRIX Projection = XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
	XMStoreFloat4x4(&projection, Projection);
}

void CameraData::Shake()
{
	provisionalTarget = { (float)(rand() % 6) - 3.0f,(float)(rand() % 4) - 2.0f,0.0f };
	provisionalTarget *= shakePower;
}
