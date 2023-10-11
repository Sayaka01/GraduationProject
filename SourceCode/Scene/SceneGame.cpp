#include "Stdafx.h"

#include "SceneGame.h"

#include "SceneManager.h"

#include "../GameObject/GameObject.h"

#include "Component/SpriteRenderer.h"

#include "System/SystemManager.h"

#include <magic_enum.hpp>

void SceneGame::Initialize()
{
	gameObjectTest = new GameObject("game object test");
	gameObjectTest->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/text_FinishGame.png"));
}

void SceneGame::Finalize()
{
	gameObjectTest->Finalize();
	delete gameObjectTest;
}

void SceneGame::Update()
{
	gameObjectTest->Update();
}

void SceneGame::Draw()
{
	gameObjectTest->Draw();
}
