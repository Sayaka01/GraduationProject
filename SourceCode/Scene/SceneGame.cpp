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
	objectManager->AddChild(enemyManager);

	spriteManager = new GameObject("spriteManager");

	SpriteInitialze();


	CollideManager::Instance().CreateBoundingBox(stage->GetComponent<ModelRenderer>());
}

void SceneGame::Finalize()
{
	CollideManager::Instance().Finalize();

	objectManager->Finalize();
	delete objectManager;

	spriteManager->Finalize();
	delete spriteManager;
}

void SceneGame::Update()
{
	//�v���C���[��HP��0�ɂȂ�����GameOver�։�ʂ�J��
	if (player->GetComponent<Player>()->GetIsDead())
	{
		sprOverText->GetComponent<SpriteRenderer>()->SetEnable(true);
		sprOverBack->GetComponent<SpriteRenderer>()->SetEnable(true);
		spriteManager->Update();
		return;
	}
	//�N�G�X�g���N���A������GameClear�։�ʂ�ύX
	else if (isQuestClear)
	{
		sprClearText->GetComponent<SpriteRenderer>()->SetEnable(true);
		sprClearBack->GetComponent<SpriteRenderer>()->SetEnable(true);
		spriteManager->Update();
		return;
	}


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

	ImGui::Begin("Game");
	ImGui::Checkbox("clear button", &isQuestClear);
	ImGui::End();
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

	player->AddComponent(new CapsuleCollider("BodyCapsule"));//Player Component����ɂ���
	player->GetComponent<CapsuleCollider>("BodyCapsule")->priority = 1;

	player->AddComponent(new SphereCollider("RightHandSphere"));//Player Component����ɂ���
	player->GetComponent<SphereCollider>("RightHandSphere")->radius = 2.5f;
	player->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(false);
	player->GetComponent<SphereCollider>("RightHandSphere")->type = Collider::Type::Offense;
	player->AddComponent(new SphereCollider("LeftHandSphere"));//Player Component����ɂ���
	player->GetComponent<SphereCollider>("LeftHandSphere")->radius = 2.5f;
	player->GetComponent<SphereCollider>("LeftHandSphere")->SetEnable(false);
	player->GetComponent<SphereCollider>("LeftHandSphere")->type = Collider::Type::Offense;

	player->AddComponent(new RigidBody());//Player Component����ɂ���
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
	enemyManager = new GameObject("enemyManager");

	//�G�̍ő吔
	const float MaxCountEnemy = 5;

	// �G�̃��f���̓ǂݍ���

	// 
#if 0
	for (int i = 0; i < MaxCountEnemy; i++)
	{
		ModelRenderer* modelRenderer = new ModelRenderer("./Resources/Model/Enemy/miniRobotOrange.fbx");

		enemies.emplace_back(new GameObject("enemy_"+std::to_string(i)));
		enemies[i]->SetTag(Tag::Enemy);
		enemies[i]->AddComponent(modelRenderer);

		// ���̓����蔻��p���̐ݒ�
		enemies[i]->AddComponent(new SphereCollider("waist"));
		enemies[i]->GetComponent<SphereCollider>("waist")->radius = 3.0f;

		// �U���p�����蔻�苅�̐ݒ�
		enemies[i]->AddComponent(new SphereCollider("HandCollider"));
		enemies[i]->GetComponent<SphereCollider>("HandCollider")->SetEnable(false);
		enemies[i]->GetComponent<SphereCollider>("HandCollider")->type = Collider::Type::Offense;
		// �ړ��ݒ�
		enemies[i]->AddComponent(new RigidBody());

		enemies[i]->AddComponent(new Enemy());

		// �p�����̐ݒ�
		enemies[i]->GetComponent<Transform>()->scale = { 0.04f, 0.04f, 0.04f };
		enemies[i]->GetComponent<Transform>()->pos = { 20+(i*20.0f), 0, 30 };

		// HP�ݒ�
		enemies[i]->AddComponent(new Health(10));

		enemyManager->AddChild(enemies[i]);

	}
#else
	for (int i = 0; i < MaxCountEnemy; i++)
	{
		GameObject* enemy = new GameObject("enemy_" + std::to_string(i));

		ModelRenderer* modelRenderer = new ModelRenderer("./Resources/Model/Enemy/miniRobotOrange.fbx");

		enemy->SetTag(Tag::Enemy);
		enemy->AddComponent(modelRenderer);

		// ���̓����蔻��p���̐ݒ�
		enemy->AddComponent(new SphereCollider("waist"));
		enemy->GetComponent<SphereCollider>("waist")->radius = 3.0f;

		// �U���p�����蔻�苅�̐ݒ�
		enemy->AddComponent(new SphereCollider("HandCollider"));
		enemy->GetComponent<SphereCollider>("HandCollider")->SetEnable(false);
		enemy->GetComponent<SphereCollider>("HandCollider")->type = Collider::Type::Offense;
		// �ړ��ݒ�
		enemy->AddComponent(new RigidBody());

		enemy->AddComponent(new Enemy(i));

		// �p�����̐ݒ�
		enemy->GetComponent<Transform>()->scale = { 0.04f, 0.04f, 0.04f };
		enemy->GetComponent<Transform>()->pos = { 20 + (i * 20.0f), 0, 30 };

		// HP�ݒ�
		enemy->AddComponent(new Health(10));

		enemyManager->AddChild(enemy);

	}
#endif

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


}

// 2D�摜�̏����ݒ�
void SceneGame::SpriteInitialze()
{
	// �v���C���[UI�摜�̓ǂݍ���
	SpriteLoad(&sprBoxBarBack, "boxBarBack", (L"./Resources/Sprite/box_bar.png"),			
		{ 147.4f,48.7f },   { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f });//pos,scale,color
	
	SpriteLoad(&sprBoxBar, "boxBar", (L"./Resources/Sprite/box_bar.png"),
		{ 147.4f,48.7f }, { 0.5f,0.5f }, { 0.3f,1.0f,0.3f,1.0f });//pos,scale,color

	SpriteLoad(&sprCircleBar,  "circleBar",  (L"./Resources/Sprite/circle_bar_satisfy.png"), 
		{ -7.12f,-17.56f },	{ 0.5f,0.5f }, { 1.0f,1.0f,0.0f,1.0f });//pos,scale,color

	SpriteLoad(&sprUiFrame,	  "uiFrame",	(L"./Resources/Sprite/hp_bar.png"),				
		{ 0.0f,0.0f },      { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f });//pos,scale,color

	// GameOver�摜�̓ǂݍ���
	SpriteLoad(&sprOverBack, "gameOverBack", (L"./Resources/Sprite/gameOverBack.png"),
		{ 0.0f,0.0f });//pos
	sprOverBack->GetComponent<SpriteRenderer>()->SetEnable(false);

	SpriteLoad(&sprOverText, "deadText", (L"./Resources/Sprite/text_dead.png"),
		{ SCREEN_WIDTH*0.5f,SCREEN_HEIGHT*0.5f });//pos
	//��_���摜�̐^�񒆂ɐݒ�
	sprOverText->GetComponent<SpriteRenderer>()->pivot = sprOverText->GetComponent<SpriteRenderer>()->GetSpriteSize() * 0.5f;
	sprOverText->GetComponent<SpriteRenderer>()->SetEnable(false);
	
	// GameClear�摜�̓ǂݍ���
	SpriteLoad(&sprClearBack, "gameClearBack", (L"./Resources/Sprite/gameClearBack.png"),
		{ 0.0f,0.0f });//pos
	sprClearBack->GetComponent<SpriteRenderer>()->SetEnable(false);

	SpriteLoad(&sprClearText, "clearText", (L"./Resources/Sprite/clear_text.png"),
		{ SCREEN_WIDTH*0.5f,255.0f });//pos
	//��_���摜�̐^�񒆂ɐݒ�
	sprClearText->GetComponent<SpriteRenderer>()->pivot = sprClearText->GetComponent<SpriteRenderer>()->GetSpriteSize() * 0.5f;
	sprClearText->GetComponent<SpriteRenderer>()->SetEnable(false);
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

	spriteManager->AddChild(*spr);
}

// UI�̐���
void SceneGame::PlayerUIUpdate()
{
	float spriteSizeX = sprBoxBar->GetComponent<SpriteRenderer>()->GetSpriteSize().x;
	// �`��T�C�Y��HP�ɍ��킹�ĕύX
	sprBoxBar->GetComponent<SpriteRenderer>()->texSize.x = spriteSizeX*player->GetComponent<Health>()->GetHpRate();
	// HP�̊�������`��ʒu�����炷
	float rate = player->GetComponent<Health>()->GetHpRate();
	sprBoxBar->GetComponent<SpriteRenderer>()->texPos.x = spriteSizeX * (1 - rate);

}
