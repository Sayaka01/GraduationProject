
#include "Transform.h"

#include "../../../imgui/imgui.h"

//������
void Transform::Initialize()
{
	name = "Transform";
}

//�X�V
void Transform::Update()
{
	
}

//�`��
void Transform::Draw()
{
	
}

//�f�o�b�O�`��
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
