
#include "Stdafx.h"

#include  "Camera.h"


//������
void Camera::Initialize()
{
	name = "Camera";
}

//�X�V
void Camera::Update()
{

}

//�`��
void Camera::Draw()
{

}

//ImGui
void Camera::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::TreePop();
	}
}
