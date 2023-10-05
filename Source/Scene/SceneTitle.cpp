#include "Stdafx.h"

#include "SceneTitle.h"

#include "SceneManager.h"

#include "../GameObject/GameObject.h"

#include "Component/SpriteRenderer.h"

#include <magic_enum.hpp>

void SceneTitle::Initialize()
{
	for (int i = 0; i < 3; i++)
	{
		menuTextName[i] = std::to_string(i) + magic_enum::enum_name((MenuTextString)i).data();
	}

	menuText.emplace(menuTextName[MenuTextString::Tutorial],new GameObject("tutorialPlay"));
	menuText[menuTextName[MenuTextString::Tutorial]]->AddComponent(new SpriteRenderer(L"./Resources/Sprite/optionBack.png"));
}

void SceneTitle::Finalize()
{
	for (auto& textes : menuText)
	{
		textes.second->Finalize();
	}
}

void SceneTitle::Update()
{
	for (auto& textes : menuText)
	{
		textes.second->Update();
	}
}

void SceneTitle::Draw()
{
	for (auto& textes : menuText)
	{
		textes.second->Draw();
	}
}
