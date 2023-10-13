#pragma once

#include <DirectXMath.h>

#include "Component.h"

#include "PlayerStates.h"

class Enemy : public Component
{
public:
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

	virtual bool ActRun() = 0;
	virtual bool ActWalk() = 0;
	virtual bool ActAttack() = 0;
	virtual bool ActDamage() = 0;
	virtual bool ActEscape() = 0;
	virtual bool ActDie() = 0;

	//-----< Getter, Setter >-----//


private:

	//-----< 関数 >-----//
	void ChangeState(std::string nextStateName);


	//-----< 変数 >-----//
	//std::vector<PlayerState::Default*> states;//状態
	//PlayerState::Default* currentState = nullptr;

	DirectX::XMFLOAT3 moveVelocity = {};//移動ベクトル
};