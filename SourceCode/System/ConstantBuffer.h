#pragma once
#include <crtdbg.h>
#include <d3d11.h>
#include <wrl.h>

#include "Misc.h"

class ConstantBuffer
{
public:
	template<class T>
	void Initialize(ID3D11Device* d, T* data)
	{
		HRESULT hr = { S_OK };
		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.ByteWidth = sizeof(T);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		hr = d->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
	}

	enum class ShaderType
	{
		ALL,
		Pixel,
		Vertex,
	};

	enum UsageType
	{
		Camera,
		Object,
		DirectionLight,
		PointLight,
		SpotLight,
		ShadowMap,
		Fog,
	};

	template<class T>
	void SetConstantBuffer(ID3D11DeviceContext* dc, ShaderType shaderType, int usageType, T* data)
	{
		dc->UpdateSubresource(constantBuffer.Get(), 0, 0, data, 0, 0);

		switch (shaderType)
		{
		case ShaderType::ALL:
			dc->PSSetConstantBuffers(usageType, 1, constantBuffer.GetAddressOf());
			dc->VSSetConstantBuffers(usageType, 1, constantBuffer.GetAddressOf());
			break;
		case ShaderType::Pixel:
			dc->PSSetConstantBuffers(usageType, 1, constantBuffer.GetAddressOf());
			break;
		case ShaderType::Vertex:
			dc->VSSetConstantBuffers(usageType, 1, constantBuffer.GetAddressOf());
			break;
		}
	}
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer{ nullptr };

};
