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
	//�uTutorialPlay�v�摜�̓ǂݍ���
	menuText.emplace(menuNameList[(int)MenuTextString::Tutorial_mst],new GameObject("tutorialPlay"));
	menuText[menuNameList[(int)MenuTextString::Tutorial_mst]]->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/text_TutorialPlay.png"));
	//�uStartGame�v�摜�̓ǂݍ���
	menuText.emplace(menuNameList[(int)MenuTextString::StartGame_mst],new GameObject("startGame"));
	menuText[menuNameList[(int)MenuTextString::StartGame_mst]]->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/text_StartGame.png"));
	//�uFinishGame�v�摜�̓ǂݍ���
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
	float spacing = 0.0f;//�摜���m�̊Ԋu
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
	//1�t���[���̌o�ߎ��Ԃ��擾
	float deltaTime = SystemManager::Instance().GetElapsedTime();
	GamePad& gamepad = SystemManager::Instance().GetGamePad();
	float rotationSpeed = 150;
	static float oldTarget=0.0f;

	//�ω�����
	float changeMaxTime = 0.3f;

	//��{�^�����������Ƃ�
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
	//���{�^�����������Ƃ�
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

	//����{�^�����������Ƃ�
	if (gamepad.GetButtonUp() & GamePad::BTN_A)
	{
		switch (selectMenuType)
		{
		case MenuTextString::Tutorial_mst://"Tutorial Play"�̂Ƃ�
			SceneManager::Instance().ChangeScene(new SceneGame);
			return;
		case MenuTextString::StartGame_mst://"Start Game"�̂Ƃ�
			SceneManager::Instance().ChangeScene(new SceneGame);
			return;
		case MenuTextString::FinishGame_mst://"Finish Game"�̂Ƃ�
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
		uiTime += deltaTime;//�o�ߎ���
		currentSelectDegree = oldTarget;
		currentUISelectDegree = oldTarget;
		//Bounce�̂��ω��ʂ��Ԃ��Ă��邽�߂ǂ�����ǂꂾ���ω����邩
		currentSelectDegree -= easing::Quad::easeOut(uiTime, 0, currentSelectDegree - targetMenuDegreeList[selectMenuType], changeMaxTime);
		currentUISelectDegree -= easing::Elastic::easeOut(uiTime, 0, currentUISelectDegree - targetMenuDegreeList[selectMenuType], changeMaxTime);

		if (uiTime > changeMaxTime)
			operateType = OperateType::None;
		break;
	case OperateType::Down:
		uiTime += deltaTime;//�o�ߎ���
		currentSelectDegree = oldTarget;
		currentUISelectDegree = oldTarget;
		//Bounce�̂��ω��ʂ��Ԃ��Ă��邽�߂ǂ�����ǂꂾ���ω����邩
		currentSelectDegree += easing::Quad::easeOut(uiTime, 0, targetMenuDegreeList[selectMenuType] - currentSelectDegree, changeMaxTime);
		currentUISelectDegree += easing::Elastic::easeOut(uiTime, 0, targetMenuDegreeList[selectMenuType] - currentUISelectDegree, changeMaxTime);
		if (uiTime > changeMaxTime)
			operateType = OperateType::None;
		break;
	}

	//UI�̈ړ�
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

//�~��ɓ�����
//rePoint:��ʒu�Adegree:��]�ʁAradius:���a
DirectX::XMFLOAT2 SceneTitle::moveRoundFloat2(DirectX::XMFLOAT2 rePoint, DirectX::XMFLOAT2 degree, DirectX::XMFLOAT2 radius)
{
	return { rePoint.x + radius.x * cosf(DirectX::XMConvertToRadians(degree.x)),
		rePoint.y + radius.y * sinf(DirectX::XMConvertToRadians(degree.y)) };
}
