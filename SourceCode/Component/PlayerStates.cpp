
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

//static変数の初期化
float Default::attackInterval = 0.0f;
bool Default::acceptAttackButton = false;
bool Default::pushAttackButton = false;
int Default::jumpCount = 0;

//-----< 基底クラス >-----//
void PlayerState::Default::Update()
{
	//攻撃ボタンが押されているかどうか
	if (acceptAttackButton && attackInterval > FLT_EPSILON)
	{
		//ゲームパッドの取得
		GamePad gamePad = SystemManager::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_B)
		{
			pushAttackButton = true;
		}
	}

	attackInterval += SystemManager::Instance().GetElapsedTime();

	if (pushAttackButton && attackInterval > acceptAttackTime)
	{
		pushAttackButton = false;
	}
}

void Default::SetMoveVelocity(DirectX::XMFLOAT3 velocity)
{
	//プレイヤーコンポーネントにMoveVelocityを設定
	parent->GetComponent<RigidBody>()->AddVelocity(velocity);
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
	//Lスティックの入力値を取得
	DirectX::XMFLOAT2 lStickVec = GetLStickVec();
	//Lスティックの入力がないなら待機ステートへ遷移
	return (LengthFloat2(lStickVec) < FLT_EPSILON);
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
bool PlayerState::Default::JudgePunchRightState()
{
	//ゲームパッドの取得
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	//Aボタンが押されていたらtrue
	return (gamePad.GetButtonDown() & GamePad::BTN_B);
}
bool PlayerState::Default::JudgePunchLeftState()
{
	//ゲームパッドの取得
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	//Aボタンが押されていたらtrue
	return (pushAttackButton && (attackInterval < acceptAttackTime));
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
	Default::Update();
}
void Idle::Exit()
{

}
std::string Idle::GetNext()
{
	//パンチ（左）ステートへ遷移できるか
	if (JudgePunchLeftState())
	{
		return "PunchLeft";
	}

	//パンチ（右）ステートへ遷移できるか
	if (JudgePunchRightState())
	{
		return "PunchRight";
	}

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

	Default::Update();
}
void Run::Exit()
{

}
std::string Run::GetNext()
{
	//パンチ（左）ステートへ遷移できるか
	if (JudgePunchLeftState())
	{
		return "PunchLeft";
	}

	//パンチ（右）ステートへ遷移できるか
	if (JudgePunchRightState())
	{
		return "PunchRight";
	}

	//ジャンプステートへ遷移できるか
	if (JudgeJumpState())
	{
		return "Jump";
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
	parent->GetComponent<RigidBody>()->Jump(parent->GetComponent<Player>()->GetJumpSpeed());
	//SetMoveVelocity({ 0.0f, parent->GetComponent<Player>()->GetJumpSpeed(), 0.0f });
}
void Jump::Update()
{
	Default::Update();
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
	Default::Update();
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
	Default::Update();
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

//-----< パンチ（右） >-----//
PunchRight::PunchRight()
{
	name = "PunchRight";
}
PunchRight::PunchRight(GameObject* parent)
{
	name = "PunchRight";
	this->parent = parent;
}
void PunchRight::Enter()
{
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Punching, false);
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(2.0f);
	
	attackInterval = 0.0f;
	
	acceptAttackButton = true;
}
void PunchRight::Update()
{
	OutputDebugLog("attackInterval", attackInterval);
	Default::Update();
}
void PunchRight::Exit()
{
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(1.0f);
}
std::string PunchRight::GetNext()
{

	//アニメーション再生が終わったら
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		//パンチ（左）ステートへ遷移できるか
		if (JudgePunchLeftState())
		{
			return "PunchLeft";
		}

		//待機ステートへ遷移
		return "Idle";
	}

	//変更なし
	return "";
}

//-----< パンチ（左） >-----//
PunchLeft::PunchLeft()
{
	name = "PunchLeft";
}
PunchLeft::PunchLeft(GameObject* parent)
{
	name = "PunchLeft";
	this->parent = parent;
}
void PunchLeft::Enter()
{
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Attack, false);

	attackInterval = 0.0f;

	acceptAttackButton = false;
	pushAttackButton = false;
}
void PunchLeft::Update()
{
	Default::Update();
}
void PunchLeft::Exit()
{

}
std::string PunchLeft::GetNext()
{
	//アニメーション再生が終わったら落下ステートへ遷移
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Idle";
	}

	//変更なし
	return "";
}
