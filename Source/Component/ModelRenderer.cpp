
#include <Stdafx.h>
#include "ModelRenderer.h"

//‰Šú‰»
void ModelRenderer::Initialize()
{
	name = "ModelRenderer";
}

//XV
void ModelRenderer::Update()
{

}

//•`‰æ
void ModelRenderer::Draw()
{

}

//ImGui
void ModelRenderer::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{

		ImGui::TreePop();
	}
}
