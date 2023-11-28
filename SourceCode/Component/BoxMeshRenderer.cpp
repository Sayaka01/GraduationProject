
#include "Stdafx.h"
#include "BoxMeshRenderer.h"

#include "System/SystemManager.h"
#include "System/Misc.h"

#include "Component/BoxCollider.h"

BoxMeshRenderer::BoxMeshRenderer()
{
	//Boxメッシュ作成
	Initialize();
}

void BoxMeshRenderer::Initialize()
{
	PrimitiveMeshRenderer::Initialize();

	float radius = 1.0f;
	int slices = 16;
	int stacks = 16;

	vertexCount = 24;

	const DirectX::XMFLOAT3 min = { -0.5f,-0.5f,-0.5f };
	const DirectX::XMFLOAT3 max = { 0.5f,0.5f,0.5f };

	DirectX::XMFLOAT3 vertices[24]{
		{min.x,max.y,min.z},{min.x,max.y,max.z},{min.x,max.y,max.z},{max.x,max.y,max.z},
		{max.x,max.y,max.z},{max.x,max.y,min.z},{max.x,max.y,min.z},{min.x,max.y,min.z},
		{min.x,max.y,min.z},{min.x,min.y,min.z},{min.x,max.y,max.z},{min.x,min.y,max.z},
		{max.x,max.y,max.z},{max.x,min.y,max.z},{max.x,max.y,min.z},{max.x,min.y,min.z},
		{min.x,min.y,min.z},{min.x,min.y,max.z},{min.x,min.y,max.z},{max.x,min.y,max.z},
		{max.x,min.y,max.z},{max.x,min.y,min.z},{max.x,min.y,min.z},{min.x,min.y,min.z},
	};

	// 頂点バッファ
	{
		D3D11_BUFFER_DESC desc = {};
		D3D11_SUBRESOURCE_DATA subresourceData = {};

		desc.ByteWidth = static_cast<UINT>(sizeof(DirectX::XMFLOAT3) * vertexCount);
		desc.Usage = D3D11_USAGE_IMMUTABLE;	// D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		subresourceData.pSysMem = vertices;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		ID3D11Device* device = SystemManager::Instance().GetDevice();
		HRESULT hr = device->CreateBuffer(&desc, &subresourceData, vertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
	}
}

void BoxMeshRenderer::DrawPrepare()
{
	PrimitiveMeshRenderer::Draw3D();
}

void BoxMeshRenderer::Draw(BoxCollider* boxCollider)
{
	//ワールド行列の作成
	DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(boxCollider->size.x,boxCollider->size.y,boxCollider->size.z) };
	DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(0.0f,0.0f,0.0f) };
	DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(boxCollider->center.x,boxCollider->center.y,boxCollider->center.z) };
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, S * R * T);

	//定数バッファ更新
	constants.world = world;
	constants.color = boxCollider->debugColor;
	ID3D11DeviceContext* dc = SystemManager::Instance().GetDeviceContext();
	constantBuffer.SetConstantBuffer(dc, ConstantBuffer::ShaderType::ALL, ConstantBuffer::UsageType::Object, &constants);

	dc->Draw(vertexCount, 0);

}
