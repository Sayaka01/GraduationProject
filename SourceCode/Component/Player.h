#pragma once

#include <DirectXMath.h>

#include "Component.h"

#include "PlayerStates.h"

class Player : public Component
{
public:
	Player() = default;
	~Player() override = default;

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

	void OnCollisionEnter(Collider* collider) override;

	//-----< Getter, Setter >-----//
	float GetRunSpeed() { return runSpeed; }
	float GetJumpSpeed() { return jumpSpeed; }
	float GetWireSpeed() { return wireSpeed; }

	float GetAttackPower() { return currentState->GetAttackPower(); }

private:

	//-----< 関数 >-----//
	void ChangeState(std::string nextStateName);


	//-----< 変数 >-----//
	std::vector<PlayerState::Default*> states;//状態
	PlayerState::Default* currentState = nullptr;

	float runSpeed = 40.0f;//走る速さ
	float jumpSpeed = 50.0f;//ジャンプの高さ
	float knockBackSpeed = 10.0f;//ノックバックの強さ
	float wireSpeed = 75.0f;//ワイヤー移動のスピード

};