#include "Stdafx.h"

#include "SceneTest.h"

#include "SceneManager.h"

#include "../GameObject/GameObject.h"
#include "../GameObject/CameraController.h"

#include "Component/SpriteRenderer.h"
#include "Component/ModelRenderer.h"
#include "System/SystemManager.h"


void SceneTest::Initialize()
{
	gameObject = new GameObject("test");
	//gameObject->AddComponent(new SpriteRenderer(L"./Resources/Sprite/optionBack.png"));
	gameObject->AddComponent(new ModelRenderer("./Resources/Model/Player/Animations0707.fbx"));

	cameraController = new CameraController();
}

void SceneTest::Finalize()
{
	gameObject->Finalize();
	delete gameObject;

	cameraController->Finalize();
	delete cameraController;
}

void SceneTest::Update()
{
	cameraController->Update();

	gameObject->Update();

	//if (SystemManager::Instance().GetGamePad().GetAxisLX() > 0)
	//{
	//	SpriteRenderer* sprite = gameObject->GetComponent<SpriteRenderer>();
	//	sprite->pos.x += 50.0f * SystemManager::Instance().GetElapsedTime();
	//}
}

void SceneTest::Draw()
{
	gameObject->Draw();
}
