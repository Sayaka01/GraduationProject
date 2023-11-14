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
	Enemy(char num) :ownIndex(num) {}
	~Enemy() override = default;

	//-----< �֐� >-----//

	//������
	void Initialize() override;
	//�X�V
	void Update() override;
	//�I������
	void Finalize()override;
	//ImGui
	void DebugGui() override;

	// AI�̃c���[�ݒ�
	void SetBehaviorTree();

	// �A�j���[�V�����̐؂�ւ�
	void ChangeAnimation(AnimationName animeIndex, bool isLoop);

	//�^�[�Q�b�g��͈͓��Ń����_���ɐݒ�
	void SetTargetPositionRandom(float maxRange)
	{
		float theta = Random::Range(0, DirectX::XM_PI*2);
		float range = Random::Range(0.0f, maxRange);
		targetPosition.x = parent->GetComponent<Transform>()->pos.x + sinf(theta) * range;
		targetPosition.y = parent->GetComponent<Transform>()->pos.y;
		targetPosition.z = parent->GetComponent<Transform>()->pos.z + cosf(theta) * range;
	}

	//�ړI�ʒu�ֈړ�����
	void MoveToTargetPosition(float elapsedTime);

	// �ړI�ʒu�܂ł̋������擾
	float GetLengthToTargetPosition()
	{
		Transform* ownerTransform = parent->GetComponent<Transform>();

		// �v���C���[�Ǝ��g�̃x�N�g�����v�Z
		DirectX::SimpleMath::Vector3 vector = targetPosition - ownerTransform->pos;

		// �x�N�g���̒���
		return vector.Length();
	}

	// �p����] v:�i�s����
	void RotateTransform(DirectX::SimpleMath::Vector3 v, float elapsedTime);

	// ���肪����������
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
private:

	//-----< �֐� >-----//


	//-----< �ϐ� >-----//
	//std::vector<PlayerState::Default*> states;//���
	//PlayerState::Default* currentState = nullptr;

	//DirectX::XMFLOAT3 moveVelocity = {};//�ړ��x�N�g��

	BehaviorTree* aiTree{ nullptr };
	BehaviorData* behaviorData{ nullptr };
	NodeBase* activeNode{ nullptr };

	float runTimer = 0.0f;//�s������
	DirectX::XMFLOAT3 targetPosition{ 0.0f,0.0f,0.0f };

	//���݂̍U����
	float currentAttackPower{ 0.0f };


	//��]���x
	float rotateSpeed = 5.0f;

	// �ړ����x
	float moveSpeed = 10.0f;

	bool hitFlag{ false };

	char ownIndex = -1;

	//-----< �萔 >-----//

	//�U�����J�n����͈�
	float attackRange{ 8.0f };

	//�ǐՂ��J�n����͈�
	float pursuitRange{ 80.0f };

	//�p�j���J�n����͈�
	float wanderRange{ 60.0f };

	//�p����]�̗�
	float rotateRatio{ 0.75f };

	//���ꂼ��̍U����
	float punchPower{ 3 };
	float slashPower{ 6 };

};