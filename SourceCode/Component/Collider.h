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


	//-----< �\���� >----//
	enum class Type
	{
		Offense,//�U������p
		Deffense,//���炢����p
	};
	Type type = Type::Deffense;

	//-----< getter, setter >-----//


	//-----< �ϐ� >-----//
	DirectX::XMFLOAT3 center = {};//�R���C�_�[�̒����ʒu�i�J�v�Z���ł͕s�g�p�j
	bool drawDebugPrimitive = true;//�f�o�b�O�`����s����
	bool drawReleasePrimitive = false;//�f�o�b�O�`����s����
	int priority = 0;//�����蔻��̗D��x�i�O����ԗD��x�����j
	bool useHitEvent = true;//�����蔻����s�����ǂ���

	DirectX::XMFLOAT4 debugColor = { 1,1,1,1 };

private:
	void (Component::*HitProcessFunc)(Collider* collider);
	Component* component = nullptr;

};