#pragma once

#include <DirectXMath.h>
#include <SimpleMath.h>

#include "Component.h"

class Transform : public Component
{
public:
	Transform() = default;
	~Transform() override = default;

	//-----< �֐� >-----//

	//������
	void Initialize() override;
	//�X�V
	void Update() override;
	//ImGui
	void DebugGui() override;

	//-----< Getter, Setter >-----//
	DirectX::XMFLOAT3 GetRight()
	{ 
		DirectX::SimpleMath::Vector3 right = { world._11,world._12,world._13 };
		right.Normalize();
		return right; 
	}
	DirectX::XMFLOAT3 GetUp() 
	{
		DirectX::SimpleMath::Vector3 up = { world._21,world._22,world._23 };
		up.Normalize();
		return up;
	}
	DirectX::XMFLOAT3 GetForward() 
	{
		DirectX::SimpleMath::Vector3 forward = { world._31,world._32,world._33 };
		forward.Normalize();
		return forward;
	}

	//-----< �\���� >-----//

	//���W�ϊ��s��
	enum class CoordinateSystem
	{
		RHS_YUP,
		LHS_YUP,
		RHS_ZUP,
		LHS_ZUP,
	};

	DirectX::XMFLOAT4X4 GetCoordinateSystemTransform(float scaleFactor = 1.0f);
	CoordinateSystem coordinateSystem;

	//-----< �ϐ� >-----//

	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };//�ʒu
	DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };//�X�P�[��
	DirectX::XMFLOAT4 orientation = { 0.0f,0.0f,0.0f,1.0f };//��]
	DirectX::XMFLOAT4X4 world{};


};