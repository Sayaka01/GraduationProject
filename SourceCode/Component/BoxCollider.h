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

	//-----< 関数 >-----//

	//ImGui
	void DebugGui() override;

	//-----< Getter, Setter >-----//

	//-----< 構造体 >-----//

	//-----< 変数 >-----//
	DirectX::XMFLOAT3 size = { 1.0f,1.0f,1.0f };
};