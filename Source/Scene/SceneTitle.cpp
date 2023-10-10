#include "Stdafx.h"

#include "SceneTitle.h"

#include "SceneManager.h"

#include "../GameObject/GameObject.h"

#include "Component/SpriteRenderer.h"

#include "System/SystemManager.h"

#include "System/easing.h"

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

	titleLogo = std::make_unique<GameObject>("TitleLogo");
	titleLogo->AddComponent(new SpriteRenderer(L"./Resources/Sprite/Title/title_logo.png"));
	titleLogo->GetComponent<SpriteRenderer>()->pos = { 400,-100 };

	currentSelectDegree = -40;
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
}

void SceneTitle::Update()
{
	DirectX::XMFLOAT2 rePoint = { 0,0 };
	static int operateType = -1;
	//1�t���[���̌o�ߎ��Ԃ��擾
	float deltaTime = SystemManager::Instance().GetElapsedTime();
	GamePad& gamepad = SystemManager::Instance().GetGamePad();
	float rotationSpeed = 150;
	static float oldTarget=0.0f;

	//�ω�����
	float changeMaxTime = 0.3f;

	if (gamepad.GetButtonDown() & GamePad::BTN_UP)
	{
		if ((int)selectMenuType > 0)
		{
			selectMenuType -= 1;
			operateType = 0;
			uiTime = 0;
			oldTarget = targetSelectDegree;
		}
	}
	if (gamepad.GetButtonDown() & GamePad::BTN_DOWN)
	{
		if ((int)selectMenuType < (int)MenuTextString::Max_mst-1)
		{
			selectMenuType += 1;
			operateType = 1;
			uiTime = 0;
			oldTarget = targetSelectDegree;
		}
	}

	switch (selectMenuType)
	{
	case 0://"Tutorial Play"�̂Ƃ�
		targetSelectDegree = 0;
		//SceneManager::Instance().ChangeScene(new SceneGame);
		break;
	case 1://"Start Game"�̂Ƃ�
		targetSelectDegree = -40;
		//SceneManager::Instance().ChangeScene(new SceneGame);
		break;
	case 2://"Finish Game"�̂Ƃ�
		targetSelectDegree = -80;
		//PostQuitMessage(0);
		break;
	}

	switch (operateType)
	{
	case -1:
		currentSelectDegree = targetSelectDegree;
		break;
	case 0:
		uiTime += deltaTime ;//�o�ߎ���
		currentSelectDegree = oldTarget;
		//Bounce�̂��ω��ʂ��Ԃ��Ă��邽�߂ǂ�����ǂꂾ���ω����邩
		currentSelectDegree += easing::Elastic::easeOut(uiTime, 0, targetSelectDegree - currentSelectDegree, changeMaxTime);

		//currentSelectDegree = currentSelectDegree + deltaTime * rotationSpeed;
		if (uiTime > changeMaxTime)
			operateType = -1;
		break;
	case 1:
		uiTime += deltaTime;//�o�ߎ���
		currentSelectDegree = oldTarget;
		//Bounce�̂��ω��ʂ��Ԃ��Ă��邽�߂ǂ�����ǂꂾ���ω����邩
		currentSelectDegree -= easing::Elastic::easeOut(uiTime, 0, currentSelectDegree - targetSelectDegree, changeMaxTime);

		//currentSelectDegree = currentSelectDegree + deltaTime * rotationSpeed;
		if (uiTime > changeMaxTime)
			operateType = -1;
		//currentSelectDegree = currentSelectDegree - deltaTime * rotationSpeed;
		//if (currentSelectDegree < targetSelectDegree)
		//	operateType = -1;
		break;
	}

	//UI�̈ړ�
	menuText[menuTextName[(int)MenuTextString::Tutorial_mst]]->GetComponent<SpriteRenderer>()->pos = moveRoundFloat2(circlePivot, { currentSelectDegree + 0,currentSelectDegree + 0 }, menuRadius);
	menuText[menuTextName[(int)MenuTextString::StartGame_mst]]->GetComponent<SpriteRenderer>()->pos = moveRoundFloat2(circlePivot, { currentSelectDegree + 40,currentSelectDegree + 40 }, menuRadius);
	menuText[menuTextName[(int)MenuTextString::FinishGame_mst]]->GetComponent<SpriteRenderer>()->pos = moveRoundFloat2(circlePivot, { currentSelectDegree + 80,currentSelectDegree + 80 }, menuRadius);


#ifdef USE_IMGUI
	ImGui::Begin("title debug ui");
	ImGui::InputInt("operateType", &operateType);
	ImGui::InputFloat("currentSelectDegree", &currentSelectDegree);
	ImGui::InputFloat2("circlePivot", &circlePivot.x);
	ImGui::InputFloat2("menuRadius", &menuRadius.x);
	ImGui::End();
#endif
}

void SceneTitle::Draw()
{
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
