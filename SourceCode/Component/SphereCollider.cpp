

#include "Stdafx.h"
#include "SphereCollider.h"

#include "System/CollideManager.h"

SphereCollider::SphereCollider()
{
	CollideManager::Instance().AddSphereCollider(this);

	name = "SphereCollider";
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::DragFloat3("center", &center.x);
		ImGui::DragFloat("radius", &radius);
		ImGui::TreePop();
	}
}
