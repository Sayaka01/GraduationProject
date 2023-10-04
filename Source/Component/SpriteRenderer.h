#pragma once

#include <DirectXMath.h>

#include "Component.h"

#include "SpriteData.h"

class SpriteRenderer : public Component
{
public:
	SpriteRenderer() = default;
	SpriteRenderer(const wchar_t* filePath, size_t max_sprites = 1) { Initialize(filePath, max_sprites); }
	~SpriteRenderer() override = default;

	//-----< ä÷êî >-----//

	//èâä˙âª
	void Initialize() override{}
	void Initialize(const wchar_t* filePath, size_t max_sprites = 1);
	//ï`âÊ
	void Draw()override;
	void Draw(ID3D11PixelShader* replacedPixelShader = nullptr, ID3D11VertexShader* replacedVertexShader = nullptr);
	//ImGui
	void DebugGui() override;

	//-----< Getter, Setter >-----//

	[[nodiscard]] float GetSpriteSizeWidth() const { return spriteData->GetTextureWidth(); }
	[[nodiscard]] float GetSpriteSizeHeight() const { return spriteData->GetTextureHeight(); }
	[[nodiscard]] DirectX::XMFLOAT2 GetSpriteSize() const { return { spriteData->GetTextureWidth(),spriteData->GetTextureHeight() }; }


	//-----< ïœêî >-----//

	std::vector<DirectX::XMFLOAT2> positions;
	DirectX::XMFLOAT2 pos = { 0.0f,0.0f };
	DirectX::XMFLOAT2 scale = { 1.0f,1.0f };
	DirectX::XMFLOAT2 pivot = { 0.0f,0.0f };
	DirectX::XMFLOAT2 texPos = { 0.0f,0.0f };
	DirectX::XMFLOAT2 texSize = { 0.0f,0.0f };
	float degree = 0.0f;
	DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };

private:
	std::unique_ptr<SpriteData> spriteData;

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

	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> shaderResourceView;
	D3D11_TEXTURE2D_DESC texture2dDesc;

	bool isLoadFile = true;


};