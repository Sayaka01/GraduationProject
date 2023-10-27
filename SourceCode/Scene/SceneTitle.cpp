#include "Stdafx.h"

#include "SceneTitle.h"

#include "SceneManager.h"

#include "../GameObject/GameObject.h"

#include "Component/SpriteRenderer.h"

#include "System/SystemManager.h"

#include "Scene/SceneGame.h"
#include "Scene/SceneLoading.h"

#include "System/easing.h"

#include "System/Common.h"

#include "System/UserFunction.h"

#include <magic_enum.hpp>

void SceneTitle::Initialize()
{
	for (int i = 0; i < (int)MenuTextString::Max_mst; i++)
	{
		menuNameList[i] = std::to_string(i) + magic_enum::enum_name((MenuTextString)i).data();
	}
	//「TutorialPlay」画像の読み込み
	sprMenuText.emplace(menuNameList[(int)MenuTextString::Tutorial_mst],new GameObject("tutorialPlay"));
	sprMenuText[menuNameList[(int)MenuTextString::Tutorial_mst]]->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/text_TutorialPlay.png"));
	//「StartGame」画像の読み込み
	sprMenuText.emplace(menuNameList[(int)MenuTextString::StartGame_mst],new GameObject("startGame"));
	sprMenuText[menuNameList[(int)MenuTextString::StartGame_mst]]->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/text_StartGame.png"));
	//「FinishGame」画像の読み込み
	sprMenuText.emplace(menuNameList[(int)MenuTextString::FinishGame_mst],new GameObject("finishGame"));
	sprMenuText[menuNameList[(int)MenuTextString::FinishGame_mst]]->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/text_FinishGame.png"));

	//タイトルロゴ画像の読み込み
	sprTitleLogo = std::make_unique<GameObject>("TitleLogo");
	sprTitleLogo->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/title_logo.png"));
	sprTitleLogo->GetComponent<SpriteRenderer>()->pos = { 400,-100 };

	//背景のギア画像読み込み
	for (int i = 0; i < 5; i++)
	{
		sprBackGears.emplace_back(std::make_unique<GameObject>("menuBack" + std::to_string(i)));
		std::wstring filename = StringToWString("./Resources/Sprite/Title/circleGear" + std::to_string(i) + ".png ");
		sprBackGears[i]->AddComponent(new SpriteRenderer(filename.c_str()));
		sprBackGears[i]->GetComponent<SpriteRenderer>()->pos = { -10,500 };
		sprBackGears[i]->GetComponent<SpriteRenderer>()->pivot = sprBackGears[i]->GetComponent<SpriteRenderer>()->GetSpriteSize() * DirectX::XMFLOAT2(0.5f, 0.5f);
		sprBackGears[i]->GetComponent<SpriteRenderer>()->color = { 1,0.26f,0.65f,0.5f };
	}

	//選択テキストの位置などを設定	
	menuRadius = { 400,300 };
	circlePivot = { -250, 400 };
	float spacing = 0.0f;//画像同士の間隔
	for (auto& textes : sprMenuText)
	{
		textes.second->GetComponent<SpriteRenderer>()->pos = moveRoundFloat2(circlePivot, { currentSelectDegree +spacing,currentSelectDegree + spacing }, menuRadius);
		spacing += 40.0f;
	}
}

void SceneTitle::Finalize()
{
	for (auto& textes : sprMenuText)
	{
		textes.second->Finalize();
	}
	sprTitleLogo->Finalize();
	for (int i = 0; i < 5; i++)
	{
		sprBackGears[i]->Finalize();
	}

}

void SceneTitle::Update()
{
	//1フレームの経過時間を取得
	float deltaTime = SystemManager::Instance().GetElapsedTime();
	GamePad& gamepad = SystemManager::Instance().GetGamePad();
	float rotationSpeed = 150;
	static float oldTarget=0.0f;

	//変化時間
	float changeMaxTime = 0.3f;

	//上ボタンを押したとき
	if ((gamepad.GetButtonUp() & GamePad::BTN_UP) )
		//|| (gamepad.GetAxisLY() >0))
	{
		if ((int)selectMenuType > 0)
		{
			oldTarget = targetMenuDegreeList[selectMenuType];
			selectMenuType -= 1;
			operateType = OperateType::Up;
			uiTime = 0;
		}
	}
	//下ボタンを押したとき
	if ((gamepad.GetButtonUp() & GamePad::BTN_DOWN))
		//|| (gamepad.GetAxisLY() <0))
	{
		if ((int)selectMenuType < (int)MenuTextString::Max_mst-1)
		{
			oldTarget = targetMenuDegreeList[selectMenuType];
			selectMenuType += 1;
			operateType = OperateType::Down;
			uiTime = 0;
		}
	}

	//決定ボタンを押したとき
	if (gamepad.GetButtonUp() & GamePad::BTN_A)
	{
		switch (selectMenuType)
		{
		case MenuTextString::Tutorial_mst://"Tutorial Play"のとき
			SceneManager::Instance().ChangeScene(new SceneGame);
			return;
		case MenuTextString::StartGame_mst://"Start Game"のとき
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
			return;
		case MenuTextString::FinishGame_mst://"Finish Game"のとき
			PostQuitMessage(0);
			return;
		}
	}


	switch (operateType)
	{
	case OperateType::None:
		currentSelectDegree = targetMenuDegreeList[selectMenuType];
		currentUISelectDegree = targetMenuDegreeList[selectMenuType];

		for (int i = 1; i < 5; i++)
		{
			float speed = 30;
			if (i % 2 == 0)speed = -speed;
			sprBackGears[i]->GetComponent<SpriteRenderer>()->degree += i* deltaTime * speed;
			sprBackGears[i]->Update();
		}
		break;
	case OperateType::Up:
		uiTime += deltaTime;//経過時間
		currentSelectDegree = oldTarget;
		currentUISelectDegree = oldTarget;
		//Bounceのよる変化量が返ってくるためどこからどれだけ変化するか
		currentSelectDegree -= easing::Quad::easeOut(uiTime, 0, currentSelectDegree - targetMenuDegreeList[selectMenuType], changeMaxTime);
		currentUISelectDegree -= easing::Elastic::easeOut(uiTime, 0, currentUISelectDegree - targetMenuDegreeList[selectMenuType], changeMaxTime);
		for (int i = 1; i < 5; i++)
		{
			sprBackGears[i]->GetComponent<SpriteRenderer>()->degree = currentUISelectDegree + 40;
			sprBackGears[i]->Update();
		}

		if (uiTime > changeMaxTime)
			operateType = OperateType::None;
		break;
	case OperateType::Down:
		uiTime += deltaTime;//経過時間
		currentSelectDegree = oldTarget;
		currentUISelectDegree = oldTarget;
		//Bounceのよる変化量が返ってくるためどこからどれだけ変化するか
		currentSelectDegree += easing::Quad::easeOut(uiTime, 0, targetMenuDegreeList[selectMenuType] - currentSelectDegree, changeMaxTime);
		currentUISelectDegree += easing::Elastic::easeOut(uiTime, 0, targetMenuDegreeList[selectMenuType] - currentUISelectDegree, changeMaxTime);
		for (int i = 1; i < 5; i++)
		{
			sprBackGears[i]->GetComponent<SpriteRenderer>()->degree = currentUISelectDegree + 40;
			sprBackGears[i]->Update();
		}
		if (uiTime > changeMaxTime)
			operateType = OperateType::None;
		break;
	}

	//UIの移動
	sprMenuText[menuNameList[(int)MenuTextString::Tutorial_mst]]->GetComponent<SpriteRenderer>()->pos = moveRoundFloat2(circlePivot, { currentSelectDegree + 0,currentSelectDegree + 0 }, menuRadius);
	sprMenuText[menuNameList[(int)MenuTextString::StartGame_mst]]->GetComponent<SpriteRenderer>()->pos = moveRoundFloat2(circlePivot, { currentSelectDegree + 40,currentSelectDegree + 40 }, menuRadius);
	sprMenuText[menuNameList[(int)MenuTextString::FinishGame_mst]]->GetComponent<SpriteRenderer>()->pos = moveRoundFloat2(circlePivot, { currentSelectDegree + 80,currentSelectDegree + 80 }, menuRadius);

	sprBackGears[0]->GetComponent<SpriteRenderer>()->degree = currentUISelectDegree + 40;
	sprBackGears[0]->Update();

	for (int i = 0; i < 3; i++)
	{
		if(selectMenuType == i)
			sprMenuText[menuNameList[i]]->GetComponent<SpriteRenderer>()->color = { 0.6f,0.8f,0.96f,1 };
		else
			sprMenuText[menuNameList[i]]->GetComponent<SpriteRenderer>()->color = { 0.6f,0.8f,0.96f,0.5f };

	}

#ifdef USE_IMGUI
	ImGui::Begin("title debug ui");
	ImGui::InputFloat("currentSelectDegree", &currentSelectDegree);
	ImGui::InputFloat2("circlePivot", &circlePivot.x);
	ImGui::InputFloat2("menuRadius", &menuRadius.x);
	ImGui::End();
#endif
}

void SceneTitle::Draw()
{
	for (int i = 0; i < 5; i++)
	{
		sprBackGears[i]->Draw();
	}
	for (auto& textes : sprMenuText)
	{
		textes.second->Draw();
	}
	sprTitleLogo->Draw();

}

//円状に動かす
//rePoint:基準位置、degree:回転量、radius:半径
DirectX::XMFLOAT2 SceneTitle::moveRoundFloat2(DirectX::XMFLOAT2 rePoint, DirectX::XMFLOAT2 degree, DirectX::XMFLOAT2 radius)
{
	return { rePoint.x + radius.x * cosf(DirectX::XMConvertToRadians(degree.x)),
		rePoint.y + radius.y * sinf(DirectX::XMConvertToRadians(degree.y)) };
}
