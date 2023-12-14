#pragma once

#include <SimpleMath.h>

#include "Component.h"

class ThrowObstacle : public Component
{
public:
	ThrowObstacle() = default;
	~ThrowObstacle() override = default;

	//-----< ä÷êî >-----//

	//èâä˙âª
	void Initialize() override;
	//çXêV
	void Update() override;
	//ImGui
	void DebugGui() override;

	//setter, getter
	bool GetThrowFlag() { return throwFlag; }
	void OnThrowFlag();
	void Throw();

private:
	bool throwFlag = false;
	DirectX::SimpleMath::Vector3 throwVec{};
	float throwSpeed = 0.0f;
	DirectX::SimpleMath::Vector3 targetPos{};
	float floorPos = 2.0f;
};