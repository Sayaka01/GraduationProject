#pragma once

#include <DirectXMath.h>
#include <SimpleMath.h>

#include "Component.h"

class Transform : public Component
{
public:
	Transform() = default;
	~Transform() override = default;

	//-----< 関数 >-----//

	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//ImGui
	void DebugGui() override;

	//-----< Getter, Setter >-----//
	DirectX::XMFLOAT3 GetRight()
	{ 
		DirectX::SimpleMath::Vector3 right = { world._11,world._12,world._13 };
		right.Normalize();
		return right; 
	}
	DirectX::XMFLOAT3 GetUp() 
	{
		DirectX::SimpleMath::Vector3 up = { world._21,world._22,world._23 };
		up.Normalize();
		return up;
	}
	DirectX::XMFLOAT3 GetForward() 
	{
		DirectX::SimpleMath::Vector3 forward = { world._31,world._32,world._33 };
		forward.Normalize();
		return forward;
	}

	//-----< 構造体 >-----//

	//座標変換行列
	enum class CoordinateSystem
	{
		RHS_YUP,
		LHS_YUP,
		RHS_ZUP,
		LHS_ZUP,
	};

	DirectX::XMFLOAT4X4 GetCoordinateSystemTransform(float scaleFactor = 1.0f);
	CoordinateSystem coordinateSystem;

	//-----< 変数 >-----//

	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };//位置
	DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };//スケール
	DirectX::XMFLOAT4 orientation = { 0.0f,0.0f,0.0f,1.0f };//回転
	DirectX::XMFLOAT4X4 world{};


};