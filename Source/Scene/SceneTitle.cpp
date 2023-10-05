#include "Stdafx.h"

#include "SceneTitle.h"

#include "SceneManager.h"

#include "../GameObject/GameObject.h"

#include "Component/SpriteRenderer.h"

#include "System/SystemManager.h"

#include <magic_enum.hpp>

void SceneTitle::Initialize()
{
	for (int i = 0; i < (int)MenuTextString::Max_mst; i++)
	{
		menuTextName[i] = std::to_string(i) + magic_enum::enum_name((MenuTextString)i).data();
	}
	//�uTutorialPlay�v�摜�̓ǂݍ���
	menuText.emplace(menuTextName[(int)MenuTextString::Tutorial_mst],new GameObject("tutorialPlay"));
	menuText[menuTextName[(int)MenuTextString::Tutorial_mst]]->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/text_TutorialPlay.png"));
	//�uStartGame�v�摜�̓ǂݍ���
	menuText.emplace(menuTextName[(int)MenuTextString::StartGame_mst],new GameObject("startGame"));
	menuText[menuTextName[(int)MenuTextString::StartGame_mst]]->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/text_StartGame.png"));
	//�uFinishGame�v�摜�̓ǂݍ���
	menuText.emplace(menuTextName[(int)MenuTextString::FinishGame_mst],new GameObject("finishGame"));
	menuText[menuTextName[(int)MenuTextString::FinishGame_mst]]->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/text_FinishGame.png"));
}

void SceneTitle::Finalize()
{
	for (auto& textes : menuText)
	{
		textes.second->Finalize();
	}
}

void SceneTitle::Update()
{
	DirectX::XMFLOAT2 rePoint = { 0,0 };
	static int operateType = -1;
	//1�t���[���̌o�ߎ��Ԃ��擾
	float deltaTime = SystemManager::Instance().GetElapsedTime();
	GamePad& gamepad = SystemManager::Instance().GetGamePad();

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
		if ((int)selectMenuType < (int)MenuTextString::Max_mst-1)
		{
			selectMenuType += 1;
			operateType = 1;
		}
	}

	switch (operateType)
	{
	case -1:
		currentSelectDegree = targetSelectDegree;
		break;
	case 0:
		currentSelectDegree = currentSelectDegree - deltaTime * 100;
		if (currentSelectDegree < targetSelectDegree)
			operateType = -1;
		break;
	case 1:
		currentSelectDegree = currentSelectDegree - deltaTime * 100;
		if (currentSelectDegree > targetSelectDegree)
			operateType = -1;
		break;
	}

	for (auto& textes : menuText)
	{
		textes.second->GetComponent<SpriteRenderer>()->pos = moveRoundFloat2({ 300, 300 }, { currentSelectDegree + rePoint.x,currentSelectDegree + rePoint.y }, 200);
		rePoint.y += 40;
		textes.second->Update();
	}
}

void SceneTitle::Draw()
{
	for (auto& textes : menuText)
	{
		textes.second->Draw();
	}
}

//�~��ɓ�����
//rePoint:��ʒu�Adegree:��]�ʁAradius:���a
DirectX::XMFLOAT2 SceneTitle::moveRoundFloat2(DirectX::XMFLOAT2 rePoint, DirectX::XMFLOAT2 degree, float radius)
{
	return { rePoint.x + radius * cosf(DirectX::XMConvertToRadians(degree.x)),
		rePoint.y + radius * sinf(DirectX::XMConvertToRadians(degree.y)) };
}
