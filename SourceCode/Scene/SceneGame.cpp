#include "Stdafx.h"

#include "SceneGame.h"

#include "SceneManager.h"

#include "../GameObject/GameObject.h"
#include "../GameObject/CameraController.h"

#include "Component/Transform.h"
#include "Component/Camera.h"
#include "Component/SpriteRenderer.h"
#include "Component/ModelRenderer.h"
#include "Component/DirectionLight.h"
#include "Component/Player.h"

#include "System/SystemManager.h"
#include "System/Common.h"


void SceneGame::Initialize()
{

	objectManager = new GameObject("objectManager");

	PlayerInitialize();

	cameraController = new CameraController();
	cameraController->GetComponent<Camera>()->SetTargetObj(player);

	lightObject = new GameObject("Light");
	lightObject->AddComponent(new DirectionLight());

	stage = new GameObject("stage");
	stage->AddComponent(new ModelRenderer("./Resources/Model/Stage/Stage.fbx"));
	stage->GetComponent<Transform>()->scale = { 10.0f, 10.0f, 10.0f };

	objectManager->AddChild(cameraController);
	objectManager->AddChild(lightObject);
	objectManager->AddChild(stage);
	objectManager->AddChild(player);
}

void SceneGame::Finalize()
{
	objectManager->Finalize();
	delete objectManager;
}

void SceneGame::Update()
{
	objectManager->Update();
}

void SceneGame::Draw()
{
	objectManager->Draw();
}

void SceneGame::PlayerInitialize()
{
	player = new GameObject("player");
	//gameObject->AddComponent(new SpriteRenderer(L"./Resources/Sprite/optionBack.png"));

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

	player->AddComponent(new Player());

	player->GetComponent<Transform>()->scale = { 0.06f, 0.06f, 0.06f };

}