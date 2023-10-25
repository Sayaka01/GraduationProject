#pragma once

#include <DirectXMath.h>

#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider();
	~SphereCollider() override;

	//-----< �֐� >-----//

	//ImGui
	void DebugGui() override;

	virtual void OnCollisionEnter() override {}
	virtual void OnCollisionExit() override {}
	virtual void OnCollisionStay() override {}

	//-----< Getter, Setter >-----//

	//-----< �\���� >-----//

	//-----< �ϐ� >-----//
	DirectX::XMFLOAT3 center = { 0.0f,0.0f,0.0f };
	float radius = 1.0f;
};