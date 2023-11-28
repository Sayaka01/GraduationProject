#pragma once
#include <DirectXMath.h>
#include <SimpleMath.h>

#include "System/ConstantBuffer.h"

class CameraData
{
public:
	CameraData();
	~CameraData();

	void Initialize();

	DirectX::XMFLOAT3 Update();

	void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

	void UpdateBuffer(const DirectX::XMFLOAT3 pos);

	void DebugGui();

	void SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ);

	void SetShakeTimer(const float shakeTime) { shakeTimer = shakeTime; }

	const DirectX::XMFLOAT3 GetFront() { return front; }
	const DirectX::XMFLOAT3 GetRight() { return right; }
	const DirectX::XMFLOAT3 GetUp() { return up; }

	void SetRight(DirectX::XMFLOAT3 right) { this->right = right; }
	void SetFront(DirectX::XMFLOAT3 front) { this->front = front; }
	void SetUp(DirectX::XMFLOAT3 up) { this->up = up; }

	[[nodiscard]] DirectX::XMFLOAT4X4 GetView() const { return view; }
	[[nodiscard]] DirectX::XMFLOAT4X4 GetProjection() const { return projection; }
	[[nodiscard]] DirectX::XMFLOAT4X4 GetViewProjection() const { return viewProjection; }

	void SetTarget(const DirectX::XMFLOAT3 t) { target = t; }
	[[nodiscard]] DirectX::XMFLOAT3 GetTarget() const { return corTarget; }

	//[[nodiscard]] DirectX::XMFLOAT3 GetAngle()const { return angle; }
	//void SetAngle(DirectX::XMFLOAT3 ang) { angle = ang; }

	[[nodiscard]] float GetRange()const { return range; }
	void SetRange(float rng) { range = rng; }


	[[nodiscard]] DirectX::XMFLOAT3 GetCameraFront() const { return front; }
	[[nodiscard]] DirectX::XMFLOAT3 GetCameraUp() const { return up; }
	[[nodiscard]] DirectX::XMFLOAT3 GetCameraRight() const { return right; }

	void SetTargetCorrection(DirectX::XMFLOAT3 cor)
	{
		targetCorrection = cor;
	}
	DirectX::XMFLOAT3 GetTargetCorrection() { return targetCorrection; }

	void SetShakePower(const DirectX::XMFLOAT3 shakePower) { this->shakePower = shakePower; }
	
	void SetEye(DirectX::SimpleMath::Vector3 eye) { this->eye = eye; }
	DirectX::SimpleMath::Vector3 GetEye() const { return eye; }
	
	void SetEyeVector(DirectX::XMFLOAT3 eye) { this->eyeVector = eye; }
	DirectX::XMFLOAT3 GetEyeVector() const { return eyeVector; }


private:
	void Shake();

private:
	//定数バッファ
	struct CameraConstants
	{
		DirectX::XMFLOAT4X4 viewProjection;
		DirectX::XMFLOAT4 position;
	}cameraConstants{};
	ConstantBuffer constantBuffer;

private:

	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMFLOAT4X4 viewProjection;

	DirectX::XMFLOAT3 up{ 0,1,0.001f };
	DirectX::XMFLOAT3 front;
	DirectX::XMFLOAT3 right;

	DirectX::XMFLOAT3 eye;//カメラの位置
	DirectX::XMFLOAT3 eyeVector{ 0,0,0 };//targetからカメラへのベクトル

	DirectX::XMFLOAT3 target = { 0,0,0 };//プレイヤーの位置
	DirectX::XMFLOAT3 corTarget = { 0.0f,0.0f,0.0f };//計算後の実際にカメラが見ている位置
	//DirectX::XMFLOAT3 angle = { 0,0,0 };
	float range = 30.0f;

	//modelを追尾するときのmodelの原点からの補正値
	DirectX::XMFLOAT3 targetCorrection = { 0.0f,10.0f, 0.0f };
	

	//カメラシェイク
	float shakeTimer = 0.0f;
	float maxShakeTime = 3.0f;
	DirectX::XMFLOAT3 shakePower = { 5.0f,5.0f,0.5f };
	DirectX::XMFLOAT3 provisionalTarget = { 0,0,0 };//仮のターゲット
	
};