
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

	//cameraData->SetAngle({ DirectX::XMConvertToRadians(10),0.0f,0.0f });

	//cameraData->SetTarget(parent->GetParent()->GetChild("player")->GetComponent<Transform>()->pos);

	cameraData->Initialize();

	cameraData->SetTargetCorrection(targetCorrection);
}

//更新
void Camera::Update()
{
	MoveCameraAngle();

	SetEyePoint();

	Tracking();

	parent->GetComponent<Transform>()->pos = cameraData->Update();

	//ゲームパッドの取得
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	if (gamePad.GetButtonUp() & GamePad::BTN_RIGHT_THUMB)// Rスティック押し込み
	{
		//プレイヤーの前方向へカメラをリセット
		if (targetObj != nullptr)
		{
			fetchFront = targetObj->GetComponent<Transform>()->GetForward();
			lockingOn = true;
		}
	}

	if (lockingOn)
	{
		DirectX::XMFLOAT3 velo = fetchFront * cameraData->GetRange();
		lockingOn = LockOnTarget(cameraData->GetTarget() + cameraData->GetTargetCorrection() - velo/*eye*/, rateInCamera);
		//if (rateInCamera < 1)
		//	rateInCamera += SystemManager::Instance().GetElapsedTime();
		//else
		//{
		//	rateInCamera = 0;
		//	lockingOn = false;
		//}

	}



	// カメラの距離を動かす
	//ChangeRange();
}

//描画
void Camera::Draw3D()
{
	UpdateBuffer();
}

//ImGui
void Camera::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		cameraData->DebugGui();
		//if (ImGui::Button("reset"))
		//{
		//	ResetDefault();
		//}

		ImGui::SliderFloat("rate", &rateInCamera,0.0f,1.0f);
		ImGui::SliderFloat("rotationSpeed", &rotationSpeed,1.0f,100.0f);

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

void Camera::MoveCameraAngle()
{
	const GamePad& gamepad = SystemManager::Instance().GetGamePad();

	//const float rx = gamepad.GetAxisRX();
	//const float ry = gamepad.GetAxisRY();

	bool inputX = SetAngleToX();
	bool inputY = SetAngleToY();

	//入力値がなければ更新しない
	if (!inputX && !inputY)return;

	DirectX::XMFLOAT3 eyeV = cameraData->GetEyeVector();

	DirectX::XMVECTOR EyeVector = DirectX::XMLoadFloat3(&eyeV);
	DirectX::XMFLOAT3 up = { 0,1,0 };//架空の上ベクトルを設定
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);

	//上下の回転
	//プレイヤーの真上と真下に近いときは回転しない
	if (verticalDegree > 0.01f || verticalDegree < -0.01f)
	{
		DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVector3Normalize(Up), DirectX::XMVector3Normalize(EyeVector));
		float dot{};
		DirectX::XMStoreFloat(&dot, Dot);
		//真上に近い時
		if (dot > 0.75f)
		{
			if (verticalDegree < -0.1f)//入力値がマイナスの場合
			{
				DirectX::XMVECTOR Right = DirectX::XMVector3Cross(EyeVector, Up);
				DirectX::XMVECTOR Axis = DirectX::XMVector3Normalize(Right);
				DirectX::XMVECTOR Quaternion = DirectX::XMQuaternionRotationAxis(Axis, DirectX::XMConvertToRadians(verticalDegree));
				EyeVector = DirectX::XMVector3Rotate(EyeVector, Quaternion);
			}
			verticalDegree = 0;
		}
		//真下に近い時
		else if (dot < -0.22f)
		{
			if (verticalDegree > 0.1f)//入力値がプラスの場合
			{
				DirectX::XMVECTOR Right = DirectX::XMVector3Cross(EyeVector, Up);
				DirectX::XMVECTOR Axis = DirectX::XMVector3Normalize(Right);
				DirectX::XMVECTOR Quaternion = DirectX::XMQuaternionRotationAxis(Axis, DirectX::XMConvertToRadians(verticalDegree));
				EyeVector = DirectX::XMVector3Rotate(EyeVector, Quaternion);
			}
			verticalDegree = 0;
		}
		//通常時
		else
		{
			DirectX::XMFLOAT3 cameraRight = cameraData->GetRight();
			DirectX::XMVECTOR CameraRight = DirectX::XMLoadFloat3(&cameraRight);
			DirectX::XMVECTOR Axis = DirectX::XMVector3Normalize(CameraRight);
			DirectX::XMVECTOR Quaternion = DirectX::XMQuaternionRotationAxis(Axis, DirectX::XMConvertToRadians(verticalDegree));
			EyeVector = DirectX::XMVector3Rotate(EyeVector, Quaternion);
			verticalDegree = 0;
		}
	}
	//左右に回転
	if (horizonDegree > 0.01f || horizonDegree < -0.01f)
	{
		DirectX::XMVECTOR Axis = DirectX::XMVector3Normalize(Up);
		DirectX::XMVECTOR Quaternion = DirectX::XMQuaternionRotationAxis(Axis, DirectX::XMConvertToRadians(horizonDegree));
		EyeVector = DirectX::XMVector3Rotate(EyeVector, Quaternion);
		horizonDegree = 0;
	}

	EyeVector = DirectX::XMVector3Normalize(EyeVector);
	DirectX::XMStoreFloat3(&eyeV, EyeVector);
	cameraData->SetEyeVector(eyeV);


	//DirectX::XMFLOAT3 angle = cameraData->GetAngle();

	//const float elapsedTime = SystemManager::Instance().GetElapsedTime();

	//angle.x -= ry * rotationSpeed * elapsedTime;
	//angle.y += rx * rotationSpeed * elapsedTime;


	//angle.x = Clamp(angle.x, DirectX::XMConvertToRadians(minDegree), DirectX::XMConvertToRadians(maxDegree));
	//angle.y = CirculateRadian(angle.y);

	//cameraData->SetAngle(angle);
}

void Camera::Tracking() const
{
	if (!tracking)return;

	//cameraData->SetTarget({0,0,0});
	cameraData->SetTarget(targetObj->GetComponent<Transform>()->pos);
}

//---- コントローラーのRスティックのX軸の移動値を取得 ----
//（戻り値：入力があるときtrue）
bool Camera::SetAngleToX()
{
	//コントローラーで操作する処理

	//ゲームパッドの取得
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	float vec = 0;
	if (targetObj->GetComponent<Player>()->GetCurrentState()->GetName() == "SwingWire"
		|| targetObj->GetComponent<Player>()->GetCurrentState()->GetName() == "WireJump")
		//コントローラーのLスティックの移動ベクトルを取得
		vec = (float)(gamePad.GetAxisLX());
	else
		//コントローラーのRスティックの移動ベクトルを取得
		vec = (float)(gamePad.GetAxisRX());

	if (vec > 0.1f || vec < -0.1f)
	{
		horizonDegree = rotationSpeed * vec * SystemManager::Instance().GetElapsedTime();
		return true;
	}
	return false;
}

//---- コントローラーのRスティックのY軸の移動値を取得 ----
//（戻り値：入力があるときtrue）
bool Camera::SetAngleToY()
{
	//コントローラーで操作する処理
	//ゲームパッドの取得
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	//コントローラーのRスティックの移動ベクトルを取得
	float vec = (float)(gamePad.GetAxisRY());

	if (vec > 0.1f || vec < -0.1f)
	{
		verticalDegree = rotationSpeed * -vec * SystemManager::Instance().GetElapsedTime();
		return true;
	}
	return false;
}

//---- カメラの位置と注視点を算出し設定 ----
//（戻り値：入力があるときtrue）
void Camera::SetEyePoint()
{
	// カメラの位置
	DirectX::XMFLOAT3 eye = cameraData->GetTarget() + cameraData->GetEyeVector() * cameraData->GetRange();

	//値を保存
	cameraData->SetLookAt(eye, cameraData->GetTarget(), { 0.0f,1,0.0001f });
}

// プレイヤーのステートによってカメラの距離を動かす
void Camera::ChangeRange()
{
	float range = cameraData->GetRange();

	//ワイヤー中はカメラをplayerから少し離す
	if (targetObj->GetComponent<Player>()->GetCurrentState()->GetName() == "SwingWire"
		|| targetObj->GetComponent<Player>()->GetCurrentState()->GetName() == "WireJump")
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
bool Camera::LockOnTarget(DirectX::XMFLOAT3 eyeP, float rate)
{
	//回転後のeyeからtargetへのベクトル
	DirectX::SimpleMath::Vector3 eyeVector = eyeP - cameraData->GetTarget();
	eyeVector.Normalize();//正規化

	//現在のカメラの前ベクトル
	DirectX::SimpleMath::Vector3 currentVector = cameraData->GetEyeVector();
	currentVector.Normalize();//正規化

	DirectX::SimpleMath::Vector3 up = CrossFloat3(eyeVector, currentVector);
	up.Normalize();
	if (up.Length() == 0)return false;
	float dot = DotFloat3(eyeVector, currentVector);
	dot = acosf(dot);// 角度算出

	if (fabsf(DirectX::XMConvertToDegrees(dot)) <= 1)return false;

	DirectX::XMVECTOR EyeVector = DirectX::XMLoadFloat3(&eyeVector);

	//左右に回転
	//if (horizonDegree > 0.01f || horizonDegree < -0.01f)
	{
		DirectX::XMVECTOR Axis = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&up));
		DirectX::XMVECTOR Quaternion = DirectX::XMQuaternionRotationAxis(DirectX::XMLoadFloat3(&up),0);
		EyeVector = DirectX::XMVector3Rotate(EyeVector, Quaternion);
	}

#ifdef _DEBUG
	ImGui::Begin("camera reset");
	ImGui::SliderFloat3("up",&up.x, -1.0f, 1.0f);
	ImGui::End();

#endif

	EyeVector = DirectX::XMVector3Normalize(EyeVector);
	DirectX::XMStoreFloat3(&eyeVector, EyeVector);
	cameraData->SetEyeVector(eyeVector);

	//cameraData->SetTarget(cameraData->GetEye() + eyeVector * cameraData->GetRange());

	//float cy = currentVector.y;
	//float ey = eyeVector.y;

	//currentVector.y = 0;
	//eyeVector.y = 0;



	//線形補間する
	//currentVector = DirectX::XMVectorLerp(currentVector, -eyeVector, rate);
	//cameraData->SetEyeVector(currentVector);
	//currentVector.y = cy;


	//カメラの位置を算出
	//DirectX::SimpleMath::Vector3 target = cameraData->GetTarget();
	//DirectX::SimpleMath::Vector3 targetCorrection = cameraData->GetTargetCorrection();
	//DirectX::SimpleMath::Vector3 eye = target + targetCorrection + (currentVector * cameraData->GetRange());

	//cameraData->SetEye(eye);

	//カメラの情報を設定
	//cameraData->SetLookAt(eye, target, { 0.0f,1,0.0001f });

	return true;
}