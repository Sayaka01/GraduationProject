#pragma once

#include <DirectXMath.h>

#include "Component.h"

class Collider : public Component
{
public:
	Collider() = default;
	~Collider() override = default;

	//-----< ŠÖ” >-----//
	virtual void OnCollisionEnter() {}
	virtual void OnCollisionExit() {}
	virtual void OnCollisionStay() {}

	//-----< Getter, Setter >-----//

	//-----< \‘¢‘Ì >-----//

protected:
	//-----< •Ï” >-----//
	bool hitFlag = false;
	bool drawDebugPrimitive = true;

};