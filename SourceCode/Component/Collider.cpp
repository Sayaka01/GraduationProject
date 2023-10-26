#include "Stdafx.h"
#include "Collider.h"

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
