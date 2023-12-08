
#include "Stdafx.h"

#include "PlayerStates.h"

#include "GameObject/GameObject.h"

#include "Component/Transform.h"
#include "Component/ModelRenderer.h"
#include "Component/SpriteRenderer.h"
#include "Component/Camera.h"
#include "Component/RigidBody.h"
#include "Component/Player.h"
#include "Component/SphereCollider.h"
#include "Component/CapsuleCollider.h"

#include "System/SystemManager.h"
#include "System/UserFunction.h"


using namespace PlayerState;
using namespace DirectX::SimpleMath;

//static変数の初期化
float Default::attackInterval = 0.0f;
bool Default::acceptAttackButton = false;
bool Default::pushAttackButton = false;
int Default::jumpCount = 0;
DirectX::XMFLOAT3 Default::parameter = { 0.0f,0.0f,0.0f };
bool Default::isAvoid = false;

//-----< 基底クラス >-----//
void PlayerState::Default::Update()
{
	//攻撃ボタンが押されているかどうか
	if (acceptAttackButton && attackInterval > FLT_EPSILON)
	{
		//ゲームパッドの取得
		GamePad gamePad = SystemManager::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_X)
		{
			pushAttackButton = true;
		}
	}

	attackInterval += SystemManager::Instance().GetElapsedTime();

	if (attackInterval > acceptAttackTime)
	{
		parent->GetComponent<Player>()->InitAttackPhase();

		if(pushAttackButton)pushAttackButton = false;
	}

	//ターゲットスプライトの位置計算
	SearchThrowObj();
}

void Default::SetMoveVelocity(DirectX::XMFLOAT3 velocity)
{
	//RigidBodyコンポーネントにMoveVelocityを設定
	parent->GetComponent<RigidBody>()->SetVelocity(velocity);
}
void PlayerState::Default::AddMoveVelocity(DirectX::XMFLOAT3 velocity)
{
	//RigidBodyコンポーネントにMoveVelocityを追加
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
#if _APPEND
void PlayerState::Default::SearchNearEnemy(Transform* transform)
{
	transform = nullptr;

	//今は敵が1体なのでこのやり方。後で頑張る
	GameObject* enemyManager = parent->GetParent()->GetChild("enemyManager");
	DirectX::SimpleMath::Vector3 playerPos = parent->GetComponent<Transform>()->pos;
	if (enemyManager != nullptr)
	{
		int enemyCounut = int(enemyManager->GetChildrenCount());
		float minLength = FLT_MAX;

		if (enemyCounut > 0)
		{
			for (int i = 0; i < enemyCounut; i++)
			{
				Transform* enemyTransform = enemyManager->GetGameObj(i)->GetComponent<Transform>();
				Vector3 enemyPos= enemyTransform->pos;
				Vector3 vec = playerPos - enemyPos;
				float length = vec.Length();

				if (minLength > length)
				{
					minLength = length;
					parameter = enemyPos;

					transform = enemyTransform;
				}
			}
			return;
		}
	}

	//敵が見つからない場合は自分自身の位置を入れる
	parameter = playerPos;

	//GameObject* enemy = parent->GetParent()->GetChild("enemy");
	//parameter = enemy->GetComponent<Transform>()->pos;
}
#endif
float PlayerState::Default::GetAnimationSpeed(int index)
{
	return parent->GetComponent<Player>()->GetAnimationSpeed(index);
}
void PlayerState::Default::SetAnimationSpeed(int index, float speed)
{
	parent->GetComponent<Player>()->SetAnimationSpeed(index, speed);
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
bool PlayerState::Default::JudgeAttackState()
{
	int phase = parent->GetComponent<Player>()->GetAttackPhase();

	if (phase == 0)
	{
		//ゲームパッドの取得
		GamePad gamePad = SystemManager::Instance().GetGamePad();
		//Aボタンが押されていたらtrue
		return (gamePad.GetButtonDown() & GamePad::BTN_X);
	}
	else
	{
		//Aボタンが押されていたらtrue
		return (pushAttackButton && (attackInterval < acceptAttackTime));
	}
}
bool PlayerState::Default::JudgeAimWireState()
{
	return false;

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
	return (gamePad.GetButton() & GamePad::BTN_RIGHT_SHOULDER) && (parent->GetComponent<Transform>()->pos.y > 11.5f);
}
bool PlayerState::Default::JudgeAvoidState()
{
	//ゲームパッドの取得
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	//Bボタンが押されていたらtrue
	return (gamePad.GetButtonDown() & GamePad::BTN_B);
}
bool PlayerState::Default::JudgeWieldThrowState()
{
	//ゲームパッドの取得
	GamePad gamePad = SystemManager::Instance().GetGamePad();

	//RTボタンが押されていなければreturn false
	if (!(gamePad.GetButtonDown() & GamePad::BTN_RIGHT_TRIGGER))return false;

	//プレイヤーコンポーネントのThrowObjがnullがどうか
	return (parent->GetComponent<Player>()->GetThrowObj());
}

void PlayerState::Default::SearchThrowObj()
{
#if _APPEND
	if (name == "WieldThrow")return;
#endif

	//manager枠のオブジェクトを取得
	GameObject* throwObjects = parent->GetParent()->GetChild("throwObjects");

	GameObject* throwObj = nullptr;//ターゲット用オブジェクト
	DirectX::XMFLOAT2 nearScreenPos = { 0.0f,0.0f };
	float minScreenDist = FLT_MAX;//最短距離
	float minWorldDist = FLT_MAX;//最短距離

	for (size_t i = 0; i < throwObjects->GetChildrenCount(); i++)
	{
		//オブジェクトの取得
		GameObject* obj = throwObjects->GetGameObj(i);
		DirectX::XMFLOAT3 objPos = obj->GetComponent<Transform>()->pos;
		//プレイヤーの位置を取得
		DirectX::XMFLOAT3 plPos = parent->GetComponent<Transform>()->pos;
		//プレイヤーとオブジェクトのベクトルを計算
		DirectX::XMFLOAT3 vec = objPos - plPos;
		//プレイヤーとの距離を計算
		float plObjDist = LengthFloat3(vec);

		//距離がサーチ範囲を超えるならcontinue
		if (plObjDist > searchRange)continue;


		//カメラの取得
		Camera* camera = parent->GetParent()->GetChild("CameraController")->GetComponent<Camera>();
		//カメラの前方向ベクトルを取得
		DirectX::XMFLOAT3 cameraFront = camera->GetCameraFront();
		//カメラの位置を取得
		DirectX::XMFLOAT3 cameraEye = camera->GetEye();
		//カメラの位置からオブジェクトへのベクトルを作成
		vec = objPos - cameraEye;

		//カメラの方向にオブジェクトがいるかどうか計算
		float dot = DotFloat3(cameraFront, vec);
		if (dot < 0.0f)continue;

		//オブジェクトの位置をスクリーン座標系に変換する
		ID3D11DeviceContext* dc = SystemManager::Instance().GetDeviceContext();
		DirectX::XMFLOAT2 screenPos = GetScreenPosition(dc, objPos, camera->GetProjection(), camera->GetView());

		//画面外にいるかどうか
		float radius = obj->GetComponent<SphereCollider>()->radius;
		DirectX::XMFLOAT2 minSearchPos = { radius,radius };
		DirectX::XMFLOAT2 maxSearchPos = { (float)SCREEN_WIDTH - radius,(float)SCREEN_HEIGHT - radius };
		if (minSearchPos.x < screenPos.x && minSearchPos.y < screenPos.y)
		{
			if (maxSearchPos.x > screenPos.x && maxSearchPos.y > screenPos.y)
			{
				//画面中央位置
				DirectX::XMFLOAT2 screenCenter = { (float)SCREEN_WIDTH * 0.5f,(float)SCREEN_HEIGHT * 0.5f };
				//画面中央との距離を計算
				float distance = LengthFloat2(screenCenter - screenCenter);
				//最短距離なら保存
				if (distance < minScreenDist)
				{
					minScreenDist = distance;
					minWorldDist = plObjDist;
					nearScreenPos = screenPos;
					throwObj = obj;
				}
			}
		}
	}

	//計算した場所にスプライトを表示
	if (throwObj)
	{
		parent->GetComponent<SpriteRenderer>()->SetEnable(true);
		parent->GetComponent<SpriteRenderer>()->pos = nearScreenPos;
		float defaultDistance = 50.0f;
		float scale = (defaultDistance - minWorldDist) / searchRange;
		parent->GetComponent<SpriteRenderer>()->scale = { 1.1f + scale, 1.1f + scale };

		parent->GetComponent<Player>()->SetThrowObj(throwObj);
	}
	else
	{
		parent->GetComponent<SpriteRenderer>()->SetEnable(false);
		parent->GetComponent<Player>()->SetThrowObj(nullptr);
	}
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
	//アニメーションの変更
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Idle, true);
	//アニメーションスピードの調整
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Idle));
	
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
	//攻撃ステートに遷移できるか
	if (JudgeAttackState())
	{
		int phase = parent->GetComponent<Player>()->GetAttackPhase();
		
		switch (phase)
		{
		case 0://パンチ（右）ステートへ遷移できるか
			return "PunchRight";
		case 1://パンチ（左）ステートへ遷移できるか
			return "PunchLeft";
		case 2://キックステートに遷移できるか
			return "Kick";
		}

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

	//回避ステートに遷移できるか
	if (JudgeAvoidState())
	{
		return "Avoid";
	}

	if (JudgeWieldThrowState())
	{
		return "WieldThrow";
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
	//アニメーションの切り替え
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Running, true);
	//アニメーションスピードの調整
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Running));
}
void Run::Update()
{
	DirectX::XMFLOAT3 moveVelocity = CalcMoveVec();
	moveVelocity *= parent->GetComponent<Player>()->GetRunSpeed();

	AddMoveVelocity(moveVelocity);
	YAxisRotate(moveVelocity);

	DirectX::XMFLOAT3 p = parent->GetComponent<Transform>()->pos;
	p.y += 5.0f;
	parent->GetComponent<SphereCollider>("DebugSphere")->center = p;

	Default::Update();
}
void Run::Exit()
{

}
std::string Run::GetNext()
{
	//攻撃ステートに遷移できるか
	if (JudgeAttackState())
	{
		int phase = parent->GetComponent<Player>()->GetAttackPhase();

		switch (phase)
		{
		case 0://パンチ（右）ステートへ遷移できるか
			return "PunchRight";
		case 1://パンチ（左）ステートへ遷移できるか
			return "PunchLeft";
		case 2://キックステートに遷移できるか
			return "Kick";
		}

	}

	//回避ステートに遷移できるか
	if (JudgeAvoidState())
	{
		return "Avoid";
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
	{
		//アニメーションの切り替え
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Falling, true);
		//アニメーションスピードの調整
		parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Falling));
	}
	else//それ以降
	{
		//アニメーションの切り替え
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::JumpFlip, false);
		//アニメーションスピードの調整
		parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::JumpFlip));
	}

	parent->GetComponent<RigidBody>()->Jump(parent->GetComponent<Player>()->GetJumpSpeed());

	parent->GetComponent<Player>()->InitAttackPhase();
}
void Jump::Update()
{
	DirectX::XMFLOAT3 moveVelocity = CalcMoveVec();
	moveVelocity *= parent->GetComponent<Player>()->GetRunSpeed();

	AddMoveVelocity(moveVelocity);
	YAxisRotate(moveVelocity);

	if (jumpCount > 1)
	{
		//腰の位置のモーションによる移動を停止
		parent->GetComponent<ModelRenderer>()->StopMotionVelocity("FallingHip");
	}

	Default::Update();
}
void Jump::Exit()
{

}
std::string Jump::GetNext()
{
	//空中攻撃ステートに遷移できるか
	if (JudgeAttackState())
	{
		return "JumpAttack";
	}

	//回避ステートに遷移できるか
	if (JudgeAvoidState() && !isAvoid)
	{
		return "AvoidJump";
	}

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
	//アニメーションの切り替え
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Falling, true);
	//アニメーションスピードの調整
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Falling));

	//敵への攻撃フラグをOFF
	parent->GetComponent<Player>()->SetIsHitAttackToEnemy(false);

}
void Falling::Update()
{
	DirectX::XMFLOAT3 moveVelocity = CalcMoveVec();
	moveVelocity *= parent->GetComponent<Player>()->GetRunSpeed();

	AddMoveVelocity(moveVelocity);
	YAxisRotate(moveVelocity);

	Default::Update();
}
void Falling::Exit()
{

}
std::string Falling::GetNext()
{
	//空中攻撃ステートに遷移できるか
	if (JudgeAttackState())
	{
		return "JumpAttack";
	}

	//回避ステートに遷移できるか
	if (JudgeAvoidState() && !isAvoid)
	{
		return "AvoidJump";
	}

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
	//アニメーションの切り替え
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Landing, false);
	//アニメーションスピードの調整
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Landing));

	jumpCount = 0;

	isAvoid = false;
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

	//回避ステートに遷移できるか
	if (JudgeAvoidState())
	{
		return "Avoid";
	}

	//アニメーション再生が終わったら待機ステートへ遷移
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
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::PunchRight, false);
	//アニメーションスピードの調整
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::PunchRight));

	//今から攻撃するので初期化
	attackInterval = 0.0f;

	//2段以降の攻撃の入力の取得を開始
	acceptAttackButton = true;

	//攻撃用当たり判定のコンポーネントを有効化
	parent->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(true);

	//攻撃力を設定
	attackPower = 1.0f;

	//一番近い敵の位置をparameterに格納
	SearchNearEnemy();

	//敵への攻撃フラグをOFF
	parent->GetComponent<Player>()->SetIsHitAttackToEnemy(false);

	//攻撃中フラグをtrueに
	parent->GetComponent<Player>()->SetIsAttack(true);

	parent->GetComponent<Player>()->NextAttackPhase();
}
void PunchRight::Update()
{
	DirectX::XMFLOAT3 vec = parameter - parent->GetComponent<Transform>()->pos;
	vec.y = 0.0f;
	float length = LengthFloat3(vec);
	if (length < attackRangeMax)
	{
		YAxisRotate(vec);
		if (length > attackRangeMin)
			AddMoveVelocity(NormalizeFloat3(vec) * parent->GetComponent<Player>()->GetRunSpeed());
	}


	Default::Update();
}
void PunchRight::Exit()
{
	parent->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(false);

	//攻撃中フラグをfalseに
	parent->GetComponent<Player>()->SetIsAttack(false);
}
std::string PunchRight::GetNext()
{

	//アニメーション再生が終わったら
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		//パンチ（左）ステートへ遷移できるか
		if (JudgeAttackState())
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
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::PunchLeft, false);
	//アニメーションスピードの調整
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::PunchLeft));

	//今から攻撃するので初期化
	attackInterval = 0.0f;

	//攻撃の入力フラグなどを初期化
	pushAttackButton = false;

	//攻撃用当たり判定のコンポーネントを有効化
	parent->GetComponent<SphereCollider>("LeftHandSphere")->SetEnable(true);

	//攻撃力を設定
	attackPower = 3.0f;

	//一番近い敵の位置をparameterに格納
	SearchNearEnemy();

	//敵への攻撃フラグをOFF
	parent->GetComponent<Player>()->SetIsHitAttackToEnemy(false);

	//攻撃中フラグをtrueに
	parent->GetComponent<Player>()->SetIsAttack(true);

	parent->GetComponent<Player>()->NextAttackPhase();
}
void PunchLeft::Update()
{
	DirectX::XMFLOAT3 vec = parameter - parent->GetComponent<Transform>()->pos;
	vec.y = 0.0f;
	float length = LengthFloat3(vec);
	if (length < attackRangeMax)
	{
		YAxisRotate(vec);
		if (length > attackRangeMin)
			AddMoveVelocity(NormalizeFloat3(vec) * parent->GetComponent<Player>()->GetRunSpeed());
	}

	Default::Update();
}
void PunchLeft::Exit()
{
	parent->GetComponent<SphereCollider>("LeftHandSphere")->SetEnable(false);

	//攻撃中フラグをfalseに
	parent->GetComponent<Player>()->SetIsAttack(false);
}
std::string PunchLeft::GetNext()
{
	//アニメーション再生が終わったら
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		//パンチ（左）ステートへ遷移できるか
		if (JudgeAttackState())
		{
			return "Kick";
		}

		//待機ステートへ遷移
		return "Idle";
	}

	//変更なし
	return "";
}

//-----< キック >-----//
Kick::Kick()
{
	name = "Kick";
}
Kick::Kick(GameObject* parent)
{
	name = "Kick";
	this->parent = parent;
}
void Kick::Enter()
{
	//アニメーションの再生
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Kick, false);
	//アニメーションスピードの調整
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Kick));

	//今から攻撃するので初期化
	attackInterval = 0.0f;

	//攻撃の入力フラグなどを初期化
	acceptAttackButton = false;
	pushAttackButton = false;

	//攻撃用当たり判定のコンポーネントを有効化
	parent->GetComponent<SphereCollider>("RightAnkleSphere")->SetEnable(true);

	//攻撃力を設定
	attackPower = 5.0f;

	//一番近い敵の位置をparameterに格納
	SearchNearEnemy();

	//敵への攻撃フラグをOFF
	parent->GetComponent<Player>()->SetIsHitAttackToEnemy(false);

	//攻撃中フラグをtrueに
	parent->GetComponent<Player>()->SetIsAttack(true);

	parent->GetComponent<Player>()->NextAttackPhase();
}
void Kick::Update()
{
	DirectX::XMFLOAT3 vec = parameter - parent->GetComponent<Transform>()->pos;
	vec.y = 0.0f;
	float length = LengthFloat3(vec);
	if (length < attackRangeMax)
	{
		YAxisRotate(vec);
		if (length > attackRangeMin)
			AddMoveVelocity(NormalizeFloat3(vec) * parent->GetComponent<Player>()->GetRunSpeed());
	}

	Default::Update();
}
void Kick::Exit()
{
	parent->GetComponent<SphereCollider>("RightAnkleSphere")->SetEnable(false);

	//攻撃中フラグをfalseに
	parent->GetComponent<Player>()->SetIsAttack(false);
}
std::string Kick::GetNext()
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
	//アニメーションの切り替え
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Damage, false);
	//アニメーションスピードの調整
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Damage));
}
void Damage::Update()
{
	//ノックバックのvelocity
	AddMoveVelocity(parameter);

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
	//アニメーションの切り替え
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Death, false);
	//アニメーションスピードの調整
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Death));
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

	//アニメーションの切り替え
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

	//回避ステートに遷移できるか
	if (JudgeAvoidState() && isAvoid)
	{
		return "AvoidJump";
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
	//ワイヤーを斜辺とした90°,45°,45°の直角三角形とみて計算する
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

	//アニメーションの切り替え
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::DangleWire, true);
	//アニメーションスピードの調整
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::DangleWire));


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

	//ワイヤーの長さに基づいて補正
	DirectX::XMFLOAT3 vec = pos - parameter;
	vec = NormalizeFloat3(vec) * maxWireLength;
	pos = parameter + vec;
	parent->GetComponent<Transform>()->pos = pos;

	//位置の保存
	oldPosition[0] = oldPosition[1];
	oldPosition[1] = pos;

	//Debug描画
	pos.y += parent->GetComponent<Player>()->GetHeight();
	parent->GetComponent<CapsuleCollider>("WireCapsule")->end = pos;

	parent->GetComponent<SphereCollider>("DebugSphere")->center = pos;

	//腰の位置のモーションによる移動を停止
	parent->GetComponent<ModelRenderer>()->StopMotionVelocity("FallingHip");

	Default::Update();
}
void SwingWire::Exit()
{
	parent->GetComponent<CapsuleCollider>("WireCapsule")->SetEnable(false);

	parent->GetComponent<RigidBody>()->SetUseGravity(true);

	parameter = oldPosition[0];
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
	float l = parent->GetComponent<Player>()->GetWireSpeed() * SystemManager::Instance().GetElapsedTime();
	if (length < l * 0.05f || swingTimer > maxSwingTime)
	{
		return "WireJump";
	}

	//ZRボタンを離したらJumpステートに遷移
	if (SystemManager::Instance().GetGamePad().GetButtonUp() & GamePad::BTN_RIGHT_SHOULDER)
	{
		return "WireJump";
	}

	//pos.y < 0.0f なら着地ステートへ
	if (parent->GetComponent<Transform>()->pos.y < 0.0f)
	{
		return "Landing";
	}

	//回避ステートに遷移できるか
	if (JudgeAvoidState() && !isAvoid)
	{
		return "AvoidJump";
	}


	//変更なし
	return "";
}

//-----< ワイヤーでの弧を書いた移動後のジャンプ >-----//
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
	jumpSpeed = (moveDirection.y < 0.0f ? 0.0f : moveDirection.y) + parent->GetComponent<Player>()->GetJumpSpeed();
	moveDirection.y = 0.0f;

	//初期化
	oldPosition[0] = pos;
	oldPosition[1] = pos;

	//ジャンプカウントの更新
	jumpCount++;
	//アニメーションの切り替え
	if (jumpCount <= 1)//ジャンプ一回目
	{
		//アニメーションの切り替え
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Falling, true);
		//アニメーションスピードの調整
		parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Falling));
	}
	else//それ以降
	{
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::JumpFlip, false);
		//アニメーションスピードの調整
		parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::JumpFlip));
	}

	//ジャンプ
	parent->GetComponent<RigidBody>()->Jump(jumpSpeed);

	coolTimer = maxCoolTime;
	falling = false;
}
void WireJump::Update()
{
	//位置の保存
	DirectX::XMFLOAT3 pos = parent->GetComponent<Transform>()->pos;
	oldPosition[0] = oldPosition[1];
	oldPosition[1] = pos;

	DirectX::XMFLOAT3 inputVec = CalcMoveVec();
	DirectX::XMFLOAT3 moveVec{};
	if (LengthFloat3(inputVec) > FLT_EPSILON)
		moveVec = (inputVec * parent->GetComponent<Player>()->GetRunSpeed() * inputRatio) + (moveDirection * (1.0f - inputRatio));
	else
		moveVec = moveDirection;

	//カメラの前方向に進む(XZ平面）
	AddMoveVelocity(moveVec);
	YAxisRotate(inputVec);

	if (jumpCount > 1)
	{
		if (!falling)
		{
			//腰の位置のモーションによる移動を停止
			parent->GetComponent<ModelRenderer>()->StopMotionVelocity("FallingHip");
			if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
			{
				falling = true;
				//アニメーションの切り替え
				parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Falling, true);
				//アニメーションスピードの調整
				parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Falling));
			}
		}
	}

	coolTimer -= SystemManager::Instance().GetElapsedTime();

	Default::Update();
}
void WireJump::Exit()
{
}
std::string WireJump::GetNext()
{
	//空中攻撃ステートに遷移できるか
	if (JudgeAttackState())
	{
		return "JumpAttack";
	}

	//ワイヤーでの弧を書いた移動ステートに遷移できるか
	if (JudgeSwingWireState() && coolTimer < 0.0f)
	{
		return "SwingWire";
	}

	//ワイヤーでの直線移動ステートに遷移できるか
	if (JudgeAimWireState() && coolTimer < 0.0f)
	{
		return "AimWire";
	}

	//回避ステートに遷移できるか
	if (JudgeAvoidState() && !isAvoid)
	{
		return "AvoidJump";
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

//-----< 空中攻撃（右パンチ） >-----//
JumpAttack::JumpAttack()
{
	name = "JumpAttack";
}
JumpAttack::JumpAttack(GameObject* parent)
{
	name = "JumpAttack";
	this->parent = parent;
}
void JumpAttack::Enter()
{
	//アニメーションの再生
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::JumpPunch, false);
	//アニメーションの再生速度の変更
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::JumpPunch));

	//攻撃用当たり判定のコンポーネントを有効化
	parent->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(true);

	//攻撃力を設定
	attackPower = 2.0f;

	//一番近い敵の位置をparameterに格納
	SearchNearEnemy();

	//移動ベクトルを計算
	DirectX::XMFLOAT3 vec = parameter - parent->GetComponent<Transform>()->pos;
	float length = LengthFloat3(vec);

	parameter = { 0.0f,0.0f,0.0f };

	if (length < attackRangeMax)
	{
		if (length > attackRangeMin)
		{
			float animSpeed = parent->GetComponent<ModelRenderer>()->GetAnimationSpeed();
			parameter = NormalizeFloat3(vec) * (length / maxMoveTime * animSpeed);

			//重力を無視する
			parent->GetComponent<RigidBody>()->SetUseGravity(false);

			//2段以降の攻撃の入力の取得を開始
			acceptAttackButton = true;

			//今から攻撃するので初期化
			attackInterval = 0.0f;
		}
	}


	//攻撃中増え続ける
	attackTimer = 0.0f;

	//敵への攻撃フラグをOFF
	parent->GetComponent<Player>()->SetIsHitAttackToEnemy(false);

	//攻撃中フラグをtrueに
	parent->GetComponent<Player>()->SetIsAttack(true);

	parent->GetComponent<Player>()->NextAttackPhase();

}
void JumpAttack::Update()
{
	float animSpeed = parent->GetComponent<ModelRenderer>()->GetAnimationSpeed();
	attackTimer += SystemManager::Instance().GetElapsedTime() * animSpeed;

	if (attackTimer < maxMoveTime && LengthFloat3(parameter) > FLT_EPSILON)
	{
		SetMoveVelocity(parameter);
		YAxisRotate(parameter);
	}
	else
	{
		parent->GetComponent<RigidBody>()->SetUseGravity(true);
	}

	if (LengthFloat3(parameter) < FLT_EPSILON)
	{
		DirectX::XMFLOAT3 moveVelocity = CalcMoveVec();
		moveVelocity *= parent->GetComponent<Player>()->GetRunSpeed();

		AddMoveVelocity(moveVelocity);
		YAxisRotate(moveVelocity);
	}


	Default::Update();

}
void JumpAttack::Exit()
{
	parent->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(false);

	//攻撃中フラグをfalseに
	parent->GetComponent<Player>()->SetIsAttack(false);

}
std::string JumpAttack::GetNext()
{
	//アニメーション再生が終わったら落下ステートへ遷移
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

//-----< 回避 >-----//
Avoid::Avoid()
{
	name = "Avoid";
}
Avoid::Avoid(GameObject* parent)
{
	name = "Avoid";
	this->parent = parent;
}
void Avoid::Enter()
{
	//アニメーションの再生
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Avoid, false);
	//アニメーションの再生速度の変更
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Avoid));

	//回避ベクトルを作成
	avoidVec = CalcMoveVec() * avoidSpeed;

	//Lスティック入力がないならカメラの前方向に回避
	if (LengthFloat3(avoidVec) < FLT_EPSILON)
	{
		avoidVec = GetCameraFront() * avoidSpeed;
	}
}
void Avoid::Update()
{
	AddMoveVelocity(avoidVec);
	YAxisRotate(avoidVec);

	//腰の位置のモーションによる移動を停止
	parent->GetComponent<ModelRenderer>()->StopMotionXZVelocity("IdleHip");

	Default::Update();
}
void Avoid::Exit()
{
}
std::string Avoid::GetNext()
{
	//アニメーション再生が終わったら待機ステートへ遷移
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Idle";
	}

	//変更なし
	return "";
}

//-----< 空中回避 >-----//
AvoidJump::AvoidJump()
{
	name = "AvoidJump";
}
AvoidJump::AvoidJump(GameObject* parent)
{
	name = "AvoidJump";
	this->parent = parent;
}
void AvoidJump::Enter()
{
	//アニメーションの再生
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::JumpFlip, false);
	//アニメーションの再生速度の変更
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::JumpFlip));

	//回避ベクトルを作成
	avoidVec = CalcMoveVec() * avoidSpeed;

	//Lスティック入力がないならカメラの前方向に回避
	if (LengthFloat3(avoidVec) < FLT_EPSILON)
	{
		avoidVec = GetCameraFront() * avoidSpeed;
	}

	parent->GetComponent<RigidBody>()->Jump(avoidJumpPower);

	isAvoid = true;
}
void AvoidJump::Update()
{
	AddMoveVelocity(avoidVec);
	YAxisRotate(avoidVec);

	//腰の位置のモーションによる移動を停止
	parent->GetComponent<ModelRenderer>()->StopMotionVelocity("FallingHip");

	Default::Update();
}
void AvoidJump::Exit()
{
}
std::string AvoidJump::GetNext()
{
	//アニメーション再生が終わったら待機ステートへ遷移
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Falling";
	}

	//変更なし
	return "";
}

//-----< オブジェクトにワイヤーを刺して振り回して投げる >-----//
WieldThrow::WieldThrow()
{
	name = "WieldThrow";
}
WieldThrow::WieldThrow(GameObject* parent)
{
	name = "WieldThrow";
	this->parent = parent;
}
void WieldThrow::Enter()
{
#if _APPEND
	//コンポーネントの取得
	ModelRenderer* modelRenderer = parent->GetComponent<ModelRenderer>();

	//アニメーションの再生
	modelRenderer->PlayAnimation((int)Animation::Thrust, false);
	//アニメーションの再生速度の変更
	modelRenderer->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Thrust));

	//ステートの初期化
	state = Thrust;

	Vector3 objPos = parent->GetComponent<Player>()->GetThrowObj()->GetComponent<Transform>()->pos;
	Vector3 pos = parent->GetComponent<Transform>()->pos;

	float samplingRate = modelRenderer->GetSamplingRate();
	float length = (objPos - pos).Length();
	wireSpeed = length * samplingRate * GetAnimationSpeed((int)Animation::Thrust);

	appearWire = false;
	thrust = false;
#endif
}
void WieldThrow::Update()
{
	switch (state)
	{
	case Thrust:
#if _APPEND
	{
		//オブジェクトの方を向く
		GameObject* throwObj = parent->GetComponent<Player>()->GetThrowObj();
		Vector3 objPos = throwObj->GetComponent<Transform>()->pos;
		Vector3 pos = parent->GetComponent<Transform>()->pos;
		Vector3 plObjVec = objPos - pos;
		YAxisRotate(plObjVec);

		//コンポーネントの取得
		ModelRenderer* modelRenderer = parent->GetComponent<ModelRenderer>();

		//アニメーションが一定のフレームを過ぎたらワイヤーを出す
		if (modelRenderer->GetPlayAnimTimer() > maxThrustInterval)
		{
			CapsuleCollider* capsule = parent->GetComponent<CapsuleCollider>("WireCapsule");

			//右手の位置
			Vector3 rightHandPos = modelRenderer->GetBonePositionFromName("rightHand");

			//ワイヤーの出現
			if (!appearWire)
			{
				capsule->SetEnable(true);

				wireTipPos = rightHandPos;

				appearWire = true;
			}

			//ワイヤーの移動
			if (!thrust)
			{
				Vector3 handObjVec = objPos - rightHandPos;
				handObjVec.Normalize();
				wireTipPos += handObjVec * wireSpeed * SystemManager::Instance().GetElapsedTime();
			}

			//ワイヤーの先端とオブジェクトの距離がオブジェクトのスフィアの半径以下ならオブジェクトにワイヤーを刺せた
			Vector3 wireTipObjvec = objPos - wireTipPos;
			if (wireTipObjvec.Length() < throwObj->GetComponent<SphereCollider>()->radius
				|| plObjVec.Dot(wireTipObjvec) < 0.0f)
			{
				thrust = true;
				wireTipPos = objPos;
			}

			capsule->begin = wireTipPos;
			capsule->end = rightHandPos;

		}

		//アニメーションが終わったら次のステートへ
		if (modelRenderer->IsFinishAnimation())
		{
			//アニメーションの再生
			modelRenderer->PlayAnimation((int)Animation::Wield, false);
			//アニメーションの再生速度の変更
			modelRenderer->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Wield));

			state = Wield;

			//現在の回転角を計算
			//rotateRadian = atan2f(plObjVec.z, plObjVec.x);
			//回転速度を計算
			float samplingRate = modelRenderer->GetSamplingRate();
			rotateSpeed = maxWieldRadian / ((float)modelRenderer->GetPlayAnimMaxTimer() / (samplingRate * GetAnimationSpeed((int)Animation::Wield)));

			rotateVec = objPos - pos;
		}

		break;
	}
#endif
	case Wield:
#if _APPEND
	{
		//回転軸
		const Vector3 axis = { 0.0f,1.0f,0.0f };

		//クオータニオンの作成
		const Quaternion Q = Quaternion::CreateFromAxisAngle(axis, -rotateSpeed * SystemManager::Instance().GetElapsedTime());

		//回転行列の作成
		const Matrix R = Matrix::CreateFromQuaternion(Q);

		//ベクトルを回転させる
		Vector4 v = Float4MultiplyFloat4x4({ rotateVec.x,rotateVec.y,rotateVec.z,1.0f }, R);
		rotateVec = { v.x,v.y,v.z };

		//オブジェクトの位置の更新
		GameObject* throwObj = parent->GetComponent<Player>()->GetThrowObj();
		Vector3 objPos = throwObj->GetComponent<Transform>()->pos;
		Vector3 pos = parent->GetComponent<Transform>()->pos;

		objPos = pos + rotateVec;
		throwObj->GetComponent<Transform>()->pos = objPos;

		//コンポーネントの取得
		ModelRenderer* modelRenderer = parent->GetComponent<ModelRenderer>();

		//右手の位置を取得
		Vector3 rightHandPos = modelRenderer->GetBonePositionFromName("rightHand");

		//カプセルを取得
		CapsuleCollider* capsule = parent->GetComponent<CapsuleCollider>("WireCapsule");
		//カプセルの位置を更新
		capsule->begin = objPos;
		capsule->end = rightHandPos;

		if (modelRenderer->IsFinishAnimation())
		{
			//アニメーションの再生
			modelRenderer->PlayAnimation((int)Animation::Throw, false);
			//アニメーションの再生速度の変更
			modelRenderer->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Throw));

			state = Throw;

			//回転速度を計算
			float samplingRate = modelRenderer->GetSamplingRate();
			float t = (float)modelRenderer->GetPlayAnimMaxTimer();
			rotateSpeed = maxThrowRadian / (maxRotateTime / (samplingRate * GetAnimationSpeed((int)Animation::Throw)));
			rotateAxis = { 0.0f,1.0f,0.0f };
			rotateRadian = 0.0f;

			//誰に投げつけるか決める
			enemyTransform = nullptr;
			SearchNearEnemy(enemyTransform);//一番近い敵のTransformを取得
			
			if (enemyTransform != nullptr)
			{
				//敵の位置
				Vector3 enePos = enemyTransform->pos;
				//プレイヤーから敵へのベクトル
				Vector3 vec = enePos - pos;
				//敵までの距離
				float distance = vec.Length();
				//敵までの距離が一定より離れているならクリア
				if (distance > searchRange)
				{
					enemyTransform = nullptr;
				}
			}
		}

		break;
	}
#endif
	case Throw:
#if _APPEND
	{
		//コンポーネントの取得
		ModelRenderer* modelRenderer = parent->GetComponent<ModelRenderer>();

		float animTimer = modelRenderer->GetPlayAnimTimer();
		if (animTimer < maxThrowTime)
		{
			//右手の位置を取得
			Vector3 rightHandPos = modelRenderer->GetBonePositionFromName("rightHand");
			//プレイヤーの位置
			Vector3 pos = parent->GetComponent<Transform>()->pos;

			//回転軸をプレイヤーの前方向を軸に回転する
			if (animTimer > maxRotateTime)
			{
				rotateSpeed = DirectX::XMConvertToRadians(360.0f);
				//回転角度を計算
				float speed = rotateSpeed * SystemManager::Instance().GetElapsedTime();
				rotateRadian += speed;
				constexpr float maxRad = DirectX::XMConvertToRadians(90.0f);
				if (rotateRadian > maxRad)
				{
					speed -= rotateRadian - maxRad;
					rotateRadian = maxRad;
				}

				//プレイヤーの前方向
				Vector3 axis = parent->GetComponent<Transform>()->GetForward();
				//クオータニオンの作成
				const Quaternion Q = Quaternion::CreateFromAxisAngle(axis, speed);
				//回転行列の作成
				const Matrix R = Matrix::CreateFromQuaternion(Q);

				//ベクトルを回転させる
				Vector4 v = Float4MultiplyFloat4x4({ rotateAxis.x,rotateAxis.y,rotateAxis.z,1.0f }, R);

				rotateAxis = { v.x,v.y,v.z };
			}

			//クオータニオンの作成
			const Quaternion Q = Quaternion::CreateFromAxisAngle(rotateAxis, -rotateSpeed * SystemManager::Instance().GetElapsedTime());

			//回転行列の作成
			const Matrix R = Matrix::CreateFromQuaternion(Q);

			//ベクトルを回転させる
			Vector4 v = Float4MultiplyFloat4x4({ rotateVec.x,rotateVec.y,rotateVec.z,1.0f }, R);
			rotateVec = { v.x,v.y,v.z };

			//オブジェクトの位置の更新
			GameObject* throwObj = parent->GetComponent<Player>()->GetThrowObj();
			Vector3 objPos = throwObj->GetComponent<Transform>()->pos;

			objPos = pos + rotateVec;
			throwObj->GetComponent<Transform>()->pos = objPos;

			//カプセルを取得
			CapsuleCollider* capsule = parent->GetComponent<CapsuleCollider>("WireCapsule");
			//カプセルの位置を更新
			capsule->begin = objPos;
			capsule->end = rightHandPos;
		}

		//腰の位置のモーションによる移動を停止
		modelRenderer->StopMotionXZVelocity("IdleHip");

		break;
	}
#endif
	}

	Default::Update();
}
void WieldThrow::Exit()
{
	//カプセルを非表示に
	parent->GetComponent<CapsuleCollider>("WireCapsule")->SetEnable(false);
}
std::string WieldThrow::GetNext()
{
	//アニメーション再生が終わったら待機ステートへ遷移
	if (state == Throw && parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Idle";
	}

	//変更なし
	return "";
}

void WieldThrow::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		float degree = DirectX::XMConvertToDegrees(maxWieldRadian);
		ImGui::DragFloat("maxWieldDegree", &degree);
		maxWieldRadian = DirectX::XMConvertToRadians(degree);
		
		ImGui::DragFloat("maxThrowTime", &maxThrowTime);

		ImGui::TreePop();
	}
}