#pragma once
#include <DirectXMath.h>

#include "../Component/Component.h"

#include "System/ConstantBuffer.h"

class Health : public Component
{
public:
	Health(float maxHp) :maxHp(maxHp) { currentHp = maxHp; }
	~Health()override = default;

	//ŠÖ”
	//void Draw()override;
	//void DebugGui()override;

	void SetHealth(float value) { maxHp = value; }

	//Œ¸Z
	void SubtructHp(float value) { currentHp -= value; }

	//‰ÁZ
	void AddHp(float value) { currentHp += value; }

	//Œ»İ‚ÌHP‚ÌŠ„‡
	float GetHpRate() { return currentHp / maxHp; }

	enum class HealthState
	{
		health,//Œ’N
		wrong,//•sŒ’N
	};

private:

	float currentHp{ 0.0f };//Œ»İ‚ÌHP
	float maxHp{ 10.0f };//Å‘å‚ÌHP

	HealthState state = HealthState::health;


};