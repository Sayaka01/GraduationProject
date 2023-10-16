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

	bool UpdateState() {}

	//-----< Getter, Setter >-----//


private:

	//-----< 関数 >-----//


	//-----< 変数 >-----//
	//std::vector<PlayerState::Default*> states;//状態
	//PlayerState::Default* currentState = nullptr;

	DirectX::XMFLOAT3 moveVelocity = {};//移動ベクトル

	BehaviorTree* aiTree{ nullptr };
	BehaviorData* behaviorData{ nullptr };
	NodeBase* activeNode{ nullptr };

};