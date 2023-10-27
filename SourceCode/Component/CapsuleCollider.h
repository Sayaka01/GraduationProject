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

	//-----< 関数 >-----//

	//ImGui
	void DebugGui() override;

	//任意軸回転
	void CalcCapsuleParam(DirectX::XMFLOAT3 begin, DirectX::XMFLOAT3 end);

	//中央位置の更新
	void UpdateCenter();

	//-----< Getter, Setter >-----//

	//-----< 構造体 >-----//

	//-----< 変数 >-----//
	DirectX::XMFLOAT3 begin{};
	DirectX::XMFLOAT3 end{};
	float radius = 1.0f;
	float cylinderSize = 1.0f;
	DirectX::XMFLOAT3 radian{};
	DirectX::XMFLOAT4 quaternion;
};