

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
}

void SphereCollider::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		DirectX::XMFLOAT3 center = this->GetCenter();
		ImGui::DragFloat3("center", &center.x);
		this->SetCenter(center);

		ImGui::DragFloat("radius", &radius);
		Collider::DebugGui();
		ImGui::TreePop();
	}
}
