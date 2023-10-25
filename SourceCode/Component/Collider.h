#pragma once

#include <DirectXMath.h>

#include "Component.h"

class Collider : public Component
{
public:
	Collider() = default;
	~Collider() override = default;

	//-----< �֐� >-----//
	virtual void OnCollisionEnter() {}
	virtual void OnCollisionExit() {}
	virtual void OnCollisionStay() {}

	//-----< Getter, Setter >-----//

	//-----< �\���� >-----//

protected:
	//-----< �ϐ� >-----//
	bool hitFlag = false;
	bool drawDebugPrimitive = true;

};