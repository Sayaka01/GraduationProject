
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
	if(useGravity)
		AddGravity();


	assert(mass > 0);

	Transform* ownerTransform = parent->GetComponent<Transform>();

	acceleration =  resultant;
	velocity = acceleration * SystemManager::Instance().GetElapsedTime();
	AddDrag();
	ownerTransform->pos += velocity;
	resultant = {};
}

//ImGui
void RigidBody::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::DragFloat3("velocity", &velocity.x, 0.1f);
		ImGui::DragFloat("mass", &mass, 0.1f);
		ImGui::Checkbox("useGravity", &useGravity);
		ImGui::TreePop();
	}
}

void RigidBody::AddForce(DirectX::SimpleMath::Vector3 force)
{
	resultant += force;
}

