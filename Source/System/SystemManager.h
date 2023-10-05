#pragma once

#include <d3d11.h>


class SystemManager final
{
private:
	//コンストラクタ
	SystemManager() = default;
	//デストラクタ
	virtual ~SystemManager() = default;

public:
	//インスタンス取得
	static SystemManager& Instance()
	{
		static SystemManager instance;
		return instance;
	}

	void Initialize();

	//-----< State >-----//
	enum class BlendState
	{
		None,
		Alpha,
		Add,
		Subtract,
		Replace,
		Multiply,
		Lighten,
		Darken,
		Screen,
		StateNum,
	};

	enum class DepthStencilState
	{
		SortOnWriteOn,
		SortOnWriteOff,
		SortOffWriteOn,
		SortOffWriteOff,
		Font,
		StateNum,
	};

	enum class RasterizerState
	{
		SolidCullingOn,
		WireframeCullingOn,
		SolidCullingOff,
		WireframeCullingOff,
		Primitive2D,
		Primitive3D,
		StateNum,
	};


	//-----< Getter, Setter >-----//

	//Device
	[[nodiscard]] ID3D11Device* GetDevice() const { return device; }
	void SetDevice(ID3D11Device* device)
	{
		this->device = device;
	}


	//DeviceContext
	[[nodiscard]] ID3D11DeviceContext* GetDeviceContext() const { return dc; }
	void SetDeviceContext(ID3D11DeviceContext* dc)
	{
		this->dc = dc;
	}


	//elapsedTime
	[[nodiscard]] float GetElapsedTime() const { return elapsedTime; }
	void SetElapsedTime(float elapsedTime)
	{
		this->elapsedTime = elapsedTime;
	}


	//BlendState
	[[nodiscard]] ID3D11BlendState* GetBlendState(BlendState state) const { return blendStates[static_cast<int>(state)].Get(); }
	void SetBlendState(BlendState state) const
	{
		dc->OMSetBlendState(blendStates[static_cast<int>(state)].Get(), nullptr, 0xFFFFFFFF);
	}


	//DepthStencilState
	[[nodiscard]] ID3D11DepthStencilState* GetDepthStencilState(DepthStencilState state) const { return depthStencilState[static_cast<int>(state)].Get(); }
	void SetDepthStencilState(DepthStencilState state) const
	{
		dc->OMSetDepthStencilState(depthStencilState[static_cast<int>(state)].Get(), 1);
	}

	//RasterizerState
	[[nodiscard]] ID3D11RasterizerState* GetRasterizerState(RasterizerState state) const { return rasterizerStates[static_cast<int>(state)].Get(); }
	void SetRasterizerState(RasterizerState state) const
	{
		dc->RSSetState(rasterizerStates[static_cast<int>(state)].Get());
	}



private:

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* dc = nullptr;

	float elapsedTime = 0.0f;


	Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[static_cast<int>(BlendState::StateNum)];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState[static_cast<int>(DepthStencilState::StateNum)];
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[static_cast<int>(RasterizerState::StateNum)];

};