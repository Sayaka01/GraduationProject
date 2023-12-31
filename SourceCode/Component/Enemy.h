#pragma once

#include <DirectXMath.h>

#include "Component.h"
#include "Component/Transform.h"
#include "GameObject/GameObject.h"
#include "System/UserFunction.h"
#include "Component/RigidBody.h"

class BehaviorTree;
class BehaviorData;
class NodeBase;
class ActionBase;

//#define OpenConsole



class Enemy : public Component
{
public:
	// アニメーションステート
	enum class AnimationName
	{
		TakeDamege,
		Spawn,
		RightClawAttack,
		LeftSlashAttack,
		Idle2,
		Idle1,
		FlyForward,
		EyeBeamAttack,
		Die,
		DashForward,
		ChargeEyeBeam,
		CastSpell,
		BrastAttack,
	};

	enum class StateName
	{
		Idle,
		Punch,
		Slash,
		Break,
		Wander,
		Pursuit,
		Escape,
		Die,
		Damage,
	};

	Enemy() = default;
	Enemy(char num) :ownIndex(num) {}
	~Enemy() override = default;

	//-----< 関数 >-----//

	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//終了処理
	void Finalize()override;
	//ImGui
	void DebugGui() override;

	// AIのツリー設定
	void SetBehaviorTree();

	// アニメーションの切り替え
	void ChangeAnimation(AnimationName animeIndex, bool isLoop);

	//ターゲットを範囲内でランダムに設定
	void SetTargetPositionRandom(float maxRange)
	{
		float theta = Random::Range(0, DirectX::XM_PI*2);
		float range = Random::Range(0.0f, maxRange);
		targetPosition.x = sinf(theta) * range;
		targetPosition.y;
		targetPosition.z = cosf(theta) * range;
		//targetPosition.x = parent->GetComponent<Transform>()->pos.x + sinf(theta) * range;
		//targetPosition.y = parent->GetComponent<Transform>()->pos.y;
		//targetPosition.z = parent->GetComponent<Transform>()->pos.z + cosf(theta) * range;
	}

	//目的位置へ移動する
	void MoveToTargetPosition(float elapsedTime);

	// 目的位置までの距離を取得
	float GetLengthToTargetPosition()
	{
		Transform* ownerTransform = parent->GetComponent<Transform>();

		// プレイヤーと自身のベクトルを計算
		DirectX::SimpleMath::Vector3 vector = targetPosition - ownerTransform->pos;

		// ベクトルの長さ
		return vector.Length();
	}

	// 姿勢回転 v:進行方向
	void RotateTransform(DirectX::SimpleMath::Vector3 v, float elapsedTime);

	// 判定が当たったら
	void OnCollisionEnter(Collider* collider);

	//-----< Getter, Setter >-----//

	void SetRunTimer(float time) { runTimer = time; }
	float GetRunTimer() { return runTimer; }

	float GetAttackRange() { return attackRange; }
	float GetPursuitRange() { return pursuitRange; }
	float GetWanderRange() { return wanderRange; }

	void SetTargetPosition(DirectX::XMFLOAT3 pos) { targetPosition = pos; }
	DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }

	bool GetHitFlag() { return hitFlag; }

	float GetAttackPower() { return currentAttackPower; }
	void SetAttackPower(float power) { currentAttackPower = power; }

	float GetPunchAttackPower() { return punchPower; }
	float GetSlashAttackPower() { return slashPower; }

	char GetOwnIndex() { return ownIndex; }

	void SetStateName(StateName sName) { stateName = sName; }
	StateName GetStateName() { return stateName; }

	void SetIsHitPlayerAttack(bool b) { isHitPlayerAttack = b; }
	bool GetIsHitPlayerAttack() { return isHitPlayerAttack; }
private:

	//-----< 関数 >-----//


	//-----< 変数 >-----//
	//std::vector<PlayerState::Default*> states;//状態
	//PlayerState::Default* currentState = nullptr;

	//DirectX::XMFLOAT3 moveVelocity = {};//移動ベクトル

	BehaviorTree* aiTree{ nullptr };
	BehaviorData* behaviorData{ nullptr };
	NodeBase* activeNode{ nullptr };

	float runTimer = 0.0f;//行動時間
	DirectX::XMFLOAT3 targetPosition{ 0.0f,0.0f,0.0f };

	//現在の攻撃力
	float currentAttackPower{ 0.0f };


	//回転速度
	float rotateSpeed = 5.0f;

	// 移動速度
	float moveSpeed = 10.0f;

	bool hitFlag{ false };

	char ownIndex = -1;

	//-----< 定数 >-----//

	//攻撃を開始する範囲
	float attackRange{ 8.0f };

	//追跡を開始する範囲
	float pursuitRange{ 80.0f };

	//徘徊を開始する範囲
	float wanderRange{ 100.0f };

	//姿勢回転の量
	float rotateRatio{ 0.75f };

	//それぞれの攻撃力
	float punchPower{ 2 };
	float slashPower{ 4 };

	bool isHitPlayerAttack;//プレイヤーの攻撃が当たったフラグ

	StateName stateName = StateName::Idle;

};