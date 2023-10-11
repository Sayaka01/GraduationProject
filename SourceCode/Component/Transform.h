#pragma once

#include <DirectXMath.h>

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
	//描画
	void Draw() override;
	//ImGui
	void DebugGui() override;

	//-----< Getter, Setter >-----//


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