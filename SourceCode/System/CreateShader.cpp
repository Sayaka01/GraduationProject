#include "Stdafx.h"

#include "CreateShader.h"
#include <memory>
#include "Misc.h"
#include <cstdio>

#include "System/SystemManager.h"

HRESULT CreateVsFromCso(const char* csoName, ID3D11VertexShader** vertexShader,
                        ID3D11InputLayout** inputLayout, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT numElements)
{
    ID3D11Device* device = SystemManager::Instance().GetDevice();

    FILE* fp{ nullptr };
    fopen_s(&fp, csoName, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);
    long csoSz{ ftell(fp) };
    fseek(fp, 0, SEEK_SET);

    std::unique_ptr<unsigned char[]> cso_data{ std::make_unique<unsigned char[]>(csoSz) };
    fread(cso_data.get(), csoSz, 1, fp);
    fclose(fp);

    HRESULT hr{ S_OK };
    hr = device->CreateVertexShader(cso_data.get(), csoSz, nullptr, vertexShader);
    _ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

    if (inputLayout)
    {
        hr = device->CreateInputLayout(inputElementDesc, numElements,
            cso_data.get(), csoSz, inputLayout);
        _ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
    }

    return hr;
}

HRESULT CreatePsFromCso(const char* csoName, ID3D11PixelShader** pixelShader)
{
    ID3D11Device* device = SystemManager::Instance().GetDevice();

    FILE* fp{ nullptr };
    fopen_s(&fp, csoName, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);
    const long csoSz{ ftell(fp) };
    fseek(fp, 0, SEEK_SET);

    const std::unique_ptr<unsigned char[]> csoData{ std::make_unique<unsigned char[]>(csoSz) };
    fread(csoData.get(), csoSz, 1, fp);
    fclose(fp);

    HRESULT hr{ S_OK };
    hr = device->CreatePixelShader(csoData.get(), csoSz, nullptr, pixelShader);
    _ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

    return hr;
}
