#pragma once

#include <DirectXMath.h>

#include "Component.h"

#include "PlayerStates.h"

class BehaviorTree;
class BehaviorData;
class NodeBase;
class ActionBase;

#define OpenConsole



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
	//描画
	void Draw() override;
	//終了処理
	void Finalize()override;
	//ImGui
	void DebugGui() override;

	bool UpdateState() {}

	//-----< Getter, Setter >-----//

	void SetRunTimer(float time) { runTimer = time; }


	// パンチアクション
	virtual void EnterPunchState();
	virtual bool RunPunchState();
	virtual void ExitPunchState();

	// 重撃アクション
	virtual void EnterSkillState();
	virtual bool RunSkillState();
	virtual void ExitSkillState();

	// 休憩アクション
	virtual void EnterBreakState();
	virtual bool RunBreakState();
	virtual void ExitBreakState();

	// 徘徊アクション
	virtual void EnterWanderState();
	virtual bool RunWanderState();
	virtual void ExitWanderState();

	// 待機アクション
	virtual void EnterIdleState();
	virtual bool RunIdleState();
	virtual void ExitIdleState();

	// 追跡アクション
	virtual void EnterPursuitState();
	virtual bool RunPursuitState();
	virtual void ExitPursuitState();

	// 逃走アクション
	virtual void EnterEscapeState();
	virtual bool RunEscapeState();
	virtual void ExitEscapeState();

	// 死亡アクション
	virtual void EnterDieState();
	virtual bool RunDieState();
	virtual void ExitDieState();

	// 死亡アクション
	virtual void EnterDamageState();
	virtual bool RunDamageState();
	virtual void ExitDamageState();


private:

	//-----< 関数 >-----//


	//-----< 変数 >-----//
	//std::vector<PlayerState::Default*> states;//状態
	//PlayerState::Default* currentState = nullptr;

	DirectX::XMFLOAT3 moveVelocity = {};//移動ベクトル

	BehaviorTree* aiTree{ nullptr };
	BehaviorData* behaviorData{ nullptr };
	NodeBase* activeNode{ nullptr };

	float runTimer = 0.0f;

};