#include "Stdafx.h"
#include "Collider.h"

#include "GameObject/GameObject.h"

#include "Component/Transform.h"

void Collider::Update()
{
	if (useTransform)
	{
		center = parent->GetComponent<Transform>()->pos;
	}
}

void Collider::DebugGui()
{
	ImGui::DragInt("priority", &priority);
	ImGui::Checkbox("drawDebugPrimitive", &drawDebugPrimitive);
}

void Collider::OnCollisionEnter(Collider* collider)
{
	if (component)
		(component->*HitProcessFunc)(collider);
}
