
#include <Stdafx.h>
#include "ModelRenderer.h"

//������
void ModelRenderer::Initialize()
{
	name = "ModelRenderer";
}

//�X�V
void ModelRenderer::Update()
{

}

//�`��
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
