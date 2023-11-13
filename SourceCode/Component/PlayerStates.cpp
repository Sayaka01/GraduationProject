
#include "Stdafx.h"

#include "PlayerStates.h"

#include "GameObject/GameObject.h"

#include "Component/Transform.h"
#include "Component/ModelRenderer.h"
#include "Component/Camera.h"
#include "Component/RigidBody.h"
#include "Component/Player.h"
#include "Component/SphereCollider.h"
#include "Component/CapsuleCollider.h"

#include "System/SystemManager.h"
#include "System/UserFunction.h"

using namespace PlayerState;

//static変数の初期化
float Default::attackInterval = 0.0f;
bool Default::acceptAttackButton = false;
bool Default::pushAttackButton = false;
int Default::jumpCount = 0;
DirectX::XMFLOAT3 Default::parameter = { 0.0f,0.0f,0.0f };

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
DirectX::XMFLOAT3 Default::GetCameraFront()
{
	//カメラの取得
	Camera* camera = parent->GetParent()->GetChild("CameraController")->GetComponent<Camera>();

	//カメラの前方向ベクトルを取得
	DirectX::XMFLOAT3 cameraFront = camera->GetCameraFront();
	cameraFront.y = 0.0f;

	return NormalizeFloat3(cameraFront);
}
DirectX::XMFLOAT3 PlayerState::Default::GetCameraRight()
{
	//カメラの取得
	Camera* camera = parent->GetParent()->GetChild("CameraController")->GetComponent<Camera>();

	//カメラの右方向ベクトルを取得
	DirectX::XMFLOAT3 cameraRight = camera->GetCameraRight();
	cameraRight.y = 0.0f;

	return NormalizeFloat3(cameraRight);
}
DirectX::XMFLOAT3 Default::CalcMoveVec()
{
	//カメラの前方向ベクトルを取得
	DirectX::XMFLOAT3 cameraFront = GetCameraFront();
	//カメラの右方向ベクトルを取得
	DirectX::XMFLOAT3 cameraRight = GetCameraRight();

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
void PlayerState::Default::CalcEnemyDistance()
{
	//今は敵が1体なのでこのやり方。後で頑張る
	GameObject* enemy = parent->GetParent()->GetChild("enemy");
	parameter = enemy->GetComponent<Transform>()->pos;
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
	if (jumpCount >= maxJumpCount)return false;

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
bool PlayerState::Default::JudgeAimWireState()
{
	//ゲームパッドの取得
	GamePad gamePad = SystemManager::Instance().GetGamePad();

	//ZLボタンが押されていたらtrue
	return (gamePad.GetButtonDown() & GamePad::BTN_LEFT_SHOULDER);
}
bool PlayerState::Default::JudgeSwingWireState()
{
	//ゲームパッドの取得
	GamePad gamePad = SystemManager::Instance().GetGamePad();

	//ZRボタンが押されていたらtrue
	return (gamePad.GetButtonDown() & GamePad::BTN_RIGHT_SHOULDER);
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
	jumpCount = 0;
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

	//ワイヤーでの直線移動ステートに遷移できるか
	if (JudgeAimWireState())
	{
		return "AimWire";
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

	//ワイヤーでの直線移動ステートに遷移できるか
	if (JudgeAimWireState())
	{
		return "AimWire";
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
	jumpCount++;
	if (jumpCount <= 1)//ジャンプ一回目
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Jump, false);
	else//それ以降
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::JumpFlip, false);

	parent->GetComponent<RigidBody>()->Jump(parent->GetComponent<Player>()->GetJumpSpeed());

}
void Jump::Update()
{
	DirectX::XMFLOAT3 moveVelocity = CalcMoveVec();
	moveVelocity *= parent->GetComponent<Player>()->GetRunSpeed();

	SetMoveVelocity(moveVelocity);
	YAxisRotate(moveVelocity);

	Default::Update();
}
void Jump::Exit()
{

}
std::string Jump::GetNext()
{
	//ワイヤーでの弧を書いた移動ステートに遷移できるか
	if (JudgeSwingWireState())
	{
		return "SwingWire";
	}

	//ワイヤーでの直線移動ステートに遷移できるか
	if (JudgeAimWireState())
	{
		return "AimWire";
	}

	//ジャンプステートへ遷移できるか
	if (JudgeJumpState())
	{
		return "Jump";
	}

	//アニメーション再生が終わったら落下ステートへ遷移
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Falling";
	}

	//pos.y < 0.0f なら着地ステートへ
	if (parent->GetComponent<Transform>()->pos.y < 0.0f)
	{
		return "Landing";
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
	DirectX::XMFLOAT3 moveVelocity = CalcMoveVec();
	moveVelocity *= parent->GetComponent<Player>()->GetRunSpeed();

	SetMoveVelocity(moveVelocity);
	YAxisRotate(moveVelocity);

	Default::Update();
}
void Falling::Exit()
{

}
std::string Falling::GetNext()
{
	//ワイヤーでの弧を書いた移動ステートに遷移できるか
	if (JudgeSwingWireState())
	{
		return "SwingWire";
	}

	//ワイヤーでの直線移動ステートに遷移できるか
	if (JudgeAimWireState())
	{
		return "AimWire";
	}

	//pos.y < 0.0f なら着地ステートへ
	if (parent->GetComponent<Transform>()->pos.y < 0.0f)
	{
		return "Landing";
	}

	//ジャンプステートへ遷移できるか
	if (JudgeJumpState())
	{
		return "Jump";
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

	jumpCount = 0;
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
	//ワイヤーでの直線移動ステートに遷移できるか
	if (JudgeAimWireState())
	{
		return "AimWire";
	}

	//走りステートへ遷移できるか
	if (JudgeRunState())
	{
		return "Run";
	}

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
	//アニメーションの再生
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Punching, false);
	//アニメーションスピードの調整
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(2.0f);
	
	//今から攻撃するので初期化
	attackInterval = 0.0f;
	
	//2段以降の攻撃の入力の取得を開始
	acceptAttackButton = true;

	//攻撃用当たり判定のコンポーネントを有効化
	parent->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(true);

	//攻撃力を設定
	attackPower = 1.0f;

	//一番近い敵の位置をparameterに格納
	CalcEnemyDistance();
}
void PunchRight::Update()
{
	DirectX::XMFLOAT3 vec = parameter - parent->GetComponent<Transform>()->pos;
	vec.y = 0.0f;
	float length = LengthFloat3(vec);
	if (length < attackRangeMax && length > attackRangeMin)
	{
		SetMoveVelocity(NormalizeFloat3(vec) * parent->GetComponent<Player>()->GetRunSpeed());
		YAxisRotate(vec);
	}

	Default::Update();
}
void PunchRight::Exit()
{
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(1.0f);
	parent->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(false);
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
	//アニメーションの再生
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Attack, false);

	//今から攻撃するので初期化
	attackInterval = 0.0f;

	//攻撃の入力フラグなどを初期化
	acceptAttackButton = false;
	pushAttackButton = false;

	//攻撃用当たり判定のコンポーネントを有効化
	parent->GetComponent<SphereCollider>("LeftHandSphere")->SetEnable(true);

	//攻撃力を設定
	attackPower = 3.0f;

	//一番近い敵の位置をparameterに格納
	CalcEnemyDistance();
}
void PunchLeft::Update()
{
	DirectX::XMFLOAT3 vec = parameter - parent->GetComponent<Transform>()->pos;
	vec.y = 0.0f;
	float length = LengthFloat3(vec);
	if (length < attackRangeMax && length > attackRangeMin)
	{
		SetMoveVelocity(NormalizeFloat3(vec) * parent->GetComponent<Player>()->GetRunSpeed());
		YAxisRotate(vec);
	}

	Default::Update();
}
void PunchLeft::Exit()
{
	parent->GetComponent<SphereCollider>("LeftHandSphere")->SetEnable(false);
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

//-----< ダメージ >-----//
Damage::Damage()
{
	name = "Damage";
}
Damage::Damage(GameObject* parent)
{
	name = "Damage";
	this->parent = parent;
}
void Damage::Enter()
{
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::GetHit2, false);
}
void Damage::Update()
{
	//ノックバックのvelocity
	SetMoveVelocity(parameter);

	Default::Update();
}
void Damage::Exit()
{

}
std::string Damage::GetNext()
{
	//アニメーション再生が終わったら待機ステートへ遷移
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		if (parent->GetComponent<Transform>()->pos.y > 0.0f)
			return "Falling";
		else
			return "Idle";
	}

	//変更なし
	return "";
}

//-----< 死亡 >-----//
Death::Death()
{
	name = "Death";
}
Death::Death(GameObject* parent)
{
	name = "Death";
	this->parent = parent;
}
void Death::Enter()
{
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Death, false);

}
void Death::Update()
{
	// アニメーションが終わったら死亡フラグを立てる
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		parent->GetComponent<Player>()->SetIsDead(true);
	}
}
void Death::Exit()
{

}
std::string Death::GetNext()
{
	//変更なし
	return "";
}

//-----< 狙った位置にワイヤーを刺す、直線的なワイヤー移動 >-----//
AimWire::AimWire()
{
	name = "AimWire";
}
AimWire::AimWire(GameObject* parent)
{
	name = "AimWire";
	this->parent = parent;
}
void AimWire::Enter()
{
	jumpCount = 0;

	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Jump, false);

}
void AimWire::Update()
{
	Default::Update();
}
void AimWire::Exit()
{

}
std::string AimWire::GetNext()
{
	//ジャンプステートへ遷移できるか
	if (JudgeJumpState())
	{
		return "Jump";
	}

	//アニメーション再生が終わったら落下ステートへ遷移
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Falling";
	}

	//変更なし
	return "";
}

//-----< ワイヤーでの弧を書いた移動 >-----//
SwingWire::SwingWire()
{
	name = "SwingWire";
}
SwingWire::SwingWire(GameObject* parent)
{
	name = "SwingWire";
	this->parent = parent;
}
void SwingWire::Enter()
{
	jumpCount = 0;

	//カメラの前方向ベクトルを取得
	DirectX::XMFLOAT3 cameraFront = GetCameraFront();

	//ワイヤーの刺す位置（仮）
	//ワイヤーを斜辺とした90°,30°,60°の直角三角形とみて計算する
	DirectX::XMFLOAT3 pos = parent->GetComponent<Transform>()->pos;
	parameter = pos + (cameraFront * (maxWireLength / sqrtf(2.0f)));//自分の位置からワイヤーを刺すXZ平面上の点
	parameter.y += maxWireLength / sqrtf(2.0f);//ワイヤーを刺す位置

	//初期化
	oldPosition[0] = pos;
	oldPosition[1] = pos;

	//Debug描画
	parent->GetComponent<CapsuleCollider>("WireCapsule")->SetEnable(true);
	parent->GetComponent<CapsuleCollider>("WireCapsule")->begin = parameter;
	pos.y += parent->GetComponent<Player>()->GetHeight();
	parent->GetComponent<CapsuleCollider>("WireCapsule")->end = pos;

	//parent->GetComponent<RigidBody>()->SetUseGravity(false);


	swingTimer = 0.0f;
}
void SwingWire::Update()
{	
	//経過時間
	swingTimer += SystemManager::Instance().GetElapsedTime();

	//カメラの前方向に進む(XZ平面）
	DirectX::XMFLOAT3 velocity = GetCameraFront() * parent->GetComponent<Player>()->GetWireSpeed();
	SetMoveVelocity(velocity);
	YAxisRotate(velocity);

	DirectX::XMFLOAT3 pos = parent->GetComponent<Transform>()->pos;
	OutputDebugLog("velocity", velocity);
	OutputDebugLog("BeforePos", pos);

	//ワイヤーの長さに基づいて補正
	DirectX::XMFLOAT3 vec = pos - parameter;
	vec = NormalizeFloat3(vec) * maxWireLength;
	pos = parameter + vec;
	parent->GetComponent<Transform>()->pos = pos;

	OutputDebugLog("AfterPos", pos);
	//位置の保存
	oldPosition[0] = oldPosition[1];
	oldPosition[1] = pos;

	//Debug描画
	pos.y += parent->GetComponent<Player>()->GetHeight();
	parent->GetComponent<CapsuleCollider>("WireCapsule")->end = pos;

	parent->GetComponent<SphereCollider>("DebugSphere")->center = pos;

	Default::Update();
}
void SwingWire::Exit()
{
	parent->GetComponent<CapsuleCollider>("WireCapsule")->SetEnable(false);

	parent->GetComponent<RigidBody>()->SetUseGravity(true);

	parameter = oldPosition[0];

	OutputDebugLog("\n");

	OutputDebugLog("SwingWire::Exit()\n");
	OutputDebugLog("oldPosition[0]", oldPosition[0]);
	OutputDebugLog("oldPosition[1]", oldPosition[1]);

	OutputDebugLog("\n");
}
std::string SwingWire::GetNext()
{
	//ジャンプステートへ遷移できるか
	if (JudgeJumpState())
	{
		return "WireJump";
	}

	DirectX::XMFLOAT3 pos = parent->GetComponent<Transform>()->pos;
	float length = LengthFloat3(oldPosition[1] - oldPosition[0]);
	if (length < 0.15f || swingTimer > maxSwingTime)
	{
		//OutputDebugLog("swingTimer", swingTimer);
		return "WireJump";
	}

	//ZRボタンを離したらJumpステートに遷移
	if (SystemManager::Instance().GetGamePad().GetButtonUp() & GamePad::BTN_RIGHT_SHOULDER)
	{
		return "WireJump";
	}



	//変更なし
	return "";
}

//-----< ワイヤーでの弧を書いた移動 >-----//
WireJump::WireJump()
{
	name = "WireJump";
}
WireJump::WireJump(GameObject* parent)
{
	name = "WireJump";
	this->parent = parent;
}
void WireJump::Enter()
{
	//ジャンプするベクトルを計算
	DirectX::XMFLOAT3 pos = parent->GetComponent<Transform>()->pos;
	DirectX::XMFLOAT3 vec = pos - parameter;
	moveDirection = vec / SystemManager::Instance().GetElapsedTime();
	jumpSpeed = moveDirection.y + parent->GetComponent<Player>()->GetJumpSpeed();
	DirectX::XMFLOAT3 v = parent->GetComponent<RigidBody>()->GetVelocity();
	OutputDebugLog("v", v);
	OutputDebugLog("pos", pos);
	OutputDebugLog("parameter", parameter);
	OutputDebugLog("vec", vec);
	OutputDebugLog("moveDirection", moveDirection);
	OutputDebugLog("\n");
	moveDirection.y = 0.0f;

	//初期化
	oldPosition[0] = pos;
	oldPosition[1] = pos;

	//ジャンプカウントの更新
	jumpCount++;
	//アニメーションの切り替え
	if (jumpCount <= 1)//ジャンプ一回目
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Jump, false);
	else//それ以降
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::JumpFlip, false);

	//ジャンプ
	parent->GetComponent<RigidBody>()->Jump(jumpSpeed);

	falling = false;
}
void WireJump::Update()
{	
	//位置の保存
	DirectX::XMFLOAT3 pos = parent->GetComponent<Transform>()->pos;
	oldPosition[0] = oldPosition[1];
	oldPosition[1] = pos;



	//カメラの前方向に進む(XZ平面）
	SetMoveVelocity(moveDirection);
	YAxisRotate(CalcMoveVec());

	//アニメーション再生が終わったら落下ステートへ遷移
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		//アニメーションの切り替え
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Falling, true);

		falling = true;
	}

	
	Default::Update();
}
void WireJump::Exit()
{
	OutputDebugLog("\n");

}
std::string WireJump::GetNext()
{

	//ワイヤーでの弧を書いた移動ステートに遷移できるか
	if (JudgeSwingWireState() && falling)
	{
		return "SwingWire";
	}

	//ワイヤーでの直線移動ステートに遷移できるか
	if (JudgeAimWireState() && falling)
	{
		return "AimWire";
	}

	//ジャンプステートへ遷移できるか
	if (JudgeJumpState())
	{
		parameter = oldPosition[0];

		return "WireJump";
	}

	//pos.y < 0.0f なら着地ステートへ
	if (parent->GetComponent<Transform>()->pos.y < 0.0f)
	{
		return "Landing";
	}

	//変更なし
	return "";
}

