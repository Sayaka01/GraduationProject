#include "Stdafx.h"

#include "SceneGame.h"

#include "SceneManager.h"

#include "../GameObject/GameObject.h"

#include "Component/SpriteRenderer.h"

#include "System/SystemManager.h"

#include <magic_enum.hpp>

void SceneGame::Initialize()
{
	for (int i = 0; i < (int)MenuTextString::Max_mst; i++)
	{
		menuTextName[i] = std::to_string(i) + magic_enum::enum_name((MenuTextString)i).data();
	}
	//「TutorialPlay」画像の読み込み
	menuText.emplace(menuTextName[(int)MenuTextString::Tutorial_mst], new GameObject("tutorialPlay"));
	menuText[menuTextName[(int)MenuTextString::Tutorial_mst]]->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/text_TutorialPlay.png"));
	//「StartGame」画像の読み込み
	menuText.emplace(menuTextName[(int)MenuTextString::StartGame_mst], new GameObject("startGame"));
	menuText[menuTextName[(int)MenuTextString::StartGame_mst]]->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/text_StartGame.png"));
	//「FinishGame」画像の読み込み
	menuText.emplace(menuTextName[(int)MenuTextString::FinishGame_mst], new GameObject("finishGame"));
	menuText[menuTextName[(int)MenuTextString::FinishGame_mst]]->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/text_FinishGame.png"));

	titleLogo = std::make_unique<GameObject>("TitleLogo");
	titleLogo->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/title_logo.png"));
	titleLogo->GetComponent<SpriteRenderer>()->pos = { 400,-100 };

	currentSelectDegree = -120;
	menuRadius = { 400,300 };
	circlePivot = { -250, 400 };
	float spacing = 0.0f;//画像同士の間隔
	for (auto& textes : menuText)
	{
		currentSelectDegree = spacing;
		textes.second->GetComponent<SpriteRenderer>()->pos = moveRoundFloat2(circlePivot, { currentSelectDegree + spacing,currentSelectDegree + spacing }, menuRadius);
		spacing += 40.0f;
	}
}

void SceneGame::Finalize()
{
	for (auto& textes : menuText)
	{
		textes.second->Finalize();
	}
	titleLogo->Finalize();
}

void SceneGame::Update()
{
	DirectX::XMFLOAT2 rePoint = { 0,0 };
	static int operateType = -1;
	//1フレームの経過時間を取得
	float deltaTime = SystemManager::Instance().GetElapsedTime();
	GamePad& gamepad = SystemManager::Instance().GetGamePad();
	float rotationSpeed = 150;
	if (gamepad.GetButtonDown() & GamePad::BTN_UP)
	{
		if ((int)selectMenuType > 0)
		{
			selectMenuType -= 1;
			operateType = 0;
		}
	}
	if (gamepad.GetButtonDown() & GamePad::BTN_DOWN)
	{
		if ((int)selectMenuType < (int)MenuTextString::Max_mst - 1)
		{
			selectMenuType += 1;
			operateType = 1;
		}
	}

	switch (selectMenuType)
	{
	case 0://"Tutorial Play"のとき
		targetSelectDegree = 0;
		//SceneManager::Instance().ChangeScene(new SceneGame);
		break;
	case 1://"Start Game"のとき
		targetSelectDegree = -40;
		//SceneManager::Instance().ChangeScene(new SceneGame);
		break;
	case 2://"Finish Game"のとき
		targetSelectDegree = -80;
		PostQuitMessage(0);
		break;
	}

	switch (operateType)
	{
	case -1:
		currentSelectDegree = targetSelectDegree;
		break;
	case 0:
		currentSelectDegree = currentSelectDegree + deltaTime * rotationSpeed;
		if (currentSelectDegree > targetSelectDegree)
			operateType = -1;
		break;
	case 1:
		currentSelectDegree = currentSelectDegree - deltaTime * rotationSpeed;
		if (currentSelectDegree < targetSelectDegree)
			operateType = -1;
		break;
	}

	//UIの移動
	menuText[menuTextName[(int)MenuTextString::Tutorial_mst]]->GetComponent<SpriteRenderer>()->pos = moveRoundFloat2(circlePivot, { currentSelectDegree + 0,currentSelectDegree + 0 }, menuRadius);
	menuText[menuTextName[(int)MenuTextString::StartGame_mst]]->GetComponent<SpriteRenderer>()->pos = moveRoundFloat2(circlePivot, { currentSelectDegree + 40,currentSelectDegree + 40 }, menuRadius);
	menuText[menuTextName[(int)MenuTextString::FinishGame_mst]]->GetComponent<SpriteRenderer>()->pos = moveRoundFloat2(circlePivot, { currentSelectDegree + 80,currentSelectDegree + 80 }, menuRadius);


#ifdef USE_IMGUI
	ImGui::Begin("title debug ui");
	ImGui::InputFloat("currentSelectDegree", &currentSelectDegree);
	ImGui::InputFloat2("circlePivot", &circlePivot.x);
	ImGui::InputFloat2("menuRadius", &menuRadius.x);
	ImGui::End();
#endif
}

void SceneGame::Draw()
{
	for (auto& textes : menuText)
	{
		textes.second->Draw();
	}
	titleLogo->Draw();
}

//円状に動かす
//rePoint:基準位置、degree:回転量、radius:半径
DirectX::XMFLOAT2 SceneGame::moveRoundFloat2(DirectX::XMFLOAT2 rePoint, DirectX::XMFLOAT2 degree, DirectX::XMFLOAT2 radius)
{
	return { rePoint.x + radius.x * cosf(DirectX::XMConvertToRadians(degree.x)),
		rePoint.y + radius.y * sinf(DirectX::XMConvertToRadians(degree.y)) };
}
