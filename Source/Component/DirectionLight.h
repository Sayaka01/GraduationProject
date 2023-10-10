#pragma once
#include <DirectXMath.h>

#include "Component.h"

#include "System/ConstantBuffer.h"

class DirectionLight : public Component
{
public:
	DirectionLight();
	~DirectionLight()override = default;

	//�֐�
	void Draw()override;
	void DebugGui()override;

	void UpdateBuffer();

	DirectX::XMFLOAT4 ambientColor{ 0.03f, 0.03f, 0.03f, 0.03f };
	DirectX::XMFLOAT4 lightDirection{ 0.0f, -1.0f, 0.5f, 1.0f };
	DirectX::XMFLOAT4 lightColor{ 0.5f, 0.5f, 1.0f, 1.5f };


private:

	//�萔�o�b�t�@
	struct LightConstants
	{
		DirectX::XMFLOAT4 ambientColor{ 0.03f, 0.03f, 0.03f, 0.03f };
		DirectX::XMFLOAT4 lightDirection{ 0.0f, -1.0f, 0.5f, 1.0f };
		DirectX::XMFLOAT4 lightColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	}lightConstants{};

	ConstantBuffer constantBuffer;



};