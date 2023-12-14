#pragma once

#include <SimpleMath.h>

#include "Component.h"

class ThrowObstacle : public Component
{
public:
	ThrowObstacle() = default;
	~ThrowObstacle() override = default;

	//-----< 関数 >-----//

	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//ImGui
	void DebugGui() override;
	//当たり判定
	void OnCollisionEnter(Collider* collider)override;

	void GraspFromPlayer();
	void ThrowFree(DirectX::SimpleMath::Vector3 velocity, float speed);
	void ThrowTarget(DirectX::SimpleMath::Vector3 targetPos, float speed);

	//setter, getter
	bool GetThrowFlag() { return throwFlag; }


private:
	bool throwFlag = false;//投げられたかどうか
	DirectX::SimpleMath::Vector3 throwVec{};//投げるベクトル
	float throwSpeed = 0.0f;//投げるスピード
	DirectX::SimpleMath::Vector3 targetPos{};//投げつける目標位置
	bool throwTarget = false;
	float floorPos = 2.0f;
	float airResistance = 10.0f;
};
