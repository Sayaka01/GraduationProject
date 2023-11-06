#include "Stdafx.h"

#include "System/SystemManager.h"
#include "Health.h"

//void Health::Draw()
//{
//}
//

void Health::Update()
{
	//DebugGui();
}

void Health::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::DragFloat("current Hp", &currentHp);
		ImGui::DragFloat("max Hp", &maxHp);
		ImGui::TreePop();
	}
}



