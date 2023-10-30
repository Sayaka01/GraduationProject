#pragma once

#include <DirectXMath.h>
#include <string>

#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider();
	BoxCollider(std::string compName);
	~BoxCollider() override;

	//-----< �֐� >-----//

	//ImGui
	void DebugGui() override;

	//-----< Getter, Setter >-----//

	//-----< �\���� >-----//

	//-----< �ϐ� >-----//
	DirectX::XMFLOAT3 size = { 1.0f,1.0f,1.0f };
};