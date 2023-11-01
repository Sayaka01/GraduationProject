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
	float GetRunSpeed() { return runSpeed; }

private:

	//-----< �֐� >-----//
	void ChangeState(std::string nextStateName);


	//-----< �ϐ� >-----//
	std::vector<PlayerState::Default*> states;//���
	PlayerState::Default* currentState = nullptr;

	float runSpeed = 30.0f;

};