#pragma once

#include <DirectXMath.h>

#include "Component.h"

class Collider : public Component
{
public:
	Collider() = default;
	~Collider() override = default;

	//-----< 関数 >-----//
	
	//更新
	void Update()override;

	//ImGui
	void DebugGui()override;

	//衝突後の処理
	void SetHitProcessFunc(Component* component, void (Component::* pFunc)(Collider* collider)) { this->component = component; HitProcessFunc = pFunc; }
	void OnCollisionEnter(Collider* collider);


	//-----< 構造体 >----//
	enum class Type
	{
		Offense,//攻撃判定用
		Deffense,//くらい判定用
	};
	Type type = Type::Deffense;

	enum class Priority
	{
		None = -1,
		Obstacle,//ThrowObject etc.
		PlayerAttack,//playerの攻撃判定
		EnemyAttack,//enemyの攻撃判定
		PlayerBody,//playerのくらい判定
		EnemyBody,//enemyのくらい判定
		PriorityCount,
	};
	Priority priority = Priority::None;//当たり判定の優先度（０が一番優先度高い）

	//-----< getter, setter >-----//


	//-----< 変数 >-----//
	DirectX::XMFLOAT3 center = {};//コライダーの中央位置（カプセルでは不使用）
	bool drawDebugPrimitive = true;//デバッグ描画を行うか
	bool drawReleasePrimitive = false;//デバッグ描画を行うか
	bool useHitEvent = true;//当たり判定を行うかどうか

	DirectX::XMFLOAT4 debugColor = { 1,1,1,1 };

	bool useTransform = false;//parentのTransformコンポーネントを参照するかどうか

private:
	void (Component::*HitProcessFunc)(Collider* collider);
	Component* component = nullptr;

};