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
		DirectX::XMFLOAT2 pos,//��`�̍���̍��W�i�X�N���[�����W�n�j
		DirectX::XMFLOAT2 scale,//�`�悷��X�P�[��
		DirectX::XMFLOAT2 pivot,//��_
		DirectX::XMFLOAT2 texPos, //�`�悷���`�̍�����W
		DirectX::XMFLOAT2 texSize,//�`�悷���`�̃T�C�Y
		float degree,//degree
		DirectX::XMFLOAT4 color//�J���[
	);

	void Begin(ID3D11PixelShader* replacedPixelShader = nullptr, ID3D11VertexShader* replacedVertexShader = nullptr);

	void End();

	[[nodiscard]] float GetTextureWidth() const { return static_cast<float>(texture.GetTexture2dDesc().Width); }
	[[nodiscard]] float GetTextureHeight() const { return static_cast<float>(texture.GetTexture2dDesc().Height); }
	[[nodiscard]] DirectX::XMFLOAT2 GetTextureSize() const { return { static_cast<float>(texture.GetTexture2dDesc().Width),static_cast<float>(texture.GetTexture2dDesc().Height) }; }

private:
	//���_�t�H�[�}�b�g
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
