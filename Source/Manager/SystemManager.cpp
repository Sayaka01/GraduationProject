
#include "Stdafx.h"

#include "SystemManager.h"

#include "System/Misc.h"


void SystemManager::Initialize()
{
	HRESULT hr{ S_OK };

	//BlendState
	{
		//none
		D3D11_BLEND_DESC blendDesc{};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = FALSE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BlendState::None)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		//alpha
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BlendState::Alpha)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		//add
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BlendState::Add)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		//subtract
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_SUBTRACT;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BlendState::Subtract)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		//replace
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BlendState::Replace)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		//multiply (both alpha modes)
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_COLOR;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BlendState::Multiply)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		//lighten
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BlendState::Lighten)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		//darken
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MIN;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BlendState::Darken)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		//screen
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_SRC_COLOR;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BlendState::Screen)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
	}

	//DepthStencilState
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
		depthStencilDesc.DepthEnable = TRUE;//Z sort On
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;//Z write On
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilState[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		depthStencilDesc.DepthEnable = TRUE;//Z sort On
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;//Z write Off
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilState[1].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		depthStencilDesc.DepthEnable = FALSE;//Z sort Off
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;//Z write On
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilState[2].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		depthStencilDesc.DepthEnable = FALSE;//Z sort Off
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;//Z write Off
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilState[3].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		depthStencilDesc.DepthEnable = FALSE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilState[4].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
	}

	//RasterizerState
	{
		D3D11_RASTERIZER_DESC rasterizerDesc{};
		rasterizerDesc.FrontCounterClockwise = TRUE;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0;
		rasterizerDesc.SlopeScaledDepthBias = 0;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.ScissorEnable = FALSE;
		rasterizerDesc.MultisampleEnable = FALSE;
		//ソリッド描画(裏面非表示)
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		rasterizerDesc.AntialiasedLineEnable = FALSE;
		hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerStates[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		//ワイヤーフレーム描画(裏面非表示)
		rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		rasterizerDesc.AntialiasedLineEnable = TRUE;
		hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerStates[1].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		//ソリッド描画(両面表示)
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.AntialiasedLineEnable = FALSE;
		hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerStates[2].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		//ワイヤーフレーム描画(両面表示)
		rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.AntialiasedLineEnable = TRUE;
		hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerStates[3].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		//プリミティブ(2D)
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.AntialiasedLineEnable = FALSE;
		rasterizerDesc.DepthClipEnable = FALSE;
		hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerStates[4].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		//プリミティブ(3D)
		rasterizerDesc.MultisampleEnable = TRUE;
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.AntialiasedLineEnable = FALSE;
		rasterizerDesc.DepthClipEnable = TRUE;
		hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerStates[5].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
	}
}
