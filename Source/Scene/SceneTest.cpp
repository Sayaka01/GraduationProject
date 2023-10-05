#include "Stdafx.h"

#include "SceneTest.h"

#include "SceneManager.h"

#include "../GameObject/GameObject.h"

#include "Component/SpriteRenderer.h"
#include "Component/ModelRenderer.h"
#include "System/SystemManager.h"


void SceneTest::Initialize()
{
	gameObject = new GameObject("test");
	gameObject->AddComponent(new SpriteRenderer(L"./Resources/Sprite/optionBack.png"));
	gameObject->AddComponent(new ModelRenderer("./Resources/Model/Player/Animations0707.fbx"));
}

void SceneTest::Finalize()
{
	gameObject->Finalize();
	delete gameObject;
}

void SceneTest::Update()
{
	gameObject->Update();
	SpriteRenderer* sprite = gameObject->GetComponent<SpriteRenderer>();
	sprite->pos.x += 10.0f * SystemManager::Instance().GetElapsedTime();
}

void SceneTest::Draw()
{
	gameObject->Draw();
}
