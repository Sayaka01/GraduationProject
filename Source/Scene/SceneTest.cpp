#include "Stdafx.h"

#include "SceneTest.h"

#include "SceneManager.h"

#include "../GameObject/GameObject.h"
#include "../GameObject/CameraController.h"

#include "Component/SpriteRenderer.h"
#include "Component/ModelRenderer.h"
#include "Component/DirectionLight.h"
#include "System/SystemManager.h"


void SceneTest::Initialize()
{
	gameObject = new GameObject("test");
	//gameObject->AddComponent(new SpriteRenderer(L"./Resources/Sprite/optionBack.png"));
	gameObject->AddComponent(new ModelRenderer("./Resources/Model/Player/Animations0707.fbx"));

	cameraController = new CameraController();

	lightObject = new GameObject("Light");
	lightObject->AddComponent(new DirectionLight());
}

void SceneTest::Finalize()
{
	gameObject->Finalize();
	delete gameObject;

	cameraController->Finalize();
	delete cameraController;

	lightObject->Finalize();
	delete lightObject;
}

void SceneTest::Update()
{
	cameraController->Update();

	lightObject->Update();

	gameObject->Update();

	//if (SystemManager::Instance().GetGamePad().GetAxisLX() > 0)
	//{
	//	SpriteRenderer* sprite = gameObject->GetComponent<SpriteRenderer>();
	//	sprite->pos.x += 50.0f * SystemManager::Instance().GetElapsedTime();
	//}
}

void SceneTest::Draw()
{
	cameraController->Draw();

	lightObject->Draw();

	gameObject->Draw();
}
