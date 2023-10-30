#pragma once

#include <DirectXMath.h>
#include <string>

#include "Collider.h"

class CapsuleCollider : public Collider
{
public:
	CapsuleCollider();
	CapsuleCollider(std::string compName);
	~CapsuleCollider() override;

	//-----< �֐� >-----//

	//ImGui
	void DebugGui() override;

	//�C�ӎ���]
	void CalcCapsuleParam();

	//�����ʒu�̍X�V
	void UpdateCenter();

	//-----< Getter, Setter >-----//

	//-----< �\���� >-----//

	//-----< �ϐ� >-----//
	DirectX::XMFLOAT3 begin{};
	DirectX::XMFLOAT3 end{};
	float radius = 1.0f;
	float cylinderSize = 1.0f;
	DirectX::XMFLOAT3 radian{};
	DirectX::XMFLOAT4 quaternion;
};