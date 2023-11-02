
#include "Stdafx.h"

#include "PlayerStates.h"

#include "GameObject/GameObject.h"

#include "Component/Transform.h"
#include "Component/ModelRenderer.h"
#include "Component/Camera.h"
#include "Component/RigidBody.h"
#include "Component/Player.h"

#include "System/SystemManager.h"
#include "System/UserFunction.h"

using namespace PlayerState;

//-----< 基底クラス >-----//
void Default::SetMoveVelocity(DirectX::XMFLOAT3 velocity)
{
	//プレイヤーコンポーネントにMoveVelocityを設定
	parent->GetComponent<RigidBody>()->AddForce(velocity);
}
DirectX::XMFLOAT3 Default::CalcMoveVec()
{
	//カメラの取得
	Camera* camera = parent->GetParent()->GetChild("CameraController")->GetComponent<Camera>();

	//カメラの前方向ベクトルを取得
	DirectX::XMFLOAT3 cameraFront = camera->GetCameraFront();
	cameraFront.y = 0.0f;
	cameraFront = NormalizeFloat3(cameraFront);
	//カメラの右方向ベクトルを取得
	DirectX::XMFLOAT3 cameraRight = camera->GetCameraRight();
	cameraRight.y = 0.0f;
	cameraRight = NormalizeFloat3(cameraRight);

	//Lスティックの入力値を取得
	DirectX::XMFLOAT2 lStickVec = GetLStickVec();
	//カメラのベクトルから水平方向の移動ベクトルを算出
	DirectX::XMFLOAT3 vec =
	{
		cameraFront.x * lStickVec.y + cameraRight.x * lStickVec.x,
		0.0f,
		cameraFront.z * lStickVec.y + cameraRight.z * lStickVec.x,
	};

	return NormalizeFloat3(vec);
}
DirectX::XMFLOAT2 Default::GetLStickVec()
{
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	return DirectX::XMFLOAT2(gamePad.GetAxisLX() ,gamePad.GetAxisLY());
}
void Default::YAxisRotate(DirectX::XMFLOAT3 moveVelocity)
{
	//引数の移動ベクトルがゼロベクトルの場合は回転処理を行わない
	moveVelocity.y = 0.0f;
	const float length = LengthFloat3(moveVelocity);
	if (length < FLT_EPSILON)return;

	//移動ベクトルを正規化する
	moveVelocity = NormalizeFloat3(moveVelocity);

	//回転軸を求める
	DirectX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);

	//playerのワールド行列
	DirectX::XMFLOAT4X4 world = parent->GetComponent<Transform>()->world;

	//自身の回転値から前方向を求める
	DirectX::XMFLOAT3 frontVec = NormalizeFloat3({ world._31,world._32 ,world._33 });
	up = frontVec;
	//回転角を求める
	const float dot = (frontVec.x * moveVelocity.x) + (frontVec.z * moveVelocity.z);

	//回転速度を調整する
	float rot = acosf(dot);

	//左右判定
	float cross = (frontVec.x * moveVelocity.z) - (frontVec.z * moveVelocity.x);

	//orientationの取得
	DirectX::XMFLOAT4 orientation = parent->GetComponent<Transform>()->orientation;
	DirectX::XMVECTOR OldOrientation = DirectX::XMLoadFloat4(&orientation);

	DirectX::XMVECTOR Q;
	float speed = rotateSpeed * SystemManager::Instance().GetElapsedTime();
	if (speed > rot) speed = rot;
	if (cross < 0.0f)
	{
		Q = DirectX::XMQuaternionRotationAxis(Up, speed);
	}
	else
	{
		Q = DirectX::XMQuaternionRotationAxis(Up, -speed);
	}
	DirectX::XMVECTOR Orientation = DirectX::XMQuaternionMultiply(OldOrientation, Q);

	Orientation = DirectX::XMQuaternionSlerp(OldOrientation, Orientation, rotateRatio);

	DirectX::XMStoreFloat4(&orientation, Orientation);

	parent->GetComponent<Transform>()->orientation = orientation;
}

bool PlayerState::Default::JudgeIdleState()
{
	////Lスティックの入力値を取得
	//DirectX::XMFLOAT2 lStickVec = GetLStickVec();
	////Lスティックの入力がないなら待機ステートへ遷移
	//return (LengthFloat2(lStickVec) < FLT_EPSILON);
	
	//自身の移動ベクトルを取得
	DirectX::XMFLOAT3 velocity = parent->GetComponent<RigidBody>()->GetVelocity();
	//XZ平面の移動量を用いるためY方向は無視する
	velocity.y = 0.0f;
	//移動ベクトルに数値が入っていないなら待機ステートへ
	float walkThreshold = 1.0f;
	return (LengthFloat3(velocity) < walkThreshold);
}
bool PlayerState::Default::JudgeRunState()
{
	//Lスティックの入力値を取得
	DirectX::XMFLOAT2 lStickVec = GetLStickVec();
	//Lスティックの入力があるなら走りステートへ遷移
	return (LengthFloat2(lStickVec) > 0.0f);
}
bool PlayerState::Default::JudgeJumpState()
{
	//ゲームパッドの取得
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	//Aボタンが押されていたらtrue
	return (gamePad.GetButtonDown() & GamePad::BTN_A);
}

//-----< 待機 >-----//
Idle::Idle()
{
	name = "Idle";
}
Idle::Idle(GameObject* parent)
{
	name = "Idle";
	this->parent = parent;
}
void Idle::Enter()
{
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Idle, true);
}
void Idle::Update()
{

}
void Idle::Exit()
{

}
std::string Idle::GetNext()
{
	//ジャンプステートへ遷移できるか
	if (JudgeJumpState())
	{
		return "Jump";
	}

	//走りステートへ遷移できるか
	if (JudgeRunState())
	{
		return "Run";
	}
	
	//変更なし
	return "";
}

//-----< 走り >-----//
Run::Run()
{
	name = "Run";
}
Run::Run(GameObject* parent)
{
	name = "Run";
	this->parent = parent;
}
void Run::Enter()
{
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Running, true);
}
void Run::Update()
{
	DirectX::XMFLOAT3 moveVelocity = CalcMoveVec();
	moveVelocity *= parent->GetComponent<Player>()->GetRunSpeed();

	SetMoveVelocity(moveVelocity);
	YAxisRotate(moveVelocity);
}
void Run::Exit()
{

}
std::string Run::GetNext()
{
	//ジャンプステートへ遷移できるか
	if (JudgeJumpState())
	{
		return "Jump";
	}

	//歩きステートへ遷移できるかどうか（走りステートを継続できないか）
	if (!JudgeRunState() && parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Walk";
	}

	//変更なし
	return "";
}

//-----< 歩き >-----//
Walk::Walk()
{
	name = "Walk";
}
Walk::Walk(GameObject* parent)
{
	name = "Walk";
	this->parent = parent;
}
void Walk::Enter()
{
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Walking, true);
}
void Walk::Update()
{

}
void Walk::Exit()
{

}
std::string Walk::GetNext()
{
	//ジャンプステートへ遷移できるか
	if (JudgeJumpState())
	{
		return "Jump";
	}

	//走りステートへ遷移できるか
	if (JudgeRunState())
	{
		return "Run";
	}

	//待機ステートへ遷移できるかどうか
	if (JudgeIdleState())
	{
		return "Idle";
	}

	//変更なし
	return "";
}

//-----< ジャンプ >-----//
Jump::Jump()
{
	name = "Jump";
}
Jump::Jump(GameObject* parent)
{
	name = "Jump";
	this->parent = parent;
}
void Jump::Enter()
{
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Jump, false);
	SetMoveVelocity({ 0.0f, parent->GetComponent<Player>()->GetJumpSpeed(), 0.0f });
}
void Jump::Update()
{
	SetMoveVelocity({ 0.0f, parent->GetComponent<Player>()->GetJumpSpeed(), 0.0f });
}
void Jump::Exit()
{

}
std::string Jump::GetNext()
{
	//アニメーション再生が終わったら落下ステートへ遷移
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Falling";
	}

	//変更なし
	return "";
}

//-----< 落下 >-----//
Falling::Falling()
{
	name = "Falling";
}
Falling::Falling(GameObject* parent)
{
	name = "Falling";
	this->parent = parent;
}
void Falling::Enter()
{
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Falling, true);

}
void Falling::Update()
{

}
void Falling::Exit()
{

}
std::string Falling::GetNext()
{
	//pos.y < 0.0f なら着地ステートへ
	if (parent->GetComponent<Transform>()->pos.y < 0.0f)
	{
		return "Landing";
	}

	//変更なし
	return "";
}

//-----< 着地 >-----//
Landing::Landing()
{
	name = "Landing";
}
Landing::Landing(GameObject* parent)
{
	name = "Landing";
	this->parent = parent;
}
void Landing::Enter()
{
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Landing, false);
}
void Landing::Update()
{

}
void Landing::Exit()
{

}
std::string Landing::GetNext()
{
	//アニメーション再生が終わったら落下ステートへ遷移
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Idle";
	}

	//変更なし
	return "";
}
