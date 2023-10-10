#include "Stdafx.h"

#include "SceneTest.h"

#include "SceneManager.h"

#include "../GameObject/GameObject.h"
#include "../GameObject/CameraController.h"

#include "Component/Transform.h"
#include "Component/SpriteRenderer.h"
#include "Component/ModelRenderer.h"
#include "Component/DirectionLight.h"
#include "Component/Player.h"
#include "System/SystemManager.h"
#include "System/Common.h"


void SceneTest::Initialize()
{
	cameraController = new CameraController();

	lightObject = new GameObject("Light");
	lightObject->AddComponent(new DirectionLight());

	PlayerInitialize();
}

void SceneTest::Finalize()
{
	player->Finalize();
	delete player;

	cameraController->Finalize();
	delete cameraController;

	lightObject->Finalize();
	delete lightObject;
}

void SceneTest::Update()
{
	cameraController->Update();

	lightObject->Update();

	player->Update();

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

	player->Draw();
}

void SceneTest::PlayerInitialize()
{
	player = new GameObject("player");
	//gameObject->AddComponent(new SpriteRenderer(L"./Resources/Sprite/optionBack.png"));
	
	player->AddComponent(new Player());
	
	ModelRenderer* modelRenderer = new ModelRenderer("./Resources/Model/Player/Jammo.fbx");

	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Attack.fbx");
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Death.fbx");
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Falling.fbx");
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/GetHit1.fbx");
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/GetHit2.fbx");
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Idle.fbx");
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Jump.fbx");
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Jump-Flip.fbx");
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Landing.fbx");
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Revive.fbx");
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Running.fbx");
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Walking.fbx");
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Punching.fbx");

	player->AddComponent(modelRenderer);

	player->GetComponent<Transform>()->scale = { 0.1f, 0.1f, 0.1f };

}