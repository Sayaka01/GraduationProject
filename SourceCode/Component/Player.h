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

	//-----< Getter, Setter >-----//


private:

	//-----< �֐� >-----//
	void ChangeState(std::string nextStateName);


	//-----< �ϐ� >-----//
	std::vector<PlayerState::Default*> states;//���
	PlayerState::Default* currentState = nullptr;

	DirectX::XMFLOAT3 moveVelocity = {};//�ړ��x�N�g��
};