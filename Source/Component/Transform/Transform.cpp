
#include "Transform.h"

#include "../../../imgui/imgui.h"

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

//デバッグ描画
void Transform::DebugDraw()
{
	if(ImGui::TreeNode(name.c_str()))
	{
		ImGui::DragFloat3("pos", &pos.x);
		ImGui::DragFloat3("scale", &pos.x);
		ImGui::DragFloat4("orientation", &orientation.x);
		ImGui::TreePop();
	}
}
