#include "Stdafx.h"

#include "SpriteData.h"
#include "System/Misc.h"
#include <sstream>

#include <WICTextureLoader.h>

#include "System/CreateShader.h"
#include "Manager/SystemManager.h"


// #pragma warning(disable:)

SpriteData::SpriteData(const wchar_t* filePath, size_t maxSprites) : maxVertices(maxSprites * 6)
{
    HRESULT hr{ S_OK };

    std::unique_ptr<Vertex[]>vertices{ std::make_unique<Vertex[]>(maxVertices) };

    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(Vertex) * static_cast<UINT>(maxVertices);
    buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA subResourceData{};
    subResourceData.pSysMem = vertices.get();
    subResourceData.SysMemPitch = 0;
    subResourceData.SysMemSlicePitch = 0;

    hr = SystemManager::Instance().GetDevice()->CreateBuffer(&buffer_desc, &subResourceData, vertexBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

    //���̓��C�A�E�g�I�u�W�F�N�g�̐���
    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
       { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
             D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
             D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
             D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    //���_�V�F�[�_�[�I�u�W�F�N�g�̐���(���W���[����)
    const char* cso_name{ ".\\Shaders\\2D_VS.cso" };
    hr = CreateVsFromCso(cso_name, vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
    _ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

    //�s�N�Z���V�F�[�_�[�I�u�W�F�N�g�̐���(���W���[����)
    cso_name = ".\\Shaders\\2D_PS.cso";
    hr = CreatePsFromCso(cso_name, pixelShader.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

    //�e�N�X�`���̃��[�h�����W���[��������
    texture.Initialize(filePath, Texture::Type::BaseColor, static_cast<int>(Texture::Type::BaseColor));
}

void SpriteData::Draw(
    DirectX::XMFLOAT2 pos,//��`�̍���̍��W�i�X�N���[�����W�n�j
    DirectX::XMFLOAT2 scale,//�`�悷��X�P�[��
    DirectX::XMFLOAT2 pivot,//��_
    DirectX::XMFLOAT2 texpos, //�`�悷���`�̍�����W
    DirectX::XMFLOAT2 texsize,//�`�悷���`�̃T�C�Y
    float angle,//degree
    DirectX::XMFLOAT4 color//�J���[
)
{
    //�X�N���[���i�r���[�|�[�g�j�̃T�C�Y���擾����
    D3D11_VIEWPORT viewport{};
    UINT num_viewports{ 1 };
    SystemManager::Instance().GetDeviceContext()->RSGetViewports(&num_viewports, &viewport);

    float x = 0, y = 0;
    // left-top
    x = { pos.x - (pivot.x * scale.x) };
    y = { pos.y - (pivot.y * scale.y) };
    DirectX::XMFLOAT2 leftTop = { x ,y };
    // right-top
    x = { pos.x + ((texsize.x - pivot.x) * scale.x) };
    y = { pos.y - (pivot.y * scale.y) };
    DirectX::XMFLOAT2 rightTop = { x, y };
    // left-bottom
    x = { pos.x - (pivot.x * scale.x) };
    y = { pos.y + ((texsize.y - pivot.y) * scale.y) };
    DirectX::XMFLOAT2 leftBottom = { x, y };
    // right-bottom
    x = { pos.x + ((texsize.x - pivot.x) * scale.x) };
    y = { pos.y + ((texsize.y - pivot.y) * scale.y) };
    DirectX::XMFLOAT2 rightBottom = { x, y };

    //��]
    auto rotate = [](float& x, float& y, float cx, float cy, float angle)
    {
        x -= cx;
        y -= cy;

        float cos{ cosf(DirectX::XMConvertToRadians(angle)) };
        float sin{ sinf(DirectX::XMConvertToRadians(angle)) };
        float tx{ x }, ty{ y };
        x = cos * tx + -sin * ty;
        y = sin * tx + cos * ty;

        x += cx;
        y += cy;
    };

    //��]�̒��S����`�̒��S�_�ɂ����ꍇ
    float cx = pos.x;
    float cy = pos.y;
    rotate(leftTop.x, leftTop.y, cx, cy, angle);
    rotate(rightTop.x, rightTop.y, cx, cy, angle);
    rotate(leftBottom.x, leftBottom.y, cx, cy, angle);
    rotate(rightBottom.x, rightBottom.y, cx, cy, angle);

    //�X�N���[�����W�n����NDC�ւ̍��W�ϊ��������Ȃ�
    leftTop.x = 2.0f * leftTop.x / viewport.Width - 1.0f;
    leftTop.y = 1.0f - 2.0f * leftTop.y / viewport.Height;
    rightTop.x = 2.0f * rightTop.x / viewport.Width - 1.0f;
    rightTop.y = 1.0f - 2.0f * rightTop.y / viewport.Height;
    leftBottom.x = 2.0f * leftBottom.x / viewport.Width - 1.0f;
    leftBottom.y = 1.0f - 2.0f * leftBottom.y / viewport.Height;
    rightBottom.x = 2.0f * rightBottom.x / viewport.Width - 1.0f;
    rightBottom.y = 1.0f - 2.0f * rightBottom.y / viewport.Height;

    //texture space�ɕϊ�
    DirectX::XMFLOAT2 s0 = { texpos.x / texture.GetTexture2dDesc().Width,texpos.y / texture.GetTexture2dDesc().Height };
    DirectX::XMFLOAT2 s1 = { (texpos.x + texsize.x) / texture.GetTexture2dDesc().Width,texpos.y / texture.GetTexture2dDesc().Height };
    DirectX::XMFLOAT2 s2 = { texpos.x / texture.GetTexture2dDesc().Width,(texpos.y + texsize.y) / texture.GetTexture2dDesc().Height };
    DirectX::XMFLOAT2 s3 = { (texpos.x + texsize.x) / texture.GetTexture2dDesc().Width,(texpos.y + texsize.y) / texture.GetTexture2dDesc().Height };



    Vertex vertex[4] = {};
    vertex[0].position = { leftTop.x, leftTop.y , 0 };
    vertex[1].position = { rightTop.x, rightTop.y , 0 };
    vertex[2].position = { leftBottom.x, leftBottom.y , 0 };
    vertex[3].position = { rightBottom.x, rightBottom.y , 0 };

    vertex[0].color = vertex[1].color = vertex[2].color = vertex[3].color = { color.x,color.y,color.z,color.w };

    vertex[0].texCoord = s0;
    vertex[1].texCoord = s1;
    vertex[2].texCoord = s2;
    vertex[3].texCoord = s3;


    vertices.push_back(vertex[0]);
    vertices.push_back(vertex[1]);
    vertices.push_back(vertex[2]);
    vertices.push_back(vertex[2]);
    vertices.push_back(vertex[1]);
    vertices.push_back(vertex[3]);
}

SpriteData::~SpriteData()
{
    ReleaseAllTextures();
}

void SpriteData::Begin(ID3D11PixelShader* replacedPixelShader, ID3D11VertexShader* replacedVertexShader)
{
    vertices.clear();

    ID3D11DeviceContext* dc = SystemManager::Instance().GetDeviceContext();

    dc->VSSetShader(replacedVertexShader ? replacedVertexShader : vertexShader.Get(), nullptr, 0);

    dc->PSSetShader(replacedPixelShader ? replacedPixelShader : pixelShader.Get(), nullptr, 0);

    texture.SetPixelShader();
}
void SpriteData::End()
{
    ID3D11DeviceContext* dc = SystemManager::Instance().GetDeviceContext();

    //�v�Z���ʂŒ��_�o�b�t�@�I�u�W�F�N�g���X�V����
    HRESULT hr{ S_OK };
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    hr = dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    _ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

    size_t vertex_count = vertices.size();
    _ASSERT_EXPR(maxVertices >= vertex_count, "Buffer overflow");
    Vertex* data{ reinterpret_cast<Vertex*>(mapped_subresource.pData) };
    if (data != nullptr)
    {
        const Vertex* p = vertices.data();
        memcpy_s(data, maxVertices * sizeof(Vertex), p, vertex_count * sizeof(Vertex));
    }

    dc->Unmap(vertexBuffer.Get(), 0);

    //���_�o�b�t�@�[�̃o�C���h
    UINT stride{ sizeof(Vertex) };
    UINT offset{ 0 };
    dc->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

    //�v���~�e�B�u�^�C�v����уf�[�^�̏����Ɋւ�����̃o�C���h
    dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //���̓��C�A�E�g�I�u�W�F�N�g�̃o�C���h
    dc->IASetInputLayout(inputLayout.Get());

    //�v���~�e�B�u�̕`��
    dc->Draw(static_cast<UINT>(vertex_count), 0);
}
