
#include <Stdafx.h>
#include "SpriteRenderer.h"

#include "System/SystemManager.h"

//èâä˙âª
void SpriteRenderer::Initialize(const wchar_t* filePath, size_t max_sprites)
{
	name = "SpriteRenderer";

	spriteData = std::make_unique<SpriteData>(filePath, max_sprites);

	for (int i = 0; i < max_sprites; i++)
	{
		DirectX::XMFLOAT2 p = { 0,0 };
		positions.emplace_back(p);
	}

	texSize = spriteData->GetTextureSize();
}

//ï`âÊ
void SpriteRenderer::Draw2D()
{
	SystemManager::Instance().SetBlendState(SystemManager::BlendState::Alpha);
	SystemManager::Instance().SetDepthStencilState(SystemManager::DepthStencilState::SortOffWriteOff);
	SystemManager::Instance().SetRasterizerState(SystemManager::RasterizerState::SolidCullingOff);

	if (positions.size() <= 1)
		positions.at(0) = pos;

	spriteData->Begin(nullptr, nullptr);
	for (const auto& p : positions)
	{
		spriteData->Draw(p, scale, pivot, texPos, texSize, degree, color);
	}
	spriteData->End();

	SystemManager::Instance().SetDepthStencilState(SystemManager::DepthStencilState::SortOnWriteOn);
	SystemManager::Instance().SetRasterizerState(SystemManager::RasterizerState::SolidCullingOn);

}

//ImGui
void SpriteRenderer::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::DragFloat2("pos", &pos.x, 0.01f);
		ImGui::DragFloat2("scale", &scale.x, 0.01f);
		ImGui::DragFloat2("texSize", &texSize.x, 0.01f);
		float c[] = { color.x,color.y,color.z };
		ImGui::ColorEdit3("color", c);
		color = { c[0],c[1],c[2],color.w };

		ImGui::TreePop();
	}
}
