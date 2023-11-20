
#include "Stdafx.h"

#include "sceneLoading.h"
#include "sceneManager.h"
#include <thread>
#include "Component/SpriteRenderer.h"
#include <SimpleMath.h>
#include "System/SystemManager.h"

SceneLoading::SceneLoading(Scene* nextScene)
{
	this->nextScene = nextScene;
}


void SceneLoading::Initialize()
{
	sprLoading = new GameObject("loading");
	sprLoading->AddComponent(new SpriteRenderer(L"./Resources/Sprite/LoadingIcon.png"));
	sprLoading->GetComponent<SpriteRenderer>()->pos = {1150,600};
	sprLoading->GetComponent<SpriteRenderer>()->scale = { 1,1 };
	sprLoading->GetComponent<SpriteRenderer>()->pivot = sprLoading->GetComponent<SpriteRenderer>()->GetSpriteSize()*DirectX::SimpleMath::Vector2(0.5f, 0.5f);


	//�X���b�h�J�n
	std::thread thread(LoadingThread, this);
	//�X���b�h�̊Ǘ������
	thread.detach();
}
void SceneLoading::Finalize()
{
	sprLoading->Finalize();
	delete sprLoading;
}
void SceneLoading::Update()
{
	sprLoading->GetComponent<SpriteRenderer>()->degree += 90 * SystemManager::Instance().GetElapsedTime();
	sprLoading->Update();

	//���̃V�[���̏���������������V�[����؂�ւ���
	if (nextScene->IsReady())
	{
		SceneManager::Instance().ChangeScene(nextScene, true);
	}
}
void SceneLoading::Draw()
{
	sprLoading->Draw();
}

void SceneLoading::LoadingThread(SceneLoading* scene)
{
	//COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
	CoInitialize(nullptr);

	//���̃V�[���̏��������s��
	scene->nextScene->Initialize();

	//�X���b�h���I���O��COM�֘A�̏I����
	CoUninitialize();

	//���̃V�[���̏��������ݒ�
	scene->nextScene->SetReady();
	
}