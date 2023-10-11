#pragma once

#include <DirectXMath.h>

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
	//�`��
	void Draw() override;
	//ImGui
	void DebugGui() override;

	//-----< Getter, Setter >-----//


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