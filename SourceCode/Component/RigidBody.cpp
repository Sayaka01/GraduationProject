
#include <Stdafx.h>

//#include "../Sources/Meshes/ConstantBuffer.h"
//#include "../Sources/Tools/Logger.h"
//#include "../Sources/Tools/InlineUtl.h"

#include "RigidBody.h"
#include "GameObject/GameObject.h"

#include "Component/RigidBody.h"
#include "Component/Transform.h"
#include "System/SystemManager.h"
#include "System/UserFunction.h"
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

//‰Šú‰»
void RigidBody::Initialize()
{
	name = "RigidBody";
}

//I—¹ˆ—
void RigidBody::Finalize()
{
}

//XV
void RigidBody::Update()
{
	Transform* ownerTransform = parent->GetComponent<Transform>();

	if (useGravity && ownerTransform->pos.y > 0)
		AddGravity(SystemManager::Instance().GetElapsedTime());

	assert(mass > 0);
	//AddDragY();

	if (resultant.Length() > 0)
	{
		//AddDrag();
		acceleration = resultant * (1.0f / mass);
		velocity += acceleration * SystemManager::Instance().GetElapsedTime();
		ownerTransform->pos += velocity * SystemManager::Instance().GetElapsedTime();
		resultant = {};
	}
	else
	{
		ownerTransform->pos += velocity * SystemManager::Instance().GetElapsedTime();
		velocity = { 0,velocity.y,0 };
	}
}

//ImGui
void RigidBody::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::DragFloat3("velocity", &velocity.x, 0.1f);
		ImGui::DragFloat3("acceleration", &acceleration.x, 0.1f);
		ImGui::DragFloat("mass", &mass, 0.1f);
		ImGui::DragFloat("coefficient", &coefficient, 0.1f);
		ImGui::Checkbox("useGravity", &useGravity);
		if (ImGui::Button("ResetVelocity"))
		{
			velocity = { 0.0f,0.0f,0.0f };
		}

		ImGui::TreePop();
	}
}

void RigidBody::AddForce(DirectX::SimpleMath::Vector3 force)
{
	resultant += force;
}

void RigidBody::AddVelocity(DirectX::SimpleMath::Vector3 velo)
{
	velocity += velo;
}

