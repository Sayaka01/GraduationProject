
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

//初期化
void RigidBody::Initialize()
{
	name = "RigidBody";
}

//終了処理
void RigidBody::Finalize()
{
}

//更新
void RigidBody::Update()
{
	if(useGravity)
		AddGravity();

	AddDrag();

	assert(mass > 0);

	Transform* ownerTransform = parent->GetComponent<Transform>();

	acceleration =  resultant * (1.0f / mass);
	velocity += acceleration * SystemManager::Instance().GetElapsedTime();
	ownerTransform->pos += velocity * SystemManager::Instance().GetElapsedTime();
	resultant = {};
}

//ImGui
void RigidBody::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::DragFloat3("velocity", &velocity.x, 0.1f);
		ImGui::DragFloat3("acceleration", &acceleration.x, 0.1f);
		ImGui::DragFloat("mass", &mass, 0.1f);
		ImGui::Checkbox("useGravity", &useGravity);
		ImGui::TreePop();
	}
}

void RigidBody::AddForce(DirectX::SimpleMath::Vector3 force)
{
	resultant += force;
}

