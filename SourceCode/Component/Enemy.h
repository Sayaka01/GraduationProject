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

	Enemy() = default;
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

	// アニメーションの切り替え
	void ChangeAnimation(AnimationName animeIndex, bool isLoop);

	//ターゲットを範囲内でランダムに設定
	void SetTargetPositionRandom(float maxRange)
	{
		float theta = Random::Range(0, DirectX::XM_PI*2);
		float range = Random::Range(0.0f, maxRange);
		targetPosition.x = parent->GetComponent<Transform>()->pos.x + sinf(theta) * range;
		targetPosition.y = parent->GetComponent<Transform>()->pos.y;
		targetPosition.z = parent->GetComponent<Transform>()->pos.z + cosf(theta) * range;
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

	// 姿勢回転
	void RotateTransform(float elapsedTime);

	//-----< Getter, Setter >-----//

	void SetRunTimer(float time) { runTimer = time; }
	float GetRunTimer() { return runTimer; }

	float GetAttackRange() { return attackRange; }
	float GetPursuitRange() { return pursuitRange; }
	float GetWanderRange() { return wanderRange; }

	void SetTargetPosition(DirectX::XMFLOAT3 pos) { targetPosition = pos; }
	DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }
private:

	//-----< 関数 >-----//


	//-----< 変数 >-----//
	//std::vector<PlayerState::Default*> states;//状態
	//PlayerState::Default* currentState = nullptr;

	DirectX::XMFLOAT3 moveVelocity = {};//移動ベクトル

	BehaviorTree* aiTree{ nullptr };
	BehaviorData* behaviorData{ nullptr };
	NodeBase* activeNode{ nullptr };

	float runTimer = 0.0f;//行動時間
	DirectX::XMFLOAT3 targetPosition{ 0.0f,0.0f,0.0f };


	//回転速度
	float rotateSpeed = 5.0f;

	// 移動速度
	float moveSpeed = 10.0f;

	//-----< 定数 >-----//

	//攻撃を開始する範囲
	float attackRange{ 8.0f };

	//追跡を開始する範囲
	float pursuitRange{ 80.0f };

	//徘徊を開始する範囲
	float wanderRange{ 60.0f };

	float rotateRatio = 0.75f;

};