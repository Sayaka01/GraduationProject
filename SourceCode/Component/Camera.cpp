
#include "Stdafx.h"

#include  "Camera.h"

#include "GameObject/GameObject.h"
#include "System/Common.h"

#include "Component/Transform.h"
#include "System/SystemManager.h"
#include "System/UserFunction.h"

#include "Player.h"
#include "Component/RigidBody.h"

//初期化
void Camera::Initialize()
{
	name = "Camera";

	cameraData = std::make_unique<CameraData>();
	constexpr float aspect = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
	cameraData->SetPerspectiveFov(FovRadY, aspect, nearZ, farZ);

	cameraData->SetAngle({ DirectX::XMConvertToRadians(10),0.0f,0.0f });

	cameraData->Initialize();

	cameraData->SetTargetCorrection(targetCorrection);
}

//更新
void Camera::Update()
{
	MoveCameraAngle();

	Tracking();

	parent->GetComponent<Transform>()->pos = cameraData->Update();

	//ゲームパッドの取得
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	if (gamePad.GetButtonUp() & GamePad::BTN_RIGHT_THUMB)// Rスティック押し込み
	{
		//プレイヤーの前方向へカメラをリセット

	}

	// カメラの距離を動かす
	ChangeRange();
}

//描画
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
		if (ImGui::Button("reset"))
		{
			ResetDefault();
		}
#if 0
		ImGui::DragFloat("fov", &FovRadY, 0.1f);
		constexpr float aspect = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
		cameraData->SetPerspectiveFov(FovRadY, aspect, nearZ, farZ);
#endif
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

	cameraData->SetTarget(targetObj->GetComponent<Transform>()->pos);
}

// プレイヤーのステートによってカメラの距離を動かす
void Camera::ChangeRange()
{
	GameObject* pl = parent->GetParent()->GetChild("player");
	if (pl == nullptr)return;
	float range = cameraData->GetRange();

	//ワイヤー中はカメラをplayerから少し離す
	if (pl->GetComponent<Player>()->GetCurrentState()->GetName() == "SwingWire"
		|| pl->GetComponent<Player>()->GetCurrentState()->GetName() == "WireJump")
	{
		if (range < 40)
			cameraData->SetRange(range + SystemManager::Instance().GetElapsedTime() * 30);
	}
	else
	{
		if (range > 30)
			cameraData->SetRange(range - SystemManager::Instance().GetElapsedTime() * 60);
	}
}

//---- カメラが見ている先を目的地に設定 ----
//watcherPosition:視点の位置、targetPosition:注視点の位置
void Camera::LockOnTarget(DirectX::XMFLOAT3 targetPosition, float rate)
{
	//DirectX::SimpleMath::Vector3 angle = cameraData->GetAngle();

	////カメラの回転値を回転行列に変換
	//const DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	////回転行列から前方向ベクトルを取り出す
	//const DirectX::XMVECTOR Front = Transform.r[2];
	//Front = DirectX::XMVector3Normalize(Front);
	//DirectX::XMFLOAT3 front{};
	//DirectX::XMStoreFloat3(&front, Front);

	//DirectX::XMFLOAT3 corEye{};
	//DirectX::XMFLOAT3 corTarget{};

	////各視点のカメラ位置（eye）と注視点（target）を求める
	//corTarget.x = cameraData->GetTarget().x + targetCorrection.x * front.x;
	//corTarget.y = cameraData->GetTarget().y + targetCorrection.y;
	//corTarget.z = cameraData->GetTarget().z + targetCorrection.z * front.z;
	//corEye = corTarget - front * cameraData->GetRange();


	////DirectX::XMVECTOR WatcherPos = DirectX::XMLoadFloat3(&focus);
	////DirectX::XMVECTOR TargetPos = DirectX::XMLoadFloat3(&targetPosition);
	////目から注視点へのベクトルを算出し保存
	////DirectX::XMVECTOR EyeVector = (WatcherPos - TargetPos);
	////EyeVector = DirectX::XMVector3Normalize(EyeVector);//正規化
	////DirectX::XMVECTOR CurrentFront = DirectX::XMLoadFloat3(&eyeVectorN);
	////CurrentVector = DirectX::XMVector3Normalize(CurrentVector);
	////線形補間する
	//CurrentVector = DirectX::XMVectorLerp(CurrentVector, EyeVector, rate);
	//DirectX::XMStoreFloat3(&eyeVectorN, CurrentVector);
	//EyeVector = CurrentVector;

	////カメラの位置を算出
	//DirectX::XMVECTOR EyePoint = WatcherPos;
	//DirectX::XMVECTOR Eye = EyePoint + EyeVector * range;

	//DirectX::XMStoreFloat3(&eye, Eye);
	//DirectX::XMStoreFloat3(&focus, EyePoint);

	////カメラの情報を設定
	//cameraData->SetLookAt(eye, focus, up);
}

void Camera::ResetDefault()
{
	// playerからカメラの視点　と　defaultの視点を内積して角度を算出し代入

	//DirectX::SimpleMath::Vector3 eyeVector = 

	cameraData->SetAngle({ 0, 0, 0 });
}
