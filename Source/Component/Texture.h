#pragma once

#include <WICTextureLoader.h>

#include <wrl.h>

#include <string>
#include<map>

constexpr int PBRMaxTexture = 6;

void ReleaseAllTextures();

class Texture
{

public:
	enum class Type
	{
		None = -1,
		BaseColor,//0
		Normal,//1
		Ambient,//2
		Emissive,//3
		Metallic,//4
		Roughness,//5
		ShadowMap,//6
		Dissolve,//7
		EnvironmentMap,//8
		Ramp,//9
	};

	Type textureType = Type::None;

public:
	Texture() = default;
	//Texture(ID3D11Device* device, const wchar_t* texturePath, ShaderType textureType);
	~Texture() = default;
	
	void Initialize(ID3D11Device* device, const wchar_t* texturePath, Type textureType, int slot);

	void SetPixelShader(ID3D11DeviceContext* dc);


	//Getter
	[[nodiscard]] D3D11_TEXTURE2D_DESC GetTexture2dDesc() const { return texture2dDesc; }
	[[nodiscard]] ID3D11ShaderResourceView* GetShaderResourceView() const { return shaderResourceView.Get(); }
	[[nodiscard]] std::string GetTexturePath() const { return texturePath; }


private:
	HRESULT LoadTextureFromFile(ID3D11Device* device,
		const wchar_t* filename,
		ID3D11ShaderResourceView** shader_resource_view,
		D3D11_TEXTURE2D_DESC* texture2d_desc) const;

	HRESULT MakeDummyTexture(ID3D11Device* device, ID3D11ShaderResourceView** shader_resource_view,
		DWORD value/*0xAABBGGRR*/, UINT dimension) const;
	



private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;

	D3D11_TEXTURE2D_DESC texture2dDesc;

	int slot = -1;

	std::string texturePath = "";

};
