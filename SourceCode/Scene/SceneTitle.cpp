#include "Stdafx.h"

#include "SceneTitle.h"

#include "SceneManager.h"

#include "../GameObject/GameObject.h"

#include "Component/SpriteRenderer.h"

#include "System/SystemManager.h"

#include "Scene/SceneGame.h"

#include "System/easing.h"

#include "System/Common.h"

#include <magic_enum.hpp>

void SceneTitle::Initialize()
{
	for (int i = 0; i < (int)MenuTextString::Max_mst; i++)
	{
		menuNameList[i] = std::to_string(i) + magic_enum::enum_name((MenuTextString)i).data();
	}
	//「TutorialPlay」画像の読み込み
	menuText.emplace(menuNameList[(int)MenuTextString::Tutorial_mst],new GameObject("tutorialPlay"));
	menuText[menuNameList[(int)MenuTextString::Tutorial_mst]]->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/text_TutorialPlay.png"));
	//「StartGame」画像の読み込み
	menuText.emplace(menuNameList[(int)MenuTextString::StartGame_mst],new GameObject("startGame"));
	menuText[menuNameList[(int)MenuTextString::StartGame_mst]]->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/text_StartGame.png"));
	//「FinishGame」画像の読み込み
	menuText.emplace(menuNameList[(int)MenuTextString::FinishGame_mst],new GameObject("finishGame"));
	menuText[menuNameList[(int)MenuTextString::FinishGame_mst]]->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/text_FinishGame.png"));

	titleLogo = std::make_unique<GameObject>("TitleLogo");
	titleLogo->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/title_logo.png"));
	titleLogo->GetComponent<SpriteRenderer>()->pos = { 400,-100 };
	menuBack = std::make_unique<GameObject>("menuBack");
	menuBack->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/circleGear.png"));
	menuBack->GetComponent<SpriteRenderer>()->pos = { -10,500 };
	menuBack->GetComponent<SpriteRenderer>()->scale = { 2.5f,2.5f };
	menuBack->GetComponent<SpriteRenderer>()->pivot = menuBack->GetComponent<SpriteRenderer>()->GetSpriteSize()* DirectX::XMFLOAT2(0.5f,0.5f);
	menuBack->GetComponent<SpriteRenderer>()->color = { 1,1,1,0.5f };

	menuRadius = { 400,300 };
	circlePivot = { -250, 400 };
	float spacing = 0.0f;//画像同士の間隔
	for (auto& textes : menuText)
	{
		textes.second->GetComponent<SpriteRenderer>()->pos = moveRoundFloat2(circlePivot, { currentSelectDegree +spacing,currentSelectDegree + spacing }, menuRadius);
		spacing += 40.0f;
	}
}

void SceneTitle::Finalize()
{
	for (auto& textes : menuText)
	{
		textes.second->Finalize();
	}
	titleLogo->Finalize();
	menuBack->Finalize();
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
			SceneManager::Instance().ChangeScene(new SceneGame);
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
		break;
	case OperateType::Up:
		uiTime += deltaTime;//経過時間
		currentSelectDegree = oldTarget;
		currentUISelectDegree = oldTarget;
		//Bounceのよる変化量が返ってくるためどこからどれだけ変化するか
		currentSelectDegree -= easing::Quad::easeOut(uiTime, 0, currentSelectDegree - targetMenuDegreeList[selectMenuType], changeMaxTime);
		currentUISelectDegree -= easing::Elastic::easeOut(uiTime, 0, currentUISelectDegree - targetMenuDegreeList[selectMenuType], changeMaxTime);

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
		if (uiTime > changeMaxTime)
			operateType = OperateType::None;
		break;
	}

	//UIの移動
	menuText[menuNameList[(int)MenuTextString::Tutorial_mst]]->GetComponent<SpriteRenderer>()->pos = moveRoundFloat2(circlePivot, { currentSelectDegree + 0,currentSelectDegree + 0 }, menuRadius);
	menuText[menuNameList[(int)MenuTextString::StartGame_mst]]->GetComponent<SpriteRenderer>()->pos = moveRoundFloat2(circlePivot, { currentSelectDegree + 40,currentSelectDegree + 40 }, menuRadius);
	menuText[menuNameList[(int)MenuTextString::FinishGame_mst]]->GetComponent<SpriteRenderer>()->pos = moveRoundFloat2(circlePivot, { currentSelectDegree + 80,currentSelectDegree + 80 }, menuRadius);

	menuBack->GetComponent<SpriteRenderer>()->degree = currentUISelectDegree +40;
	menuBack->Update();

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
	menuBack->Draw();
	for (auto& textes : menuText)
	{
		textes.second->Draw();
	}
	titleLogo->Draw();

}

//円状に動かす
//rePoint:基準位置、degree:回転量、radius:半径
DirectX::XMFLOAT2 SceneTitle::moveRoundFloat2(DirectX::XMFLOAT2 rePoint, DirectX::XMFLOAT2 degree, DirectX::XMFLOAT2 radius)
{
	return { rePoint.x + radius.x * cosf(DirectX::XMConvertToRadians(degree.x)),
		rePoint.y + radius.y * sinf(DirectX::XMConvertToRadians(degree.y)) };
}
