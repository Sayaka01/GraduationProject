#pragma once

#include <DirectXMath.h>

#include "Component.h"

class Collider : public Component
{
public:
	Collider() = default;
	~Collider() override = default;

	//-----< 関数 >-----//
	virtual void OnCollisionEnter() {}
	virtual void OnCollisionExit() {}
	virtual void OnCollisionStay() {}

	//-----< Getter, Setter >-----//

	//-----< 構造体 >-----//

protected:
	//-----< 変数 >-----//
	bool hitFlag = false;
	bool drawDebugPrimitive = true;

};