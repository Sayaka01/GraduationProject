
#include <Stdafx.h>
#include "ModelRenderer.h"

//初期化
void ModelRenderer::Initialize()
{
	name = "ModelRenderer";
}

//更新
void ModelRenderer::Update()
{

}

//描画
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
