#pragma once

#include <DirectXMath.h>

#include "Component.h"

#include "SimpleMath.h"

class ConstantBuffer;

class RigidBody : public Component
{
public:
	RigidBody()
	{
		mass = 1;
		velocity = {};
		acceleration = {};
		resultant = {};
	}
	~RigidBody() override = default;

	//-----< �֐� >-----//

	//������
	void Initialize() override;
	//�I������
	void Finalize()override;
	//�X�V
	void Update() override;
	//ImGui
	void DebugGui() override;

	//RigidBody�ɗ͂�������
	void AddForce(DirectX::SimpleMath::Vector3 force);

	void AddGravity()
	{
		AddForce({0,gravity * mass ,0});
	};

	void AddDrag()
	{
		DirectX::XMVECTOR Velo = DirectX::XMLoadFloat3(&velocity);
		DirectX::XMVECTOR Drag = DirectX::XMVectorScale(Velo, -coefficient);
		AddForce(Drag);
	}

	//-----< Getter, Setter >-----//
	bool GetUseGravity() { return useGravity;}
	void SetUseGravity(bool doUse) { useGravity = doUse; }

	void SetGravity(float g) { gravity = g; }
	float GetGravity() { return gravity; }

	DirectX::SimpleMath::Vector3 GetVelocity() { return velocity; }

	//-----< �ϐ� >-----//

	//RigidBody���d�͂̉e���������邩�ǂ���
	bool useGravity{ true };

	float mass;//����

private:
	DirectX::SimpleMath::Vector3 velocity = { 0.0f, 0.0f, 0.0f };//���x
	DirectX::SimpleMath::Vector3 acceleration;//�����x
	DirectX::SimpleMath::Vector3 resultant;//���x�����Z�������v�Z�Ɏg��
	float coefficient;//��R

	float gravity = 2.0f;//�d��
};