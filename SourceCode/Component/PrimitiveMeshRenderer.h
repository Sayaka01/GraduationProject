#pragma once

#include <DirectXMath.h>

#include "Component.h"

#include "System/ConstantBuffer.h"

class PrimitiveMeshRenderer : public Component
{
public:
	PrimitiveMeshRenderer() = default;
	~PrimitiveMeshRenderer() override = default;

	//-----< 関数 >-----//

	//初期化
	void Initialize()override;
	//描画準備
	void DrawPrepare();
	//描画
	void Draw()override;

protected:
	//-----< 構造体 >-----//
	
	//定数バッファ
	struct Constants
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4 color;
	}constants{};

	//-----< 変数 >-----//

	UINT vertexCount = 0;//頂点数
	Microsoft::WRL::ComPtr <ID3D11Buffer> vertexBuffer;//頂点バッファ

	ConstantBuffer constantBuffer;//定数バッファ

	DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };//色

private:

	//-----< 変数 >-----//

	Microsoft::WRL::ComPtr <ID3D11VertexShader> vertexShader;//頂点シェーダー
	Microsoft::WRL::ComPtr <ID3D11PixelShader> pixelShader;//ピクセルシェーダー
	Microsoft::WRL::ComPtr <ID3D11InputLayout> inputLayout;//インプットレイアウト



};