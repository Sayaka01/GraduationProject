#pragma once

#include <SimpleMath.h>

#include "Component.h"

class ThrowObstacle : public Component
{
public:
	ThrowObstacle() = default;
	~ThrowObstacle() override = default;

	//-----< �֐� >-----//

	//������
	void Initialize() override;
	//�X�V
	void Update() override;
	//ImGui
	void DebugGui() override;
	//�����蔻��
	void OnCollisionEnter(Collider* collider)override;

	void GraspFromPlayer();
	void ThrowFree(DirectX::SimpleMath::Vector3 velocity, float speed);
	void ThrowTarget(DirectX::SimpleMath::Vector3 targetPos, float speed);

	//setter, getter
	bool GetThrowFlag() { return throwFlag; }


private:
	bool throwFlag = false;//������ꂽ���ǂ���
	DirectX::SimpleMath::Vector3 throwVec{};//������x�N�g��
	float throwSpeed = 0.0f;//������X�s�[�h
	DirectX::SimpleMath::Vector3 targetPos{};//��������ڕW�ʒu
	bool throwTarget = false;
	float floorPos = 2.0f;
	float airResistance = 10.0f;
};
