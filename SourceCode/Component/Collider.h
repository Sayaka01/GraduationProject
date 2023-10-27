#pragma once

#include <DirectXMath.h>

#include "Component.h"

class Collider : public Component
{
public:
	Collider() = default;
	~Collider() override = default;

	//-----< �֐� >-----//

	//ImGui
	void DebugGui()override;

	//�Փˌ�̏���
	void SetHitProcessFunc(Component* component, void (Component::* pFunc)(Collider* collider)) { this->component = component; HitProcessFunc = pFunc; }
	void OnCollisionEnter(Collider* collider);


	//-----< getter, setter >-----//
	DirectX::XMFLOAT3 GetCenter() { return center; }
	void SetCenter(DirectX::XMFLOAT3 center) { this->center = center; }

	//-----< �ϐ� >-----//
	bool drawDebugPrimitive = true;//�f�o�b�O�`����s����
	int priority = 0;//�����蔻��̗D��x�i�O����ԗD��x�����j

private:
	void (Component::*HitProcessFunc)(Collider* collider);
	Component* component = nullptr;

	DirectX::XMFLOAT3 center = {};
};