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
#include "Component/ThrowObstacle.h"
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
#if 1
	// まっつの就活用作成ステージ
	stage->AddComponent(new ModelRenderer("./Resources/Model/Stage/Stage.fbx"));
	stage->GetComponent<Transform>()->pos = { 0.0f, -1.0f, 0.0f };
	stage->GetComponent<Transform>()->scale = { 10.0f, 10.0f, 10.0f };
#else
	// Unity Asset Storeからとったステージ
	stage->AddComponent(new ModelRenderer("./Resources/Model/Stage/GameObject.fbx"));
	stage->GetComponent<Transform>()->pos = { 0.0f, 4.0f, 0.0f };
	stage->GetComponent<Transform>()->scale = { 0.045f, 0.045f, 0.045f };
#endif

	objectManager->AddChild(cameraController);
	objectManager->AddChild(lightObject);
	objectManager->AddChild(stage);
	objectManager->AddChild(player);

	EnemyInitialize();
	objectManager->AddChild(enemyManager);


	objectManager->AddChild(throwObjects);



	spriteManager = new GameObject("spriteManager");
	SpriteInitialze();



	//CollideManager::Instance().CreateBoundingBox(stage->GetComponent<ModelRenderer>());
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
	//ゲームが終わっている場合(GameClear || GameOver)
	if (isFinishGame)
	{
		spriteManager->Update();
		ResultUpdate();
		return;
	}

	//ゲームパッドの取得
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	if (gamePad.GetButtonUp() & GamePad::BTN_START)
	{
		isOpenMenu = !isOpenMenu;
	}

	//メニュー画面を開いているとき
	if (isOpenMenu)
	{
#if _DEBUG
		ImGui::Begin("Menu Screen");
		ImGui::End();
#endif

		if (gamePad.GetButtonUp() & GamePad::BTN_BACK)
			SceneManager::Instance().ChangeScene(new SceneTitle);
		return;
	}

	JudgeResult();


	objectManager->Update();

	CollideManager::Instance().Collide();

	PlayerUIUpdate();
	spriteManager->Update();

	//-----< IMGUI >-----//
#if _DEBUG
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("ImGui"))
			{
				if (ImGui::MenuItem("ObjectManager", "", &imguiObjctManager)) {}

				if (ImGui::MenuItem("SpriteManager", "", &imguiSpriteManager)) {}

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		if(imguiObjctManager) { objectManager->DebugGui(); }
		if(imguiSpriteManager) { spriteManager->DebugGui(); }
	}
#endif
}

void SceneGame::Draw()
{
	objectManager->Draw3D();

	CollideManager::Instance().Draw();

	objectManager->Draw2D();

	spriteManager->Draw2D();
}

void SceneGame::PlayerInitialize()
{
	player = new GameObject("player");

	player->SetTag(Tag::Player);

	player->GetComponent<Transform>()->pos.z = -170.0f;

	ModelRenderer* modelRenderer = new ModelRenderer("./Resources/Model/Player/Jammo.fbx");

	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/KnifeIdle.fbx");//Idle
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Walking.fbx");//Walking
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Running.fbx");//Running

	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/StandingJumpRunningToRunForward.fbx");//Jump
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/FallALoop.fbx");//Falling
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/FallALandToStandingIdle01.fbx");//Landing
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/RunningForwardFlip.fbx");//JumpFlip（空中回避）

	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/JumpOver.fbx");//Avoid

	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/CrossPunch.fbx");//PunchRight
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/Punching.fbx");//PunchLeft

	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/MmaKick.fbx");//Kick

	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/HookPunch.fbx");//JumpPunch

	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/BigHitToHead.fbx");//Damage
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/StandingReactDeathBackward.fbx");//Death

	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/StylishFlip.fbx");//DangleWire

	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/UpwardThrust.fbx");//Thrust
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/GreatSwordSlash.fbx");//Wield
	modelRenderer->AppendAnimation("./Resources/Model/Player/Animations/SwordAndShieldSlash.fbx");//Throw




	player->AddComponent(modelRenderer);//Player Componentより先につける

	player->AddComponent(new CapsuleCollider("BodyCapsule"));//Player Componentより先につける
	player->GetComponent<CapsuleCollider>("BodyCapsule")->priority = Collider::Priority::PlayerBody;

	player->AddComponent(new SphereCollider("RightHandSphere"));
	player->GetComponent<SphereCollider>("RightHandSphere")->radius = 2.5f;
	player->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(false);
	player->GetComponent<SphereCollider>("RightHandSphere")->type = Collider::Type::Offense;
	player->GetComponent<SphereCollider>("RightHandSphere")->priority = Collider::Priority::PlayerAttack;

	player->AddComponent(new SphereCollider("LeftHandSphere"));
	player->GetComponent<SphereCollider>("LeftHandSphere")->radius = 2.5f;
	player->GetComponent<SphereCollider>("LeftHandSphere")->SetEnable(false);
	player->GetComponent<SphereCollider>("LeftHandSphere")->type = Collider::Type::Offense;
	player->GetComponent<SphereCollider>("LeftHandSphere")->priority = Collider::Priority::PlayerAttack;

	player->AddComponent(new SphereCollider("RightAnkleSphere"));
	player->GetComponent<SphereCollider>("RightAnkleSphere")->radius = 2.0f;
	player->GetComponent<SphereCollider>("RightAnkleSphere")->SetEnable(false);
	player->GetComponent<SphereCollider>("RightAnkleSphere")->type = Collider::Type::Offense;
	player->GetComponent<SphereCollider>("RightAnkleSphere")->priority = Collider::Priority::PlayerAttack;

	player->AddComponent(new RigidBody());//Player Componentより先につける
	player->GetComponent<RigidBody>()->mass = 10.0f;

	player->AddComponent(new Player());

	player->GetComponent<Transform>()->scale = { 0.06f, 0.06f, 0.06f };

	player->AddComponent(new Health(30));

	player->AddComponent(new CapsuleCollider("WireCapsule"));
	player->GetComponent<CapsuleCollider>("WireCapsule")->radius = 0.05f;
	//player->GetComponent<CapsuleCollider>("WireCapsule")->radius = 2.0f;
	player->GetComponent<CapsuleCollider>("WireCapsule")->useHitEvent = false;
	player->GetComponent<CapsuleCollider>("WireCapsule")->SetEnable(false);
	player->GetComponent<CapsuleCollider>("WireCapsule")->drawReleasePrimitive = true;

	SpriteRenderer* spriteRenderer = new SpriteRenderer(L"./Resources/Sprite/AimTarget.png");
	spriteRenderer->pivot = spriteRenderer->texSize * 0.5f;
	player->AddComponent(spriteRenderer);



	//-----< Playerが投げる用のオブジェクト >-----//
	{
		throwObjects = new GameObject("throwObjects");

		GameObject* throwObj = new GameObject("throwObj");
		throwObj->SetTag(Tag::Player);
		throwObj->AddComponent(new ModelRenderer("./Resources/Model/Object/cube.fbx"));
		throwObj->GetComponent<Transform>()->pos = { 0.0f, 2.0f, -150.0f };
		throwObj->GetComponent<Transform>()->scale = { 2.0f, 2.0f, 2.0f };

		SphereCollider* sphereCollider = new SphereCollider();
		throwObj->AddComponent(sphereCollider);
		sphereCollider->radius = 2.0f;
		sphereCollider->useTransform = true;
		sphereCollider->priority = Collider::Priority::Obstacle;
		throwObj->AddComponent(new RigidBody());
		throwObj->AddComponent(new ThrowObstacle());//SphereColliderより後につける

		throwObjects->AddChild(throwObj);
	}
}

void SceneGame::EnemyInitialize()
{
	enemyManager = new GameObject("enemyManager");

	//敵の最大数
	const float MaxCountEnemy = 5;

	// 敵のモデルの読み込み

	// 
#if 0
	for (int i = 0; i < MaxCountEnemy; i++)
	{
		ModelRenderer* modelRenderer = new ModelRenderer("./Resources/Model/Enemy/miniRobotOrange.fbx");

		enemies.emplace_back(new GameObject("enemy_" + std::to_string(i)));
		enemies[i]->SetTag(Tag::Enemy);
		enemies[i]->AddComponent(modelRenderer);

		// 腰の当たり判定用球の設定
		enemies[i]->AddComponent(new SphereCollider("waist"));
		enemies[i]->GetComponent<SphereCollider>("waist")->radius = 3.0f;

		// 攻撃用当たり判定球の設定
		enemies[i]->AddComponent(new SphereCollider("HandCollider"));
		enemies[i]->GetComponent<SphereCollider>("HandCollider")->SetEnable(false);
		enemies[i]->GetComponent<SphereCollider>("HandCollider")->type = Collider::Type::Offense;
		// 移動設定
		enemies[i]->AddComponent(new RigidBody());

		enemies[i]->AddComponent(new Enemy());

		// 姿勢等の設定
		enemies[i]->GetComponent<Transform>()->scale = { 0.04f, 0.04f, 0.04f };
		enemies[i]->GetComponent<Transform>()->pos = { 20 + (i * 20.0f), 0, 30 };

		// HP設定
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

		// 腰の当たり判定用球の設定
		enemy->AddComponent(new SphereCollider("waist"));
		enemy->GetComponent<SphereCollider>("waist")->radius = 3.0f;
		enemy->GetComponent<SphereCollider>("waist")->type = Collider::Type::Deffense;

		// 攻撃用当たり判定球の設定
		enemy->AddComponent(new SphereCollider("HandCollider"));
		enemy->GetComponent<SphereCollider>("HandCollider")->SetEnable(false);
		enemy->GetComponent<SphereCollider>("HandCollider")->type = Collider::Type::Offense;
		enemy->GetComponent<SphereCollider>("HandCollider")->priority = Collider::Priority::EnemyAttack;

		// 移動設定
		enemy->AddComponent(new RigidBody());

		enemy->AddComponent(new Enemy(i));

		// 姿勢等の設定
		enemy->GetComponent<Transform>()->scale = { 0.04f, 0.04f, 0.04f };
		enemy->GetComponent<Transform>()->pos = { 20 + (i * 20.0f), 0, 30 };

		// HP設定
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

// 2D画像の初期設定
void SceneGame::SpriteInitialze()
{
	// プレイヤーUI画像の読み込み
	SpriteLoad(&sprBoxBarBack, "boxBarBack", (L"./Resources/Sprite/box_bar.png"),
		{ 147.4f,48.7f }, { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f });//pos,scale,color

	SpriteLoad(&sprBoxBar, "boxBar", (L"./Resources/Sprite/box_bar.png"),
		{ 147.4f,48.7f }, { 0.5f,0.5f }, { 0.3f,1.0f,0.3f,1.0f });//pos,scale,color

	SpriteLoad(&sprCircleBar, "circleBar", (L"./Resources/Sprite/circle_bar_satisfy.png"),
		{ -7.12f,-17.56f }, { 0.5f,0.5f }, { 1.0f,1.0f,0.0f,1.0f });//pos,scale,color

	SpriteLoad(&sprUiFrame, "uiFrame", (L"./Resources/Sprite/hp_bar.png"),
		{ 0.0f,0.0f }, { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f });//pos,scale,color

	// GameOver画像の読み込み
	SpriteLoad(&sprOverBack, "gameOverBack", (L"./Resources/Sprite/gameOverBack.png"),
		{ 0.0f,0.0f });//pos
	sprOverBack->GetComponent<SpriteRenderer>()->SetEnable(false);

	SpriteLoad(&sprOverText, "deadText", (L"./Resources/Sprite/text_dead.png"),
		{ SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f });//pos
	//基準点を画像の真ん中に設定
	sprOverText->GetComponent<SpriteRenderer>()->pivot = sprOverText->GetComponent<SpriteRenderer>()->GetSpriteSize() * 0.5f;
	sprOverText->GetComponent<SpriteRenderer>()->SetEnable(false);

	// GameClear画像の読み込み
	SpriteLoad(&sprClearBack, "gameClearBack", (L"./Resources/Sprite/gameClearBack.png"),
		{ 0.0f,0.0f });//pos
	sprClearBack->GetComponent<SpriteRenderer>()->SetEnable(false);

	SpriteLoad(&sprClearText, "clearText", (L"./Resources/Sprite/clear_text.png"),
		{ SCREEN_WIDTH * 0.5f,255.0f });//pos
	//基準点を画像の真ん中に設定
	sprClearText->GetComponent<SpriteRenderer>()->pivot = sprClearText->GetComponent<SpriteRenderer>()->GetSpriteSize() * 0.5f;
	sprClearText->GetComponent<SpriteRenderer>()->SetEnable(false);

	// "Title"の画像読み込み
	SpriteLoad(&sprTitleText, "ToTitleText", (L"./Resources/Sprite/title_text.png"),
		{ SCREEN_WIDTH * 0.5f,650 }, { 0.6f,0.6f });//pos,scale
	sprTitleText->GetComponent<SpriteRenderer>()->pivot = sprTitleText->GetComponent<SpriteRenderer>()->GetSpriteSize() * 0.5f;
	sprTitleText->GetComponent<SpriteRenderer>()->SetEnable(false);

	// "Retry"の画像読み込み
	SpriteLoad(&sprRetryText, "RetryText", (L"./Resources/Sprite/retry_text.png"),
		{ SCREEN_WIDTH * 0.5f,570 }, { 0.6f,0.6f });//pos,scale
	sprRetryText->GetComponent<SpriteRenderer>()->pivot = sprRetryText->GetComponent<SpriteRenderer>()->GetSpriteSize() * 0.5f;
	sprRetryText->GetComponent<SpriteRenderer>()->color = { 0.9f,0.9f,0,1 };
	sprRetryText->GetComponent<SpriteRenderer>()->SetEnable(false);

}

void SceneGame::SpriteLoad(GameObject** spr, std::string name, const wchar_t* filepath, SimpleMath::Vector2 pos, SimpleMath::Vector2 scale, SimpleMath::Vector4 color)
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

// UIの制御
void SceneGame::PlayerUIUpdate()
{
	float spriteSizeX = sprBoxBar->GetComponent<SpriteRenderer>()->GetSpriteSize().x;
	// 描画サイズをHPに合わせて変更
	sprBoxBar->GetComponent<SpriteRenderer>()->texSize.x = spriteSizeX * player->GetComponent<Health>()->GetHpRate();
	// HPの割合から描画位置をずらす
	float rate = player->GetComponent<Health>()->GetHpRate();
	sprBoxBar->GetComponent<SpriteRenderer>()->texPos.x = spriteSizeX * (1 - rate);

}

// ゲーム終了時の更新処理
void SceneGame::ResultUpdate()
{
	float elapsedTime = SystemManager::Instance().GetElapsedTime();
	//ゲームパッドの取得
	GamePad gamePad = SystemManager::Instance().GetGamePad();


	if (gamePad.GetAxisLY() >= 0.3f)//↑
	{
		if (selectNextScene == NextSelectName::Title)
		{
			selectNextScene = NextSelectName::Game;
			sprRetryText->GetComponent<SpriteRenderer>()->color = { 0.9f,0.9f,0,1 };
			sprTitleText->GetComponent<SpriteRenderer>()->color = { 1,1,1,1 };
		}
	}
	else if (gamePad.GetAxisLY() <= -0.3f)//↓
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

	//Aボタンが押されていたらtrue
	if (gamePad.GetButtonUp() & GamePad::BTN_A)
	{
		//retryを選択
		if (selectNextScene == NextSelectName::Game)
		{
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
		}
		//titleを選択
		else if (selectNextScene == NextSelectName::Title)
		{
			SceneManager::Instance().ChangeScene(new SceneTitle);
		}
	}
}

//UIのカラーを点滅させる(暗→明)
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

// ゲームが終了した場合の処理
void SceneGame::JudgeResult()
{
	bool isDeadP = player->GetComponent<Player>()->GetIsDead();
	if (isDeadP || enemyManager->GetChildrenCount() <= 0/*クエストがクリアされたら*/)
	{
		if (isDeadP)
		{
			//プレイヤーのHPが0になったらGameOverへ画面を遷移
			sprOverText->GetComponent<SpriteRenderer>()->SetEnable(true);
			sprOverBack->GetComponent<SpriteRenderer>()->SetEnable(true);
		}
		else
		{
			//クエストをクリアしたらGameClearへ画面を変更
			sprClearText->GetComponent<SpriteRenderer>()->SetEnable(true);
			sprClearBack->GetComponent<SpriteRenderer>()->SetEnable(true);
		}

		// "Title","Retry"画像を描画ON
		sprTitleText->GetComponent<SpriteRenderer>()->SetEnable(true);
		sprRetryText->GetComponent<SpriteRenderer>()->SetEnable(true);
		isFinishGame = true;
	}

}
