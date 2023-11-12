

#include "Stdafx.h"
#include "SphereCollider.h"

#include "System/CollideManager.h"

SphereCollider::SphereCollider()
{
	CollideManager::Instance().AddSphereCollider(this);

	name = "SphereCollider";
}

SphereCollider::SphereCollider(std::string compName)
{
	CollideManager::Instance().AddSphereCollider(this);

	name = compName;
}

SphereCollider::~SphereCollider()
{
	CollideManager::Instance().RemoveSphereCollider(this);
}

void SphereCollider::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::DragFloat3("center", &center.x);

		ImGui::DragFloat("radius", &radius);
		Collider::DebugGui();
		ImGui::TreePop();
	}
}
