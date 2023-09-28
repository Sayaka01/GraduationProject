#pragma once

#include <DirectXMath.h>

#include "Component.h"

class Transform : public Component
{
public:
	Transform() = default;
	~Transform() = default;

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
	


	//-----< �ϐ� >-----//

	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };//�ʒu
	DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };//�X�P�[��
	DirectX::XMFLOAT4 orientation = { 0.0f,0.0f,0.0f,1.0f };//��]


};