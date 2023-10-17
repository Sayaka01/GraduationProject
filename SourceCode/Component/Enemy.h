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
	// �A�j���[�V�����X�e�[�g
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

	//-----< �֐� >-----//

	//������
	void Initialize() override;
	//�X�V
	void Update() override;
	//�`��
	void Draw() override;
	//�I������
	void Finalize()override;
	//ImGui
	void DebugGui() override;

	bool UpdateState() {}

	//-----< Getter, Setter >-----//

	void SetRunTimer(float time) { runTimer = time; }


	// �p���`�A�N�V����
	virtual void EnterPunchState();
	virtual bool RunPunchState();
	virtual void ExitPunchState();

	// �d���A�N�V����
	virtual void EnterSkillState();
	virtual bool RunSkillState();
	virtual void ExitSkillState();

	// �x�e�A�N�V����
	virtual void EnterBreakState();
	virtual bool RunBreakState();
	virtual void ExitBreakState();

	// �p�j�A�N�V����
	virtual void EnterWanderState();
	virtual bool RunWanderState();
	virtual void ExitWanderState();

	// �ҋ@�A�N�V����
	virtual void EnterIdleState();
	virtual bool RunIdleState();
	virtual void ExitIdleState();

	// �ǐՃA�N�V����
	virtual void EnterPursuitState();
	virtual bool RunPursuitState();
	virtual void ExitPursuitState();

	// �����A�N�V����
	virtual void EnterEscapeState();
	virtual bool RunEscapeState();
	virtual void ExitEscapeState();

	// ���S�A�N�V����
	virtual void EnterDieState();
	virtual bool RunDieState();
	virtual void ExitDieState();

	// ���S�A�N�V����
	virtual void EnterDamageState();
	virtual bool RunDamageState();
	virtual void ExitDamageState();


private:

	//-----< �֐� >-----//


	//-----< �ϐ� >-----//
	//std::vector<PlayerState::Default*> states;//���
	//PlayerState::Default* currentState = nullptr;

	DirectX::XMFLOAT3 moveVelocity = {};//�ړ��x�N�g��

	BehaviorTree* aiTree{ nullptr };
	BehaviorData* behaviorData{ nullptr };
	NodeBase* activeNode{ nullptr };

	float runTimer = 0.0f;

};