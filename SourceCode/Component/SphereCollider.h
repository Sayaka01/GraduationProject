#pragma once

#include <DirectXMath.h>

#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider();
	~SphereCollider() override;

	//-----< 関数 >-----//

	//ImGui
	void DebugGui() override;

	//-----< Getter, Setter >-----//

	//-----< 構造体 >-----//

	//-----< 変数 >-----//
	float radius = 1.0f;
};