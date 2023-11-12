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
#include "Component/Enemy.h"
#include "Component/SphereCollider.h"
#include "Component/BoxCollider.h"
#include "Component/CapsuleCollider.h"
#include "Component/SpriteRenderer.h"
#include "../Component/Health.h"

#include "System/SystemManager.h"
#include "System/Common.h"

#include "System/CollideManager.h"


void SceneGame::Initialize()
{
	CollideManager::Instance().Initialize();

	objectManager = new GameObject("objectManager");

	PlayerInitialize();
	EnemyInitialize();

	cameraController = new CameraController();
	cameraController->GetComponent<Camera>()->SetTargetObj(player);

	lightObject = new GameObject("Light");
	lightObject->AddComponent(new DirectionLight());

	stage = new GameObject("stage");
	stage->AddComponent(new ModelRenderer("./Resources/Model/Stage/Stage.fbx"));
	stage->GetComponent<Transform>()->pos = { 0.0f, -1.0f, 0.0f };
	stage->GetComponent<Transform>()->scale = { 10.0f, 10.0f, 10.0f };

	objectManager->AddChild(cameraController);
	objectManager->AddChild(lightObject);
	objectManager->AddChild(stage);
	objectManager->AddChild(player);
	objectManager->AddChild(enemy);

	spriteManager = new GameObject("spriteManager");

	SpriteInitialze();

	spriteManager->AddChild(sprBoxBarBack);
	spriteManager->AddChild(sprBoxBar);
	spriteManager->AddChild(sprCircleBar);
	spriteManager->AddChild(sprUiFrame);

	CollideManager::Instance().CreateBoundingBox(stage->GetComponent<ModelRenderer>());
}

void SceneGame::Finalize()
{
	CollideManager::Instance().Finalize();

	objectManager->Finalize();
	delete objectManager;

	spriteManager->Finalize();
	delete spriteManager;
	sprOverBack->Finalize();
	delete sprOverBack;
	sprOverText->Finalize();
	delete sprOverText;
}

void SceneGame::Update()
{
	objectManager->Update();

	CollideManager::Instance().Collide();

	PlayerUIUpdate();
	spriteManager->Update();
}

void SceneGame::Draw()
{
	objectManager->Draw();

	CollideManager::Instance().Draw();

	spriteManager->Draw();

	// GameOver時に描画
	//sprOverBack->Draw();
	//sprOverText->Draw();
}

void SceneGame::PlayerInitialize()
{
	player = new GameObject("player");

	player->SetTag(Tag::Player);

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

	player->AddComponent(new CapsuleCollider("BodyCapsule"));//Player Componentより先につける
	player->GetComponent<CapsuleCollider>("BodyCapsule")->priority = 1;

	player->AddComponent(new SphereCollider("RightHandSphere"));//Player Componentより先につける
	player->GetComponent<SphereCollider>("RightHandSphere")->radius = 2.5f;
	player->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(false);
	player->GetComponent<SphereCollider>("RightHandSphere")->type = Collider::Type::Offense;
	player->AddComponent(new SphereCollider("LeftHandSphere"));//Player Componentより先につける
	player->GetComponent<SphereCollider>("LeftHandSphere")->radius = 2.5f;
	player->GetComponent<SphereCollider>("LeftHandSphere")->SetEnable(false);
	player->GetComponent<SphereCollider>("LeftHandSphere")->type = Collider::Type::Offense;

	player->AddComponent(new RigidBody());//Player Componentより先につける
	player->GetComponent<RigidBody>()->mass = 10.0f;

	player->AddComponent(new Player());

	player->GetComponent<Transform>()->scale = { 0.06f, 0.06f, 0.06f };


	player->AddComponent(new Health(30));

	player->AddComponent(new CapsuleCollider("WireCapsule"));
	player->GetComponent<CapsuleCollider>("WireCapsule")->radius = 0.05f;
	player->GetComponent<CapsuleCollider>("WireCapsule")->useHitEvent = false;
	player->GetComponent<CapsuleCollider>("WireCapsule")->SetEnable(false);

	player->AddComponent(new SphereCollider("DebugSphere"));
	player->GetComponent<SphereCollider>("DebugSphere")->debugColor = { 1.0f,0.5f,0.5f, 1.0f };
}

void SceneGame::EnemyInitialize()
{
	enemy = new GameObject("enemy");

	enemy->SetTag(Tag::Enemy);

	ModelRenderer* modelRenderer = new ModelRenderer("./Resources/Model/Enemy/miniRobotOrange.fbx");

	//modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Attack.fbx");
	//modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Death.fbx");
	//modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Falling.fbx");
	//modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/GetHit1.fbx");
	//modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/GetHit2.fbx");
	//modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Idle.fbx");
	//modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Jump.fbx");
	//modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Jump-Flip.fbx");
	//modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Landing.fbx");
	//modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Revive.fbx");
	//modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Running.fbx");
	//modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Walking.fbx");
	//modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Punching.fbx");

	enemy->AddComponent(modelRenderer);

	enemy->AddComponent(new SphereCollider("waist"));
	enemy->AddComponent(new SphereCollider("attackRightHand"));
	enemy->AddComponent(new RigidBody());

	enemy->AddComponent(new Enemy());

	enemy->GetComponent<Transform>()->scale = { 0.04f, 0.04f, 0.04f };
	enemy->GetComponent<Transform>()->pos = { 20, 0, 30 };

	enemy->AddComponent(new Health(10));
}

// 2D画像の初期設定
void SceneGame::SpriteInitialze()
{
	//タイトルロゴ画像の読み込み
	SpriteLoad(&sprBoxBar,	  "boxBar",	    (L"./Resources/Sprite/box_bar.png"),
		{ 147.4f,48.7f },   { 0.5f,0.5f }, { 0.3f,1.0f,0.3f,1.0f });//pos,scale,color

	SpriteLoad(&sprBoxBarBack, "boxBarBack", (L"./Resources/Sprite/box_bar.png"),			
		{ 147.4f,48.7f },   { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f });//pos,scale,color

	SpriteLoad(&sprCircleBar,  "circleBar",  (L"./Resources/Sprite/circle_bar_satisfy.png"), 
		{ -7.12f,-17.56f },	{ 0.5f,0.5f }, { 1.0f,1.0f,0.0f,1.0f });//pos,scale,color

	SpriteLoad(&sprUiFrame,	  "uiFrame",	(L"./Resources/Sprite/hp_bar.png"),				
		{ 0.0f,0.0f },      { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f });//pos,scale,color

	// GameOver画像の読み込み
	SpriteLoad(&sprOverBack, "gameOverBack", (L"./Resources/Sprite/gameOverBack.png"),
		{ 0.0f,0.0f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });//pos,scale,color

	SpriteLoad(&sprOverText, "gameOverBack", (L"./Resources/Sprite/text_dead.png"),
		{ SCREEN_WIDTH*0.5f,SCREEN_HEIGHT*0.5f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });//pos,scale,color
	//基準点を画像の真ん中に設定
	sprOverText->GetComponent<SpriteRenderer>()->pivot = sprOverText->GetComponent<SpriteRenderer>()->GetSpriteSize() * 0.5f;
}

void SceneGame::SpriteLoad(GameObject** spr,std::string name,const wchar_t* filepath, SimpleMath::Vector2 pos, SimpleMath::Vector2 scale, SimpleMath::Vector4 color)
{
	if (name == "")
		(*spr) = new GameObject();
	else
		(*spr) = new GameObject(name);
	(*spr)->AddComponent(new SpriteRenderer(filepath));
	(*spr)->GetComponent<SpriteRenderer>()->pos = pos;
	(*spr)->GetComponent<SpriteRenderer>()->scale = scale;
	(*spr)->GetComponent<SpriteRenderer>()->color = color;
}

// UIの制御
void SceneGame::PlayerUIUpdate()
{
	float spriteSizeX = sprBoxBar->GetComponent<SpriteRenderer>()->GetSpriteSize().x;
	// 描画サイズをHPに合わせて変更
	sprBoxBar->GetComponent<SpriteRenderer>()->texSize.x = spriteSizeX*player->GetComponent<Health>()->GetHpRate();
	// HPの割合から描画位置をずらす
	float rate = player->GetComponent<Health>()->GetHpRate();
	sprBoxBar->GetComponent<SpriteRenderer>()->texPos.x = spriteSizeX * (1 - rate);

}
