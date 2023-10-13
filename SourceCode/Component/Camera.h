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

	//-----< 関数 >-----//

	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;
	//ImGui
	void DebugGui() override;

	//定数バッファの更新
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

	//modelを追尾するときのmodelの原点からの補正値をセットする
	void SetTargetCorrection(DirectX::XMFLOAT3 correction) { targetCorrection = correction; }

	[[nodiscard]] DirectX::XMFLOAT3 GetCameraFront() const { return cameraData->GetCameraFront(); }
	[[nodiscard]] DirectX::XMFLOAT3 GetCameraUp() const { return cameraData->GetCameraUp(); }
	[[nodiscard]] DirectX::XMFLOAT3 GetCameraRight() const { return cameraData->GetCameraRight(); }


private:
	//-----< 変数 >-----//

	std::unique_ptr<CameraData> cameraData = nullptr;

	float rotationSpeed = DirectX::XMConvertToRadians(90);

	GameObject* targetObj = nullptr;
	bool tracking = false;

	//modelを追尾するときのmodelの原点からの補正値
	DirectX::XMFLOAT3 targetCorrection = { 0.0f,10.0f,0.0f };

	float maxDegree = 85.0f;
	float minDegree = -12.5f;
	

	float FovRadY = DirectX::XMConvertToRadians(45.0f);//視野の広さ
	float nearZ = 0.1f;//カメラの最近距離
	float farZ = 2000.0f;//カメラの最遠距離

};