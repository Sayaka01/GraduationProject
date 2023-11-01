#pragma once

#include <DirectXMath.h>

#include "Component.h"

#include "PlayerStates.h"

class Player : public Component
{
public:
	Player() = default;
	~Player() override = default;

	//-----< ŠÖ” >-----//

	//‰Šú‰»
	void Initialize() override;
	//XV
	void Update() override;
	//•`‰æ
	void Draw() override;
	//I—¹ˆ—
	void Finalize()override;
	//ImGui
	void DebugGui() override;

	void OnCollisionEnter(Collider* collider) override;

	//-----< Getter, Setter >-----//
	float GetRunSpeed() { return runSpeed; }

private:

	//-----< ŠÖ” >-----//
	void ChangeState(std::string nextStateName);


	//-----< •Ï” >-----//
	std::vector<PlayerState::Default*> states;//ó‘Ô
	PlayerState::Default* currentState = nullptr;

	float runSpeed = 30.0f;

};