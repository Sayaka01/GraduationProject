
#include "Stdafx.h"

#include  "Camera.h"


//初期化
void Camera::Initialize()
{
	name = "Camera";
}

//更新
void Camera::Update()
{

}

//描画
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
