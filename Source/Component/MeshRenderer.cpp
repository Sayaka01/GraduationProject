
#include <Stdafx.h>
#include "MeshRenderer.h"

//初期化
void MeshRenderer::Initialize()
{
	name = "MeshRenderer";
}

//更新
void MeshRenderer::Update()
{

}

//描画
void MeshRenderer::Draw()
{

}

//ImGui
void MeshRenderer::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		//ImGui::DragFloat3("pos", &pos.x);

		ImGui::TreePop();
	}
}
