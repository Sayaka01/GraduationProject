#pragma once

#include <DirectXMath.h>

#include "Component.h"

class Collider : public Component
{
public:
	Collider() = default;
	~Collider() override = default;

	//-----< 関数 >-----//

	//ImGui
	void DebugGui()override;

	//衝突後の処理
	void SetHitProcessFunc(Component* component, void (Component::* pFunc)(Collider* collider)) { this->component = component; HitProcessFunc = pFunc; }
	void OnCollisionEnter(Collider* collider);


	//-----< 変数 >-----//
	DirectX::XMFLOAT3 center = {};
	bool drawDebugPrimitive = true;//デバッグ描画を行うか
	int priority = 0;//当たり判定の優先度（０が一番優先度高い）

private:
	void (Component::*HitProcessFunc)(Collider* collider);
	Component* component = nullptr;
};