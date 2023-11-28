
#include "Stdafx.h"
#include "PrimitiveMeshRenderer.h"

#include "System/CreateShader.h"
#include "System/SystemManager.h"

void PrimitiveMeshRenderer::Initialize()
{
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[]
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	//���_�V�F�[�_�[�̍쐬
	CreateVsFromCso(".\\Shaders\\DebugPrimitive_VS.cso", vertexShader.GetAddressOf(),
		inputLayout.GetAddressOf(), inputElementDesc, ARRAYSIZE(inputElementDesc));
	//�s�N�Z���V�F�[�_�[�̍쐬
	CreatePsFromCso(".\\Shaders\\DebugPrimitive_PS.cso", pixelShader.GetAddressOf());

	//�萔�o�b�t�@�̍쐬
	constantBuffer.Initialize(SystemManager::Instance().GetDevice(), &constants);

}

void PrimitiveMeshRenderer::DrawPrepare()
{
	//�f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext* dc = SystemManager::Instance().GetDeviceContext();

	// �V�F�[�_�[�̐ݒ�
	dc->VSSetShader(vertexShader.Get(), nullptr, 0);
	dc->PSSetShader(pixelShader.Get(), nullptr, 0);
	dc->IASetInputLayout(inputLayout.Get());

	//�X�e�[�g�ύX
	SystemManager::Instance().SetRasterizerState(SystemManager::RasterizerState::Primitive3D);
	SystemManager::Instance().SetDepthStencilState(SystemManager::DepthStencilState::SortOnWriteOn);

	// �v���~�e�B�u�ݒ�
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void PrimitiveMeshRenderer::Draw3D()
{
	DrawPrepare();

	//�f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext* dc = SystemManager::Instance().GetDeviceContext();

	//���_�o�b�t�@�̐ݒ�
	UINT stride = sizeof(DirectX::XMFLOAT3);
	UINT offset = 0;
	dc->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);


}
