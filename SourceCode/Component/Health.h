#pragma once
#include <DirectXMath.h>

#include "../Component/Component.h"

#include "System/ConstantBuffer.h"

class Health : public Component
{
public:
	Health(float maxHp) :maxHp(maxHp) { name = "hp"; currentHp = maxHp; }
	Health(std::string compName, float maxHp) :maxHp(maxHp) { name = compName; currentHp = maxHp; }
	~Health()override = default;

	//Ö
	//void Draw()override;
	
	void Update() override;

	//ImGui
	void DebugGui() override;

	void SetHealth(float value) { maxHp = value; }

	//¸Z
	void SubtructHp(float value) { currentHp -= value; }

	//ÁZ
	void AddHp(float value) { currentHp += value; }

	//ťÝĚHPĚ
	float GetHpRate() { return currentHp / maxHp; }

	//ś
	bool GetIsAlive() { return (currentHp > 0.0f); }

	enum class HealthState
	{
		health,//N
		wrong,//sN
	};

private:

	float currentHp{ 0.0f };//ťÝĚHP
	float maxHp{ 10.0f };//ĹĺĚHP

	HealthState state = HealthState::health;


};