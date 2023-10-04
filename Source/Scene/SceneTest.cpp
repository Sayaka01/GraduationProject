#include "Stdafx.h"

#include "SceneTest.h"

#include "SceneManager.h"

#include "../GameObject/GameObject.h"

#include "Component/SpriteRenderer.h"


void SceneTest::Initialize()
{
	gameObject = new GameObject("test");
	gameObject->AddComponent(new SpriteRenderer(L"./Resources/Sprite/optionBack.png"));
}

void SceneTest::Finalize()
{
	gameObject->Finalize();
	delete gameObject;
}

void SceneTest::Update()
{
	gameObject->Update();
}

void SceneTest::Draw()
{
	gameObject->Draw();
}
