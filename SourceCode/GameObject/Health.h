#pragma once
#include <DirectXMath.h>

#include "../Component/Component.h"

#include "System/ConstantBuffer.h"

class Health : public Component
{
public:
	Health(float maxHp) :maxHp(maxHp) { currentHp = maxHp; }
	~Health()override = default;

	//�֐�
	//void Draw()override;
	//void DebugGui()override;

	void SetHealth(float value) { maxHp = value; }

	//���Z
	void SubtructHp(float value) { currentHp -= value; }

	//���Z
	void AddHp(float value) { currentHp += value; }

	//���݂�HP�̊���
	float GetHpRate() { return currentHp / maxHp; }

	enum class HealthState
	{
		health,//���N
		wrong,//�s���N
	};

private:

	float currentHp{ 0.0f };//���݂�HP
	float maxHp{ 10.0f };//�ő��HP

	HealthState state = HealthState::health;


};