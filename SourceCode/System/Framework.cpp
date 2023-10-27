#include "Stdafx.h"

#include "Framework.h"

#include "../Scene/SceneManager.h"
#include "../Scene/SceneGame.h"
#include "../Scene/SceneTitle.h"

#include "System/SystemManager.h"

#include "Common.h"
#include "Misc.h"


Framework::Framework(HWND hwnd) : hwnd(hwnd)
{

}

bool Framework::Initialize()
{
	//****************************************************************
    // 
	// �f�o�C�X�E�f�o�C�X�R���e�L�X�g�E�X���b�v�`�F�[���̍쐬
    // 
    //****************************************************************

	HRESULT hr{ S_OK };
	
	UINT createDeviceFlags{ 0 };
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevels{ D3D_FEATURE_LEVEL_11_0 };
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;
	swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;// Numerator=���q
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;// Denominator=����
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1; //1�s�N�Z��������̃}���`�T���v�����O��
	swapChainDesc.SampleDesc.Quality = 0; //��ʂ̃N�I���e�B�i�����Ə������d���Ȃ�j
	swapChainDesc.Windowed = !FULLSCREEN; //�t���X�N���[�����ǂ���
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, &featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc,
		swapChain.GetAddressOf(), device.GetAddressOf(), NULL, dc.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

	//****************************************************************
	// 
	// �����_�[�^�[�Q�b�g�r���[�̍쐬
	// 
	//****************************************************************

	Microsoft::WRL::ComPtr <ID3D11Texture2D> backBuffer{};
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(backBuffer.GetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

	hr = device->CreateRenderTargetView(backBuffer.Get(), NULL, renderTargetView.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

	//****************************************************************
	// 
	// �[�x�X�e���V���r���[�̍쐬
	// 
	//****************************************************************

	Microsoft::WRL::ComPtr <ID3D11Texture2D> depthStencilBuffer{};
	D3D11_TEXTURE2D_DESC texture2dDesc{};
	texture2dDesc.Width = SCREEN_WIDTH;
	texture2dDesc.Height = SCREEN_HEIGHT;
	texture2dDesc.MipLevels = 1;
	texture2dDesc.ArraySize = 1;
	texture2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // �e�N�X�`���̃}���`�T���v�����O���w�肷��\��
	texture2dDesc.SampleDesc.Count = 1;
	texture2dDesc.SampleDesc.Quality = 0;
	texture2dDesc.Usage = D3D11_USAGE_DEFAULT; // �e�N�X�`���̓ǂݎ����@�Ə������ݕ��@�����ʂ���l
	texture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texture2dDesc.CPUAccessFlags = 0;
	texture2dDesc.MiscFlags = 0;
	hr = device->CreateTexture2D(&texture2dDesc, NULL, depthStencilBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
	
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = texture2dDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
	
	//****************************************************************
	//  
	// �r���[�|�[�g�i�\���̈�)�̐ݒ�
	// 
	//****************************************************************
	D3D11_VIEWPORT viewport{};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(SCREEN_WIDTH);
	viewport.Height = static_cast<float>(SCREEN_HEIGHT);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &viewport);


	//****************************************************************
	// 
	// �T���v���[�X�e�[�g�I�u�W�F�N�g�̐���
	// 
	//****************************************************************

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = device->CreateSamplerState(&samplerDesc, samplerStates[0].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
	
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	hr = device->CreateSamplerState(&samplerDesc, samplerStates[1].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
	
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	hr = device->CreateSamplerState(&samplerDesc, samplerStates[2].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MinLOD = -D3D11_FLOAT32_MAX;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxAnisotropy = 1;
	hr = device->CreateSamplerState(&samplerDesc, samplerStates[3].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));


	//****************************************************************
	// 
	// �V�X�e���}�l�[�W���[������
	// 
	//****************************************************************
	SystemManager::Instance().SetDevice(device.Get());
	SystemManager::Instance().SetDeviceContext(dc.Get());
	SystemManager::Instance().Initialize();


	//****************************************************************
	// 
	// �G�t�F�N�g�}�l�[�W���[������
	// 
	//****************************************************************
	//EffectManager::Instance().Initialize(device.Get(),dc.Get());


	//****************************************************************
	// 
	// �V�[���}�l�[�W���[������
	// 
	//****************************************************************
	SceneManager::Instance().Initialize();
	SceneManager::Instance().ChangeScene(new SceneTitle);


	return true;
}

void Framework::Update(float elapsedTime/*Elapsed seconds from last frame*/)
{
#ifdef USE_IMGUI
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif

	SystemManager::Instance().SetElapsedTime(elapsedTime);
	SystemManager::Instance().Update();

	SceneManager::Instance().Update();
	
}

void Framework::Draw()
{
	std::lock_guard<std::mutex> lock(SceneManager::Instance().GetMutex());

	ID3D11RenderTargetView* nullRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT]{};
	dc->OMSetRenderTargets(_countof(nullRenderTargetViews), nullRenderTargetViews, 0);
	ID3D11ShaderResourceView* nullShaderResourceViews[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]{};
	dc->VSSetShaderResources(0, _countof(nullShaderResourceViews), nullShaderResourceViews);
	dc->PSSetShaderResources(0, _countof(nullShaderResourceViews), nullShaderResourceViews);

	HRESULT hr{ S_OK };

	FLOAT color[]{ 0.3f, 0.3f, 0.3f, 1.0f };
	
	dc->ClearRenderTargetView(renderTargetView.Get(), color);
	dc->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

	//****************************************************************
	// 
	// �T���v���[�X�e�[�g�I�u�W�F�N�g�̃o�C���h
	// 
	//****************************************************************

	dc->PSSetSamplers(0, 1, samplerStates[0].GetAddressOf());
	dc->PSSetSamplers(1, 1, samplerStates[1].GetAddressOf());
	dc->PSSetSamplers(2, 1, samplerStates[2].GetAddressOf());
	dc->PSSetSamplers(3, 1, samplerStates[3].GetAddressOf());


	SceneManager::Instance().Draw();


#ifdef USE_IMGUI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	// Update and Render additional Platform Windows
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();

		ImGui::RenderPlatformWindowsDefault();
	}
#endif

	UINT sync_interval{ 0 };// 1���Z�b�g�����60fps�̌Œ�t���[�����[�g�œ��삷��
	swapChain->Present(sync_interval, 0);
}

bool Framework::Uninitialize()
{
	SceneManager::Instance().Clear();

	return true;
}

Framework::~Framework()
{

}