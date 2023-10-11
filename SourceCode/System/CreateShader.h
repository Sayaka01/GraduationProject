#pragma once
#include <WICTextureLoader.h>

HRESULT CreateVsFromCso(const char* csoName, ID3D11VertexShader** vertexShader,
    ID3D11InputLayout** inputLayout, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT numElements);

HRESULT CreatePsFromCso(const char* csoName, ID3D11PixelShader** pixelShader);