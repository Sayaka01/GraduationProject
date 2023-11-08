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

	//-----< 関数 >-----//

	//初期化
	void Initialize() override;
	//終了処理
	void Finalize()override;
	//更新
	void Update() override;
	//ImGui
	void DebugGui() override;

	//RigidBodyに力を加える
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

	//-----< 変数 >-----//

	//RigidBodyが重力の影響をうけるかどうか
	bool useGravity{ true };

	float mass;//質量

private:
	DirectX::SimpleMath::Vector3 velocity = { 0.0f, 0.0f, 0.0f };//速度
	DirectX::SimpleMath::Vector3 acceleration;//加速度
	DirectX::SimpleMath::Vector3 resultant;//速度を加算合成し計算に使う
	float coefficient;//抵抗

	float gravity = 0.98f;//重力
};