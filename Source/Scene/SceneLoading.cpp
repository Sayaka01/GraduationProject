
#include "Stdafx.h"

#include "sceneLoading.h"
#include "sceneManager.h"
#include <thread>

SceneLoading::SceneLoading(Scene* nextScene)
{
	this->nextScene = nextScene;
}


void SceneLoading::Initialize(ID3D11Device* device)
{
	//�X���b�h�J�n
	std::thread thread(LoadingThread, this);
	//�X���b�h�̊Ǘ������
	thread.detach();
}
void SceneLoading::Finalize()
{

}
void SceneLoading::Update(float elapsedTime)
{
	//���̃V�[���̏���������������V�[����؂�ւ���
	if (nextScene->IsReady())
	{
		SceneManager::Instance().ChangeScene(nextScene, true);
	}
}
void SceneLoading::Draw(ID3D11DeviceContext* dc)
{


}

void SceneLoading::LoadingThread(SceneLoading* scene)
{
	//COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
	CoInitialize(nullptr);

	ID3D11Device* device = SceneManager::Instance().GetDevice();

	//���̃V�[���̏��������s��
	scene->nextScene->Initialize(device);

	//�X���b�h���I���O��COM�֘A�̏I����
	CoUninitialize();

	//���̃V�[���̏��������ݒ�
	scene->nextScene->SetReady();
	
}