

#include "Stdafx.h"
#include "BoxCollider.h"

#include "System/CollideManager.h"

BoxCollider::BoxCollider()
{
	CollideManager::Instance().AddBoxCollider(this);

	name = "BoxCollider";
}

BoxCollider::BoxCollider(std::string compName)
{
	CollideManager::Instance().AddBoxCollider(this);

	name = compName;
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::DragFloat3("center", &center.x);

		ImGui::DragFloat3("size", &size.x);
		Collider::DebugGui();
		ImGui::TreePop();
	}
}
