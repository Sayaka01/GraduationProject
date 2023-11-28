
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

	//頂点シェーダーの作成
	CreateVsFromCso(".\\Shaders\\DebugPrimitive_VS.cso", vertexShader.GetAddressOf(),
		inputLayout.GetAddressOf(), inputElementDesc, ARRAYSIZE(inputElementDesc));
	//ピクセルシェーダーの作成
	CreatePsFromCso(".\\Shaders\\DebugPrimitive_PS.cso", pixelShader.GetAddressOf());

	//定数バッファの作成
	constantBuffer.Initialize(SystemManager::Instance().GetDevice(), &constants);

}

void PrimitiveMeshRenderer::DrawPrepare()
{
	//デバイスコンテキストの取得
	ID3D11DeviceContext* dc = SystemManager::Instance().GetDeviceContext();

	// シェーダーの設定
	dc->VSSetShader(vertexShader.Get(), nullptr, 0);
	dc->PSSetShader(pixelShader.Get(), nullptr, 0);
	dc->IASetInputLayout(inputLayout.Get());

	//ステート変更
	SystemManager::Instance().SetRasterizerState(SystemManager::RasterizerState::Primitive3D);
	SystemManager::Instance().SetDepthStencilState(SystemManager::DepthStencilState::SortOnWriteOn);

	// プリミティブ設定
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void PrimitiveMeshRenderer::Draw3D()
{
	DrawPrepare();

	//デバイスコンテキストの取得
	ID3D11DeviceContext* dc = SystemManager::Instance().GetDeviceContext();

	//頂点バッファの設定
	UINT stride = sizeof(DirectX::XMFLOAT3);
	UINT offset = 0;
	dc->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);


}
