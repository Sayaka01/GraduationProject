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

	//-----< ŠÖ” >-----//

	//ImGui
	void DebugGui() override;

	//-----< Getter, Setter >-----//

	//-----< \‘¢‘Ì >-----//

	//-----< •Ï” >-----//
	DirectX::XMFLOAT3 size = { 1.0f,1.0f,1.0f };
};