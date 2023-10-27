
#include "Stdafx.h"
#include "SphereMeshRenderer.h"

#include "System/SystemManager.h"
#include "System/Misc.h"

#include "Component/SphereCollider.h"

SphereMeshRenderer::SphereMeshRenderer()
{
	//Sphereメッシュ作成
	Initialize();
}

void SphereMeshRenderer::Initialize()
{
	PrimitiveMeshRenderer::Initialize();

	float radius = 1.0f;
	int slices = 16;
	int stacks = 16;

	vertexCount = stacks * slices * 2 + slices * stacks * 2;
	std::unique_ptr<DirectX::XMFLOAT3[]> vertices = std::make_unique<DirectX::XMFLOAT3[]>(vertexCount);

	const float phiStep = DirectX::XM_PI / static_cast<float>(stacks);
	float thetaStep = DirectX::XM_2PI / static_cast<float>(slices);

	DirectX::XMFLOAT3* p = vertices.get();

	//横
	for (int i = 0; i < stacks; ++i)
	{
		const float phi = static_cast<float>(i) * phiStep;
		const float y = radius * cosf(phi);
		const float r = radius * sinf(phi);

		for (int j = 0; j < slices; ++j)
		{
			float theta = static_cast<float>(j) * thetaStep;
			p->x = r * sinf(theta);
			p->y = y;
			p->z = r * cosf(theta);
			p++;

			theta += thetaStep;

			p->x = r * sinf(theta);
			p->y = y;
			p->z = r * cosf(theta);
			p++;
		}
	}

	//縦
	thetaStep = DirectX::XM_2PI / static_cast<float>(stacks);
	for (int i = 0; i < slices; ++i)
	{
		const DirectX::XMMATRIX M = DirectX::XMMatrixRotationY(static_cast<float>(i) * thetaStep);
		for (int j = 0; j < stacks; ++j)
		{
			float theta = static_cast<float>(j) * thetaStep;
			const DirectX::XMVECTOR V1 = DirectX::XMVectorSet(radius * sinf(theta), radius * cosf(theta), 0.0f, 1.0f);
			const DirectX::XMVECTOR P1 = DirectX::XMVector3TransformCoord(V1, M);
			DirectX::XMStoreFloat3(p++, P1);

			int n = (j + 1) % stacks;
			theta += thetaStep;

			const DirectX::XMVECTOR V2 = DirectX::XMVectorSet(radius * sinf(theta), radius * cosf(theta), 0.0f, 1.0f);
			const DirectX::XMVECTOR P2 = DirectX::XMVector3TransformCoord(V2, M);
			DirectX::XMStoreFloat3(p++, P2);
		}
	}

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
		subresourceData.pSysMem = vertices.get();
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		ID3D11Device* device = SystemManager::Instance().GetDevice();
		HRESULT hr = device->CreateBuffer(&desc, &subresourceData, vertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
	}
}

void SphereMeshRenderer::DrawPrepare()
{
	PrimitiveMeshRenderer::Draw();
}

void SphereMeshRenderer::Draw(SphereCollider* sphereCollider)
{
	//ワールド行列の作成
	float r = sphereCollider->radius;
	DirectX::XMFLOAT3 center = sphereCollider->GetCenter();
	DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(r,r,r) };
	DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(0.0f,0.0f,0.0f) };
	DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(center.x,center.y,center.z) };
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, S * R * T);

	//定数バッファ更新
	constants.world = world;
	constants.color = color;
	ID3D11DeviceContext* dc = SystemManager::Instance().GetDeviceContext();
	constantBuffer.SetConstantBuffer(dc, ConstantBuffer::ShaderType::ALL, ConstantBuffer::UsageType::Object, &constants);

	dc->Draw(vertexCount, 0);

}
