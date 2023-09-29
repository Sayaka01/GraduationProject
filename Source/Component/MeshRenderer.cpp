
#include <Stdafx.h>
#include "MeshRenderer.h"

//‰Šú‰»
void MeshRenderer::Initialize()
{
	name = "MeshRenderer";
}

//XV
void MeshRenderer::Update()
{

}

//•`‰æ
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
