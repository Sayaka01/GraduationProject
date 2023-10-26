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
	DirectX::XMFLOAT3 GetMoveVelocity() { return moveVelocity; }
	void SetMoveVelocity(DirectX::XMFLOAT3 velocity) { moveVelocity = velocity; }


private:

	//-----< 関数 >-----//
	void ChangeState(std::string nextStateName);


	//-----< 変数 >-----//
	std::vector<PlayerState::Default*> states;//状態
	PlayerState::Default* currentState = nullptr;

	DirectX::XMFLOAT3 moveVelocity = {};//移動ベクトル
};