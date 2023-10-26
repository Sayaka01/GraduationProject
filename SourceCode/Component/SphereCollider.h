#pragma once

#include <DirectXMath.h>
#include <string>

#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider();
	SphereCollider(std::string compName);
	~SphereCollider() override;

	//-----< ŠÖ” >-----//

	//ImGui
	void DebugGui() override;

	//-----< Getter, Setter >-----//

	//-----< \‘¢‘Ì >-----//

	//-----< •Ï” >-----//
	float radius = 1.0f;
};