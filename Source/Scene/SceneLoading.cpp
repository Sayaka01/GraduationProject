
#include "Stdafx.h"

#include "sceneLoading.h"
#include "sceneManager.h"
#include <thread>

SceneLoading::SceneLoading(Scene* nextScene)
{
	this->nextScene = nextScene;
}


void SceneLoading::Initialize()
{
	//�X���b�h�J�n
	std::thread thread(LoadingThread, this);
	//�X���b�h�̊Ǘ������
	thread.detach();
}
void SceneLoading::Finalize()
{

}
void SceneLoading::Update()
{
	//���̃V�[���̏���������������V�[����؂�ւ���
	if (nextScene->IsReady())
	{
		SceneManager::Instance().ChangeScene(nextScene, true);
	}
}
void SceneLoading::Draw()
{


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