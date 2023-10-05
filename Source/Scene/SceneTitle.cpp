#include "Stdafx.h"

#include "SceneTitile.h"

#include "SceneManager.h"

#include "../GameObject/GameObject.h"

#include "Component/SpriteRenderer.h"


void SceneTitle::Initialize()
{
	gameObject = new GameObject("test");
	gameObject->AddComponent(new SpriteRenderer(L"./Resources/Sprite/optionBack.png"));
}

void SceneTitle::Finalize()
{
	gameObject->Finalize();
	delete gameObject;
}

void SceneTitle::Update()
{
	gameObject->Update();
}

void SceneTitle::Draw()
{
	gameObject->Draw();
}
