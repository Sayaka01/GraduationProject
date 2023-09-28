
#include <Stdafx.h>
#include "Transform.h"

//初期化
void Transform::Initialize()
{
	name = "Transform";
}

//更新
void Transform::Update()
{
	
}

//描画
void Transform::Draw()
{
	
}

//ImGui
void Transform::DebugGui()
{
	if(ImGui::TreeNode(name.c_str()))
	{
		ImGui::DragFloat3("pos", &pos.x);
		ImGui::DragFloat3("scale", &pos.x);
		ImGui::DragFloat4("orientation", &orientation.x);
		ImGui::TreePop();
	}
}
