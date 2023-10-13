#pragma once

#include <DirectXMath.h>

#include "Component.h"

#include "PlayerStates.h"

class Enemy : public Component
{
public:
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

	virtual bool ActRun() = 0;
	virtual bool ActWalk() = 0;
	virtual bool ActAttack() = 0;
	virtual bool ActDamage() = 0;
	virtual bool ActEscape() = 0;
	virtual bool ActDie() = 0;

	//-----< Getter, Setter >-----//


private:

	//-----< �֐� >-----//
	void ChangeState(std::string nextStateName);


	//-----< �ϐ� >-----//
	//std::vector<PlayerState::Default*> states;//���
	//PlayerState::Default* currentState = nullptr;

	DirectX::XMFLOAT3 moveVelocity = {};//�ړ��x�N�g��
};