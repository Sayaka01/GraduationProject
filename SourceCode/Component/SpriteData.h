#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <vector>
#include <wrl.h>

#include "System/ConstantBuffer.h"

#include "System/Texture.h"

class SpriteData
{

public:
	SpriteData(const wchar_t* filePath, size_t maxSprites = 1);
	~SpriteData();

	void Draw(
		DirectX::XMFLOAT2 pos,//矩形の左上の座標（スクリーン座標系）
		DirectX::XMFLOAT2 scale,//描画するスケール
		DirectX::XMFLOAT2 pivot,//基準点
		DirectX::XMFLOAT2 texPos, //描画する矩形の左上座標
		DirectX::XMFLOAT2 texSize,//描画する矩形のサイズ
		float degree,//degree
		DirectX::XMFLOAT4 color//カラー
	);

	void Begin(ID3D11PixelShader* replacedPixelShader = nullptr, ID3D11VertexShader* replacedVertexShader = nullptr);

	void End();

	[[nodiscard]] float GetTextureWidth() const { return static_cast<float>(texture.GetTexture2dDesc().Width); }
	[[nodiscard]] float GetTextureHeight() const { return static_cast<float>(texture.GetTexture2dDesc().Height); }
	[[nodiscard]] DirectX::XMFLOAT2 GetTextureSize() const { return { static_cast<float>(texture.GetTexture2dDesc().Width),static_cast<float>(texture.GetTexture2dDesc().Height) }; }

private:
	//頂点フォーマット
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texCoord;
	};

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

	Texture texture;

	const size_t maxVertices;
	std::vector<Vertex> vertices;

};
