#include "Stdafx.h"
#include "Collider.h"

#include "GameObject/GameObject.h"

#include "Component/Transform.h"

#include <magic_enum.hpp>

void Collider::Update()
{
	if (useTransform)
	{
		center = parent->GetComponent<Transform>()->pos;
	}
}

void Collider::DebugGui()
{
#if _APPEND
	int p = (int)priority;
	ImGui::SliderInt("priority", &p, 0, (int)Priority::PriorityCount);
	ImGui::Text(magic_enum::enum_name((Priority)p).data());
#endif
	ImGui::Checkbox("drawDebugPrimitive", &drawDebugPrimitive);
}

void Collider::OnCollisionEnter(Collider* collider)
{
	if (component)
		(component->*HitProcessFunc)(collider);
}
