
#include "Stdafx.h"
#include "CapsuleMeshRenderer.h"

#include "System/SystemManager.h"
#include "System/Misc.h"

#include "Component/CapsuleCollider.h"

CapsuleMeshRenderer::CapsuleMeshRenderer()
{
	//カプセルメッシュ作成
	Initialize();
}

void CapsuleMeshRenderer::Initialize()
{
	PrimitiveMeshRenderer::Initialize();

	float radius = 1.0f;
	float height = 1.0f;
	int slices = 16;
	int stacks = 16;

	ID3D11Device* device = SystemManager::Instance().GetDevice();

	//---< 円柱 >---//
	{
		cylinderVertexCount = 2 * slices * 2;
		std::unique_ptr<DirectX::XMFLOAT3[]> vertices = std::make_unique<DirectX::XMFLOAT3[]>(cylinderVertexCount);

		DirectX::XMFLOAT3* p = vertices.get();

		for (int i = 0; i < slices; ++i)
		{
			float stackHeight = height;

			// vertices of ring
			float dTheta = DirectX::XM_2PI / static_cast<float>(slices);
			int n = (i + 1) % slices;

			float c1 = cosf(i * dTheta);
			float s1 = sinf(i * dTheta);

			float c2 = cosf(n * dTheta);
			float s2 = sinf(n * dTheta);

			p->x = radius * c1;
			p->y = -height * 0.5f;
			p->z = radius * s1;
			p++;

			p->x = radius * c1;
			p->y = height * 0.5f;
			p->z = radius * s1;
			p++;
		}
		// 頂点バッファ
		{
			D3D11_BUFFER_DESC desc = {};
			D3D11_SUBRESOURCE_DATA subresourceData = {};

			desc.ByteWidth = static_cast<UINT>(sizeof(DirectX::XMFLOAT3) * cylinderVertexCount);
			desc.Usage = D3D11_USAGE_IMMUTABLE;	// D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			subresourceData.pSysMem = vertices.get();
			subresourceData.SysMemPitch = 0;
			subresourceData.SysMemSlicePitch = 0;

			HRESULT hr = device->CreateBuffer(&desc, &subresourceData, cylinderVertexBuffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		}
	}

	//-----< 球 >-----//
	{
		halfSphereVertexCount = (stacks * slices * 2 + slices * stacks * 2) / 2;
		
		//球(上）
		{
			std::unique_ptr<DirectX::XMFLOAT3[]> vertices = std::make_unique<DirectX::XMFLOAT3[]>(halfSphereVertexCount);

			DirectX::XMFLOAT3* p = vertices.get();

			float phiStep = DirectX::XM_PI / static_cast<float>(stacks);
			float thetaStep = DirectX::XM_2PI / static_cast<float>(slices);

			//横
			for (int i = 0; i <= stacks / 2; ++i)
			{
				float phi = i * phiStep;
				float y = radius * cosf(phi) + height * 0.5f;
				float r = radius * sinf(phi);

				for (int j = 0; j < slices; ++j)
				{
					float theta = j * thetaStep;
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
				DirectX::XMMATRIX M = DirectX::XMMatrixRotationY(i * thetaStep);
				for (int j = 0; j < stacks / 4; ++j)
				{
					float theta = j * thetaStep;
					DirectX::XMVECTOR V1 = DirectX::XMVectorSet(radius * sinf(theta), radius * cosf(theta), 0.0f, 1.0f);
					DirectX::XMVECTOR P1 = DirectX::XMVector3TransformCoord(V1, M);
					DirectX::XMFLOAT3 p1;
					DirectX::XMStoreFloat3(&p1, P1);
					p1.y += height * 0.5f;
					p->x = p1.x;
					p->y = p1.y;
					p->z = p1.z;
					p++;

					theta += thetaStep;

					DirectX::XMVECTOR V2 = DirectX::XMVectorSet(radius * sinf(theta), radius * cosf(theta), 0.0f, 1.0f);
					DirectX::XMVECTOR P2 = DirectX::XMVector3TransformCoord(V2, M);
					DirectX::XMFLOAT3 p2;
					DirectX::XMStoreFloat3(&p2, P2);
					p2.y += height * 0.5f;
					p->x = p2.x;
					p->y = p2.y;
					p->z = p2.z;
					p++;
				}
			}
			// 頂点バッファ
			{
				D3D11_BUFFER_DESC desc = {};
				D3D11_SUBRESOURCE_DATA subresourceData = {};

				desc.ByteWidth = static_cast<UINT>(sizeof(DirectX::XMFLOAT3) * halfSphereVertexCount);
				desc.Usage = D3D11_USAGE_IMMUTABLE;	// D3D11_USAGE_DEFAULT;
				desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = 0;
				desc.StructureByteStride = 0;
				subresourceData.pSysMem = vertices.get();
				subresourceData.SysMemPitch = 0;
				subresourceData.SysMemSlicePitch = 0;

				HRESULT hr = device->CreateBuffer(&desc, &subresourceData, halfSphereUpVertexBuffer.GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
			}
		}
		
		//球(下)
		{
			std::unique_ptr<DirectX::XMFLOAT3[]> vertices = std::make_unique<DirectX::XMFLOAT3[]>(halfSphereVertexCount);

			DirectX::XMFLOAT3* p = vertices.get();

			float phiStep = DirectX::XM_PI / static_cast<float>(stacks);
			float thetaStep = DirectX::XM_2PI / static_cast<float>(slices);

			//横
			for (int i = stacks / 2; i < stacks; ++i)
			{
				float phi = i * phiStep;
				float y = radius * cosf(phi) - height * 0.5f;
				float r = radius * sinf(phi);

				for (int j = 0; j < slices; ++j)
				{
					float theta = j * thetaStep;
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
				DirectX::XMMATRIX M = DirectX::XMMatrixRotationY(i * thetaStep);
				for (int j = stacks / 4; j < stacks / 2; ++j)
				{
					float theta = j * thetaStep;
					DirectX::XMVECTOR V1 = DirectX::XMVectorSet(radius * sinf(theta), radius * cosf(theta), 0.0f, 1.0f);
					DirectX::XMVECTOR P1 = DirectX::XMVector3TransformCoord(V1, M);
					DirectX::XMFLOAT3 p1;
					DirectX::XMStoreFloat3(&p1, P1);
					p1.y -= height * 0.5f;
					p->x = p1.x;
					p->y = p1.y;
					p->z = p1.z;
					p++;

					theta += thetaStep;

					DirectX::XMVECTOR V2 = DirectX::XMVectorSet(radius * sinf(theta), radius * cosf(theta), 0.0f, 1.0f);
					DirectX::XMVECTOR P2 = DirectX::XMVector3TransformCoord(V2, M);
					DirectX::XMFLOAT3 p2;
					DirectX::XMStoreFloat3(&p2, P2);
					p2.y -= height * 0.5f;
					p->x = p2.x;
					p->y = p2.y;
					p->z = p2.z;
					p++;
				}
			}
			// 頂点バッファ
			{
				D3D11_BUFFER_DESC desc = {};
				D3D11_SUBRESOURCE_DATA subresourceData = {};

				desc.ByteWidth = static_cast<UINT>(sizeof(DirectX::XMFLOAT3) * halfSphereVertexCount);
				desc.Usage = D3D11_USAGE_IMMUTABLE;	// D3D11_USAGE_DEFAULT;
				desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = 0;
				desc.StructureByteStride = 0;
				subresourceData.pSysMem = vertices.get();
				subresourceData.SysMemPitch = 0;
				subresourceData.SysMemSlicePitch = 0;

				HRESULT hr = device->CreateBuffer(&desc, &subresourceData, halfSphereDownVertexBuffer.GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
			}
		}
	}
}

void CapsuleMeshRenderer::DrawPrepare()
{
	PrimitiveMeshRenderer::DrawPrepare();
}

void CapsuleMeshRenderer::Draw(CapsuleCollider* capsuleCollider)
{
	capsuleCollider->UpdateCenter();//begin, endからcenterを更新

	//デバイスコンテキストの取得
	ID3D11DeviceContext* dc = SystemManager::Instance().GetDeviceContext();

	UINT stride = sizeof(DirectX::XMFLOAT3);
	UINT offset = 0;

	//-----< 円柱部分 >-----//
	dc->IASetVertexBuffers(0, 1, cylinderVertexBuffer.GetAddressOf(), &stride, &offset);

	float correctionY = max((capsuleCollider->radius - capsuleCollider->cylinderSize), 0.0f);
	if (capsuleCollider->cylinderSize < capsuleCollider->radius)correctionY = 0.0f;

	DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(capsuleCollider->radius,capsuleCollider->cylinderSize + correctionY,capsuleCollider->radius) };
	DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(capsuleCollider->radian.x,capsuleCollider->radian.y,capsuleCollider->radian.z) };
	DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(capsuleCollider->center.x,capsuleCollider->center.y,capsuleCollider->center.z) };
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, S * R * T);
	constants.world = world;
	constants.color = capsuleCollider->debugColor;
	constantBuffer.SetConstantBuffer(dc, ConstantBuffer::ShaderType::ALL, ConstantBuffer::UsageType::Object, &constants);

	dc->Draw(cylinderVertexCount, 0);

	//球（上）
	dc->IASetVertexBuffers(0, 1, halfSphereUpVertexBuffer.GetAddressOf(), &stride, &offset);

	DirectX::XMFLOAT3 correction = { 0.0f, (capsuleCollider->cylinderSize - capsuleCollider->radius) * 0.5f, 0.0f };
	DirectX::XMStoreFloat3(&correction, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&correction), R));

	S = DirectX::XMMatrixScaling(capsuleCollider->radius, capsuleCollider->radius, capsuleCollider->radius);
	T = DirectX::XMMatrixTranslation(capsuleCollider->center.x + correction.x, capsuleCollider->center.y + correction.y, capsuleCollider->center.z + correction.z);
	DirectX::XMStoreFloat4x4(&world, S * R * T);
	constants.world = world;
	constantBuffer.SetConstantBuffer(dc, ConstantBuffer::ShaderType::ALL, ConstantBuffer::UsageType::Object, &constants);

	dc->Draw(halfSphereVertexCount, 0);

	//球（下）
	dc->IASetVertexBuffers(0, 1, halfSphereDownVertexBuffer.GetAddressOf(), &stride, &offset);

	S = DirectX::XMMatrixScaling(capsuleCollider->radius, capsuleCollider->radius, capsuleCollider->radius);
	T = DirectX::XMMatrixTranslation(capsuleCollider->center.x - correction.x, capsuleCollider->center.y - correction.y, capsuleCollider->center.z - correction.z);
	DirectX::XMStoreFloat4x4(&world, S * R * T);
	constants.world = world;
	constantBuffer.SetConstantBuffer(dc, ConstantBuffer::ShaderType::ALL, ConstantBuffer::UsageType::Object, &constants);

	dc->Draw(halfSphereVertexCount, 0);



}
