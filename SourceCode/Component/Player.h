#pragma once

#include <DirectXMath.h>

#include "Component.h"

#include "PlayerStates.h"

class Player : public Component
{
public:
	Player() = default;
	~Player() override = default;

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

	void OnCollisionEnter(Collider* collider) override;

	//-----< Getter, Setter >-----//
	float GetHeight() { return height; }
	float GetRunSpeed() { return runSpeed; }
	float GetJumpSpeed() { return jumpSpeed; }
	float GetWireSpeed() { return wireSpeed; }

	float GetAttackPower() { return currentState->GetAttackPower(); }
	float GetAttackInterval() { return currentState->GetAttackInterval(); }

	void SetIsDead(bool b) { isDead = b; }
	bool GetIsDead() { return isDead; }

	bool GetIsHitAttackToEnemy() { return isHitAttackToEnemy; }
	void SetIsHitAttackToEnemy(bool b) { isHitAttackToEnemy = b; }

	bool GetIsAttack() { return isAttack; }
	void SetIsAttack(bool flag) { isAttack = flag; }

private:

	//-----< �֐� >-----//
	void ChangeState(std::string nextStateName);


	//-----< �ϐ� >-----//
	std::vector<PlayerState::Default*> states;//���
	PlayerState::Default* currentState = nullptr;

	float height = 6.0f;//�v���C���[�̐g��

	float runSpeed = 40.0f;//���鑬��
	float jumpSpeed = 50.0f;//�W�����v�̍���
	float knockBackSpeed = 10.0f;//�m�b�N�o�b�N�̋���
	float wireSpeed = 100.0f;//���C���[�ړ��̃X�s�[�h

	bool isDead = false;//���S�Ɏ���ł��邩�ǂ���

	float stepDegree = 30.0f;

	bool isHitAttackToEnemy = false;//�G�p�̍U�����������Ă��邩�̃t���O

	bool isAttack = false;//�v���C���[���U���X�e�[�g���ǂ���
};