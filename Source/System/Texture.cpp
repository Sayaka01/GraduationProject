#include "Stdafx.h"

#include "Texture.h"

#include <filesystem>

#include "Misc.h"
#include <memory>

#include "DDSTextureLoader.h"

#include "System/SystemManager.h"

using namespace DirectX;

using namespace Microsoft::WRL;

using namespace std;

static map<wstring, ComPtr<ID3D11ShaderResourceView>> resources;

void Texture::Initialize(const wchar_t* texturePath, Type textureType, int slot)
{
    this->textureType = textureType;
    if (texturePath)
    {
        const std::filesystem::path path(texturePath);
        this->texturePath = path.string();
    }

    HRESULT hr{ S_OK };

    this->slot = slot;

    if (textureType == Type::None)
    {
        unsigned int dummyColor = 0xFFFFFFFF;
        if (slot == static_cast<int>(Type::Normal))dummyColor = 0xFFFF7F7F;//NormalMap
        if (slot == static_cast<int>(Type::Emissive))dummyColor = 0xFF000000;//Emissive
        MakeDummyTexture(shaderResourceView.GetAddressOf(), dummyColor, 16);
        _ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
    }
    else
    {
        LoadTextureFromFile(texturePath, shaderResourceView.GetAddressOf(), &texture2dDesc);
        _ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
    }
}


void Texture::SetPixelShader()
{
    SystemManager::Instance().GetDeviceContext()->PSSetShaderResources(slot, 1, shaderResourceView.GetAddressOf());
}


HRESULT Texture::LoadTextureFromFile(
    const wchar_t* filePath,
    ID3D11ShaderResourceView** shaderResourceView,
    D3D11_TEXTURE2D_DESC* texture2dDesc) const
{
    HRESULT hr{ S_OK };
    ComPtr<ID3D11Resource> resource;

    ID3D11Device* device = SystemManager::Instance().GetDevice();

#if 0

    auto it = resources.find(filePath);
    if (it != resources.end())
    {
        *shader_resource_view = it->second.Get();
        (*shader_resource_view)->AddRef();
        (*shader_resource_view)->GetResource(resource.GetAddressOf());
    }
    else
    {
        hr = CreateWICTextureFromFile(device, filePath, resource.GetAddressOf(), shader_resource_view);
        _ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
        resources.insert(make_pair(filePath, *shader_resource_view));
    }
#else
    std::filesystem::path ddsFilename(filePath);
    ddsFilename.replace_extension("dds");
    if (std::filesystem::exists(ddsFilename.c_str()))
    {
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context;
        device->GetImmediateContext(immediate_context.GetAddressOf());
        hr = DirectX::CreateDDSTextureFromFile(device, immediate_context.Get(), ddsFilename.c_str(),
            resource.GetAddressOf(), shaderResourceView);

        if(hr!=S_OK)
        _ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
    }
    else
    {
        if(ddsFilename.extension() == "tga")
        {
            ddsFilename.replace_extension("png");
        }
        hr = CreateWICTextureFromFile(device, filePath, resource.GetAddressOf(), shaderResourceView);
        _ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
        resources.insert(make_pair(filePath, *shaderResourceView));
    }

#endif

    ComPtr<ID3D11Texture2D> texture2d;
    hr = resource.Get()->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
    texture2d->GetDesc(texture2dDesc);

    return hr;
}

HRESULT Texture::MakeDummyTexture(ID3D11ShaderResourceView** shaderResourceView,
    DWORD value/*0xAABBGGRR*/, UINT dimension) const
{
    HRESULT hr{ S_OK };

    D3D11_TEXTURE2D_DESC texture2d_desc{};
    texture2d_desc.Width = dimension;
    texture2d_desc.Height = dimension;
    texture2d_desc.MipLevels = 1;
    texture2d_desc.ArraySize = 1;
    texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texture2d_desc.SampleDesc.Count = 1;
    texture2d_desc.SampleDesc.Quality = 0;
    texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
    texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    UINT texels = dimension * dimension;
    std::unique_ptr<DWORD[]> sysmem{ std::make_unique< DWORD[]>(texels) };
    for (size_t i = 0; i < texels; ++i) sysmem[i] = value;

    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = sysmem.get();
    subresource_data.SysMemPitch = sizeof(DWORD) * dimension;

    ID3D11Device* device = SystemManager::Instance().GetDevice();

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
    hr = device->CreateTexture2D(&texture2d_desc, &subresource_data, &texture2d);
    _ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
    shaderResourceViewDesc.Format = texture2d_desc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;
    hr = device->CreateShaderResourceView(texture2d.Get(), &shaderResourceViewDesc,
        shaderResourceView);
    _ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

    return hr;
}

void ReleaseAllTextures()
{
    resources.clear();
}
