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

	//ä÷êî
	//void Draw()override;
	
	void Update() override;

	//ImGui
	void DebugGui() override;

	void SetHealth(float value) { maxHp = value; }

	//å∏éZ
	void SubtructHp(float value) { currentHp -= value; }

	//â¡éZ
	void AddHp(float value) { currentHp += value; }

	//åªç›ÇÃHPÇÃäÑçá
	float GetHpRate() { return currentHp / maxHp; }

	enum class HealthState
	{
		health,//åíçN
		wrong,//ïsåíçN
	};

private:

	float currentHp{ 0.0f };//åªç›ÇÃHP
	float maxHp{ 10.0f };//ç≈ëÂÇÃHP

	HealthState state = HealthState::health;


};