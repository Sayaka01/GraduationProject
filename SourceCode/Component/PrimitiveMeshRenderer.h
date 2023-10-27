#pragma once

#include <DirectXMath.h>

#include "Component.h"

#include "System/ConstantBuffer.h"

class PrimitiveMeshRenderer : public Component
{
public:
	PrimitiveMeshRenderer() = default;
	~PrimitiveMeshRenderer() override = default;

	//-----< �֐� >-----//

	//������
	void Initialize()override;
	//�`�揀��
	void DrawPrepare();
	//�`��
	void Draw()override;

protected:
	//-----< �\���� >-----//
	
	//�萔�o�b�t�@
	struct Constants
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4 color;
	}constants{};

	//-----< �ϐ� >-----//

	UINT vertexCount = 0;//���_��
	Microsoft::WRL::ComPtr <ID3D11Buffer> vertexBuffer;//���_�o�b�t�@

	ConstantBuffer constantBuffer;//�萔�o�b�t�@

	DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };//�F

private:

	//-----< �ϐ� >-----//

	Microsoft::WRL::ComPtr <ID3D11VertexShader> vertexShader;//���_�V�F�[�_�[
	Microsoft::WRL::ComPtr <ID3D11PixelShader> pixelShader;//�s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr <ID3D11InputLayout> inputLayout;//�C���v�b�g���C�A�E�g



};