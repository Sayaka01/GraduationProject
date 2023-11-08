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
		coefficient = 0.999f;
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

	void AddVelocity(DirectX::SimpleMath::Vector3 velocity);

	void AddGravity()
	{
		AddVelocity({0, -gravity/** mass*/ ,0});
	};

	void SetVelocity(DirectX::SimpleMath::Vector3 vec)
	{
		velocity = vec;
	}

	void AddDrag()
	{
		DirectX::XMVECTOR Velo = DirectX::XMLoadFloat3(&velocity);
		DirectX::XMVECTOR Drag = DirectX::XMVectorScale(Velo, -coefficient * mass);
		AddForce(Drag);
	}

	void AddDragY()
	{
		//DirectX::XMVECTOR Velo = DirectX::XMLoadFloat3(&velocity);
		//DirectX::XMVECTOR Drag = DirectX::XMVectorScale(Velo, -coefficient * mass);
		//DirectX::XMFLOAT3 drag;
		//DirectX::XMStoreFloat3(&drag, Drag);
		//drag.x = 0.0f;
		//drag.z = 0.0f;
		//AddVelocity(DirectX::XMLoadFloat3(&drag));
	}

	void Jump(float jumpSpeed)
	{
		velocity.y = 0;
		AddVelocity({ 0,jumpSpeed,0 });
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

	float gravity = 0.98f;//�d��
};