#include "Stdafx.h"

#include "SceneGame.h"

#include "SceneManager.h"

#include "SceneLoading.h"
#include "SceneTitle.h"

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
#include "../Component/Health.h"

#include "System/SystemManager.h"
#include "System/Common.h"

#include "System/CollideManager.h"


void SceneGame::Initialize()
{
	CollideManager::Instance().Initialize();

	objectManager = new GameObject("objectManager");

	PlayerInitialize();

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

	EnemyInitialize();
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
	//�Q�[�����I����Ă���ꍇ(GameClear || GameOver)
	if (isFinishGame)
	{
		spriteManager->Update();
		ResultUpdate();
		return;
	}

	JudgeResult();


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
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/StylishFlip.fbx");
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/HookPunch.fbx");
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/JumpOver.fbx");
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/RunningForwardFlip.fbx");

	player->AddComponent(modelRenderer);//Player Component����ɂ���

	player->AddComponent(new CapsuleCollider("BodyCapsule"));//Player Component����ɂ���
	player->GetComponent<CapsuleCollider>("BodyCapsule")->priority = 2;

	player->AddComponent(new SphereCollider("RightHandSphere"));//Player Component����ɂ���
	player->GetComponent<SphereCollider>("RightHandSphere")->radius = 2.5f;
	player->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(false);
	player->GetComponent<SphereCollider>("RightHandSphere")->type = Collider::Type::Offense;
	player->GetComponent<SphereCollider>("RightHandSphere")->priority = 0;
	player->AddComponent(new SphereCollider("LeftHandSphere"));//Player Component����ɂ���
	player->GetComponent<SphereCollider>("LeftHandSphere")->radius = 2.5f;
	player->GetComponent<SphereCollider>("LeftHandSphere")->SetEnable(false);
	player->GetComponent<SphereCollider>("LeftHandSphere")->type = Collider::Type::Offense;
	player->GetComponent<SphereCollider>("LeftHandSphere")->priority = 0;

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
		enemy->GetComponent<SphereCollider>("waist")->type = Collider::Type::Deffense;

		// �U���p�����蔻�苅�̐ݒ�
		enemy->AddComponent(new SphereCollider("HandCollider"));
		enemy->GetComponent<SphereCollider>("HandCollider")->SetEnable(false);
		enemy->GetComponent<SphereCollider>("HandCollider")->type = Collider::Type::Offense;
		enemy->GetComponent<SphereCollider>("HandCollider")->priority = 1;
		// �ړ��ݒ�
		enemy->AddComponent(new RigidBody());

		enemy->AddComponent(new Enemy(i));

		// �p�����̐ݒ�
		enemy->GetComponent<Transform>()->scale = { 0.04f, 0.04f, 0.04f };
		enemy->GetComponent<Transform>()->pos = { 20 + (i * 20.0f), 0, 30 };

		// HP�ݒ�
		enemy->AddComponent(new Health(5));

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

	// "Title"�̉摜�ǂݍ���
	SpriteLoad(&sprTitleText, "ToTitleText", (L"./Resources/Sprite/title_text.png"),
		{ SCREEN_WIDTH * 0.5f,650 }, { 0.6f,0.6f });//pos,scale
	sprTitleText->GetComponent<SpriteRenderer>()->pivot = sprTitleText->GetComponent<SpriteRenderer>()->GetSpriteSize() * 0.5f;
	sprTitleText->GetComponent<SpriteRenderer>()->SetEnable(false);

	// "Retry"�̉摜�ǂݍ���
	SpriteLoad(&sprRetryText, "RetryText", (L"./Resources/Sprite/retry_text.png"),
		{ SCREEN_WIDTH * 0.5f,570 }, { 0.6f,0.6f });//pos,scale
	sprRetryText->GetComponent<SpriteRenderer>()->pivot = sprRetryText->GetComponent<SpriteRenderer>()->GetSpriteSize() * 0.5f;
	sprRetryText->GetComponent<SpriteRenderer>()->color = { 0.9f,0.9f,0,1 };
	sprRetryText->GetComponent<SpriteRenderer>()->SetEnable(false);

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

// �Q�[���I�����̍X�V����
void SceneGame::ResultUpdate()
{
	float elapsedTime = SystemManager::Instance().GetElapsedTime();
	//�Q�[���p�b�h�̎擾
	GamePad gamePad = SystemManager::Instance().GetGamePad();

	
	if (gamePad.GetAxisLY() >= 0.3f)//��
	{
		if (selectNextScene == NextSelectName::Title)
		{
			selectNextScene = NextSelectName::Game;
			sprRetryText->GetComponent<SpriteRenderer>()->color = { 0.9f,0.9f,0,1 };
			sprTitleText->GetComponent<SpriteRenderer>()->color = { 1,1,1,1 };
		}
	}
	else if (gamePad.GetAxisLY() <= -0.3f)//��
	{
		if (selectNextScene == NextSelectName::Game)
		{
			selectNextScene = NextSelectName::Title;
			sprRetryText->GetComponent<SpriteRenderer>()->color = { 1,1,1,1 };
			sprTitleText->GetComponent<SpriteRenderer>()->color = { 0.9f, 0.9f, 0, 1 };
		}
	}

	if (selectNextScene == NextSelectName::Game)
	{
		FlashUiColor(sprRetryText->GetComponent<SpriteRenderer>());
	}
	else if (selectNextScene == NextSelectName::Title)
	{
		FlashUiColor(sprTitleText->GetComponent<SpriteRenderer>());
	}

	//A�{�^����������Ă�����true
	if (gamePad.GetButtonUp() & GamePad::BTN_A)
	{
		//retry��I��
		if (selectNextScene ==NextSelectName::Game)
		{
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
		}
		//title��I��
		else if (selectNextScene == NextSelectName::Title)
		{
			SceneManager::Instance().ChangeScene(new SceneTitle);
		}
	}
}

//UI�̃J���[��_�ł�����(�Á���)
void SceneGame::FlashUiColor(SpriteRenderer* spr)
{
	switch (flashUIState)
	{
	case 0:
		spr->color.x -= SystemManager::Instance().GetElapsedTime() * 0.7f;
		spr->color.y -= SystemManager::Instance().GetElapsedTime() * 0.7f;
		if (spr->color.x <= 0.55f)
			flashUIState = 1;
		break;
	case 1:
		spr->color.x += SystemManager::Instance().GetElapsedTime() * 0.7f;
		spr->color.y += SystemManager::Instance().GetElapsedTime() * 0.7f;
		if (spr->color.x >= 1.0)
			flashUIState = 0;
		break;
	}
}

// �Q�[�����I�������ꍇ�̏���
void SceneGame::JudgeResult()
{
	bool isDeadP = player->GetComponent<Player>()->GetIsDead();
	if (isDeadP || enemyManager->GetChildrenCount()<=0/*�N�G�X�g���N���A���ꂽ��*/)
	{
		if (isDeadP)
		{
			//�v���C���[��HP��0�ɂȂ�����GameOver�։�ʂ�J��
			sprOverText->GetComponent<SpriteRenderer>()->SetEnable(true);
			sprOverBack->GetComponent<SpriteRenderer>()->SetEnable(true);
		}
		else
		{
			//�N�G�X�g���N���A������GameClear�։�ʂ�ύX
			sprClearText->GetComponent<SpriteRenderer>()->SetEnable(true);
			sprClearBack->GetComponent<SpriteRenderer>()->SetEnable(true);
		}

		// "Title","Retry"�摜��`��ON
		sprTitleText->GetComponent<SpriteRenderer>()->SetEnable(true);
		sprRetryText->GetComponent<SpriteRenderer>()->SetEnable(true);
		isFinishGame = true;
	}

}
